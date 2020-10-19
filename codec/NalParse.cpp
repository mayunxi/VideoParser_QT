
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <memory.h>
#include<stdarg.h>
#include "codec/NalParse.h"

NalParser::NalParser()
{
    m_nType = FILE_H264; // default
    m_filename = NULL;
    m_hH264 = NULL;
    m_hH265 = NULL;
    m_naluData = NULL;
    dlg = NULL;
    m_pFile = NULL;

    memset(m_tmpStore, '\0', 1024);
    memset(m_outputInfo, '\0', OUTPUT_SIZE);
}

NalParser::~NalParser()
{
    release();
}

int NalParser::init(const char* filename)
{
    m_sliceIndex =0;//
    m_filename = filename;

    // judge file 判断文件类型
    m_nType = judeVideoFile(m_filename);

    if (m_nType == FILE_UNK)
    {
        return -1;
    }
    // init handle
    else if (m_nType == FILE_H265)
    {
        if (m_hH265 != NULL)
        {
            h265_free(m_hH265);
        }
        m_hH265 = h265_new();
    }
    else
    {
        if (m_hH264 != NULL)
        {
            h264_free(m_hH264);
        }
        m_hH264 = h264_new();
    }
    


    if (m_pFile != NULL)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }

    m_pFile = fopen(m_filename, "r+b");

    return 0;
}

int NalParser::release(void)
{
    if (m_hH264 != NULL)
    {
        h264_free(m_hH264);
        m_hH264 = NULL;
    }
    if (m_hH265 != NULL)
    {
        h265_free(m_hH265);
        m_hH265 = NULL;
    }
    if (m_pFile != NULL)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }

    return 0;
}

int NalParser::probeNALU(vector<NALU_t>& vNal, int num)
{
    NALU_t n;
    int nal_num=0;
    int offset=0;
    int nalLen = 0;

    memset(&n, '\0', sizeof(NALU_t));

    n.codecType = m_nType;

    offset = findFirstNALU(m_pFile, &(n.startcodeLen));

    if (offset < 0)
    {
        return -1;
    }
    fseek(m_pFile, offset, SEEK_SET);
    while (!feof(m_pFile))
    {
        if (num > 0 && nal_num == num)
        {
            break;
        }
        nalLen = getAnnexbNALU(m_pFile, &n);//每执行一次，文件的指针指向本次找到的NALU的末尾，下一个位置即为下个NALU的起始码0x000001
        n.offset = offset;
        n.num = nal_num;
        offset = offset + nalLen;

        vNal.push_back(n);

        nal_num++;
    }
    return 0;
}



std::string NalParser::string_format(const char* format, ...)
{

#if 1 // 最大长度限制：1024 - 1
    va_list args;
    va_start(args, format);
    char buf[4096] = {0};
    vsprintf(buf, format, args);
    va_end(args);
    std::string str(buf);
    return str;
#else // 无长度限制
    va_list args;
    va_start(args, format);
    int count = vsnprintf(NULL, 0, format, args); // 使用vsnprintf：warning C4996; 使用vsnprintf_s：无法自动计算长度
    va_end(args);
    va_start(args, format);
    char* buff = (char*)malloc(count * sizeof(wchar_t));
    vsnprintf(buff, count, format, args);
    va_end(args);
    std::string str(buff, count);
    free(buff);
    return str;
#endif

}

