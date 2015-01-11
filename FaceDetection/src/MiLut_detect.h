#ifndef __XIAOMI_LUT_OBJDETECT_H__
#define __XIAOMI_LUT_OBJDETECT_H__
#include "MiTypes_detect.h"
#include "MiBaseClass_detect.h"

 enum{ HAAR=0, LBP, HOG};

class LUTDetect{
public:
   

    LUTDetect();
    ~LUTDetect();
    bool init_LUTDetect(const string frontXMLDir,const string LeftProfileXMLDir,const string RightProfile);
    bool destroy_LUTDetect();

    vector<XmRect> Run_LUTDetect(unsigned char *img,int width,int height,int widthStep,int nchannels);
#ifndef RemoveOpencv
    vector<XmRect> Run_LUTDetect( Mat &_img );
    vector<XmRect> Run_LUTDetect( const string imgPath );
#endif

    CvLUTClassifierCascade* cvLoadLUTClassifierCascade(
        const string cascadeDirName, XmSize winSize,int multiNum =0);

    vector<XmRect> cvLUTDetectObjects( const XmArr* image,
        CvLUTClassifierCascade** MultiPoseCascade,int cascadeCount,
        double scale_factor =1.2,
        XmSize min_size = XmSize(0,0), XmSize max_size = XmSize(0,0));   
//-----------…Ë÷√ºÏ≤‚≤Œ ˝---------------
    void setDetectParams_MaxPhoto_Size(int d_Max_Size);
    void setDetectParams_Scale_Factor(float _scale_factor );
    void setDetectParams_USE_Parallel(bool isUse );
    void setDetectParams_Min_Max_ObjectSize_Pixel(int _MinSize,int _MaxSize);
  //  void setDetectParams_Min_Max_ObjectSize_Scale(int _MinSize,int _MaxSize);
    void setDetectParams_Speed_RateMode(int mode=0);

    void setDetectParams_MinNeighbors(int front,int left,int right);

#ifdef ConvertXMLToTxt
    bool ConvertXMLModelToTxtModel(const string cascadeDirName);
#endif
protected:
    CvLUTClassifierCascade* cvLoadLUTClassifierCascadeFromTxtFile(const string cascadeDirName);
    
    bool cvReadLUTStageClassifier(CvLUTStageClassifier **_stageLUT, std::ifstream &node);

    
#ifdef ConvertXMLToTxt
 //   bool ConvertXMLModelToTxtModel(const string cascadeDirName);
    bool cvReadLUTStageClassifier(CvLUTStageClassifier **_stageLUT,const FileNode &node);
    CvLUTClassifierCascade* cvLoadLUTClassifierCascadeFromFile(const string cascadeDirName,int numStages);

    int convertClassifierFromXmlToTxt(CvLUTClassifierCascade* cascade,string DstCascadefileName);
    int convertMultiPoseClassifierFromXmlToTxt( CvLUTClassifierCascade** MultiPoseCascade,string DstCascadeDirName);
#endif

    /* create more efficient internal representation of LUT classifier cascade */
     CvHidLUTClassifierCascade* icvCreateHidLUTClassifierCascade(CvLUTClassifierCascade *cascade);
    void sort_Stage_Classifier(CvLUTClassifierCascade* cascade,int startStage);

    void cvSetImagesForLUTClassifierCascade(CvLUTClassifierCascade* _cascade,const XmArr* _sum,
                                            const XmArr* _sqsum,
                                             double scale);

    void allocateSumMempry(int size);

private:
    

    //vector<XmRect> DetectFaces;
    DetectParams mDetectParams;
    int mCascadeCount;   //calssifier cascade Count [0,3]
    XmSize winSize;   //winSize 24x24
    CvLUTClassifierCascade *MultiPoseCascade[3];  //Multi Pose calssifier Cascade
    int mMinObjectSize;
    int mMaxObjectSize;

    int mfilterCascadePlus;
    int mfilterCascadeSub;

    unsigned char *mSumMemory;
    unsigned char *mSqSumMemory;
    
    unsigned char *mMaskSumMemory;


    unsigned char *mImgMemory;
    unsigned char *mGrayMemory;

    unsigned char *mYuvMemory;

    unsigned char *mSkinMaskMemory;

    XmMat *mSkinMaskMat;
   
    int srcImgIsGray;
};


#endif