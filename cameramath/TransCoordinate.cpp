#include "TransCoordinate.h"
#include "CameraMath.h"
#pragma execution_character_set("utf-8")

#define hikmodel "DS-2DC7423IW-A"

//const double RHik(1960.145);
//const double RTiandy(2000.145);
//const double RHuaWei(1554.145);
const double pi(3.14159265);

COORDINATE_TRANS::COORDINATE_TRANS(void)
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

COORDINATE_TRANS::~COORDINATE_TRANS(void)
{

}

//计算当前球机图像的20*20格点的pt值，方便后面插值
void COORDINATE_TRANS::ComputTheGrid(float p,float t,float z,int nType,IPC_MODEL_INFO modelinfo)
{
    Q_UNUSED(p);

    double Rparam = 0;
     Rparam = modelinfo.rdata;


    float t1=CAMERA_MATH::GetDecInCircle(t,0);
    for (int i=0;i<ROW_NUMBER;i++)
    {
        for (int j=0;j<COLUMN_NUMBER;j++)
        {

            float y=-513+i*54;
            float x=-912+j*96;
            float tempP11=x/(Rparam*cos(t*pi/180)-y*sin(t*pi/180));
            float tempP22=atan2(tempP11,1.0);//输出弧度
            float tempP33=(tempP22*180/pi);//角度

            Mark_P[i][j]=tempP33/(z*1.0f);

            //垂直偏移.视角变化.也是P和T的运动造成,T关系到图像的扭曲，P关系到补偿
            float tempT11=(Rparam*sin(t*pi/180))+y*cos(t*pi/180);
            float tempT22=(Rparam*cos(t*pi/180)-y*sin(t*pi/180));
            float tempT12=cos(tempP33*pi/180);
            float tempT33=tempT11/(tempT22/tempT12);
            float tempT44=atan2(tempT33,1.0);//偏移量
            tempT44=tempT44*180/pi;
            tempT44=tempT44-t;
            tempT44=tempT44/(z*1.0f);
            Mark_T[i][j]=tempT44*modelinfo.sceneParam;

            //qDebug()<<"P:"<<p<<"  T:"<<t<<" P1:"<<Mark_P[i][j]<<" P2:"<<tempP33<<" T1"<<Mark_T[i][j]<<"  T2:"<<tempT44<<endl;
        }
    }
}

//使用插值法计算标签的xy值
//bool COORDINATE_TRANS::ComputCoord(TargetPoint& Point_Screen, const STRUCT_PT_POINT& ptPoint, const float p, const float t, const float z,int nType,IPC_MODEL_INFO modelinfo)//计算坐标返回1080P条件下坐标的值
//{
//    Q_UNUSED(z);
//    //计算出当前pt对应的网格
//    this->ComputTheGrid(p, t, z,nType,modelinfo);
////    qDebug()<<"P:"<<p<<"  T:"<<t<<endl;
//    //由pt逆推回xy
//    int y_num=-999;
//    int x_num=-999;
//    float direction_p;
//    if(nType == Dahua)//20181116
//    {
//       direction_p = - CAMERA_MATH::GetDecInCircle(ptPoint.ptP, p);   //计算在圆周上的角度差值
//    }
//    else
//    {
//       direction_p =  CAMERA_MATH::GetDecInCircle(ptPoint.ptP, p);    //
//    }
//    float direction_t = CAMERA_MATH::GetDecInCircle(ptPoint.ptT, t);

//    bool bFind = false;
//    for (int i=1; i<ROW_NUMBER; i++)
//    {
//        for (int j=1; j<COLUMN_NUMBER; j++)
//        {
//            if (    (Mark_P[i][j]>direction_p)&&
//                    (Mark_T[i][j]>direction_t)&&
//                    (CAMERA_MATH::GetDecInCircle(Mark_P[i][j-1],Mark_P[i][j])< CAMERA_MATH::GetDecInCircle(direction_p ,Mark_P[i][j]))&&
//                    (CAMERA_MATH::GetDecInCircle(Mark_T[i-1][j],Mark_T[i][j])<CAMERA_MATH::GetDecInCircle(direction_t ,Mark_T[i][j])))
//            {
//                y_num=i - 1;
//                x_num=j - 1;
//                bFind = true;
//                break;
//            }
//        }
//    }

//    if (!bFind)
//    {
//        //        qDebug() << "can't find" << ptPoint.ptP << ptPoint.ptT;
//        return false;
//    }