void NalParser::naluInfoStr(NALU_t *nalu)
{
   string &strOffset = nalu->strOffset;
   string &strNalLen = nalu->strNalLen;
   string &strStartCode = nalu->strStartCode;

   string &strNalUnitType = nalu->strNalUnitType;
   string &strNalInfo = nalu->strNalInfo;
   //RGB table:molandi
   nalu->backgroundColor="#f0ebe5";
   nalu->textColor="black";
   if (nalu->codecType == 0)
   {
       // NAL单元类型
       switch (nalu->nalType)
       {
       case 0:
           strNalUnitType="Unspecified";
           break;
       case 1:
           strNalUnitType="Coded slice of a non-IDR picture";
           switch (nalu->sliceType)
           {
           case 0:
           case 5:
               nalu->textColor="blue";
               strNalInfo=string_format("P Slice #%d", m_sliceIndex);
               break;
           case 1:
           case 6:
               nalu->textColor="green";
               strNalInfo=string_format("B Slice #%d", m_sliceIndex);
               break;
           case 2:
           case 7:
               nalu->textColor="red";
               strNalInfo=string_format("I Slice #%d", m_sliceIndex);
               break;
           }
           m_sliceIndex++;
           break;
       case 2:
           nalu->backgroundColor="#b5c4b1";
           strNalUnitType="DPA";
           break;
       case 3:
           nalu->backgroundColor="#96a48b";
           strNalUnitType="DPB";
           break;
       case 4:
           nalu->backgroundColor="#7b8b6f";
           strNalUnitType="DPC";
           break;
       case 5:
           nalu->backgroundColor="#965454";
           strNalUnitType=string_format("Coded slice of an IDR picture");
           strNalInfo=string_format("IDR #%d", m_sliceIndex);
           m_sliceIndex++;
           break;
       case 6:
           nalu->backgroundColor="#9ca8b8";
           strNalUnitType="Supplemental enhancement information";
           strNalInfo="SEI";
           break;
       case 7:
           nalu->backgroundColor="#FFA54F";
           strNalUnitType="Sequence parameter set";
           strNalInfo="SPS";
           break;
       case 8:
           nalu->backgroundColor="#7b8b6f";
           strNalUnitType="Picture parameter set";
           strNalInfo="PPS";
           break;
       case 9:
           nalu->backgroundColor="#656565";
           strNalUnitType="Access UD";
           strNalInfo="AUD";
           break;
       case 10:
           nalu->backgroundColor="#7a7281";
           strNalUnitType="END_SEQUENCE";
           break;
       case 11:
           nalu->backgroundColor="#6b5152";
           strNalUnitType="END_STREAM";
           break;
       case 12:
           strNalUnitType="FILLER_DATA";
           break;
       case 13:
           strNalUnitType="SPS_EXT";
           break;
       case 19:
           strNalUnitType="AUXILIARY_SLICE";
           break;
       default:
           strNalUnitType="Other";
           break;
       }
   }
   else  //h265
   {
       // NAL单元类型
       switch (nalu->nalType)
       {
       // to confirm type...
       case NAL_UNIT_CODED_SLICE_TRAIL_N:
       case NAL_UNIT_CODED_SLICE_TRAIL_R:
           strNalUnitType=string_format("Coded slice segment of a non-TSA, non-STSA trailing picture");
           switch (nalu->sliceType)
           {
           case H265_SH_SLICE_TYPE_B:
               strNalInfo=string_format("B Slice #%d", m_sliceIndex);
               break;
           case H265_SH_SLICE_TYPE_P:
               strNalInfo=string_format("P Slice #%d", m_sliceIndex);
               break;
           case H265_SH_SLICE_TYPE_I:
               strNalInfo=string_format("I Slice #%d", m_sliceIndex);
               break;
           }
           m_sliceIndex++;
           break;
       case NAL_UNIT_CODED_SLICE_TSA_N:
       case NAL_UNIT_CODED_SLICE_TSA_R:
           strNalUnitType=string_format("Coded slice segment of a TSA picture");
           switch (nalu->sliceType)
           {
           case H265_SH_SLICE_TYPE_B:
               strNalInfo=string_format("B Slice #%d", m_sliceIndex);
               break;
           case H265_SH_SLICE_TYPE_P:
               strNalInfo=string_format("P Slice #%d", m_sliceIndex);
               break;
           case H265_SH_SLICE_TYPE_I:
               strNalInfo=string_format("I Slice #%d", m_sliceIndex);
               break;
           }
           m_sliceIndex++;
           break;
       case NAL_UNIT_CODED_SLICE_RADL_N:
       case NAL_UNIT_CODED_SLICE_RADL_R:
           strNalUnitType=string_format("Coded slice segment of a TSA picture");
           switch (nalu->sliceType)
           {
           case H265_SH_SLICE_TYPE_B:
               strNalInfo=string_format("B Slice #%d", m_sliceIndex);
               break;
           case H265_SH_SLICE_TYPE_P:
               strNalInfo=string_format("P Slice #%d", m_sliceIndex);
               break;
           case H265_SH_SLICE_TYPE_I:
               strNalInfo=string_format("I Slice #%d", m_sliceIndex);
               break;
           }
           m_sliceIndex++;
           break;
       case NAL_UNIT_CODED_SLICE_IDR_W_RADL:
       case NAL_UNIT_CODED_SLICE_IDR_N_LP:
           strNalUnitType=string_format("Coded slice of an IDR picture");
           strNalInfo=string_format("IDR #%d", m_sliceIndex);
           m_sliceIndex++;
           break;
       case NAL_UNIT_CODED_SLICE_CRA:
           strNalUnitType=string_format("Coded slice segment of a CRA picture");
           strNalInfo=string_format("CRA #%d", m_sliceIndex);
           m_sliceIndex++;
           break;
       case NAL_UNIT_PREFIX_SEI:
       case NAL_UNIT_SUFFIX_SEI:
           strNalUnitType=string_format("Supplemental enhancement information");
           strNalInfo=string_format("SEI");
           break;
       case NAL_UNIT_VPS:
           strNalUnitType=string_format("Video parameter set");
           strNalInfo=string_format("VPS");
           break;
       case NAL_UNIT_SPS:
           strNalUnitType=string_format("Sequence parameter set");
           strNalInfo=string_format("SPS");
           break;
       case NAL_UNIT_PPS:
           strNalUnitType=string_format("Picture parameter set");
           strNalInfo=string_format("PPS");
           break;
       case NAL_UNIT_AUD:
           strNalUnitType=string_format("Access UD");
           strNalInfo=string_format("AUD");
           break;
       case NAL_UNIT_EOS:
           strNalUnitType=string_format("END_SEQUENCE");
           break;
       case NAL_UNIT_EOB:
           strNalUnitType=string_format("END_STREAM");
           break;
       case NAL_UNIT_FILLER_DATA:
           strNalUnitType=string_format("FILLER_DATA");
           break;
       default:
           strNalUnitType=string_format("Unknown");
           break;
       }
   }

   // 数据偏移
   strOffset=string_format("%08X", nalu->offset);
   // 长度
   strNalLen=string_format("%d",nalu->len);
   // 起始码
   strStartCode=string_format("%s", nalu->startcodeBuffer);

}

