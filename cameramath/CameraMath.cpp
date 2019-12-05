#include "CameraMath.h"
#include "MarkPoint.h"
#include "TransCoordinate.h"
#include "SkyCommon.h"
#include "tools/Help.h"

#pragma execution_character_set("utf-8")

namespace CAMERA_MATH
{
    float AngleCorrectTo360(float &fAngle)
    {
        if (fAngle >= 360)	fAngle -= 360;
        if (fAngle < 0)	fAngle += 360;

        return fAngle;
    }

    int HikHexToInt(float fHex)
    {
        //小数部分
        float fHexSmall = fHex - (int)fHex;

        //整数部分
        int nHexBig = fHex - fHexSmall;
        //乘以10
        nHexBig *= 10;
        int re=0;   // 保存转换为10进制的结果
        int n=1;    // 位权
        int k=16;   // 16进制
        while(nHexBig != 0)
        {
            re += (nHexBig%10)*n;  // 取出各位位码值，并乘以对应的位权值
            nHexBig /= 10;   // 去掉16进制数的最低位，次低位变为最低位
            n *= k;     // 位权乘以16
        }

        return re + 16 * fHexSmall;
    }

    float HikIntToHex(int nNum)
    {
        //如果只有2位
        if (nNum < 16)
        {
            return nNum / 16.0;
        }

        static const int MAX_HEX_COUNT = 5;
        char pcHex[MAX_HEX_COUNT];
        sprintf(pcHex, "%x", nNum);

        //检查十六进制的最后一位
        for (int i = 0; i < MAX_HEX_COUNT; i++)
        {
            if (pcHex[i] == '\0')
            {
                //最后一位是否为字母位
                IF_NOT (i - 1 > 0)
                {
                    return 0.0f;
                }

                if (pcHex[i - 1] == 'A' ||
                    pcHex[i - 1] == 'B' ||
                    pcHex[i - 1] == 'C' ||
                    pcHex[i - 1] == 'D' ||
                    pcHex[i - 1] == 'E')
                {
                    //将最后一位替换成0, 之后的转换可以直接转换成16进制数的整数位
                    pcHex[i - 1] = '0';
                }
            }
        }

        //由整数部分和小数部分组成
        float k = atoi(pcHex) / 10 + ((nNum % 16) * 1.0) / 16.0 ;

        return k;
    }

    //获取两角度在圆上(0-360度)的差值
    float GetDecInCircle(float fAngle1, float fAngle2)
    {
        static const int HALF_CIRCLE_ANGLE = 180;
        static const int CIRCLE_ANGLE = 360;

        float fDec = fAngle1 - fAngle2;

        float fRet;
        if (fabs(fDec) > HALF_CIRCLE_ANGLE)
        {
            if (fDec > 0)
            {
                fRet = fDec - CIRCLE_ANGLE;
            }
            else
            {
                fRet = fDec + CIRCLE_ANGLE;
            }
        }
        else
        {
            fRet = fDec;
        }

        return fRet;
    }

    void GetPtPoint(STRUCT_PT_POINT& ptPoint, const IPC_PTZPOS& ptz, const QPoint& screen_coord, const int nFrameWidth, const int nFrameHeight,int nType,int zoomMAX)
    {
        STRUCT_PTZ_POINT ptzPoint;     //十六进制
        GetPosPTZ(ptz,nType,ptzPoint,zoomMAX);

        float fDeltaPx = (PA_PARAM - PB_PARAM * ptzPoint.ptzt) * ptzPoint.ptzz;
        float fDeltePy = T_PARAM * ptzPoint.ptzz;

        float ptP = ptzPoint.ptzp - (nFrameWidth / 2 - screen_coord.x()) / fDeltaPx;
        float ptT = ptzPoint.ptzt - (nFrameHeight / 2 - screen_coord.y()) / fDeltePy;

        ptPoint.ptP = CAMERA_MATH::AngleCorrectTo360(ptP);
        ptPoint.ptT = CAMERA_MATH::AngleCorrectTo360(ptT);

        return;
    }

    void GetPtPoint_CS(STRUCT_PT_POINT& ptPoint, const IPC_PTZPOS& ptz, const QPoint& screen_coord, const int nFrameWidth, const int nFrameHeight,int nType,IPC_MODEL_INFO modelinfo)
    {
        static MARK_POINTS mark_points;

        STRUCT_PTZ_POINT ptzPoint;
        GetPosPTZ(ptz,nType,ptzPoint,modelinfo.zoomMax);  // 为什么用十六进制来计算PT

        PointPT result;
        mark_points.ComputPT(result, ptzPoint.ptzp, ptzPoint.ptzt, ptzPoint.ptzz, screen_coord.x() / (nFrameWidth * 1.0f), screen_coord.y() / ((nFrameHeight * 1.0f)),nType,modelinfo);

        ptPoint.ptP = result.P;
        ptPoint.ptT = result.T;

        return;
    }

