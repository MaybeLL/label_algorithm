#pragma once
#include "SkyCommon.h"
#pragma execution_character_set("utf-8")

#define ROW_NUMBER 20      //定义对一张1倍率下图像的分割行数
#define COLUMN_NUMBER 20      //定义对一张1倍率下图像的分割行数
#define RESOLUTION_W 1920
#define RESOLUTION_H 1080
#define RESOLUTION_H20 54
#define RESOLUTION_W20 96

typedef struct  TargetPoint//用于返回计算结果在当前1080P图像中的位置。
{
    int PointID;//距离该标记点的ID
    float P;//该标记点的P与T的信息，，观察点的Z=1.
    float T;
    float column_x;
    float row_y;//该标记点的P与T的信息，，观察点的Z=1.
}TargetPoint;

class COORDINATE_TRANS
{
public:
    COORDINATE_TRANS(void);
    ~COORDINATE_TRANS(void);
    bool ComputCoord(TargetPoint& Point_Screen, const STRUCT_PT_POINT& ptPoint, const float p, const float t, const float z,int nType,IPC_MODEL_INFO modelinfo);//计算坐标返回1080P条件下坐标的值
    void ComputCoordinates(std::vector <TargetPoint>& Point_Screen, const std::vector <STRUCT_PT_POINT>& Point, const float p, const float t, const float z,IPC_MODEL_INFO modelinfo);//计算坐标返回1080P条件下坐标的值

private:
    void ComputTheGrid(float p,float t,float z,int nType,IPC_MODEL_INFO modelinfo);

private:
    float Mark_P0[COLUMN_NUMBER];
    float Mark_T0[ROW_NUMBER];
    float Mark_P[ROW_NUMBER][COLUMN_NUMBER];//用于记录最大倍率与1倍图像进行标记的行关系，其中的值为相对于中心位置的偏移量
    float Mark_T[ROW_NUMBER][COLUMN_NUMBER];//用于记录最大倍率与1倍图像进行标记的列关系
};