void NalParser::VideoInfoStr(videoinfo_t *videoInfo)
{

    string &strSimpleInfo = videoInfo->strSimpleInfo;
    string &strProfileInfo = videoInfo->strProfileInfo;
    string &strLevelInfo = videoInfo->strLevelInfo;
    string &strTierInfo= videoInfo->strTierInfo;
    string &strVideoFormat = videoInfo->strVideoFormat;
    string &strBitDepth = videoInfo->strBitDepth;
    // H.265
    if (videoInfo->type)
    {
        // profile类型
        switch (videoInfo->profile_idc)
        {
        case PROFILE_NONE:
            strProfileInfo=string_format("None");
            break;
        case PROFILE_MAIN:
            strProfileInfo=string_format("Main");
            break;
        case PROFILE_MAIN10:
            strProfileInfo=string_format("Main10");
            break;
        case PROFILE_MAINSTILLPICTURE:
            strProfileInfo=string_format("Main Still Picture");
            break;
        case PROFILE_MAINREXT:
            strProfileInfo=string_format("Main RExt");
            break;
        case PROFILE_HIGHTHROUGHPUTREXT:
            strProfileInfo=string_format("High Throughput RExt");
            break;
        default:
            strProfileInfo=string_format("Unkown");
            break;
        }
        switch (videoInfo->level_idc)
        {
        case LEVEL_NONE:
            strLevelInfo=string_format(("none(%d)"), LEVEL_NONE);
            break;
        case LEVEL1:
            strLevelInfo=string_format(("1(%d)"), LEVEL1);
            break;
        case LEVEL2:
            strLevelInfo=string_format(("2(%d)"), LEVEL2);
            break;
        case LEVEL2_1:
            strLevelInfo=string_format(("2.1(%d)"), LEVEL2_1);
            break;
        case LEVEL3:
            strLevelInfo=string_format(("3(%d)"), LEVEL3);
            break;
        case LEVEL3_1:
            strLevelInfo=string_format(("3.1(%d)"), LEVEL3_1);
            break;
        case LEVEL4:
            strLevelInfo=string_format(("4(%d)"), LEVEL4);
            break;
        case LEVEL4_1:
            strLevelInfo=string_format(("4.1(%d)"), LEVEL4_1);
            break;
        case LEVEL5:
            strLevelInfo=string_format(("5(%d)"), LEVEL5);
            break;
        case LEVEL5_1:
            strLevelInfo=string_format(("5.1(%d)"), LEVEL5_1);
            break;
        case LEVEL5_2:
            strLevelInfo=string_format(("5.2(%d)"), LEVEL5_2);
            break;
        case LEVEL6:
            strLevelInfo=string_format(("6(%d)"), LEVEL6);
            break;
        case LEVEL6_1:
            strLevelInfo=string_format(("6.1(%d)"), LEVEL6_1);
            break;
        case LEVEL6_2:
            strLevelInfo=string_format(("6.2(%d)"), LEVEL6_2);
            break;
        case LEVEL8_5:
            strLevelInfo=string_format(("8.5(%d)"), LEVEL8_5);
            break;
        default:
            strLevelInfo=string_format("Unkown");
            break;
        }
        switch (videoInfo->tier_idc)
        {
        case 1:
            strTierInfo=string_format("Tier High");
            break;
        case 0:
        default:
            strTierInfo=string_format("Tier Main");
            break;
        }
    }
    else // h264
    {
        // profile类型
        switch (videoInfo->profile_idc)
        {
        case 66:
            strProfileInfo=string_format("Baseline");
            break;
        case 77:
            strProfileInfo=string_format("Main");
            break;
        case 88:
            strProfileInfo=string_format("Extended");
            break;
        case 100:
            strProfileInfo=string_format("High");
            break;
        case 110:
            strProfileInfo=string_format("High 10");
            break;
        case 122:
            strProfileInfo=string_format("High 422");
            break;
        case 144:
            strProfileInfo=string_format("High 444");
            break;
        default:
            strProfileInfo=string_format("Unkown");
            break;
        }
        strTierInfo=string_format("");
        strLevelInfo=string_format(("%d"), videoInfo->level_idc);
    }
    // common
    // bit depth
    strBitDepth=string_format(("Luma bit: %d Chroma bit: %d"), videoInfo->bit_depth_luma, videoInfo->bit_depth_chroma);

    // chroma format
    switch (videoInfo->chroma_format_idc)
    {
    case 1:
        strVideoFormat=string_format("YUV420");
        break;
    case 2:
        strVideoFormat=string_format("YUV422");
        break;
    case 3:
        strVideoFormat=string_format("YUV444");
        break;
    case 0:
        strVideoFormat=string_format("monochrome");
        break;
    default:
        strVideoFormat=string_format("Unkown");
        break;
    }

    //"Video Format: xxx\r\n"

    strSimpleInfo=string_format(
        "%s File Information\r\n\r\n"
        "Picture Size \t: %dx%d\r\n"
        "  - Cropping Left \t: %d\r\n"
        "  - Cropping Right \t: %d\r\n"
        "  - Cropping Top \t: %d\r\n"
        "  - Cropping Bottom : %d\r\n"
        "Video Format \t: %s %s\r\n"
        "Stream Type \t: %s Profile @ Level %s %s\r\n"
        "Encoding Type \t: %s\r\n"
        "Max fps \t\t: %.03f\r\n"
        "Frame Count \t: %d\r\n",
        videoInfo->type ? "H.265/HEVC" : "H.264/AVC",
        videoInfo->width, videoInfo->height,
        videoInfo->crop_left, videoInfo->crop_right,
        videoInfo->crop_top, videoInfo->crop_bottom,
        strVideoFormat.c_str(), strBitDepth.c_str(),
        strProfileInfo.c_str(), strLevelInfo.c_str(), strTierInfo.c_str(),
        videoInfo->encoding_type ? "CABAC" : "CAVLC",
        videoInfo->max_framerate, m_sliceIndex
        );
    //GetDlgItem(IDC_EDIT_SIMINFO)->SetWindowText(strSimpleInfo);
    m_fCanPlay = true;


}

