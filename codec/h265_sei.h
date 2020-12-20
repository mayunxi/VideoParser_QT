#include <stdint.h>

#ifndef _H265_SEI_H
#define _H265_SEI_H        1

#include <stdint.h>

#include "bs.h"
#include "h265_stream.h"

h265_sei_t* h265_sei_new();
void h265_sei_free(h265_sei_t* s);
void h265_read_sei_payload(h265_stream_t* h, bs_t* b, int payloadType, int payloadSize);

//D.1 SEI payload syntax
#define H265_SEI_TYPE_BUFFERING_PERIOD 0
#define H265_SEI_TYPE_PIC_TIMING       1
#define H265_SEI_TYPE_PAN_SCAN_RECT    2
#define H265_SEI_TYPE_FILLER_PAYLOAD   3
#define H265_SEI_TYPE_USER_DATA_REGISTERED_ITU_T_T35  4
#define H265_SEI_TYPE_USER_DATA_UNREGISTERED  5
#define H265_SEI_TYPE_RECOVERY_POINT   6
#define H265_SEI_TYPE_DEC_REF_PIC_MARKING_REPETITION 7
#define H265_SEI_TYPE_SPARE_PIC        8
#define H265_SEI_TYPE_SCENE_INFO       9
#define H265_SEI_TYPE_SUB_SEQ_INFO    10
#define H265_SEI_TYPE_SUB_SEQ_LAYER_CHARACTERISTICS  11
#define H265_SEI_TYPE_SUB_SEQ_CHARACTERISTICS  12
#define H265_SEI_TYPE_FULL_FRAME_FREEZE  13
#define H265_SEI_TYPE_FULL_FRAME_FREEZE_RELEASE  14
#define H265_SEI_TYPE_FULL_FRAME_SNAPSHOT  15
#define H265_SEI_TYPE_PROGRESSIVE_REFINEMENT_SEGMENT_START  16
#define H265_SEI_TYPE_PROGRESSIVE_REFINEMENT_SEGMENT_END  17
#define H265_SEI_TYPE_MOTION_CONSTRAINED_SLICE_GROUP_SET  18
#define H265_SEI_TYPE_FILM_GRAIN_CHARACTERISTICS  19
#define H265_SEI_TYPE_DEBLOCKING_FILTER_DISPLAY_PREFERENCE  20
#define H265_SEI_TYPE_STEREO_VIDEO_INFO  21

#endif
