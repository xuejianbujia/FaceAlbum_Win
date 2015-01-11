#ifndef __XIAOMI_BASE_CLASS_DETECT_H__
#define __XIAOMI_BASE_CLASS_DETECT_H__

#include "MiTypes_detect.h"
#include "MiTypes_c.h"

class DetectParams{
public:
    int detect_MaxPhoto_Size;  //检测的最大图片宽高值，默认是640
    int detect_Front_MinNeighbors; //正面的默认group大小 默认2
    int detect_Left_MinNeighbors;  //左侧面分类器的默认group大小 默认3
    int detect_Right_MinNeighbors; //右侧面的默认group大小 默认2
    float detect_Scale_Factor;  //缩放尺度 默认1.20
    bool detect_USE_Parallel;   //是否使用并行  true为使用，false为不实用
    int minObjectSize;       //设置最小的检测人脸
    int maxObjectSize;       //设置最大的检测人脸
    int speedMode;      //设置速度和检测率的模式 0、1、2  0最快检测率最低  2最慢 检测率最高 默认 0
    DetectParams();

};

vector<XmRect> combineMultiPoseFace( vector<XmRect> &allCandidates,vector<XmRect> &allCandidatesLeftProfile,vector<XmRect> &allCandidatesRightProfile,int frontMin,int leftMin,int rightMin,int srcImgIsGray);

vector<XmRect> filterNoiseFaceBySkinDetection(vector<XmRect> &rectFace,XmMat *maskMat,int groupMin);

#endif