#include "videoparser.h"
#include <string>
using namespace std;
VideoParser::VideoParser()
{

}

void VideoParser::init(string fileName)
{
    m_vNalTypeVector.clear();

    int nMaxNalNum = -1;
    const char *file = fileName.c_str();
    m_nalParser.init(file);
    m_nalParser.probeNALU(m_vNalTypeVector, nMaxNalNum);
    m_nValTotalNum = m_vNalTypeVector.size();
    if (m_nValTotalNum > 0)
    {
        for(int i = 0;i < m_nValTotalNum;i++)
        {
            m_nalParser.naluInfoStr(&m_vNalTypeVector.at(i));
        }
    }
    m_nalParser.getVideoInfo(&m_cVideoInfo);

}
