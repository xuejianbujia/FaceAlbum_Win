#ifndef _FaceFeature_EXTRACTOR_H
#define _FaceFeature_EXTRACTOR_H

#include "opencv/cv.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"


//#include "CgtFace_Extractor.h"
//#include "CgtFace_MB_LBP.h"
#include "FaceGabor.h"
#include <vector>

using namespace std;


enum
{
	 FEA_LBP_ALLEN = 0 ,
	 FEA_GABOR_ALLEN = 1 ,
	 FEA_LBP_ZB = 2, 
	 FEA_GABOR_FX = 3
};

enum
{
	DIS_COSIN = 0 ,
	DIS_L1 = 1 ,
	DIS_HIST_BHATTACHARYYA = 2 ,
	DIS_HIST_INTERSECT = 3
};


typedef struct EyeArray
{
	int x_left ;
	int y_left ;
	int x_right ;
	int y_right ;
}EyeArray;


#define LBP_ALLEN_LEN	600
#define GABOR_ALLEN_LEN	600

#define  EPS_F	0.000001

// Face Eye Dtetcion Engine
class FaceFeatureExtractor
{
public:
	FaceFeatureExtractor();

	~FaceFeatureExtractor();

	int Init(const string &fea_model_path);

	bool FeatureGet(const unsigned char *pImageGray, const int iWidth, const int iHeight, const EyeArray &EyePt, vector< float> &vec_Feature,int nMethod=FEA_GABOR_FX);

	bool FeatureGet(const cv::Mat &ImageGray, const EyeArray &EyePt, vector< float> &vec_Feature,int nMethod=FEA_GABOR_FX);

	float ScoreComapre(const vector< float> &vec_Feature_query,const vector< float> &vec_Feature_gallery,int nMethod=DIS_COSIN);
public:

private:
//	Cgt_Gabor_Face_Param m_pgf_param ;

//	LBP_AL_ENGINE m_pLBPEngine;

	MX_G_HANGLE_FR m_pMxGHandleFR;

};

#endif
