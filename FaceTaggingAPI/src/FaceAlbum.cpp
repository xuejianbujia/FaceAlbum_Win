#include "FaceAlbum.h"
#include "FaceTaggingCommonDef.h"
#include "MiLut_detect.h"
#include"EXIF.h"


#define IMAGE_JPG_SIZE 2500

//#define PRINT_LOG


#ifdef PRINT_LOG

#ifndef LINUX
#include<process.h>
#include<windows.h>
#else
#include <sys/syscall.h>
 #include <sys/types.h>
 #include <linux/unistd.h>
 #include <errno.h>

#endif
#endif

FaceAlbum::FaceAlbum()
{
    mFaceDetectPtr = NULL;
   
	m_Eyerunner = NULL ;
    mFeatureExtractorPtr=NULL;
	

    mFeatureFlag="1001";

	// Allen modify
    mVersion = "1.0.0";


	m_resImageInfo.mFaceNum = 0;
	m_resImageInfo.mFace_version=0;
	m_resImageInfo.mImageFlag="";
	m_resImageInfo.mFaceInfo = NULL;

	CreateFileHandle( );
   // mImageInfo.mFace_version = 1001;
}


FaceAlbum::~FaceAlbum()
{
	CloseFileHandle( );
    if (mFaceDetectPtr)
    {
        delete mFaceDetectPtr;
        mFaceDetectPtr = NULL;
    }
    if (mFeatureExtractorPtr)
    {
        delete mFeatureExtractorPtr;
        mFeatureExtractorPtr = NULL;
    }
	if(m_Eyerunner)
	{
		delete m_Eyerunner;
		m_Eyerunner = NULL;
	}
	if(m_resImageInfo.mFaceInfo != NULL)
	{
		delete []m_resImageInfo.mFaceInfo;
		m_resImageInfo.mFaceInfo = NULL;
	}
	DestroyImageInfoMemory();
}
int FaceAlbum::DestroyImageInfoMemory()
{
	if(m_resImageInfo.mFaceInfo != NULL)
	{
		delete []m_resImageInfo.mFaceInfo;
		m_resImageInfo.mFaceInfo = NULL;
	}
	return 0;
}
/*----------------------------------Face Detect API---------------------------------------------*/
int FaceAlbum::FaceDetect_Init( const string frontclassifier,const string leftClassifierDir,const string rightClassifierDir )
{
    int nres = XMFACE_NORMAL;
    if(mFaceDetectPtr==NULL)
    {
        mFaceDetectPtr = new LUTDetect;
        bool bres =  mFaceDetectPtr->init_LUTDetect(frontclassifier,leftClassifierDir,rightClassifierDir);
        if (bres== false)
        {
            nres = XMFACE_ERR_INITIALIZE;
        }
    }
      return nres;
}

int FaceAlbum::FaceDetect_Init( const string model_path )
{
    if (model_path.empty())
    {
        return XMFACE_ERR_INITIALIZE;
    }
    const string frontclassifier = model_path+"/front.model";
    const string leftClassifierDir = model_path+"/left.model";
    const string rightClassifierDir = model_path+"/right.model";

    return FaceDetect_Init(frontclassifier,leftClassifierDir,rightClassifierDir);
}


vector<XmRect> FaceAlbum::FaceDetect_Image( unsigned char *imgBuffer,int width,int height,int nchanels )
{
    vector<XmRect> vFaceDetected;
    
    if (imgBuffer==NULL || width<=0 || height<=0 || nchanels<=0)
    {
        cout<<"image is error!!"<<endl;
        return vFaceDetected;
    }
    if (mFaceDetectPtr == NULL)
    {
        cout<<"FaceDetect Ptr Not Init!!"<<endl;
        return vFaceDetected;
    }
    Mat img=Mat(height,width,CV_8UC(nchanels));
    img.data = imgBuffer;
   
    vector<XmRect> FaceDetected = mFaceDetectPtr->Run_LUTDetect(imgBuffer,width,height,width*nchanels,nchanels);

    for (int i=0;i<FaceDetected.size();++i)
    {
        vFaceDetected.push_back(FaceDetected[i]);
    }
  

   // printf("Face Detect time is %0.3f s \n",oneImgTime);
    return vFaceDetected;

}

vector<XmRect> FaceAlbum::FaceDetect_Image( Mat &img )
{
    vector<XmRect> vFaceDetected;
    if (img.empty())
    {
        return vFaceDetected;
    }
    double oneImgTime =  -time(0);
    
   // imwrite("d:/4.jpg",img);
    vector<XmRect> FaceDetected  = mFaceDetectPtr->Run_LUTDetect(img.data,img.cols,img.rows,img.step,img.channels());


    for (int i=0;i<FaceDetected.size();++i)
    {
        vFaceDetected.push_back(FaceDetected[i]);
    }
    oneImgTime +=time(0);

    //printf("Face Detect time is %0.3f s \n",oneImgTime);

    return vFaceDetected;
}


/*----------------------------------Eye Detect API---------------------------------------------*/
int FaceAlbum::EyeDetect_Init( const string &modelDir )
{
	 std::string frontModel=modelDir +"/sdm_frontal_200_Gallery_stage_5_11_20.bin";
	 std::string leftModel=modelDir +"/sdm_yangsong_left_160_11_20.bin";
	 std::string rightModel=modelDir +"/sdm_yangsong_right_160_11_20.bin";

	m_Eyerunner = allen_facemarks::SDMModelRunner::GetRunner(frontModel,leftModel,rightModel);
	if(m_Eyerunner->IsGood())
	{
		cout << "Load Runner success." << endl << endl;
		return 0;
	}
	else
	{
		cout << "Load Runner failed." << endl << endl;
		return -1;
	}
	
}

vector< vector <Point2f> >  FaceAlbum::GetLandMarkPoints_Image( Mat &img,vector<XmRect> detectResult )
{
	vector< vector <Point2f> > keyPoints;
	vector<Rect> faces_Rect;
	std::vector<int> pose;
	for(size_t i=0; i<detectResult.size(); ++i)
	{
		cv::Rect face_rect = cv::Rect(detectResult[i].x, detectResult[i].y, detectResult[i].width, detectResult[i].height*1.2);
		faces_Rect.push_back(face_rect);

		int front = detectResult[i].facePose[0];
		int left = detectResult[i].facePose[1];
		int right = detectResult[i].facePose[2];

		if(front )
		{
			pose.push_back(0);
		}
		else if(left && (!right))
		{
			pose.push_back(1);
		}
		else if(right &&(!left))
		{
			pose.push_back(2);
		}
		else
		{
			pose.push_back(0);
		}

	}

	m_Eyerunner->Run(img, faces_Rect, pose, keyPoints);
	return keyPoints;
}

