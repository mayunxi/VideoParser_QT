#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include "H264Decode.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //resize(300,200);
    //菜单栏
    QMenuBar *mBar = menuBar();     //添加菜单
    QMenu *pFile = mBar->addMenu("文件");     //添加菜单项，添加动作
    QAction *popen = pFile->addAction("打开");
    connect(popen,&QAction::triggered,this,&MainWindow::openFile);

    QAction *pNew = pFile->addAction("退出");
    connect(pNew, &QAction::triggered,
    [=]()
    {
        qDebug() << "exit";
        exit(0);
    });
    //pFile->addSeparator();//添加分割线

    QMenu *pPlay = mBar->addMenu("播放");
    QAction *playAct = pPlay->addAction("播放");
    connect(playAct,&QAction::triggered,this,&MainWindow::play);

    QMenu *pAbout = mBar->addMenu("关于");
    QAction *aboutAct = pAbout->addAction("关于");
    QAction *helpAct = pAbout->addAction("帮助");


    QTableWidget *tableWidget = ui->tableWidget;
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁止编辑
    tableWidget->setRowCount(12); //设置行数
    tableWidget->setColumnCount(6); //设置列
    tableWidget->setColumnWidth(0, 60);  //设置列宽
    tableWidget->setColumnWidth(1, 120);
    tableWidget->setColumnWidth(2, 80);
    tableWidget->setColumnWidth(2, 120);
    tableWidget->setColumnWidth(4, 260);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中

    QStringList header;
    header<<"No."<<"Offset"<<"Length" << "Start Code"<<"NAL Type"<<"Info";
    tableWidget->setHorizontalHeaderLabels(header);
    tableWidget->verticalHeader()->setVisible(false);   //隐藏列表头
    //tableWidget->horizontalHeader()->setVisible(false); //隐藏行表头
    QFont font ;//定义一个字体变量
    font.setBold(true);  //设置粗体
    tableWidget->horizontalHeader()->setFont(font);//把字体变量属性加进表头中
    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头的背景色
    connect(tableWidget,&QTableWidget::itemClicked,this,&MainWindow::tableItemClick);

    QColor color("#e0e5df");
    QPalette pal;
    pal.setBrush(QPalette::Base, color);
    ui->textEdit_info->setPalette(pal);

    hexEdit = new QHexEdit(ui->frame_hex);
    //hexEdit->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    hexEdit->resize(831,221);

    m_naluTreeWidget = new QTreeWidget(ui->widget);
    m_naluTreeWidget->setFixedSize(ui->widget->size());
    m_naluTreeWidget->show();
    m_naluTreeWidget->headerItem()->setText(0,QString());    //设置表头为空


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    QString curPath=QDir::currentPath();//获取系统当前目录
    QString dlgTitle="打开视频文件"; //对话框标题
    QString filter="h264/h265文件(*.h264 *.264 *.h265 *.265);;视频文件(*.mp4 *.flv *.avi);;所有文件(*.*)"; //文件过滤器
    aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
    if (aFileName.isEmpty())
      return;
    this->setWindowTitle(aFileName+"-Video Parser");
    m_videoParser.init(aFileName.toStdString());
    m_videoParser.m_nalParser.m_pTree = m_naluTreeWidget;
    ui->tableWidget->clearContents();
    tableIndex = 0;
    //表格窗口显示NALU信息
    for (int i = 0; i < m_videoParser.m_nValTotalNum; i++)
    {
        ShowNLInfo(&m_videoParser.m_vNalTypeVector[i]);
    }
    //视频信息
    ui->textEdit_info->setText(QString::fromStdString(m_videoParser.m_cVideoInfo.strSimpleInfo));
}

void MainWindow::play()
{
    if (aFileName != "")
    {

        playWin.show();
        connect(this,&MainWindow::sendFileName,&playWin,&playDialog::play);
        emit sendFileName(aFileName);
    }
}
void MainWindow::tableItemClick(QTableWidgetItem* item)
{
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    int count = items.count();
    QTableWidgetItem *num_item = items.at(0);
    int pos = num_item->text().toInt();
    char* nalData = NULL;
    char* nalInfo = NULL;
    NALU_t *nal = &m_videoParser.m_vNalTypeVector[pos];
    int ret = m_videoParser.m_nalParser.parseNALU(*nal, &nalData, &nalInfo);
    /*************************display data with hex***************************************/
    QByteArray byte;
    byte = QByteArray(nalData,nal->len); //一定要加长度，否则QByteArray遇到0就停止拷贝
    hexEdit->setData(byte);

}


int MainWindow::ShowNLInfo(NALU_t* nalu)
{

    QTableWidget *tableWidget = ui->tableWidget;

    int iRow = tableWidget->rowCount();
    if (tableIndex >= iRow)
    {
        tableWidget->setRowCount(iRow + 1); //在最后添加一行或者，tableWidget->insertRow(iRow); //在iRow后面插入一行
    }

    QColor backColor;
    backColor.setNamedColor(QString::fromStdString(nalu->backgroundColor));
    QColor textColor;
    textColor.setNamedColor(QString::fromStdString(nalu->textColor));

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setBackgroundColor(backColor);
    item->setText(QString::number(nalu->num));
    tableWidget->setItem(tableIndex,0,item);

    item = new QTableWidgetItem;
    item->setBackgroundColor(backColor);
    item->setText(QString::fromStdString(nalu->strOffset));
    tableWidget->setItem(tableIndex,1,item);

    item = new QTableWidgetItem;
    item->setBackgroundColor(backColor);
    item->setText(QString::fromStdString(nalu->strNalLen));
    tableWidget->setItem(tableIndex,2,item);

    item = new QTableWidgetItem;
    item->setBackgroundColor(backColor);
    item->setText(QString::fromStdString(nalu->strStartCode));
    tableWidget->setItem(tableIndex,3,item);

    item = new QTableWidgetItem;
    item->setBackgroundColor(backColor);
    item->setText(QString::fromStdString(nalu->strNalUnitType));
    tableWidget->setItem(tableIndex,4,item);

    item = new QTableWidgetItem;
    item->setBackgroundColor(backColor);
    item->setTextColor(textColor);
    item->setText(QString::fromStdString(nalu->strNalInfo));
    tableWidget->setItem(tableIndex,5,item);


    tableIndex++;
    return 1;
}
