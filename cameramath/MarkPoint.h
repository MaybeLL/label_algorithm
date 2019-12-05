#pragma once
#include <iostream>
#include <vector>
#include "SkyCommon.h"
#pragma execution_character_set("utf-8")

#define ROW_NUMBER 20      //定义对一张1倍率下图像的分割行数
#define COLUMN_NUMBER 20      //定义对一张1倍率下图像的分割行数
#define RESOLUTION_W 1920
#define RESOLUTION_H 1080
#define RESOLUTION_H20 54
#define RESOLUTION_W20 96

typedef struct  PointPT//用于存放返回标记点的PT
{
    float P;
    float T;
}PointPT;


class MARK_POINTS
{
public:
    MARK_POINTS(void);//构造函数
    ~MARK_POINTS(void);

    void ComputPT(PointPT& ptPoint, const float p, const float t, const float z, const float x_normalization, const float y_normalization,int nType,IPC_MODEL_INFO modelinfo);//x,y以左上角的点为原点,1倍率情况下计算,1倍情况下标点（归一化坐标）

private:
    void ComputTheGrid(float p, float t, float z,int nType,IPC_MODEL_INFO modelinfo);

private:
    float Mark_P0[COLUMN_NUMBER];
    float Mark_T0[ROW_NUMBER];
    float Mark_P[ROW_NUMBER][COLUMN_NUMBER];//用于记录最大倍率与1倍图像进行标记的行关系，其中的值为相对于中心位置的偏移量
    float Mark_T[ROW_NUMBER][COLUMN_NUMBER];//用于记录最大倍率与1倍图像进行标记的列关系

public:
    int p;
    int t;
};