vector< vector <Point2f> >  FaceAlbum::GetLandMarkPoints_Image( unsigned char *imgBuffer,int width,int height,int nchannels,vector<XmRect> detectResult )
{
	vector< vector <Point2f> > keyPoints;
	if (imgBuffer==NULL || width<=0 || height<=0 || nchannels<=0)
	{
		cout<<"image is error!!"<<endl;
		return keyPoints;
	}
	Mat img=Mat(height,width,CV_8UC(nchannels));
	img.data = imgBuffer;

	return GetLandMarkPoints_Image(img,detectResult);
}


vector< EyeArray >  FaceAlbum::EyeDetect_Image( Mat &img,vector< vector <Point2f> > &keyPoints )
{
	vector< EyeArray > vEyeDetected;
	cv::Point Eye_left;
	cv::Point Eye_right;
	for (int i=0; i<keyPoints.size();i++)
	{
		vector <Point2f> vecLandMark = keyPoints[i];
		m_Eyerunner->GetEyePosition((const vector <Point2f> )vecLandMark,Eye_left,Eye_right);

		EyeArray eyePt;
		eyePt.x_left = Eye_left.x;
		eyePt.y_left = Eye_left.y;
		eyePt.x_right = Eye_right.x;
		eyePt.y_right = Eye_right.y;
		vEyeDetected.push_back(eyePt);
	}

	return vEyeDetected;
}
vector< EyeArray >  FaceAlbum::EyeDetect_Image( Mat &img,vector<XmRect> detectResult )
{
    vector< EyeArray > vEyeDetected;
	cv::Point Eye_left;
	cv::Point Eye_right;
	//vector< vector <Point2f> > keyPoints =GetLandMarkPoints_Image( img,detectResult );
	mkeyPointsLandMark.clear();
	mkeyPointsLandMark =GetLandMarkPoints_Image( img,detectResult );
	


	for (int i=0; i<mkeyPointsLandMark.size();i++)
	{
		vector <Point2f> vecLandMark = mkeyPointsLandMark[i];
		m_Eyerunner->GetEyePosition((const vector <Point2f> )vecLandMark,Eye_left,Eye_right);

		EyeArray eyePt;
		eyePt.x_left = Eye_left.x;
		eyePt.y_left = Eye_left.y;
		eyePt.x_right = Eye_right.x;
		eyePt.y_right = Eye_right.y;
		vEyeDetected.push_back(eyePt);
	}

    return vEyeDetected;

}



vector< EyeArray >  FaceAlbum::EyeDetect_Image( unsigned char *imgBuffer,int width,int height,int nchannels,vector<XmRect> detectResult )
{
    vector< EyeArray > vEyeDetected;
    if (imgBuffer==NULL || width<=0 || height<=0 || nchannels<=0)
    {
        cout<<"image is error!!"<<endl;
        return vEyeDetected;
    }
    Mat img=Mat(height,width,CV_8UC(nchannels));
    img.data = imgBuffer;

    return EyeDetect_Image(img,detectResult);
}
/*----------------------------------Feature Extraction API---------------------------------------------*/
int FaceAlbum::FeatureExtractor_Init( const string &featrueModePath )
{
    int nres = XMFACE_NORMAL;
    if (mFeatureExtractorPtr ==NULL)
    {
        mFeatureExtractorPtr = new FaceFeatureExtractor();
        if( XMFACE_NORMAL != mFeatureExtractorPtr->Init(featrueModePath) )
            nres = XMFACE_ERR_INITIALIZE;
        else
        {
            cout<<"FaceFeatureExtraction is Init success!!"<<endl;
        }
    }

    return nres;
}

/*----------------------------------3d correcttion API---------------------------------------------*/
// 3d initial
#ifdef CORRECT_3D

template<typename T>
int FaceAlbum::GetEyePosition(const vector <T> &pts_95,cv::Point &Eye_left,cv::Point &Eye_right)
{
	int k;

	{
		float fum_x=0;
		float fum_y=0;
		for (k=39; k<=50; k++)
		{
			fum_x +=pts_95[k].x;
			fum_y +=pts_95[k].y;
		}
		Eye_left.x = fum_x/12;
		Eye_left.y = fum_y/12;

		fum_x=0;
		fum_y=0;
		for (k=51; k<=62; k++)
		{
			fum_x +=pts_95[k].x;
			fum_y +=pts_95[k].y;
		}
		Eye_right.x = fum_x/12;
		Eye_right.y = fum_y/12;
	}
	return 0;
}
#ifdef CORRECT_3D
int FaceAlbum::Correct3d_Init(const string model_3d_path)
{

	if (model_3d_path.empty() )
	{
		return XMFACE_ERR_INITIALIZE;
		
	}
	string sAlignment_model =model_3d_path+"alignment48.txt";
	string sAverageModel=model_3d_path+"3dmodel.mdl";
	 m_formmodel.InitAll(sAlignment_model,sAverageModel);
	 return 0;
	
}
#endif

template<typename T>
vector<Image3dWarping> FaceAlbum::ImageCorrect3d(cv::Mat &image,vector< vector<T> >&pts,vector<XmRect> &face_rect)
{
	vector<Image3dWarping> vec_Image3d_result;
	for (int index =0; index <pts.size(); index++)
	{
		Image3dWarping imageWarp;
		vector<T> pt_tmp = pts[index];

		//if the image is frontal  ,we should set imageWarp structure by orignal image and the pts
	
	if ( (!face_rect[index].facePose[1])&& (!face_rect[index].facePose[2]) )
		{
			imageWarp.WarpingFlag = 0 ;
			
		}
		else //coorect the image by 3d method
		{
			imageWarp.WarpingFlag = 1 ;
			vector<cv::Point> pts_95;
			for (int i=0; i<pt_tmp.size(); i++)
			{
				cv::Point pt  ;
				pt.x = pt_tmp[i].x;
				pt.y = pt_tmp[i].y;
				pts_95.push_back(pt);
			}

			m_formmodel.ReconstructionFace(image,512,512,pts_95,15/180.0*CV_PI);
			imageWarp.pts_Warping=m_formmodel.locateFeaturePoints(0,0,0);

			cv::Point Eye_left;
			cv::Point Eye_right;
			GetEyePosition(imageWarp.pts_Warping,Eye_left,Eye_right);
			imageWarp.EyePt.x_left = Eye_left.x;
			imageWarp.EyePt.y_left = Eye_left.y;
			imageWarp.EyePt.x_right = Eye_right.x;
			imageWarp.EyePt.y_right = Eye_right.y;

			imageWarp.imageWarping=m_formmodel.RenderFace(0,0,0);
		}
		vec_Image3d_result.push_back(imageWarp);
		
	}
	
	
	return vec_Image3d_result;
}

