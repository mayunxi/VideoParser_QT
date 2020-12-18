#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "videoparser.h"
#include "qhexedit.h"
#include "playdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public slots:
    void openFile();
    void play();
    void tableItemClick(QTableWidgetItem* item);
public:
    VideoParser m_videoParser;
    QHexEdit *hexEdit;
public:
    int ShowNLInfo(NALU_t* nalu);
private:
    QString aFileName="";
    playDialog playWin;
    int tableIndex=0;
signals:
    void sendFileName(QString name);
};

#endif // MAINWINDOW_H
