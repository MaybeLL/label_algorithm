#include <iostream>
#include <cmath>
#include <vector>
#include "MarkPoint.h"
#include "CameraMath.h"

#pragma execution_character_set("utf-8")

#define hikmodel "DS-2DC7423IW-A"

//const double RHik(1960.145);
//const double RTiandy(2000.145);
//const double RHuaWei(1554.145);
const double pi(3.14159265);

MARK_POINTS::MARK_POINTS(void)
{
    //角度偏移爲0的P標定col
    Mark_P0[0]=-24.75;                Mark_P0[1]=-22.0625;          Mark_P0[2]=-19.9375;           Mark_P0[3]=-16.9375;           Mark_P0[4]=-14.9375;
    Mark_P0[5]=-11.9375;           Mark_P0[6]=-9.8125;            Mark_P0[7]=-6.9375;               Mark_P0[8]=-4;                       Mark_P0[9]=-1.75;
    Mark_P0[10]=1.75;                 Mark_P0[11]=4;                    Mark_P0[12]=6.9375;              Mark_P0[13]=9.8125;            Mark_P0[14]=11.9375;
    Mark_P0[15]=14.8125;           Mark_P0[16]=16.9375;        Mark_P0[17]=19.9375;           Mark_P0[18]=22.0625;          Mark_P0[19]=24.75;
    //角度偏移爲0的時候T的標定row
    Mark_T0[0]=-19.0625;            Mark_T0[1]=-16.5625;          Mark_T0[2]=-14.5;                 Mark_T0[3]=-13;                     Mark_T0[4]=-11.3125;
    Mark_T0[5]=-9.1875;              Mark_T0[6]=-7.125;              Mark_T0[7]=-5.0625;             Mark_T0[8]=-3.0625;              Mark_T0[9]=-1.875;
    Mark_T0[10]=1.875;               Mark_T0[11]=3.0625;           Mark_T0[12]=5.0625;            Mark_T0[13]=7.125;               Mark_T0[14]=9.1875;
    Mark_T0[15]=11.3125;           Mark_T0[16]=13;                  Mark_T0[17]=14.5;                 Mark_T0[18]=16.5625;          Mark_T0[19]=19.0625;
}
MARK_POINTS::~MARK_POINTS(void)
{

}

//void MARK_POINTS::ComputTheGrid(float p,float t,float z,int nType,IPC_MODEL_INFO modelinfo)
//{
//    Q_UNUSED(p);

//    double Rparam = 0;
//    Rparam = modelinfo.rdata;


////    float t1=CAMERA_MATH::GetDecInCircle(t,0);
//    for (int i=0;i<ROW_NUMBER;i++)
//    {
//        for (int j=0;j<COLUMN_NUMBER;j++)
//        {
//            float y=-513+i*54;
//            float x=-912+j*96;
//            float tempP11=x/(Rparam*cos(t*pi/180)-y*sin(t*pi/180));
//            float tempP22=atan2(tempP11,1.0);//输出弧度
//            float tempP33=(tempP22*180/pi);//角度
//            Mark_P[i][j]=tempP33/(z*1.0f);

//            /*垂直偏移.视角变化.也是P和T的运动造成,T关系到图像的扭曲，P关系到补偿*/
//            float tempT11=(Rparam*sin(t*pi/180))+y*cos(t*pi/180);
//            float tempT22=(Rparam*cos(t*pi/180)-y*sin(t*pi/180));
//            float tempT12=cos(tempP33*pi/180);
//            float tempT33=tempT11/(tempT22/tempT12);
//            float tempT44=atan2(tempT33,1.0);//偏移量
//            tempT44=tempT44*180/pi;
//            tempT44=tempT44-t;
//            tempT44=tempT44/(z*1.0f)*modelinfo.sceneParam;

//            Mark_T[i][j]=tempT44;
//        }
//    }
//}



//void MARK_POINTS::ComputPT(PointPT& ptPoint, const float p, const float t, const float z, const float x_normalization, const float y_normalization,int nType,IPC_MODEL_INFO modelinfo)
//{
//    Q_UNUSED(z);
//    //计算出当前pt对应的网格
//    this->ComputTheGrid(p, t, z,nType,modelinfo);

