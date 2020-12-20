#ifndef PALPARSE_H
#define PALPARSE_H

#include "h264_stream.h"
#include "h265_stream.h"

#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
//**********************
#include <QTreeWidget>
typedef QTreeWidgetItem *  TREE_NALU;
//**********************
using std::vector;
using namespace std;
typedef struct
{
    int codecType;                       // 0 -- h.264; 1 -- h.265
    unsigned int num;               // 序号
    unsigned int len;               // 含起始码的总的长度
    unsigned int offset;			// nal包在文件中的偏移
    int sliceType;					// 帧类型
    int nalType;					// NAL类型
    int startcodeLen;				// start code长度
    char startcodeBuffer[16];       // 起始码，字符串形式

    string strOffset;
    string strNalLen;
    string strStartCode;

    string strNalUnitType;
    string strNalInfo;

    string textColor="black";
    string backgroundColor="white";
} NALU_t;

typedef struct
{
    int profile_idc;
    int level_idc;
    int width;
    int height;
    int crop_left;
    int crop_right;
    int crop_top;
    int crop_bottom;
    float max_framerate;  // 由SPS计算得到的帧率，为0表示SPS中没有相应的字段计算
    int chroma_format_idc;  // YUV颜色空间 0: monochrome 1:420 2:422 3:444
}SPSInfo_t;

typedef struct
{
    int encoding_type;  // 为1表示CABAC 0表示CAVLC

}PPSInfo_t;

enum FileType
{
    FILE_H264 = 0,
    FILE_H265 = 1,
    FILE_UNK  = 2,
};

const int MAX_NAL_SIZE = 1*1024*1024;
const int OUTPUT_SIZE = 512*1024;

class NalParser
{
public:
    NalParser();
    ~NalParser();

    int init(const char* filename);
    int release(void);

    // 搜索视频文件的ANL单元，记录偏移及长度
    int probeNALU(vector<NALU_t>& vNal, int num);

    string string_format(const char* format, ...);
    //NALU信息字符串化
    void naluInfoStr(NALU_t *nalu);
    void VideoInfoStr(videoinfo_t *videoinfo);
    // 解析offset处大小为length的数据，十六进制数据传递到naluData，NAL信息传递到naluInfo
    int parseNALU(NALU_t& vNal, char** naluData, char** naluInfo);

    void getVideoInfo(videoinfo_t* videoInfo)
    {
        if (m_nType) memcpy(videoInfo, m_hH265->info, sizeof(videoinfo_t));
        else memcpy(videoInfo, m_hH264->info, sizeof(videoinfo_t));
        VideoInfoStr(videoInfo);
    }
public:

private:
    inline int findStartcode3(unsigned char *buffer)
    {
        return (buffer[0]==0 && buffer[1]==0 && buffer[2]==1);
    }
    inline int findStartcode4(unsigned char *buffer)
    {
        return (buffer[0]==0 && buffer[1]==0 && buffer[2]==0 && buffer[3]==1);
    }

    int getAnnexbNALU (FILE* fp, NALU_t *nalu);

    int findFirstNALU(FILE* fp, int* startcodeLenght);

    FileType judeVideoFile(const char* filename);

private:
    TREE_NALU AddTreeItem(TREE_NALU _item);
    TREE_NALU AddTreeItem(QTreeWidget *tree);
    void h264_debug_sps(sps_t* sps,TREE_NALU root);
    void h264_debug_pps(pps_t* pps,TREE_NALU root);
    void h264_debug_slice_header(h264_stream_t* h,TREE_NALU root);
    void h264_debug_aud(aud_t* aud,TREE_NALU root);
    void h264_debug_seis( h264_stream_t* h,TREE_NALU root);

    void h264_debug_nal_t(h264_stream_t* h, nal_t* nal);

    void h265_debug_ptl(profile_tier_level_t* ptl, int profilePresentFlag, int max_sub_layers_minus1,TREE_NALU root);
    void h265_debug_sub_layer_hrd_parameters(sub_layer_hrd_parameters_t* subhrd, int sub_pic_hrd_params_present_flag, int CpbCnt, int subLayerId, const char* p,TREE_NALU root);
    void h265_debug_hrd_parameters(hrd_parameters_t* hrd, int commonInfPresentFlag, int maxNumSubLayersMinus1,TREE_NALU root);
    void h265_debug_vps(h265_vps_t* vps,TREE_NALU root);
    void h265_debug_scaling_list(scaling_list_data_t* sld,TREE_NALU root);
    void h265_debug_short_term_ref_pic_set(h265_sps_t* sps, st_ref_pic_set_t*st, referencePictureSets_t* rps, int stRpsIdx,TREE_NALU root);
    void h265_debug_vui_parameters(vui_parameters_t* vui, int maxNumSubLayersMinus1,TREE_NALU root);
    void h265_debug_sps(h265_sps_t* sps,TREE_NALU root);
    void h265_debug_pps(h265_pps_t* pps,TREE_NALU root);
    void h265_debug_aud(h265_aud_t* aud,TREE_NALU root);
    void h265_debug_seis(h265_stream_t* h,TREE_NALU root);
    void h265_debug_ref_pic_lists_modification(h265_slice_header_t* hrd,TREE_NALU root);
    void h265_debug_pred_weight_table(h265_stream_t* h,TREE_NALU root);
    void h265_debug_slice_header(h265_stream_t* h,TREE_NALU root);

    void h265_debug_nal(h265_stream_t* h, h265_nal_t* nal);
    void h265_debug_nal_t(h265_stream_t* h, h265_nal_t* nal);


private:
    // todo：不使用这种写死空间的做法
    //存放解析出来的字符串
    char m_tmpStore[1024];
    char m_outputInfo[OUTPUT_SIZE];
public:
    int m_sliceIndex = 0;
    bool m_fCanPlay=false;
private:
    h264_stream_t* m_hH264;
    h265_stream_t* m_hH265;
    FileType m_nType; // 0:264 1:265
    const char* m_filename;
    uint8_t* m_naluData;
    void* dlg;
    FILE* m_pFile;
public:
    QTreeWidget *m_pTree;
    TREE_NALU  m_root = NULL;
};
#endif
