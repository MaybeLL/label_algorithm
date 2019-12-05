#ifndef __CAMARA_MATH_H__
#define __CAMARA_MATH_H__
#pragma execution_character_set("utf-8")

#include "SkyCommon.h"
//#include "HCNetSDK.h"
//#include "PlayM4.h"

//135算法参数
//#define PA_PARAM 22.7809
//#define PB_PARAM 0.1790
//#define T_PARAM 20.5714

//138算法参数
#define PA_PARAM 36
#define PB_PARAM 0.183
#define T_PARAM 25

namespace CAMERA_MATH{
    float AngleCorrectTo360(float &fAngle);
    int HikHexToInt(float fHex);
    float HikIntToHex(int nNum);
    float GetDecInCircle(float fAngle1, float fAngle2);  //获取两角度在圆上(0-360度)的差值
    void GetPtPoint(STRUCT_PT_POINT& ptPoint, const NET_DVR_PTZPOS& ptz, const QPoint& screen_coord, const int nFrameWidth, const int nFrameHeight);
    bool GetScreenCoord(QPoint& screen_coord, const NET_DVR_PTZPOS& ptz, const STRUCT_PT_POINT& ptPoint, const int nFrameWidth, const int nFrameHeight);
    void GetPtPoint_CS(STRUCT_PT_POINT& ptPoint, const NET_DVR_PTZPOS& ptz, const QPoint& screen_coord, const int nFrameWidth, const int nFrameHeight);
    bool GetScreenCoord_CS(QPoint& screen_coord, const NET_DVR_PTZPOS& ptz, const STRUCT_PT_POINT& ptPoint, const int nFrameWidth, const int nFrameHeight);
};

#endif //__CAMARA_MATH_H__