#endif




void FaceAlbum::GetColorRectFaceImg(cv::Mat &image,EyeArray &EyePt,cv::Mat &NormImg )
{
	Rect faceRect;

	int midY = (EyePt.y_right + EyePt.y_left)/2;
	int midX = (EyePt.x_right + EyePt.x_left)/2;

	int ew = sqrtf((EyePt.x_right - EyePt.x_left)*(EyePt.x_right - EyePt.x_left) +(EyePt.y_right - EyePt.y_left)*(EyePt.y_right - EyePt.y_left) );

	if (ew<0)
	{
		int tmpX = EyePt.x_right;
		EyePt.x_right = EyePt.x_left;
		EyePt.x_left = tmpX;
		ew = -ew;
	}

	faceRect.x = midX - 1.5*ew;
	faceRect.x = faceRect.x>=0?faceRect.x : 0;
	faceRect.y = midY - 2*ew;
	faceRect.y = faceRect.y>=0?faceRect.y : 0;

	faceRect.width =3*ew;
	faceRect.height = 8*ew;
	if (faceRect.width + faceRect.x >image.cols)
	{
		faceRect.width = image.cols - faceRect.x -1;
	}
	if (faceRect.height + faceRect.y >image.rows)
	{
		faceRect.height = image.rows - faceRect.y -1;
	}

	Mat subImg = image(faceRect);
#ifdef debug_test
	cvCircle( &((IplImage)subImg),cvPoint(EyePt.x_left - faceRect.x,EyePt.y_left - faceRect.y),2,CV_RGB(255,0,0),1,16 ); 
	cvCircle( &((IplImage)subImg),cvPoint(EyePt.x_right - faceRect.x,EyePt.y_right - faceRect.y),2,CV_RGB(255,0,0),1,16 ); 

#endif
    //imwrite("D:\\data\\tmpdata\\1.jpg",subImg);

	Mat rsizeMat =Mat::zeros(400,200,image.type()); 


	resize(subImg,rsizeMat,cv::Size(200,400));

	NormImg = rsizeMat.clone();
}

#ifdef _WIN32
vector<string>  FaceAlbum::FeatureExtractor_Image( Mat &img,vector< EyeArray>&vec_Eye,vector<Rect>&FaceRect )
{
    vector<string> vFeatureStr;
    assert(mFeatureExtractorPtr);
    if (vec_Eye.size()==0)
    {
        return vFeatureStr;
    }


    int FaceIndex=0;
    EyeArray EyePt;
    Mat imageGray;

    if (img.channels() > 1)
    {
        cvtColor(img,imageGray,CV_BGR2GRAY);
    }


    while (FaceIndex < vec_Eye.size())
    {
        EyePt = vec_Eye[FaceIndex];
        vector< float> vec_Feature;
        double oneImgTime = -time(0);

        //1 提取人脸脸部特征
        if (imageGray.empty())  //要用灰度图进行特征提取
        {
            mFeatureExtractorPtr->FeatureGet(img,EyePt,vec_Feature);
        }
        else
        {
            mFeatureExtractorPtr->FeatureGet(imageGray,EyePt,vec_Feature);
        }

        // 2 提取人脸脸部周围的区域特征
        mpeg7_feature_color mMpeg7FeaturePtr;
        cv::Mat NormImg;
        GetColorRectFaceImg(img,EyePt,NormImg );

        mMpeg7FeaturePtr.feature_get(NormImg,FEATURE_COLORFUSION);


        oneImgTime +=time(0);

        //  printf("Feature extraction time  is %0.3lf s   \n",oneImgTime);

        //   string featureStr = FeatureExtractor_ConvertFloatToString(vec_Feature);
        string featureStr = FeatureExtractor_ConvertFeatureToString(vec_Feature,mMpeg7FeaturePtr);
        vFeatureStr.push_back(featureStr);
        FaceIndex++;
    }

    return vFeatureStr;
}
#endif


#ifdef CORRECT_3D
vector<string>  FaceAlbum::FeatureExtractor_Image( Mat &img,vector<Image3dWarping> &vec_Image3d,vector< EyeArray>&vec_Eye )
{
	vector<string> vFeatureStr;
	assert(mFeatureExtractorPtr);
	if (vec_Eye.size()==0 || vec_Image3d.size()!= vec_Eye.size())
	{
		return vFeatureStr;
	}


	int FaceIndex=0;
	EyeArray EyePt;
	Mat imageGray;

	if (img.channels() > 1)
	{
		cvtColor(img,imageGray,CV_BGR2GRAY);
	}


	while (FaceIndex < vec_Eye.size())
	{
		EyePt = vec_Eye[FaceIndex];
		vector< float> vec_Feature;
		double oneImgTime = -time(0);

		if (vec_Image3d[FaceIndex].WarpingFlag == 1) // Use the correct image 
		{
			mFeatureExtractorPtr->FeatureGet(vec_Image3d[FaceIndex].imageWarping,vec_Image3d[FaceIndex].EyePt,vec_Feature);
		}
		else // Use the original image 
		{
			//1 Extract the face gabor fature
			if (imageGray.empty())  //extract it by gray image
			{
				mFeatureExtractorPtr->FeatureGet(img,EyePt,vec_Feature);
			}
			else
			{
				mFeatureExtractorPtr->FeatureGet(imageGray,EyePt,vec_Feature);
			}
		}
		
		
		

		// 2 提取人脸脸部周围的区域特征
		mpeg7_feature_color mMpeg7FeaturePtr;
		cv::Mat NormImg;
		GetColorRectFaceImg(img,EyePt,NormImg );

		mMpeg7FeaturePtr.feature_get(NormImg,FEATURE_COLORFUSION);

		string featureStr = FeatureExtractor_ConvertFeatureToString(vec_Feature,mMpeg7FeaturePtr);
		vFeatureStr.push_back(featureStr);
		FaceIndex++;
	}

	return vFeatureStr;
}
#endif