int NalParser::parseNALU(NALU_t& vNal, char** naluData, char** naluInfo)
{
    if (m_naluData != NULL)
    {
        free(m_naluData);
        m_naluData = NULL;
    }
    m_naluData = (uint8_t *)malloc(vNal.len);

    int ret = fseek(m_pFile, vNal.offset, SEEK_SET);  //SEEK_SET=file head
    if (ret != 0)
    {
        perror("fseek Error: ");
        return -1;
    }

    fread(m_naluData, vNal.len, 1, m_pFile);

    // 不需要再次查询nal
    //find_nal_unit(m_naluData, vNal.len, &nal_start, &nal_end);
    memset(m_outputInfo, '\0', OUTPUT_SIZE); // 输出内容先清空
    if (m_nType == 1)
    {
        // 此函数分析时，是不包含startcode的，所以要减去startcodeLen
        h265_read_nal_unit(m_hH265, &m_naluData[vNal.startcodeLen], vNal.len - vNal.startcodeLen);
        //h265_debug_nal_t(m_hH265,m_hH265->nal);    // 打印到m_outputInfo中,show in the tree
    }
    else
    {
        read_nal_unit(m_hH264, &m_naluData[vNal.startcodeLen], vNal.len - vNal.startcodeLen);
        //h264_debug_nal_t(m_hH264, m_hH264->nal);  // 打印到m_outputInfo中
    }

    *naluData = (char*)m_naluData;
    *naluInfo = m_outputInfo;

    return 0;
}

