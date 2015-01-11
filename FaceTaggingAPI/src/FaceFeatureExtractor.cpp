//#include <Windows.h>
#include "CgtFace_Type.h"
//#include "CgtFace_Extractor.h"
//#include "PointLocate.h"

#include "Chope.h"
#include "FaceFeatureExtractor.h"

const int FACE_SIZE = 149; 

using namespace cv;
FaceFeatureExtractor::FaceFeatureExtractor()
{
	
//	m_pLBPEngine.g_proj_mat = NULL ;
//	m_pLBPEngine.g_mean_mat = NULL ;
//	m_pLBPEngine.g_proj_h = 0 ;
//	m_pLBPEngine.g_proj_w = 0 ;
//	m_pLBPEngine.g_lbp_proj_len = 0 ;
//	m_pLBPEngine.g_lbp_match_len = 0 ;
	
}


FaceFeatureExtractor::~FaceFeatureExtractor()
{
	//CgtFace_FExtractor_Release(&m_pgf_param);

//	LBP_FExtractor_Release(&m_pLBPEngine);

	UnInitialFeatExtract(&m_pMxGHandleFR,NULL,NULL);

}

int FaceFeatureExtractor::Init(const string &fea_model_path)
{
	int nres ;
// 	int nres = CgtFace_FExtractor_Init(&m_pgf_param,fea_model_path.c_str(), 600);
// 	if (nres != 0)
// 	{
// 		return false ;
// 	}
// 	
// 	nres = LBP_FExtractor_Init(&m_pLBPEngine,fea_model_path.c_str(), 600);
// 	if (nres != true)
// 	{
// 		return false ;
// 	}
	nres = InitFeatExtract(&m_pMxGHandleFR,(char *)(fea_model_path.c_str()), NULL,NULL);

	return nres;
}

bool FaceFeatureExtractor::FeatureGet(const unsigned char *pImageGray, const int iWidth, const int iHeight, const EyeArray &EyePt, vector< float> &vec_Feature,int nMethod)
{
	bool nres =false ;
	return nres ;
	
}

bool FaceFeatureExtractor::FeatureGet(const cv::Mat &ImageGray,  const EyeArray &EyePt, vector< float> &vec_Feature,int nMethod)
{
//	int t2 =GetTickCount();
	bool nres =true ;
	int i,j;
	int hr = 0 ;
	int iWidth = ImageGray.cols ;
	int iHeight= ImageGray.rows ;
	unsigned char *pImageGray= (unsigned char *)malloc(sizeof(unsigned char)*ImageGray.cols*ImageGray.rows);
	for(int y=0; y<ImageGray.rows; y++)
	{
		for (int x=0; x<ImageGray.cols; x++)
		{
			pImageGray[y*iWidth+x] = ImageGray.at<unsigned char>(y,x) ;
		}

	}

	switch(nMethod)
	{
	case FEA_GABOR_FX:
		{
			float *pFeature =(float *)malloc(sizeof(float)*1200);
//			int t1 = GetTickCount();
			ExtracFeature_ByImageEye(&m_pMxGHandleFR,pFeature, pImageGray,8,iWidth,iHeight,EyePt.x_right,EyePt.y_right,EyePt.x_left,EyePt.y_left);
		//	 t1 = GetTickCount() -t1;
		//	 printf("ExtracFeature_ByImageEye time is %d\n",t1);

			 int nLen = GetFeatureLen();
			 float fNormFea = 0 ;
			 for (i=0 ; i<nLen; i++)
			 {
				 fNormFea +=pFeature[i]*pFeature[i];
			 }
			 fNormFea = sqrtf(fNormFea);
			for (i=0 ; i<nLen; i++)
			{
				vec_Feature.push_back(pFeature[i]/fNormFea);
			}
			free(pFeature);
			break;
		}
	default:
		break;
	}

	free(pImageGray);


	return nres ;
}

float FaceFeatureExtractor::ScoreComapre(const vector< float> &vec_Feature_query,const vector< float> &vec_Feature_gallery,int nMethod)
{
	int i,j;
	float fScore=0.0;

	if (vec_Feature_query.size() != vec_Feature_gallery.size())
	{
		return fScore ;
	}
	
	switch(nMethod)
	{
	case DIS_COSIN:
		{
			float fA=0,fB=0,fAB=0 ;
			for (i=0; i<vec_Feature_query.size(); i++)
			{
				fA += vec_Feature_query.at(i)*vec_Feature_query.at(i);
				fB += vec_Feature_gallery.at(i)*vec_Feature_gallery.at(i);
				fAB +=vec_Feature_query.at(i)*vec_Feature_gallery.at(i);
			}
			if (fA<EPS_F || fB<EPS_F)
			{
				return fScore ;
			}
			else
			{
				fScore = (fAB/sqrtf(fA*fB) + 1.0f)/2;
			}
			break;
		}
	case DIS_HIST_BHATTACHARYYA:
		{
			 Mat mat_query = Mat::zeros(vec_Feature_query.size(), 1, CV_32FC1);
			  Mat mat_Gallery = Mat::zeros(vec_Feature_query.size(), 1, CV_32FC1);
			  for (i=0; i<vec_Feature_query.size(); i++)
			  {
				  mat_query.at<float>(i,0) = vec_Feature_query.at(i);
				  mat_Gallery.at<float>(i,0) = vec_Feature_gallery.at(i);
			  }
			  
			fScore =compareHist(mat_query, mat_Gallery, CV_COMP_BHATTACHARYYA);	
			break;
		}
	case DIS_HIST_INTERSECT:
		{
			Mat mat_query = Mat::zeros(vec_Feature_query.size(), 1, CV_32FC1);
			Mat mat_Gallery = Mat::zeros(vec_Feature_query.size(), 1, CV_32FC1);
			for (i=0; i<vec_Feature_query.size(); i++)
			{
				mat_query.at<float>(0,i) = vec_Feature_query.at(i);
				mat_Gallery.at<float>(0,i) = vec_Feature_gallery.at(i);
			}

			fScore = compareHist(mat_query, mat_Gallery, CV_COMP_INTERSECT);	
			break;
		}
	default:
		break;
	}
	return fScore;
}