vector<string>  FaceAlbum::FeatureExtractor_Image( Mat &img,vector< EyeArray>&vec_Eye )
{
    vector<string> vFeatureStr;
    assert(mFeatureExtractorPtr);
    if (vec_Eye.size()==0)
    {
        return vFeatureStr;
    }
    

    int FaceIndex=0;
    EyeArray EyePt;
    Mat imageGray;
    
    if (img.channels() > 1)
    {
        cvtColor(img,imageGray,CV_BGR2GRAY);
    }
    

    while (FaceIndex < vec_Eye.size())
    {
        EyePt = vec_Eye[FaceIndex];
        vector< float> vec_Feature;
        double oneImgTime = -time(0);

		//1 提取人脸脸部特征
        if (imageGray.empty())  //要用灰度图进行特征提取
        {
            mFeatureExtractorPtr->FeatureGet(img,EyePt,vec_Feature);
        }
        else
        {
            mFeatureExtractorPtr->FeatureGet(imageGray,EyePt,vec_Feature);
        }

		// 2 提取人脸脸部周围的区域特征
		mpeg7_feature_color mMpeg7FeaturePtr;
		cv::Mat NormImg;
		GetColorRectFaceImg(img,EyePt,NormImg );

		mMpeg7FeaturePtr.feature_get(NormImg,FEATURE_COLORFUSION);
        

        oneImgTime +=time(0);

      //  printf("Feature extraction time  is %0.3lf s   \n",oneImgTime);
        
     //   string featureStr = FeatureExtractor_ConvertFloatToString(vec_Feature);
		string featureStr = FeatureExtractor_ConvertFeatureToString(vec_Feature,mMpeg7FeaturePtr);
        vFeatureStr.push_back(featureStr);
        FaceIndex++;
    }

    return vFeatureStr;
}

string FaceAlbum::FeatureExtractor_ConvertFloatToString( vector<float>&vec_Feature )
{

    string featureStr = mFeatureFlag;  //初始化标志位
    int featureNum = vec_Feature.size();
    assert(featureNum == FEATURE_NUM);
    //featureStr+=" ";
    char digitStr[100];
    sprintf(digitStr," %d",featureNum);
    featureStr+=digitStr;

    for (int i=0;i<featureNum;++i)
    {
        sprintf(digitStr," %0.6f",vec_Feature[i]);
        featureStr+=digitStr;
    }

    featureStr+="#";

    return featureStr;
}


 int FaceAlbum::rotationImageWithOrientation( Mat &Image,int orientation )
 {
     if (orientation == 0)
     {
         return 0;
     }
     if (orientation <1 || orientation > 8 )
     {
         cout<<"orientation is Error!!"<<endl;
         return  -1;
     }
     int Angle = 0;
     switch(orientation)
     {
     case 8:
         Angle = 90;
         break;
     case 3:
         Angle = 180;
         break;
     case 6:
         Angle = 270;
         break;
     default:
         return 0;
     }
     int flag = 1;
     

     int angle = 360-Angle;


    
    Mat dstMat = Mat::zeros(Image.cols,Image.rows,Image.type());

    float m[6];
    int w = Image.cols;
    int h = Image.rows;
    m[0] = (float) cos(flag * angle * CV_PI / 180.);
    m[1] = (float) sin(flag * angle * CV_PI / 180.);
    m[3] = -m[1];
    m[4] = m[0];
    // 将旋转中心移至图像中间
    m[2] = w * 0.5f;
    m[5] = h * 0.5f;

    CvMat M = cvMat(2, 3, CV_32F, m);
    cvGetQuadrangleSubPix(&(IplImage)Image, &(IplImage)dstMat, &M);

    Image = dstMat.clone();
    return Angle;
 }

string FaceAlbum::FeatureExtractor_ConvertFeatureToString( vector<float>&vec_Feature ,mpeg7_feature_color &mMpeg7FeaturePtr)
{

	string featureStr = mFeatureFlag;  //初始化标志位
	int featureNum = vec_Feature.size();
	assert(featureNum == FEATURE_NUM);
	//featureStr+=" ";
	char digitStr[100];
	sprintf(digitStr," %d",featureNum);
	featureStr+=digitStr;

	for (int i=0;i<featureNum;++i)
	{
		sprintf(digitStr," %0.6f",vec_Feature[i]);
		featureStr+=digitStr;
	}

	featureStr+=" ";

	// add the image feature
	int nFeaImgNum = mMpeg7FeaturePtr.vec_y_coeff_Layout.size()+mMpeg7FeaturePtr.vec_cb_coeff_Layout.size()+mMpeg7FeaturePtr.vec_cr_coeff_Layout.size()+mMpeg7FeaturePtr.vec_colorstructure.size();
	sprintf(digitStr,"%d",nFeaImgNum);
	featureStr+=digitStr;
	for (int i=0;i< mMpeg7FeaturePtr.vec_y_coeff_Layout.size();++i)
	{
		sprintf(digitStr," %d",mMpeg7FeaturePtr.vec_y_coeff_Layout[i]);
		featureStr+=digitStr;
	}
	for (int i=0;i< mMpeg7FeaturePtr.vec_cb_coeff_Layout.size();++i)
	{
		sprintf(digitStr," %d",mMpeg7FeaturePtr.vec_cb_coeff_Layout[i]);
		featureStr+=digitStr;
	}
	for (int i=0;i< mMpeg7FeaturePtr.vec_cr_coeff_Layout.size();++i)
	{
		sprintf(digitStr," %d",mMpeg7FeaturePtr.vec_cr_coeff_Layout[i]);
		featureStr+=digitStr;
	}
	for (int i=0;i< mMpeg7FeaturePtr.vec_colorstructure.size();++i)
	{
		sprintf(digitStr," %d",mMpeg7FeaturePtr.vec_colorstructure[i]);
		featureStr+=digitStr;
	}
//	featureStr+="";

	return featureStr;
}



