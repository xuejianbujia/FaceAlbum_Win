#ifndef __XIAOMI_COMMON_DETECT_H__
#define __XIAOMI_COMMON_DETECT_H__

#include "MiTypes_c.h"
#include <map>
#include <deque>
#include <vector>
#include <assert.h>
#include <time.h>
#include <iomanip>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;
//#define Mobile_Project
#define ARGBToGRAY

#ifdef Mobile_Project
#include "../FaceDetectionAPI_jni.h"
//#define ARGBToGRAY
#else
#define LOGI printf
#define LOGE printf
#define LOGD printf

#endif

/****************************************************************************************\
*                         LUT-like Object Detection functions                           *
\****************************************************************************************/

typedef int sumtype;
typedef double sqsumtype;
typedef int type_int;
typedef unsigned char type_uchar;

#define shift_bit 12
#define shift_bit_sum 8
#define shift_bit_weight 20

#define shift_8bit_Num 256
#define shift_20bit_Num 1048576


#define CV_LUT_FEATURE_MAX  3
#define CV_LUT_LBP_NUM 16




#define CV_LUT_LBP_BLOCK 9
#define StagePreCalculateNum 20  


#define Detect_USE_Parallel 0 //是否并行 1并行 0 否

#define NBINS_1 63
#define LBP_MAX_MIN 255
//#define USE_OPT_NEST
#define Float_Fixed_Point       //进行定点化优化
#define LUT_Sort_Classifier    //对haar和lbp特征进行排序
//#define LUT_HAAR_Sort_Classifier  //对haar中的特征按类型排列

enum{Not_Haar = -1,HAAR_Other = 0,HAAE_1N1,HAAE_1N2,HAAE_1N3};

//#define Init_Check
//---------优化预定义----end----

//#define RemoveOpencv
//#define ConvertXMLToTxt


// #include "opencv/cv.h"
// #include <opencv2/highgui/highgui.hpp>
//---------优化预定义-----begin---

//using namespace cv;


#ifndef RemoveOpencv
#include "opencv/cv.h"
#include <opencv2/highgui/highgui.hpp>
//---------优化预定义-----begin---

using namespace cv;

#endif

#define  EXPAND_LAB_GAP

#define CV_LUT_ELAB_NUM 36

typedef struct CvHaarFeatureRect
{
    XmRect r;
    int weight;
    CvHaarFeatureRect():weight(0)
    {
        //weight=0;
    }
}CvHaarFeatureRect;


typedef struct CvLUTFeature
{
    //------HAAR--------------------
    int tilted;
    int haarType;
    CvHaarFeatureRect rect[CV_LUT_FEATURE_MAX];

    //------LBP--------------------
    XmRect lbprect;
    int lbpGap;

} CvLUTFeature;


typedef struct tag_xmImage
{
    type_int		size;
    type_int		width;
    type_int		height;
    type_int		depth;
    type_int		channel;
    type_int		linebytes;
    type_uchar	*data;
}XmImage;

typedef struct CvLUTClassifier
{

    vector<double> binH;
    float Vmin, Vmax;
    float threshold;
    int featureType;

    //int count;
    CvLUTFeature LUT_feature;

} CvLUTClassifier;

typedef struct CvLUTStageClassifier
{
    int  count;
    float threshold;
    int binCount;

    int HaarCount;
    int Haar_1N1;
    int Haar_1N2;
    int Haar_1N3;
    int Haar_other;

    int LBPCount;
    CvLUTClassifier*  classifier;
    CvLUTClassifier*  order_classifier;
    
} CvLUTStageClassifier;

typedef struct CvHidLUTClassifierCascade CvHidLUTClassifierCascade;

typedef struct CvLUTClassifierCascade
{
    int  flags;
    int  count;
    //int binCount;
    XmSize orig_window_size;
    XmSize real_window_size;
    double scale;
    
    vector< CvLUTStageClassifier* >  stage_classifier;
    CvHidLUTClassifierCascade* hid_cascade;
} CvLUTClassifierCascade;

//#define  SAVE_FILE_DEBUG

typedef struct CvHidRect
{
    sumtype *p0, *p1, *p2, *p3;


    double   weight;
    type_int weightInt;
    CvHidRect()
    {
        weight=0;
        weightInt=0;
    }
    CvHidRect(int *_p0,int *_p1,int *_p2,int *_p3)
    {
        p0 = _p0;
        p1 = _p1;
        p2 = _p2;
        p3 = _p3;
    }
}CvHidRect;

typedef struct CvHidLBPRect
{
#ifdef EXPAND_LAB_GAP
    sumtype *p[CV_LUT_ELAB_NUM];
#else
    sumtype *p[CV_LUT_LBP_NUM];
#endif
 
   
    CvHidRect pp[CV_LUT_LBP_BLOCK];

}CvHidLBPRect;

typedef struct CvHidLUTFeature
{

    CvHidRect rect[CV_LUT_FEATURE_MAX];
    CvHidLBPRect lbprect;
} CvHidLUTFeature;

typedef struct CvHidLUTClassifier
{
    //CvLUTFeature      origFeature;
    CvHidLUTFeature   hidFeature;
    int featureType;
    double min;
    double max;
    double fmax_min;


#ifdef Float_Fixed_Point
    vector<type_int> binHInt;
    type_int thresholdInt;
    type_int minInt;
    type_int maxInt;
    type_int max_min;
#else
    float threshold;
    
    vector<double> binH;
#endif

}
CvHidLUTClassifier;


typedef struct CvHidLUTStageClassifier
{
    int  count;
    float threshold;
    type_int thresholdInt;

    CvHidLUTClassifier* classifier;  //弱特征数组，count为元素个数
    int binCount;

    int Haar_1N1;
    int Haar_1N2;
    int Haar_1N3;
    int Haar_other;

    int HaarCount;
    int LBPCount;

} CvHidLUTStageClassifier;//强特征结构


typedef struct CvHidLUTClassifierCascade
{
    int  count;
   
   
 //   int  has_tilted_features;
    int  is_tree;
    float inv_window_area;

    XmMat sum,sqsum;

    

    vector<CvHidLUTStageClassifier* > stage_classifier;//强特征数组，长度为count
    sqsumtype *pq0, *pq1, *pq2, *pq3;
    sumtype *p0, *p1, *p2, *p3;

    
    void** ipp_stages;
} CvHidLUTClassifierCascade;


#define  WinSize_Width 24
#define  WinSize_Height 24
#define Deafault_Scale_Factor 1.20
#define  Deafault_Cascade_Count 3

#define Detect_Resize_Use 

#define Detect_Multi_Pose 3
#define Detect_Max_Photo_Size 640

#define Detect_Front_MinNeighbors 2
#define Detect_left_MinNeighbors 2
#define Detect_Right_MinNeighbors 2

#define Detect_Min_ObjectSize 24
#define Detect_Max_ObjectSize 640

//#define MixMultiPose  


//采用阈值置信度的 对检测到的人脸过滤
#define USE_Threshold_B_Confidence

#ifdef USE_Threshold_B_Confidence
#define Front_Threshold_B 0.8
#define Left_Threshold_B 0.1  
#define Right_Threshold_B 0.1

// #define Front_Threshold_B 0.8
// #define Left_Threshold_B 0.1  
// #define Right_Threshold_B 0.1 
#endif
//采用肤色检测过滤错误人脸以及加快 多姿态检测速度
//#define USE_Skin_Detect

//#define USE_Skin_Detect_Speed


#define time( arg ) (((double) clock()) / CLOCKS_PER_SEC)
#endif