    bool GetScreenCoord(QPoint& screen_coord, const IPC_PTZPOS& ptz, const STRUCT_PT_POINT& ptPoint, const int nFrameWidth, const int nFrameHeight,int nType,int zoomMAX)
    {
        STRUCT_PTZ_POINT ptzPoint;
        GetPosPTZ(ptz,nType,ptzPoint,zoomMAX);

        float fDeltaPx = (PA_PARAM - PB_PARAM * ptzPoint.ptzt) * ptzPoint.ptzz;
        float fDeltePy = T_PARAM * ptzPoint.ptzz;

        float ptX = nFrameWidth / 2 - CAMERA_MATH::GetDecInCircle(ptzPoint.ptzp, ptPoint.ptP) * fDeltaPx;
        float ptY = nFrameHeight / 2 - CAMERA_MATH::GetDecInCircle(ptzPoint.ptzt, ptPoint.ptT) * fDeltePy;

        if (
                ptX > nFrameWidth ||
                ptX < 0 ||
                ptY > nFrameHeight ||
                ptY < 0
                )
        {
            return false;
        }

        screen_coord.setX(ptX);
        screen_coord.setY(ptY);

        return true;
    }

    bool GetScreenCoord_CS(QPoint& screen_coord, const IPC_PTZPOS& ptz, const STRUCT_PT_POINT& ptPoint, const int nFrameWidth, const int nFrameHeight,int nType,IPC_MODEL_INFO modelinfo)
    {
        STRUCT_PTZ_POINT ptzPoint;
        GetPosPTZ(ptz,nType,ptzPoint,modelinfo.zoomMax);
        qDebug()<<"P:"<<ptzPoint.ptzp<<"  T:"<<ptzPoint.ptzt<<endl;
        //归一化坐标
        TargetPoint Point_Screen_normalization;

        static COORDINATE_TRANS tran_coords;
        if (!tran_coords.ComputCoord(Point_Screen_normalization, ptPoint, ptzPoint.ptzp, ptzPoint.ptzt, ptzPoint.ptzz,nType,modelinfo))  //1080下坐标
        {
            return false;
        }



        screen_coord.setX(Point_Screen_normalization.column_x * nFrameWidth);  //任意分辨率坐标
        screen_coord.setY(Point_Screen_normalization.row_y * nFrameHeight);

        return true;
    }

    float IntToHex(int nNum, int nType)
    {

        switch (nType) {
        case Hikvision:
            return HikIntToHex(nNum);
            break;
        case Dahua:
            return DHIntToHex(nNum);
            break;
        case Tiandy:
            return TiandyIntToHex(nNum);
            break;
        case HuaWei:
            return HWIntToHex(nNum);
            break;
        default:
            break;
        }
    }
    float DHIntToHex(int nNum)
    {
        float renum;
        renum=(nNum*1.0)/10.0;
        return renum;

    }
    float DHZoomIntToHex(int nNum,int zoomMax)
    {
        float renum;
        renum=(nNum*zoomMax*1.0)/128.0;
        if(renum < 1)
        {
            renum = 1;
        }
        return renum;

    }
    int DHHexToInt(float fHex)
    {
        return fHex*=10;
    }

    int HexToInt(float fHex, int nType)
    {
        switch (nType) {
        case Hikvision:
            return HikHexToInt(fHex);
            break;
        case Dahua:
            return DHHexToInt(fHex);
            break;
        case Tiandy:
            return TiandyHexToInt(fHex);
            break;
        case HuaWei:
            return HWHexToInt(fHex);
            break;
        default:
            break;
        }
    }

    int DHZoomHexToInt(float fHex,int zoomMax)
    {
        int renum;
        renum=(fHex*128)/zoomMax;
        return renum;

    }

    int TiandyHexToInt(float fHex)
    {
        return fHex*=100;
    }

    float TiandyIntToHex(int nNum)
    {
        float renum;
        renum=(nNum*1.0)/100.0;
        return renum;
    }
    // 将球机的十进制PTZ状态转换为十六进制的PTZ
    void GetPosPTZ(const IPC_PTZPOS &ptz, int nType, STRUCT_PTZ_POINT& ptzPoint,int zoommax)
    {
        switch (nType) {
        case Hikvision:
            HKGetPTZ(ptz,ptzPoint);
            break;
        case Dahua:
            DHGetPTZ(ptz,ptzPoint,zoommax);
            break;
        case Tiandy:
            TDGetPTZ(ptz,ptzPoint);
            break;
        case HuaWei:
            HWGetPTZ(ptz,ptzPoint);
            break;
        default:
            break;
        }
    }