//    //    qDebug() << "PT->XY MAT" << x_num << y_num;

//    float p_x = CAMERA_MATH::GetDecInCircle(ptPoint.ptP, p);
//    float t_y = ptPoint.ptT - t;
//    //    qDebug() << "pxty" << p_x << t_y;
//    float kkk = Mark_P[y_num][x_num];
//    float kkk2 = Mark_P[y_num+1][x_num+1];
//    float x_bili = 0;
//    if(nType == Dahua)
//    {
//        x_bili = ( - p_x - Mark_P[y_num][x_num]) / (Mark_P[y_num+1][x_num+1]-Mark_P[y_num][x_num]);
//    }
//    else
//    {
//        x_bili = (p_x - Mark_P[y_num][x_num]) / (Mark_P[y_num+1][x_num+1]-Mark_P[y_num][x_num]);
//    }
//    float y_bili = (t_y - Mark_T[y_num][x_num]) / (Mark_T[y_num+1][x_num+1]-Mark_T[y_num][x_num]);

//    //    qDebug() << x_bili << y_bili;

//    float x = x_bili * RESOLUTION_W20 + RESOLUTION_W20/2 + x_num*RESOLUTION_W20;
//    float y = y_bili * RESOLUTION_H20 + RESOLUTION_H20/2 + y_num*RESOLUTION_H20;
//    //    qDebug() << "xy"<< x << y;
//    Point_Screen.column_x =  x / (RESOLUTION_W * 1.0f);
//    Point_Screen.row_y = y / (RESOLUTION_H * 1.0f);;

//    return true;
//}


//void COORDINATE_TRANS::ComputCoordinates(std::vector <TargetPoint> &Point_Screen, const std::vector <STRUCT_PT_POINT>& Point, const float p, const float t, const float z,IPC_MODEL_INFO modelinfo)//计算坐标返回归一化坐标的值
//{
//    //计算出当前pt对应的网格
//    this->ComputTheGrid(p, t, z,1,modelinfo);
//    //重置屏幕标签
//    Point_Screen.clear();

//    //cout<<"Point_Screen_Size:"<<Point_Screen.size()<<endl;
//    //遍历一遍所有的标注好的点，逐一计算坐标.看看在不在当前ptz的1倍可视范围之内
//    //最大范围
//    float T_min=Mark_T[0][0];
//    float T_max=Mark_T[19][19];
//    float P_min=Mark_P[19][0];
//    float P_max=Mark_P[19][19];
//    //cout<<T_min<<"  "<<T_max<<"  "<<P_min<<"  "<<P_max<<endl;
//    //cout<<"Objects_Size:"<<objects.size()<<endl;

//    //初筛
//    std::vector <STRUCT_PT_POINT> candidate;
//    for(unsigned int j=0; j < Point.size(); j++)//
//    {
//        float P_chazhi=CAMERA_MATH::GetDecInCircle(Point[j].ptP, p);
//        float T_chazhi=CAMERA_MATH::GetDecInCircle(Point[j].ptT, t);
//        //cout<<"P_chazhi:"<<P_chazhi<<"  T_chazhi:"<<T_chazhi<<endl;
//        if (P_chazhi>P_min&&P_chazhi<P_max&&T_chazhi>T_min&&T_chazhi<T_max)//如果计算的偏移量在可视范围内，我们认为这些点为候选点
//        {
//            candidate.push_back(Point[j]);
//        }
//    }

//    qDebug() << candidate.size();

//    //找到每个候选点的坐标偏移量用像素点表示
//    for (unsigned int n=0;n<candidate.size();n++)
//    {
//        //计算每一个点所属的格子
//        int box_p=-999;
//        int box_t=-999;
//        float direction_p = CAMERA_MATH::GetDecInCircle(candidate[n].ptP, p);
//        float direction_t = CAMERA_MATH::GetDecInCircle(candidate[n].ptT, t);
//        TargetPoint Point_Scr;
//        //&&(GetDecInCircle(Mark_P[i][j-1],Mark_P[i][j])<GetDecInCircle(direction_p ,Mark_P[i][j]))&&(GetDecInCircle(Mark_T[i-1][j],Mark_T[i][j])<GetDecInCircle(direction_t ,Mark_T[i][j]))
//        for (int i=1; i<ROW_NUMBER; i++)
//        {
//            for (int j=1; j<COLUMN_NUMBER; j++)
//            {
//                if (    (Mark_P[i][j]>direction_p)&&
//                        (Mark_T[i][j]>direction_t)&&
//                        (CAMERA_MATH::GetDecInCircle(Mark_P[i][j-1],Mark_P[i][j])<CAMERA_MATH::GetDecInCircle(direction_p ,Mark_P[i][j]))&&
//                        (CAMERA_MATH::GetDecInCircle(Mark_T[i-1][j],Mark_T[i][j])<CAMERA_MATH::GetDecInCircle(direction_t ,Mark_T[i][j])))
//                {
//                    box_t=i;
//                    box_p=j;
//                    break;
//                }
//            }
//        }