//    //归一化坐标转换成标准坐标
//    int x = x_normalization * RESOLUTION_W;
//    int y = y_normalization * RESOLUTION_H;

//     //首先计算x与y所在的格子
//    if((    x>RESOLUTION_W20/2)&&
//            (x<(RESOLUTION_W-RESOLUTION_W20/2))&&
//            (y>RESOLUTION_H20/2)&&
//            (y<(RESOLUTION_H-RESOLUTION_H20/2)))//x,y有一个范围,不能太靠近边缘,边缘为长宽的1/40
//    {
//        int x_num=(x-RESOLUTION_W20/2)/ RESOLUTION_W20;
//        int y_num=(y-RESOLUTION_H20/2)/ RESOLUTION_H20;
//        qDebug() << "XY->PT MAT" << x_num << y_num;
//        //计算坐标偏移量比例
//        float x_bili=float(x-RESOLUTION_W20/2-x_num*RESOLUTION_W20)/(RESOLUTION_W20 * 1.0f);
//        float y_bili=float(y-RESOLUTION_H20/2-y_num*RESOLUTION_H20)/(RESOLUTION_H20 * 1.0f);

//        qDebug() << x_bili << y_bili;

//        //查一下格子
//        float p_x=Mark_P[y_num][x_num]+(Mark_P[y_num+1][x_num+1]-Mark_P[y_num][x_num])*x_bili;
//        float t_y=Mark_T[y_num][x_num]+(Mark_T[y_num+1][x_num+1]-Mark_T[y_num][x_num])*y_bili;
//       qDebug() << "pxty" << p_x << t_y;
//       float  temp_p;
//        if(nType == Dahua)
//        {
//             temp_p=p - p_x;////////
//        }
//        else
//        {
//             temp_p=p + p_x;////////
//        }

//        temp_p = CAMERA_MATH::AngleCorrectTo360(temp_p);
//        float temp_t=t+t_y;
//        ptPoint.P=temp_p;
//        ptPoint.T=temp_t;

//        //p = temp_p;
//       // t = temp_t;

//        qDebug() << "&&&&&&&&  p=" << ptPoint.P <<"******   t=" <<ptPoint.T;
//        qDebug() << "pxty" << temp_p <<"_________"<<temp_t;
//    }
//    return;
//}

void MARK_POINTS::ComputPT(PointPT& ptPoint, const float p, const float t, const float z, const float x_normalization, const float y_normalization,int nType,IPC_MODEL_INFO modelinfo)
{
    Q_UNUSED(z);

    double Rparam = 0;
    Rparam = modelinfo.rdata;

//    //计算出当前pt对应的网格
//    this->ComputTheGrid(p, t, z,nType,modelinfo);

    //归一化坐标转换成标准坐标，在转换为△x和△y,并且将倍率还原到1倍率图像。
    float x = (x_normalization - 0.5) * RESOLUTION_W/(z*1.0f);
    float y = (y_normalization - 0.5) * RESOLUTION_H/(z*1.0f);


    //计算目标点p值
    float tempP11=x/(Rparam*cos(t*pi/180)-y*sin(t*pi/180));
    float tempP22=atan2(tempP11,1.0);//输出弧度
    float p_x=(tempP22*180/pi);//角度
    float  temp_p;
    if(nType == Dahua)
    {
         temp_p=p - p_x;      //大华正方向不同
    }
    else
    {
         temp_p=p + p_x;           //目标的p值
    }

    temp_p = CAMERA_MATH::AngleCorrectTo360(temp_p); //角度转到0~360

    /*计算目标点t值*/
    float tempT11=(Rparam*sin(t*pi/180))+y*cos(t*pi/180);
    float tempT22=(Rparam*cos(t*pi/180)-y*sin(t*pi/180));
    float tempT12=cos(tempP22);
    float tempT33=tempT11/(tempT22/tempT12);
    float tempT44=atan2(tempT33,1.0);  //△T
    float temp_t=tempT44*180/pi;  //输出角度

    ptPoint.P=temp_p;
    ptPoint.T=temp_t;

    qDebug() << "&&&&&&&&  p=" << ptPoint.P <<"******   t=" <<ptPoint.T;
    qDebug() << "pxty" << temp_p <<"_________"<<temp_t;

    return;
}
