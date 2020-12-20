#ifndef PLAYDIALOG_H
#define PLAYDIALOG_H

#include <QDialog>
#include "H264Decode.h"
#include "getframethread.h"

namespace Ui {
class playDialog;
}

class playDialog : public QDialog
{
    Q_OBJECT

public:
    explicit playDialog(QWidget *parent = 0);
    ~playDialog();
     CH264Decoder m_h264Dec;
     bool m_play_flag = false;
     GetFrameThread *getFrame = NULL;
public slots:
    void play(QString name);
private slots:
    void on_Button_play_clicked();

    void on_Button_pause_clicked();

    void recFrame( unsigned char *rgbbuf);
    void on_pushButton_next_clicked();

private:
    Ui::playDialog *ui;
};

#endif // PLAYDIALOG_H