//        qDebug() << "PT->XY MAT" << box_t << box_p;

//        //        cout<<"box_t&box_p: "<<box_t<<"  "<<box_t<<endl;

//        //-----------------计算下x,y并判断是否屏幕内，再入栈--------------------------
//        int p_x,p_y;//以左上点为原点的坐标
//        if (box_p>0&&box_t>0)//在网格中
//        {
//            //计算p值
//            int temp_x=RESOLUTION_W20*(box_p+0.5);      //网格边缘有0.5
//            //判断除数等于0的情况
//            float fDivisionx = abs(CAMERA_MATH::GetDecInCircle(Mark_P[box_t][box_p-1] ,Mark_P[box_t][box_p]));
//            if ( fDivisionx > -0.000001 &&
//                 fDivisionx < 0.000001)
//                fDivisionx = 0.0000001;

//            p_x= temp_x-(abs(CAMERA_MATH::GetDecInCircle(direction_p ,Mark_P[box_t][box_p])) / fDivisionx) * RESOLUTION_W20;

//            //计算t
//            int temp_y=RESOLUTION_H20*(box_t+0.5);

//            //判断除数等于0的情况
//            float fDivisiony = abs(CAMERA_MATH::GetDecInCircle(Mark_T[box_t-1][box_p] ,Mark_P[box_t][box_p]));
//            if ( fDivisiony > -0.000001 &&
//                 fDivisiony < 0.000001)
//                fDivisiony = 0.0000001;

//            p_y= temp_y-(abs(CAMERA_MATH::GetDecInCircle(direction_t ,Mark_T[box_t][box_p]))/fDivisiony)*RESOLUTION_H20;

//            //以中心为原点相对于中心的坐标
//            int p_cx=(p_x-RESOLUTION_W/2)*z;
//            int p_cy=(p_y-RESOLUTION_H/2)*z;

//            //判断是否在z倍率屏幕内，Point_Scr入栈
//            if((abs(p_cx)<RESOLUTION_W/2)&&
//                    (abs(p_cy)<RESOLUTION_H/2))
//            {
//                p_x=p_cx+RESOLUTION_W/2;
//                p_y=p_cy+RESOLUTION_H/2;
//                Point_Scr.P=direction_p;
//                Point_Scr.T=direction_t;
//                Point_Scr.column_x=p_x / (RESOLUTION_W * 1.0f);
//                Point_Scr.row_y=p_y / (RESOLUTION_H * 1.0f);
//                Point_Screen.push_back(Point_Scr);
//            }
//        }
//    }
//    return;
//}


bool COORDINATE_TRANS::ComputCoord(TargetPoint& Point_Screen, const STRUCT_PT_POINT& ptPoint, const float p, const float t, const float z,int nType,IPC_MODEL_INFO modelinfo)//计算坐标返回1080P条件下坐标的值
{
    Q_UNUSED(z);
    //计算出当前pt对应的网格
    this->ComputTheGrid(p, t, z,nType,modelinfo);
    double Rparam = 0;
    Rparam = modelinfo.rdata;
    //PT边界值
    float T_min=Mark_T[0][0];
    float T_max=Mark_T[19][19];
    float P_min=Mark_P[19][0];
    float P_max=Mark_P[19][19];

//    qDebug()<<"P:"<<p<<"  T:"<<t<<endl;
    //由pt逆推回xy
    float direction_p;
    if(nType == Dahua)//20181116
    {
       direction_p = - CAMERA_MATH::GetDecInCircle(ptPoint.ptP, p);   //计算在圆周上的角度差值
    }
    else
    {
       direction_p =  CAMERA_MATH::GetDecInCircle(ptPoint.ptP, p);    //
    }
    float direction_t = CAMERA_MATH::GetDecInCircle(ptPoint.ptT, t);
    if (direction_p>P_min&&direction_p<P_max&&direction_t>T_min&&direction_t<T_max)   //1倍率下
    {
        float dy11=Rparam*(tan(ptPoint.ptT*pi/180)*cos(t*pi/180) - sin(t*pi/180)*cos(direction_p*pi/180));
        float dy22=cos(t*pi/180)*cos(direction_p*pi/180) + tan(ptPoint.ptT*pi/180)*sin(t*pi/180);
        float dy=(dy11/dy22)*z;
        float dx=tan(direction_p*pi/180)*(Rparam*cos(t*pi/180) - dy*sin(t*pi/180))*z;

        float x = dx+RESOLUTION_W/2;
        float y = dy+RESOLUTION_H/2;
        //    qDebug() << "xy"<< x << y;
        Point_Screen.column_x =  x / (RESOLUTION_W * 1.0f);
        Point_Screen.row_y = y / (RESOLUTION_H * 1.0f);
        return true;
    }
    else
    {
        return false;
    }

}