void FaceAlbum::ReleaseFaceTagging()
{
    if (mFaceDetectPtr)
    {
        mFaceDetectPtr->destroy_LUTDetect();
        delete mFaceDetectPtr;
        mFaceDetectPtr=NULL;
    }
//     if (mEyeDetectPtr)
//     {
//         delete mEyeDetectPtr;
//         mEyeDetectPtr =NULL;
//     }
    
    if (mFeatureExtractorPtr)
    {
        delete mFeatureExtractorPtr;
        mFeatureExtractorPtr = NULL;
    }




}
int FaceAlbum::RunFaceTaggingFromBuffer(cv::Mat &srcImg,int doType /*= doFaceAndEye_Detect*/)
{
	DestroyImageInfoMemory();
	m_resImageInfo.mFaceNum = 0;
	m_resImageInfo.mFace_version = 1001;
	m_resImageInfo.mFaceInfo = NULL;


#ifdef PRINT_LOG
	m_ofs_log<<"RunFaceTaggingFromBuffer func step into 1"<<endl;
#endif

	if (srcImg.data ==NULL || srcImg.cols<=0 || srcImg.rows <=0 || srcImg.channels() >4 || srcImg.channels() <= 0)
	{
		printf("the image  Load Error!----------RunFaceTaggingFromBuffer\n");
		return -1;
	}
	float fwidth = srcImg.cols;
	float fheight = srcImg.rows;


	if (doType == doFace_Detect)
	{
#ifdef PRINT_LOG
		m_ofs_log<<"1 FaceDetect_Image -----------------aa"<<endl;
#endif
		vector<XmRect> FaceRect = FaceDetect_Image(srcImg);
#ifdef PRINT_LOG
		m_ofs_log<<"1 FaceDetect_Image -----------------ss"<<endl;
#endif
		m_resImageInfo.mFaceNum = FaceRect.size();
		if(m_resImageInfo.mFaceNum>0)
		{
			m_resImageInfo.mFaceInfo = new FaceInfo[m_resImageInfo.mFaceNum];
			for (int i=0;i<FaceRect.size();++i)
			{
				m_resImageInfo.mFaceInfo[i].Face_X = FaceRect[i].x;
				m_resImageInfo.mFaceInfo[i].Face_Y = FaceRect[i].y;
				m_resImageInfo.mFaceInfo[i].Face_W = FaceRect[i].width;
				m_resImageInfo.mFaceInfo[i].Face_H = FaceRect[i].height;

				m_resImageInfo.mFaceInfo[i].FaceXScale = m_resImageInfo.mFaceInfo[i].Face_X/fwidth;
				m_resImageInfo.mFaceInfo[i].FaceYScale = m_resImageInfo.mFaceInfo[i].Face_Y/fheight;
				m_resImageInfo.mFaceInfo[i].FaceWScale = m_resImageInfo.mFaceInfo[i].Face_W/fwidth;
				m_resImageInfo.mFaceInfo[i].FaceHScale = m_resImageInfo.mFaceInfo[i].Face_H/fheight;

			}
		}

	}
	else  if (doType == doFaceAndEye_Detect)
	{
#ifdef PRINT_LOG
		m_ofs_log<<"2 FaceDetect_Image -----------------aa"<<endl;
#endif
		vector<XmRect> FaceRect = FaceDetect_Image(srcImg);
#ifdef PRINT_LOG
		m_ofs_log<<"2 FaceDetect_Image -----------------ss"<<endl;
#endif

		if(FaceRect.size()<=0)
			return 0;

#ifdef PRINT_LOG
		m_ofs_log<<"3 GetLandMarkPoints_Image -----------------aa"<<endl;
#endif
		mkeyPointsLandMark.clear();
		vector<EyeArray> EyePt;
		EyePt.clear();
		if(FaceRect.size() >0 )
		{
			mkeyPointsLandMark= GetLandMarkPoints_Image(srcImg,FaceRect);

			EyePt = EyeDetect_Image(srcImg,mkeyPointsLandMark);
		}

#ifdef PRINT_LOG
		m_ofs_log<<"3 GetLandMarkPoints_Image -----------------ss"<<endl;
#endif

		//EyeDetect_Image
		m_resImageInfo.mFaceNum =  FaceRect.size();
		if(FaceRect.size()>0)
		{
			m_resImageInfo.mFaceInfo = new FaceInfo[m_resImageInfo.mFaceNum];

			for (int i=0;i<FaceRect.size();++i)
			{
				
				m_resImageInfo.mFaceInfo[i].Face_X = FaceRect[i].x;
				m_resImageInfo.mFaceInfo[i].Face_Y = FaceRect[i].y;
				m_resImageInfo.mFaceInfo[i].Face_W = FaceRect[i].width;
				m_resImageInfo.mFaceInfo[i].Face_H = FaceRect[i].height;

				m_resImageInfo.mFaceInfo[i].FaceXScale = m_resImageInfo.mFaceInfo[i].Face_X/fwidth;
				m_resImageInfo.mFaceInfo[i].FaceYScale = m_resImageInfo.mFaceInfo[i].Face_Y/fheight;
				m_resImageInfo.mFaceInfo[i].FaceWScale = m_resImageInfo.mFaceInfo[i].Face_W/fwidth;
				m_resImageInfo.mFaceInfo[i].FaceHScale = m_resImageInfo.mFaceInfo[i].Face_H/fheight;

				m_resImageInfo.mFaceInfo[i].Eye_left_x = EyePt[i].x_left;
				m_resImageInfo.mFaceInfo[i].Eye_left_y = EyePt[i].y_left;
				m_resImageInfo.mFaceInfo[i].Eye_right_x = EyePt[i].x_right;
				m_resImageInfo.mFaceInfo[i].Eye_right_y = EyePt[i].y_right;

				m_resImageInfo.mFaceInfo[i].EyeLeftXScale = m_resImageInfo.mFaceInfo[i].Eye_left_x/fwidth;
				m_resImageInfo.mFaceInfo[i].EyeLeftYScale = m_resImageInfo.mFaceInfo[i].Eye_left_y/fheight;
				m_resImageInfo.mFaceInfo[i].EyeRightXScale = m_resImageInfo.mFaceInfo[i].Eye_right_x/fwidth;
				m_resImageInfo.mFaceInfo[i].EyeRightYScale = m_resImageInfo.mFaceInfo[i].Eye_right_y/fheight;


				m_resImageInfo.mFaceInfo[i].faceFeature="0000";
				
			}
		}

	}
	else if (doType == doFaceAndEyeDetect_FeatureExtractor)
	{
#ifdef PRINT_LOG
	m_ofs_log<<"3 FaceDetect_Image -----------------aa"<<endl;
#endif
		vector<XmRect> FaceRect = FaceDetect_Image(srcImg);
#ifdef PRINT_LOG
	m_ofs_log<<"3 FaceDetect_Image -----------------ss"<<endl;
#endif
#ifdef PRINT_LOG
	m_ofs_log<<"3 GetLandMarkPoints_Image -----------------aa"<<endl;
#endif
	mkeyPointsLandMark.clear();
	vector<EyeArray> EyePt;
	EyePt.clear();
	if(FaceRect.size() >0 )
	{
		mkeyPointsLandMark= GetLandMarkPoints_Image(srcImg,FaceRect);

	    EyePt = EyeDetect_Image(srcImg,mkeyPointsLandMark);
	}
	
#ifdef PRINT_LOG
	m_ofs_log<<"3 GetLandMarkPoints_Image -----------------ss"<<endl;
#endif


		vector<string> FeatureStr;
		FeatureStr.clear();
#ifdef CORRECT_3D
		vector<Image3dWarping> image3dstr = ImageCorrect3d(srcImg,LandMarkpoints,FaceRect);
		FeatureStr = FeatureExtractor_Image(srcImg,image3dstr,EyePt);
#else
#ifdef PRINT_LOG
	m_ofs_log<<"3 FeatureExtractor_Image -----------------aa"<<endl;
#endif
	if(EyePt.size()>0)
		FeatureStr = FeatureExtractor_Image(srcImg,EyePt);

#ifdef PRINT_LOG
	m_ofs_log<<"3 FeatureExtractor_Image -----------------ss"<<endl;
#endif
#endif


	if(FaceRect.size() !=EyePt.size() ||FaceRect.size()<=0 )
	{
		return -1;
	}
	    m_resImageInfo.mFaceNum =  FaceRect.size();
		if(FaceRect.size()>0)
		{
			m_resImageInfo.mFaceInfo = new FaceInfo[m_resImageInfo.mFaceNum];
			for (int i=0;i<FaceRect.size();++i)
			{
				
				m_resImageInfo.mFaceInfo[i].Face_X = FaceRect[i].x;
				m_resImageInfo.mFaceInfo[i].Face_Y = FaceRect[i].y;
				m_resImageInfo.mFaceInfo[i].Face_W = FaceRect[i].width;
				m_resImageInfo.mFaceInfo[i].Face_H = FaceRect[i].height;

				m_resImageInfo.mFaceInfo[i].FaceXScale = m_resImageInfo.mFaceInfo[i].Face_X/fwidth;
				m_resImageInfo.mFaceInfo[i].FaceYScale = m_resImageInfo.mFaceInfo[i].Face_Y/fheight;
				m_resImageInfo.mFaceInfo[i].FaceWScale = m_resImageInfo.mFaceInfo[i].Face_W/fwidth;
				m_resImageInfo.mFaceInfo[i].FaceHScale = m_resImageInfo.mFaceInfo[i].Face_H/fheight;

				m_resImageInfo.mFaceInfo[i].Eye_left_x = EyePt[i].x_left;
				m_resImageInfo.mFaceInfo[i].Eye_left_y = EyePt[i].y_left;
				m_resImageInfo.mFaceInfo[i].Eye_right_x = EyePt[i].x_right;
				m_resImageInfo.mFaceInfo[i].Eye_right_y = EyePt[i].y_right;

				m_resImageInfo.mFaceInfo[i].EyeLeftXScale = m_resImageInfo.mFaceInfo[i].Eye_left_x/fwidth;
				m_resImageInfo.mFaceInfo[i].EyeLeftYScale = m_resImageInfo.mFaceInfo[i].Eye_left_y/fheight;
				m_resImageInfo.mFaceInfo[i].EyeRightXScale = m_resImageInfo.mFaceInfo[i].Eye_right_x/fwidth;
				m_resImageInfo.mFaceInfo[i].EyeRightYScale = m_resImageInfo.mFaceInfo[i].Eye_right_y/fheight;

				m_resImageInfo.mFaceInfo[i].faceFeature = FeatureStr[i];
				
			}
		}
		
	}
#ifdef PRINT_LOG
	m_ofs_log<<"3 FeatureExtractor_Image  return"<<endl;
#endif

	return 0;

}