    void HKGetPTZ(const IPC_PTZPOS &ptz, STRUCT_PTZ_POINT& ptzPoint)
    {
        ptzPoint.ptzp = CAMERA_MATH::IntToHex(ptz.iPanPos,Hikvision);
        ptzPoint.ptzt = CAMERA_MATH::IntToHex(ptz.iTiltPos,Hikvision);
        ptzPoint.ptzz = CAMERA_MATH::IntToHex(ptz.iZoomPos,Hikvision);
    }

    void DHGetPTZ(const IPC_PTZPOS &ptz, STRUCT_PTZ_POINT &ptzPoint,int zoommax)
    {
        ptzPoint.ptzp = CAMERA_MATH::IntToHex(ptz.iPanPos,Dahua);
        ptzPoint.ptzt = CAMERA_MATH::IntToHex(ptz.iTiltPos,Dahua);
        ptzPoint.ptzz = CAMERA_MATH::DHZoomIntToHex(ptz.iZoomPos,zoommax);
    }

    void TDGetPTZ(const IPC_PTZPOS &ptz, STRUCT_PTZ_POINT &ptzPoint)
    {
        int tile = 0;
        tile = ptz.iTiltPos-10000;
        ptzPoint.ptzp = CAMERA_MATH::IntToHex(ptz.iPanPos,Tiandy);
        ptzPoint.ptzt = CAMERA_MATH::IntToHex(tile,Tiandy);
        ptzPoint.ptzz = CAMERA_MATH::IntToHex(ptz.iZoomPos,Tiandy);

    }

    int HWHexToInt(float fHex)
    {
        return fHex;
    }

    float HWIntToHex(int nNum)
    {
        return nNum;
    }

    void HWGetPTZ(const IPC_PTZPOS &ptz, STRUCT_PTZ_POINT& ptzPoint)
    {
        ptzPoint.ptzp = CAMERA_MATH::IntToHex(ptz.iPanPos,HuaWei);
        ptzPoint.ptzt = CAMERA_MATH::IntToHex(ptz.iTiltPos,HuaWei);
        ptzPoint.ptzz = CAMERA_MATH::IntToHex(ptz.iZoomPos,HuaWei);
    }

    IPC_MODEL_INFO getIpcModelInfo(const QString &modelType, int nType)
    {
        IPC_MODEL_INFO modelinfo;
        bool isModel = false;
        modelinfo.ipcmodel = modelType;
        IPC_MODEL_INFO ipcinfo = {0};
        int num = Help::g_dhModelList.size();
        for(int i=0;i<num;i++)
        {
            ipcinfo = Help::g_dhModelList.value(i);
            //qDebug()<<ipcinfo.ipcmodel;
            if(modelType == ipcinfo.ipcmodel)
            {
                //qDebug()<<"ipc model:"<<ipcinfo.ipcmodel;
               // m_modelinfo.ipcmodel = ipcinfo.ipcmodel;
                modelinfo.zoomMax = ipcinfo.zoomMax;
                modelinfo.paParam = ipcinfo.paParam;
                modelinfo.pbParam = ipcinfo.pbParam;
                modelinfo.tParam = ipcinfo.tParam;
                modelinfo.rdata = ipcinfo.rdata;
                modelinfo.sceneParam = ipcinfo.sceneParam;
                isModel = true;
                break;
            }
        }
        if(!isModel)
        {
            switch (nType) {
            case Hikvision:
                modelinfo.zoomMax = 20;
                modelinfo.paParam = PA_PARAM;
                modelinfo.pbParam = PB_PARAM;
                modelinfo.tParam = T_PARAM;
                modelinfo.rdata = RHik;
                modelinfo.sceneParam = 1.25;
                break;
            case Dahua:
                modelinfo.zoomMax = 20;
                modelinfo.paParam = PA_PARAM;
                modelinfo.pbParam = PB_PARAM;
                modelinfo.tParam = T_PARAM;
                modelinfo.rdata = RDaHua;
                modelinfo.sceneParam = 1;
                break;
            case Tiandy:
                modelinfo.zoomMax = 20;
                modelinfo.paParam = PA_PARAM;
                modelinfo.pbParam = PB_PARAM;
                modelinfo.tParam = T_PARAM;
                modelinfo.rdata = RTiandy;
                modelinfo.sceneParam = 1;
                break;
            case HuaWei:
                modelinfo.zoomMax = 20;
                modelinfo.paParam = PA_PARAMHW;
                modelinfo.pbParam = PB_PARAMHW;
                modelinfo.tParam = T_PARAMHW;
                modelinfo.rdata = RHuaWei;
                modelinfo.sceneParam = 1;
                break;
            default:
                modelinfo.zoomMax = 20;
                modelinfo.paParam = PA_PARAM;
                modelinfo.pbParam = PB_PARAM;
                modelinfo.tParam = T_PARAM;
                modelinfo.rdata = RHik;
                modelinfo.sceneParam = 1;
                break;
            }


        }

        return modelinfo;
    }
};
