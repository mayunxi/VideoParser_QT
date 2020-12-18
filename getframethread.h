#ifndef GETFRAMETHREAD_H
#define GETFRAMETHREAD_H

#include <QObject>
#include <QThread>
#include <H264Decode.h>
class GetFrameThread : public QThread
{
    Q_OBJECT //使用信号与槽函数
public:
    GetFrameThread(QString fileName);
    ~GetFrameThread();
    //实现run接口
    void run();
    CH264Decoder m_h264Dec;
    QString m_fileName;
    int size,width,height;
    unsigned char * yuvBuf = NULL,*rgbBuf =  NULL;
    bool m_play_flag = false;
    bool m_next_flag = false;
//声明信号
signals:
    void sendFrame( unsigned char *rgbbuf);

};

#endif // GETFRAMETHREAD_H