int FaceAlbum::RunFaceTaggingFromBuffer( unsigned char *imgBuffer,int width,int height,int widthStep,int nchannels,int doType /*= doFaceAndEye_Detect*/ )
{
	DestroyImageInfoMemory();
	m_resImageInfo.mFaceNum = 0;
	m_resImageInfo.mFace_version = 1001;
	m_resImageInfo.mFaceInfo = NULL;

    if (imgBuffer ==NULL || width<=0 || height <=0 || nchannels >4 || nchannels <= 0)
    {
        printf("the image  Load Error!----------RunFaceTaggingFromBuffer\n");
        return -1;
    }

    Mat srcImg;
    srcImg = Mat(height,width,CV_8UC(nchannels));
    srcImg.data = imgBuffer;
    srcImg.step = widthStep;

	return RunFaceTaggingFromBuffer(srcImg, doType );


    
}

int FaceAlbum::RunFaceTaggingFromPath( const string imagePath,int doType /*= doFaceAndEye_Detect*/ )
{
	DestroyImageInfoMemory();
	m_resImageInfo.mFaceNum = 0;
	m_resImageInfo.mFace_version = 1001;
	m_resImageInfo.mFaceInfo = NULL;


	Mat srcImg = imread(imagePath);
	if (srcImg.empty())
	{
		printf("the image %s Load Error!\n",imagePath.c_str());
		return -1;
	}

	return RunFaceTaggingFromBuffer(srcImg, doType );

}

