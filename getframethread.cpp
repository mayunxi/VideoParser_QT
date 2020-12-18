#include "getframethread.h"

GetFrameThread::GetFrameThread(QString fileName)
{
    m_fileName = fileName;
    m_h264Dec.openVideoFile(fileName.toStdString().c_str());

    m_h264Dec.getFrame(&yuvBuf,&rgbBuf,&size,&width,&height);
}

GetFrameThread::~GetFrameThread()
{

}

void GetFrameThread::run()
{
    int ret;
    while (1)
    {
        if (m_play_flag)
        {
            ret = m_h264Dec.getFrame(&yuvBuf,&rgbBuf,&size,&width,&height);
            printf("ret=%d\n",ret);
            emit sendFrame(rgbBuf);
        }
        if (m_next_flag)
        {
            m_next_flag = false;
            ret = m_h264Dec.getFrame(&yuvBuf,&rgbBuf,&size,&width,&height);
            emit sendFrame(rgbBuf);
        }
        if (ret == 0)
        {
            m_play_flag = false;
            break;
        }
        msleep(40);
    }

}
