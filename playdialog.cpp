#include "playdialog.h"
#include "ui_playdialog.h"
#include <QThread>


playDialog::playDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::playDialog)
{
    ui->setupUi(this);
}

playDialog::~playDialog()
{
    delete ui;
    if (getFrame)
    {
        delete getFrame;
    }
}

void playDialog::play(QString name)
{
    this->setWindowTitle(name);
    printf("play Dialog start\n");
    getFrame = new GetFrameThread(name); //new a thread just for play
    connect(getFrame,&GetFrameThread::sendFrame,this,&playDialog::recFrame);
    QImage image(getFrame->rgbBuf , getFrame->width , getFrame->height , QImage::Format_RGB888);//data数组 //355宽度 //frame_len 高度//每行1005字节数//格式
//    this->resize(getFrame->width+12,getFrame->height+24);

    QPixmap pic_pixmap=QPixmap::fromImage(image);

    ui->label_pic->setAutoFillBackground(true);
    pic_pixmap=pic_pixmap.scaled(ui->label_pic->size(), Qt::KeepAspectRatioByExpanding);//自适应/等比例
    ui->label_pic->setStyleSheet("background: black;");  // 标签背景
    ui->label_pic->setAlignment(Qt::AlignCenter);  // 图片居中
    ui->label_pic->setPixmap(pic_pixmap);

}

void playDialog::on_Button_play_clicked()
{
   if (getFrame)
   {
       if (getFrame->m_play_flag == false)
       {
           getFrame->m_play_flag = true;
           getFrame->start();
       }
   }
}

void playDialog::on_Button_pause_clicked()
{
    printf("pasue button\n");
    getFrame->m_play_flag = getFrame->m_play_flag?0:1;
    printf("set play flag to:%d\n",getFrame->m_play_flag);
}
void playDialog::recFrame( unsigned char *rgbbuf)
{
    QImage image(rgbbuf, getFrame->width , getFrame->height , QImage::Format_RGB888);//data数组 //355宽度 //frame_len 高度//每行1005字节数//格式


    QPixmap pic_pixmap=QPixmap::fromImage(image);

    ui->label_pic->setAutoFillBackground(true);
    pic_pixmap=pic_pixmap.scaled(ui->label_pic->size(), Qt::KeepAspectRatio);//自适应/等比例
    ui->label_pic->setStyleSheet("background: black;");  // 标签背景
    ui->label_pic->setAlignment(Qt::AlignCenter);  // 图片居中
    ui->label_pic->setPixmap(pic_pixmap);
}

void playDialog::on_pushButton_next_clicked()
{
    printf("next button\n");
    getFrame->m_next_flag = true;
}