int FaceAlbum::CreateFileHandle( )
{
	#ifdef PRINT_LOG	
#ifndef LINUX
	int KiD = _getpid();
	int nThreadID =GetCurrentThreadId();

	
	sprintf(m_logFileName,"D:/save/fe_%d_%d_log.txt",KiD,nThreadID);

#else
	int KiD = getpid();
	//int nThreadID =gettid();
	int nThreadID = pthread_self();
	char file_name[255];
	sprintf(m_logFileName,"/data/fe_%d_%d_log.txt",KiD,nThreadID);

#endif

	m_ofs_log.open(m_logFileName,ios::app);
	
	if(!m_ofs_log.good())
		return -1;
	
#endif
	return 0;
}
int  FaceAlbum::CloseFileHandle( )
{
	#ifdef PRINT_LOG
	m_ofs_log.close();
	#endif
	return 0;
}
ofstream  *FaceAlbum::GetFileHandle( )
{
	return &m_ofs_log;
}
void FaceAlbum::CopyStruct(const sReduced_Face_Info &sreducedInfo,FaceInfo &faceInfo)
{
	faceInfo.Face_X = sreducedInfo.Face_X; //人脸框1的X坐标
     faceInfo.Face_Y=sreducedInfo.Face_Y; //人脸框1的Y坐标
     faceInfo.Face_W=sreducedInfo.Face_W; //人脸框1的Width
     faceInfo.Face_H=sreducedInfo.Face_H;//人脸框1的Height

     faceInfo.FaceXScale=sreducedInfo.FaceXScale;
     faceInfo.FaceYScale=sreducedInfo.FaceYScale;
     faceInfo.FaceWScale=sreducedInfo.FaceWScale;
     faceInfo.FaceHScale=sreducedInfo.FaceHScale;


     faceInfo.Eye_left_x=sreducedInfo.Eye_left_x;// 人脸1的左眼X
     faceInfo.Eye_left_y=sreducedInfo.Eye_left_y;//人脸1的左眼Y

     faceInfo.Eye_right_x=sreducedInfo.Eye_right_x; //人脸1的右眼X
     faceInfo.Eye_right_y=sreducedInfo.Eye_right_y;//人脸1的右眼Y


     faceInfo.EyeLeftXScale=sreducedInfo.EyeLeftXScale;
     faceInfo.EyeLeftYScale=sreducedInfo.EyeLeftYScale;
     faceInfo.EyeRightXScale=sreducedInfo.EyeRightXScale;
     faceInfo.EyeRightYScale=sreducedInfo.EyeRightYScale;
}


