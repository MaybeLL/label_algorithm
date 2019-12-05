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

const double RHik(1960.145);
const double RDaHua(1960.145);
const double RTiandy(2000.145);
const double RHuaWei(1554.145);

//138算法参数
#define PA_PARAM 36
#define PB_PARAM 0.183
#define T_PARAM 25

#define PA_PARAMHW 30
#define PB_PARAMHW 0.173
#define T_PARAMHW 27

namespace CAMERA_MATH{
    float AngleCorrectTo360(float &fAngle);

    float GetDecInCircle(float fAngle1, float fAngle2);  //获取两角度在圆上(0-360度)的差值
    void GetPtPoint(STRUCT_PT_POINT& ptPoint, const IPC_PTZPOS& ptz, const QPoint& screen_coord, const int nFrameWidth, const int nFrameHeight,int nType,int zoomMAX);
    bool GetScreenCoord(QPoint& screen_coord, const IPC_PTZPOS& ptz, const STRUCT_PT_POINT& ptPoint, const int nFrameWidth, const int nFrameHeight,int nType,int zoomMAX);
    void GetPtPoint_CS(STRUCT_PT_POINT& ptPoint, const IPC_PTZPOS& ptz, const QPoint& screen_coord, const int nFrameWidth, const int nFrameHeight,int nType,IPC_MODEL_INFO modelinfo);
    bool GetScreenCoord_CS(QPoint& screen_coord, const IPC_PTZPOS& ptz, const STRUCT_PT_POINT& ptPoint, const int nFrameWidth, const int nFrameHeight,int nType,IPC_MODEL_INFO modelinfo);

    //厂商参数转实际应用值
    int HikHexToInt(float fHex);
    int DHHexToInt(float fHex);
    int TiandyHexToInt(float fHex);
    int DHZoomHexToInt(float fHex,int zoomMax);
    int HWHexToInt(float fHex);
    int HexToInt(float fHex,int nType = 0);
    //实际应用值转厂商参数
    float HikIntToHex(int nNum);
    float DHIntToHex(int nNum);
    float TiandyIntToHex(int nNum);
    float HWIntToHex(int nNum);
    float DHZoomIntToHex(int nNum,int zoomMax);
    float IntToHex(int nNum, int nType = 0);
    //PTZ调整
    void GetPosPTZ(const IPC_PTZPOS& ptz,int nType,STRUCT_PTZ_POINT& ptzPoint,int zoommax);
    void HKGetPTZ(const IPC_PTZPOS& ptz,STRUCT_PTZ_POINT& ptPoint);
    void DHGetPTZ(const IPC_PTZPOS& ptz,STRUCT_PTZ_POINT& ptPoint,int zoommax);
    void TDGetPTZ(const IPC_PTZPOS& ptz,STRUCT_PTZ_POINT& ptPoint);
    void HWGetPTZ(const IPC_PTZPOS &ptz, STRUCT_PTZ_POINT& ptPoint);
    IPC_MODEL_INFO getIpcModelInfo(const QString &modelType,int nType);

};

#endif //__CAMARA_MATH_H__
