#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "videoparser.h"
#include "qhexedit.h"

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
    void tableItemClick(QTableWidgetItem* item);
public:
    VideoParser m_videoParser;
    QHexEdit *hexEdit;
public:
    int ShowNLInfo(NALU_t* nalu);
private:
    int tableIndex=0;
};

#endif // MAINWINDOW_H