int FaceAlbum::RunFaceTagging_ProcessExif( const unsigned char *jpgFileBuffer,int nBufSize,int doType /*= doFaceAndEye_Detect*/ )
{
	
	//m_resImageInfo.mFaceNum = 0;
	//vector<FaceInfo>().swap(m_resImageInfo.mFaceInfo); 
	//m_resImageInfo.mFace_version = 1001;
	DestroyImageInfoMemory();

	m_resImageInfo.mFaceNum = 0;
	m_resImageInfo.mFace_version = 1001;
	m_resImageInfo.mFaceInfo = NULL;



#ifdef PRINT_LOG
	m_ofs_log<<"RunFaceTagging_ProcessExif-----------------------------------------------------------------------------------start"<<endl;
#endif
    if (jpgFileBuffer ==NULL || nBufSize<=IMAGE_JPG_SIZE )
    {
#ifdef PRINT_LOG
	m_ofs_log<<"jpgFileBuffer=NULL || nBufSize<=IMAGE_JPG_SIZE"<<endl<<m_resImageInfo.mFaceNum<<endl<<m_resImageInfo.mFaceNum<<endl<<m_resImageInfo.mFace_version<<endl<<endl;
#endif
        return -1;
    }


#ifdef PRINT_LOG
	m_ofs_log<<"nBufSize="<<nBufSize<<endl;
	m_ofs_log<<"DecodeExif-----------------aa"<<endl;
#endif
	// Get the exif information
	Cexif loadExif;
	bool isLoad = loadExif.DecodeExif(jpgFileBuffer,m_ofs_log);
	if (!isLoad )
    {
#ifdef PRINT_LOG
	m_ofs_log<<"load exif error "<<endl;
#endif
    }
	
#ifdef PRINT_LOG
	m_ofs_log<<"DecodeExif -----------------ss"<<endl;
#endif

	 
	//利用opencv的decode buffer
	std::vector<unsigned char> data;
	int i;
	for (i=0; i<nBufSize; i++)
	{
		data.push_back(jpgFileBuffer[i]);
	}
	cv::Mat matrix_img= cv::imdecode(cv::Mat(data), cv::IMREAD_COLOR);
	if(matrix_img.cols<=0 ||matrix_img.rows<=0)
		return -1;

	
	
	EXIFINFO *exif = loadExif.m_exifinfo;
	
#ifdef PRINT_LOG
	m_ofs_log<<"rotationImageWithOrientation -----------------aa"<<endl;
#endif
    int Angle = rotationImageWithOrientation(matrix_img,exif->Orientation);


#ifdef PRINT_LOG
	m_ofs_log<<"rotationImageWithOrientation -----------------ss"<<endl;
#endif
	// matrix_img has been roated to the frontal
#ifdef PRINT_LOG
	m_ofs_log<<"RunFaceTaggingFromBuffer step into"<<endl;
#endif
	RunFaceTaggingFromBuffer(matrix_img,doType);
#ifdef PRINT_LOG
	m_ofs_log<<"RunFaceTaggingFromBuffer  step out"<<endl;
#endif
	
	switch (exif->Orientation)
	{
	case 8:  //90
		{
			int height_input= matrix_img.cols;
			int width_input= matrix_img.rows;

			for (int k = 0; k < m_resImageInfo.mFaceNum; k++)
			{
				sReduced_Face_Info sreducedInfo ;
			
				sreducedInfo.Face_W  =m_resImageInfo.mFaceInfo[k].Face_H;
				sreducedInfo.Face_H=m_resImageInfo.mFaceInfo[k].Face_W;
				sreducedInfo.Face_X = width_input-1-(m_resImageInfo.mFaceInfo[k].Face_Y+m_resImageInfo.mFaceInfo[k].Face_H) ;
				sreducedInfo.Face_Y= m_resImageInfo.mFaceInfo[k].Face_X;
				

				// matrix_img has been roated to the frontal   so the width and height should exchange
				sreducedInfo.FaceXScale =sreducedInfo.Face_X*1.0/width_input;
				sreducedInfo.FaceYScale=sreducedInfo.Face_Y*1.0/height_input;
				sreducedInfo.FaceWScale=sreducedInfo.Face_W*1.0/width_input;
				sreducedInfo.FaceHScale=sreducedInfo.Face_H*1.0/height_input;

				sreducedInfo.Eye_left_x = width_input-1-m_resImageInfo.mFaceInfo[k].Eye_left_y;// 人脸1的左眼X
				sreducedInfo.Eye_left_y= m_resImageInfo.mFaceInfo[k].Eye_left_x;//人脸1的左眼Y
				sreducedInfo.Eye_right_x= width_input-1-m_resImageInfo.mFaceInfo[k].Eye_right_y; //人脸1的右眼X
				sreducedInfo.Eye_right_y= m_resImageInfo.mFaceInfo[k].Eye_right_x;//人脸1的右眼Y

				sreducedInfo.EyeLeftXScale = sreducedInfo.Eye_left_x*1.0/width_input;
				sreducedInfo.EyeLeftYScale= sreducedInfo.Eye_left_y*1.0/height_input;
				sreducedInfo.EyeRightXScale= sreducedInfo.Eye_right_x*1.0/width_input;
				sreducedInfo.EyeRightYScale= sreducedInfo.Eye_right_y*1.0/height_input;

				CopyStruct(sreducedInfo,m_resImageInfo.mFaceInfo[k]);
				
			}
			break;
		}
	case 6: //270
		{
			int height_input= matrix_img.cols;
			int width_input= matrix_img.rows;
			for (int k = 0; k < m_resImageInfo.mFaceNum; k++)
			{
				
				sReduced_Face_Info sreducedInfo ;
				
				sreducedInfo.Face_W  =m_resImageInfo.mFaceInfo[k].Face_H;
				sreducedInfo.Face_H=m_resImageInfo.mFaceInfo[k].Face_W;
				sreducedInfo.Face_X = m_resImageInfo.mFaceInfo[k].Face_Y ;
				sreducedInfo.Face_Y= height_input-1-(m_resImageInfo.mFaceInfo[k].Face_X+m_resImageInfo.mFaceInfo[k].Face_W);
				

				// matrix_img has been roated to the frontal   so the width and height should exchange
				sreducedInfo.FaceXScale =sreducedInfo.Face_X*1.0/width_input;
				sreducedInfo.FaceYScale=sreducedInfo.Face_Y*1.0/height_input;
				sreducedInfo.FaceWScale=sreducedInfo.Face_W*1.0/width_input;
				sreducedInfo.FaceHScale=sreducedInfo.Face_H*1.0/height_input;

				sreducedInfo.Eye_left_x = m_resImageInfo.mFaceInfo[k].Eye_left_y;// 人脸1的左眼X
				sreducedInfo.Eye_left_y= height_input-1-m_resImageInfo.mFaceInfo[k].Eye_left_x;//人脸1的左眼Y
				sreducedInfo.Eye_right_x= m_resImageInfo.mFaceInfo[k].Eye_right_y; //人脸1的右眼X
				sreducedInfo.Eye_right_y= height_input-1-m_resImageInfo.mFaceInfo[k].Eye_right_x;//人脸1的右眼Y

				sreducedInfo.EyeLeftXScale = sreducedInfo.Eye_left_x*1.0/width_input;
				sreducedInfo.EyeLeftYScale= sreducedInfo.Eye_left_y*1.0/height_input;
				sreducedInfo.EyeRightXScale= sreducedInfo.Eye_right_x*1.0/width_input;
				sreducedInfo.EyeRightYScale= sreducedInfo.Eye_right_y*1.0/height_input;

				CopyStruct(sreducedInfo,m_resImageInfo.mFaceInfo[k]);
			}
			break;
		}
	case 3://180
		{
			int height_input= matrix_img.rows;
			int width_input= matrix_img.cols;
			for (int k = 0; k < m_resImageInfo.mFaceNum; k++)
			{
	
				sReduced_Face_Info sreducedInfo ;
				sreducedInfo.Face_W  =m_resImageInfo.mFaceInfo[k].Face_W;
				sreducedInfo.Face_H=m_resImageInfo.mFaceInfo[k].Face_H;
				    
				sreducedInfo.Face_X = width_input-1-(m_resImageInfo.mFaceInfo[k].Face_X+m_resImageInfo.mFaceInfo[k].Face_W) ;
				sreducedInfo.Face_Y=height_input-1- (m_resImageInfo.mFaceInfo[k].Face_Y+m_resImageInfo.mFaceInfo[k].Face_H) ;
				

				// matrix_img has been roated to the frontal   so the width and height should exchange
				sreducedInfo.FaceXScale =sreducedInfo.Face_X*1.0/width_input;
				sreducedInfo.FaceYScale=sreducedInfo.Face_Y*1.0/height_input;
				sreducedInfo.FaceWScale=sreducedInfo.Face_W*1.0/width_input;
				sreducedInfo.FaceHScale=sreducedInfo.Face_H*1.0/height_input;

				sreducedInfo.Eye_left_x = width_input-1-m_resImageInfo.mFaceInfo[k].Eye_left_x;// 人脸1的左眼X
				sreducedInfo.Eye_left_y= height_input-1-  m_resImageInfo.mFaceInfo[k].Eye_left_y;//人脸1的左眼Y
				sreducedInfo.Eye_right_x= width_input-1-m_resImageInfo.mFaceInfo[k].Eye_right_x; //人脸1的右眼X
				sreducedInfo.Eye_right_y= height_input-1-  m_resImageInfo.mFaceInfo[k].Eye_right_y;//人脸1的右眼Y

				sreducedInfo.EyeLeftXScale = sreducedInfo.Eye_left_x*1.0/width_input;
				sreducedInfo.EyeLeftYScale= sreducedInfo.Eye_left_y*1.0/height_input;
				sreducedInfo.EyeRightXScale= sreducedInfo.Eye_right_x*1.0/width_input;
				sreducedInfo.EyeRightYScale= sreducedInfo.Eye_right_y*1.0/height_input;

				CopyStruct(sreducedInfo,m_resImageInfo.mFaceInfo[k]);
			}
			break;
		}
	default:
		break;
	}
	
#ifdef PRINT_LOG
	m_ofs_log<<"RunFaceTagging_ProcessExif-----------------------------------------------------------------------------------over "<<endl<<endl<<endl<<endl;
#endif	
	return 0;
}
string FaceAlbum::getVersion()
{
    return mVersion;
}

int FaceAlbum::GetProcessResult(ImageInfo &info)
{
	info.mFaceNum = m_resImageInfo.mFaceNum;
	info.mImageFlag = m_resImageInfo.mImageFlag;
	info.mFace_version= m_resImageInfo.mFace_version;
	info.mFaceInfo= m_resImageInfo.mFaceInfo;


	return 0;
}