void COORDINATE_TRANS::ComputCoordinates(std::vector <TargetPoint> &Point_Screen, const std::vector <STRUCT_PT_POINT>& Point, const float p, const float t, const float z,IPC_MODEL_INFO modelinfo)//计算坐标返回归一化坐标的值
{
    //计算出当前pt对应的网格
    this->ComputTheGrid(p, t, z,1,modelinfo);
    //重置屏幕标签
    Point_Screen.clear();

    double Rparam = 0;
    Rparam = modelinfo.rdata;

    //cout<<"Point_Screen_Size:"<<Point_Screen.size()<<endl;
    //遍历一遍所有的标注好的点，逐一计算坐标.看看在不在当前ptz的1倍可视范围之内
    //最大范围
    float T_min=Mark_T[0][0];
    float T_max=Mark_T[19][19];
    float P_min=Mark_P[19][0];
    float P_max=Mark_P[19][19];
    //cout<<T_min<<"  "<<T_max<<"  "<<P_min<<"  "<<P_max<<endl;
    //cout<<"Objects_Size:"<<objects.size()<<endl;

    //初筛
    std::vector <STRUCT_PT_POINT> candidate;
    for(unsigned int j=0; j < Point.size(); j++)//
    {
        float P_chazhi=CAMERA_MATH::GetDecInCircle(Point[j].ptP, p);
        float T_chazhi=CAMERA_MATH::GetDecInCircle(Point[j].ptT, t);
        //cout<<"P_chazhi:"<<P_chazhi<<"  T_chazhi:"<<T_chazhi<<endl;
        if (P_chazhi>P_min&&P_chazhi<P_max&&T_chazhi>T_min&&T_chazhi<T_max)//如果计算的偏移量在可视范围内，我们认为这些点为候选点
        {
            candidate.push_back(Point[j]);
        }
    }

    qDebug() << candidate.size();

    //找到每个候选点的坐标偏移量用像素点表示
    for (unsigned int n=0;n<candidate.size();n++)
    {

        float direction_p = CAMERA_MATH::GetDecInCircle(candidate[n].ptP, p);
        float direction_t = CAMERA_MATH::GetDecInCircle(candidate[n].ptT, t);
        TargetPoint Point_Scr;
        //计算相对图像中心的dx和dy，1倍率
        float dy11=Rparam*(tan(candidate[n].ptT)*cos(t) - sin(t)*cos(direction_p));
        float dy22=cos(t)*cos(direction_p) + tan(candidate[n].ptT)*sin(t);
        int dy=(dy11/dy22);
        int dx=tan(direction_p)*(Rparam*cos(t) - dy*sin(t));
        //z倍率下坐标
        int p_cx=dx*z;
        int p_cy=dy*z;

        //判断是否在z倍率屏幕内，Point_Scr入栈
        if((abs(p_cx)<RESOLUTION_W/2)&&
                (abs(p_cy)<RESOLUTION_H/2))
        {
            int p_x=p_cx+RESOLUTION_W/2;
            int p_y=p_cy+RESOLUTION_H/2;
            Point_Scr.P=direction_p;
            Point_Scr.T=direction_t;
            Point_Scr.column_x=p_x / (RESOLUTION_W * 1.0f);
            Point_Scr.row_y=p_y / (RESOLUTION_H * 1.0f);
            Point_Screen.push_back(Point_Scr);
        }
    }

    return;
}

