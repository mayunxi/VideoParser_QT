/*
by stand c++
 */
#ifndef VIDEOPARSER_H
#define VIDEOPARSER_H
#include "NalParse.h"

class VideoParser
{
public:
    VideoParser();
public:
    NalParser m_nalParser;
    vector<NALU_t> m_vNalTypeVector;
    int m_nValTotalNum; // m_vNalTypeVector有多少个NALU_t
    videoinfo_t m_cVideoInfo;
public:
    void init(string fileName);
};

#endif // VIDEOPARSER_H