static int ue(char *buff, int len, int &start_bit)
{
    int zero_num = 0;
    int ret = 0;

    while (start_bit < len * 8)
    {
        if (buff[start_bit / 8] & (0x80 >> (start_bit % 8)))
        {
            break;
        }
        zero_num++;
        start_bit++;
    }
    start_bit++;

    for (int i=0; i<zero_num; i++)
    {
        ret <<= 1;
        if (buff[start_bit / 8] & (0x80 >> (start_bit % 8)))
        {
            ret += 1;
        }
        start_bit++;
    }
    return (1 << zero_num) - 1 + ret;
}

/**
解析NAL，返回两个开始字符之间间隔的字节数，即包含startcode的NALU的长度

note：一个视频文件中不同的NAL，startcode可能不一样。比如SPS为4字节，但SEI可能为3字节
todo:每次读一个字节，较慢，有无好的方法？
*/

int NalParser::getAnnexbNALU(FILE* fp, NALU_t* nalu)
{
    int pos = 0;
    int found, rewind;
    unsigned char *buffer;
    int info2=0, info3=0;
    int eof = 0;

    if ((buffer = (unsigned char*)calloc (MAX_NAL_SIZE, sizeof(char))) == NULL)
        printf("Could not allocate buffer memory\n");

    if (3 != fread (buffer, 1, 3, fp))//从码流中读3个字节
    {
        free(buffer);
        return 0;
    }
    info2 = findStartcode3(buffer);//判断是否为0x000001
    if(info2 != 1)
    {
        //如果不是，再读一个字节
        if(1 != fread(buffer+3, 1, 1, fp))//读一个字节
        {
            free(buffer);
            return 0;
        }
        info3 = findStartcode4(buffer);//判断是否为0x00000001
        if (info3 != 1)//如果不是，返回-1
        {
            free(buffer);
            return -1;
        }
        else
        {
            //如果是0x00000001,得到开始前缀为4个字节
            nalu->startcodeLen = 4;
        }
    }
    else
    {
        //如果是0x000001,得到开始前缀为3个字节
        nalu->startcodeLen = 3;
    }

    pos = nalu->startcodeLen;
    //查找下一个开始字符的标志位
    found = 0;
    info2 = 0;
    info3 = 0;

    while (!found)
    {
        if (feof(fp))//判断是否到了文件尾
        {
            eof = 1;
            goto got_nal;
        }
        buffer[pos++] = fgetc(fp);//读一个字节到BUF中

        info3 = findStartcode4(&buffer[pos-4]);//判断是否为0x00000001
        if(info3 != 1)
            info2 = findStartcode3(&buffer[pos-3]);//判断是否为0x000001

        found = (info2 == 1 || info3 == 1);
    }

    // startcode可能为3，也可能为4，故要如此判断
    rewind = (info3 == 1)? -4 : -3;

    if (0 != fseek (fp, rewind, SEEK_CUR))//把文件指针指向前一个NALU的末尾
    {
        free(buffer);
        printf("Cannot fseek in the bit stream file");
    }

got_nal:
    // 当达到文件末尾时，回退1个位置
    if (eof)
    {
        rewind = -1;
    }

    // 包括起始码在内的5个字节
    if (nalu->startcodeLen == 3)
        sprintf(nalu->startcodeBuffer, "%02x%02x%02x%02x", buffer[0], buffer[1], buffer[2], buffer[3]);
    else
        sprintf(nalu->startcodeBuffer, "%02x%02x%02x%02x%02x", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
    nalu->len = pos+rewind;

    uint8_t nal_header = 0;
    if (nalu->codecType)
    {
        m_hH265->sh->read_slice_type = 1;
        h265_read_nal_unit(m_hH265, &buffer[nalu->startcodeLen], nalu->len - nalu->startcodeLen);
        nalu->nalType = m_hH265->nal->nal_unit_type;
        nalu->sliceType = m_hH265->sh->slice_type;
        m_hH265->sh->read_slice_type = 0;
    }
    else
    {
    // simple version
#if 0
        nal_header = buffer[nalu->startcodeLen];
        nalu->nalType = nal_header & 0x1f;// 5 bit

        // 获取slice类型：I帧、P帧、B帧
        // 注：在nal类型为1~5时获取
        if (nalu->nalType <= 5 && nalu->nalType >= 1)
        {
            int start_bit = 0;
            int first_mb_in_slice = ue((char*)buffer+nalu->startcodeLen+1, 8, start_bit);
            nalu->sliceType = ue((char*)buffer+nalu->startcodeLen+1, 8, start_bit);
        }
        if (nalu->nalType == 7 || nalu->nalType == 8) // sps pps
        {
            read_nal_unit(m_hH264, &buffer[nalu->startcodeLen], nalu->len - nalu->startcodeLen);
        }
#else
        m_hH264->sh->read_slice_type = 1;
        read_nal_unit(m_hH264, &buffer[nalu->startcodeLen], nalu->len - nalu->startcodeLen);
        nalu->nalType = m_hH264->nal->nal_unit_type;
        nalu->sliceType = m_hH264->sh->slice_type;
        m_hH264->sh->read_slice_type = 0;
#endif
    }

    free(buffer);

    return (pos+rewind);//返回两个开始字符之间间隔的字节数，即包含有前缀的NALU的长度
}

int NalParser::findFirstNALU(FILE* fp, int* startcodeLenght)
{
    int found = 0;
    int info2 = 0;
    int info3 = 0;
    int eof = 0;
    int pos = 0;
    int startcode_len = 0;
    unsigned char *buffer = NULL;

    if ((buffer = (unsigned char*)calloc(MAX_NAL_SIZE, sizeof(char))) == NULL)
        printf ("Could not allocate buffer memory\n");

    while (!found && !feof(fp))
    {
        buffer[pos++] = fgetc(fp);//读一个字节到BUF中

        info3 = findStartcode4(&buffer[pos-4]);//判断是否为0x00000001
        if(info3 != 1)
        {
            info2 = findStartcode3(&buffer[pos-3]);//判断是否为0x000001
            if (info2)
            {
                startcode_len = 3;
            }
        }
        else
        {
            startcode_len = 4;
        }

        found = (info2 == 1 || info3 == 1);
        if (pos >= MAX_NAL_SIZE)
        {
            free(buffer);
            return -1;
        }
    }

    // 文件指针要恢复
    fseek(fp, -startcode_len, SEEK_CUR);

    free(buffer);
    if (startcodeLenght != NULL)
        *startcodeLenght = startcode_len;

    return pos - startcode_len;
}
bool endWith(const string &str, const string &tail) {
    return str.compare(str.size() - tail.size(), tail.size(), tail) == 0;
}
FileType NalParser::judeVideoFile(const char* filename)
{
    char szExt[16] = {0};
    FileType codecType = FILE_H264; // default

    string file = filename;
    if (endWith(file,".h265"))
    {
        codecType = FILE_H265;
    }
    else if (endWith(file,".h264"))
    {
        codecType = FILE_H264;
    }
    else
    {
        // read content 
        FILE* fp = NULL;
        int offset = 0;
        int startcode = 0;
        unsigned char nalHader = 0;
        unsigned char nalType = 0;

        fp = fopen(filename, "r+b");
        offset = findFirstNALU(fp, &startcode);
        if (offset < 0)
        {
            return FILE_UNK;
        }
        fseek(fp, offset+startcode, SEEK_SET);
        fread((void*)&nalHader,1,1,fp);
        // check h264 first...
        nalType = nalHader & 0x1f; // 5 bit
        if (nalType > 0 && nalType < 22) // ok
        {
            codecType = FILE_H264;
        }
        else
        {
            // not h264, then check h265...
            nalType = (nalHader>>1) & 0x3f; // 6 bit
            if (nalType >= 0 && nalType <= 47) // ok
            {
                codecType = FILE_H265;
            }
        }
        fclose(fp);
    }

    return codecType;
}
