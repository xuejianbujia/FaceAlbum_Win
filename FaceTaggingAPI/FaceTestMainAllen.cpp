#include "FaceIncludeHeader.h"

#include "FaceAlbum.h"
#include"Roca.h"
#include<windows.h>
#include <omp.h>

#ifndef MIN
#  define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#  define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif


#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif







//#define CORRECT_3D  

#ifdef CORRECT_3D
#include "FormModel.h"
#endif

#ifdef UserFace

//#define FaceDetect_FFP
//#define TEST_EYE_BY_FFPFace
//#define  FaceDetect_picasaRect

typedef struct _FACEINFO
{
    String ClssifierDir;
    String ClusterAlbumDir;
    

    String modelPath;
    String FaceEyeDir;
    String imageList;

    String FeaturePath;
    String FeatureList;
    String FaceList;
    String ClusterGroundTruthFaceList;

    String SaveClusterPath;
    String SaveClusterDir;

	vector<cv::Point> LandMark;
    int nFaceDetect;
    int detectMode;
    int nEyeFlag;
    int nFeaFlag;
    int nClusterFlag;
    int incrementClusterFlag;
    int nRecommendClusterFlag;
}FACEINFO;

FACEINFO gFaceInfo;

FaceAlbum gFaceAlbum;

int gThreadNum=6;
FaceAlbum gFaceAlbum_testMul[6];



string g_FFP_path="M:/zhangtao/Cluster/UserLabel/MFFP/";
int gFaceNum=0;

//#define shunfeng_detect
vector<string> FaceList;

string FaceDirName = "newFace";
string gFaceDir;
string g_nativepath="";
template<typename T>
int OverLapTwoRect(T rc1,T rc2,float ratio)
{
	int x1 = MAX(rc1.x,rc2.x);
	int y1 = MAX(rc1.y,rc2.y);
	int x2 = MIN(rc1.x+rc1.width-1,rc2.x+rc2.width-1);
	int y2 = MIN(rc1.y+rc1.height-1,rc2.y+rc2.height-1);

	if (x1 >=x2 || y1>=y2)
	{
		return 0;
	}
	else
	{

		int s1 = rc1.width*rc1.height;
		int s2 = rc2.width*rc2.height;
		int s3 = (x2-x1+1)*(y2-y1+1);
		if (s3>=ratio*s2 || s3>=ratio*s1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
		
	}
	
}


typedef struct Handle_Str
{
	int nID;
	string num_name;
	unsigned char *pFileBuf;
	FaceAlbum *pFaceAlbum;
	int nBufSize;
	ImageInfo info;
}Handle_Str;


DWORD WINAPI  FillPC_Thread(LPVOID pParam)  
{  

	Handle_Str *pHandleTH =(Handle_Str *)pParam;
	unsigned char *pFileBuf =pHandleTH->pFileBuf;
	FaceAlbum *ppFaceAlbum = pHandleTH->pFaceAlbum;
	int nBufSize =pHandleTH->nBufSize;
	int nID = pHandleTH->nID;
	ImageInfo info;
	ppFaceAlbum->RunFaceTagging_ProcessExif( pFileBuf,nBufSize,2);
	ppFaceAlbum->GetProcessResult( info);
	//ImageInfo &info = *(ppFaceAlbum->RunFaceTagging_ProcessExif( pFileBuf,nBufSize,2));
	{
		char str[255];
		sprintf(str,"D://log_mul//%d.txt",nID);
		ofstream ofs(str);
		for (int ik = 0; ik < info.mFaceNum; ik++)
		{
			ofs << info.mFaceInfo[ik].Eye_left_x<<" "<< info.mFaceInfo[ik].Eye_left_y<<" "<< info.mFaceInfo[ik].Eye_right_x<<" "<< info.mFaceInfo[ik].Eye_right_y<<endl
				<< info.mFaceInfo[ik].faceFeature<<endl;
		}
		ofs.close();

		printf("%d process finish\n",nID);
		
	}
	return 0;
}  

void Process_MultiImgByMultiThread(string img_list)
{
	ifstream ifs(img_list);
	string ss_name;
	vector<string> vec_name;
	while (ifs>>ss_name)
	{
		vec_name.push_back(ss_name);
	}
	ifs.close();


	
	DWORD *dwThreadId= new DWORD[gThreadNum]; 
	HANDLE     *hThrd= new HANDLE[gThreadNum];   // thread handle
	Handle_Str *pParam= new Handle_Str[gThreadNum];   
	
	int nCount=0;
	while (nCount<(vec_name.size()-gThreadNum))
	{
		int k=0;
		for (k=0; k<gThreadNum; k++)
		{
			//获取文件的大小
			ifstream in(vec_name[nCount+k].c_str());   
			in.seekg(0,ios::end);   
			int size = in.tellg();   
			in.close();   

			unsigned char *pFileBuf =new unsigned char [size];
			FILE *pFImg = fopen(vec_name[nCount+k].c_str(),"rb");
			if (!pFImg)	
			{
				return;
			}
			else
			{
				fread(pFileBuf,1,size,pFImg);
			}
			fclose(pFImg);

			pParam[k].nID = nCount+k;
			pParam[k].pFaceAlbum =&(gFaceAlbum_testMul[k]) ;
			pParam[k].num_name=vec_name[nCount+k].c_str();
			pParam[k].pFileBuf=pFileBuf;
			pParam[k].nBufSize =size;
			

			hThrd[k] = CreateThread(NULL, 0 , FillPC_Thread,&(pParam[k]), 0, &(dwThreadId[k])) ; 

		}

		for (k=0; k<gThreadNum; k++)
		{
			WaitForSingleObject(hThrd[k],INFINITE);
		}

		for (k=0; k<gThreadNum; k++)
		{
			CloseHandle(hThrd[k]);
		}
		nCount =nCount+gThreadNum;
	}
	
}


int WriteDebugInfo(string image_name,ImageInfo &info )
{
	string tmpPath = image_name;
	int npos = tmpPath.rfind('/',tmpPath.length());
	if(npos<0)
	{
		npos = tmpPath.rfind('\\',tmpPath.length());
	}
	string tmpname2 =tmpPath.substr(0,npos);
	int npos2 = tmpname2.rfind('/',tmpname2.length());
	if(npos2<0)
	{
		npos2 = tmpname2.rfind('\\',tmpname2.length());
	}
	if(npos <0 || npos<0)
		return -1;
	string imagetmpName = tmpPath.substr(npos+1,tmpPath.length()-npos-4);
	g_nativepath=tmpname2.substr(0,npos2);
	string gFaceDir = g_nativepath+"/"+FaceDirName+"/";
	 _mkdir(gFaceDir.c_str());
	cv::Mat image=cv::imread(image_name);	
	for (int i=0;i<info.mFaceNum;++i)
	{
		char buf[10];
		sprintf(buf,"_%d",i);

		EyeArray EyePt;
		EyePt.x_left = info.mFaceInfo[i].Eye_left_x;
		EyePt.y_left = info.mFaceInfo[i].Eye_left_y;
		EyePt.x_right = info.mFaceInfo[i].Eye_right_x;
		EyePt.y_right = info.mFaceInfo[i].Eye_right_y;
		//saveFaceImgWithEye(image,EyePt,gFaceDir+imagetmpName+buf+".jpg");
		Rect rc1;
		rc1.x = info.mFaceInfo[i].Face_X;
		rc1.y = info.mFaceInfo[i].Face_Y;
		rc1.width = info.mFaceInfo[i].Face_W;
		rc1.height = info.mFaceInfo[i].Face_H;
		saveFaceImgWithEyeAndLandMark(image, rc1,EyePt,gFaceAlbum.mkeyPointsLandMark[i] ,gFaceDir+imagetmpName+buf+".jpg");
		FaceList.push_back(gFaceDir+imagetmpName+buf+".jpg");

		saveEyePtToFile(gFaceDir+imagetmpName+buf+".FFP",EyePt);

		saveFeatureToFile(gFaceDir+imagetmpName+buf+".fea",info.mFaceInfo[i].faceFeature);


	}

	
	return 0;
}

void Process_MultiImgBySingleThread(string img_list)
{
	ifstream ifs(img_list);
	string ss_name;
	vector<string> vec_name;
	while (ifs>>ss_name)
	{
		vec_name.push_back(ss_name);
	}
	ifs.close();
	string s_save_log ;
	int npos = img_list.rfind('/',img_list.length());
			if(npos<0)
			{
				npos = img_list.rfind('\\',img_list.length());
			}
			 s_save_log = img_list.substr(npos+1,img_list.length()-npos-1);
	
	EyeRoc eyeRoc;
	for (int i = 0; i < vec_name.size(); i++)
	{
		//获取文件的大小
		printf("%d is processing\n",i);

		ifstream in(vec_name[i].c_str());   
		in.seekg(0,ios::end);   
		int size = in.tellg();   
		in.close();   

		unsigned char *pFileBuf =new unsigned char [size];
		FILE *pFImg = fopen(vec_name[i].c_str(),"rb");
		if (!pFImg)	
		{
			return;
		}
		else
		{
			fread(pFileBuf,1,size,pFImg);
		}
		fclose(pFImg);

			gFaceAlbum.RunFaceTagging_ProcessExif( pFileBuf,size,2);

	ImageInfo info;
	gFaceAlbum.GetProcessResult(info);

	
		//WriteDebugInfo(vec_name[i].c_str(),info );

		//统计眼睛的错误率
		if (false)
		{
			string tmpPath = vec_name[i];
			int npos = tmpPath.rfind('/',tmpPath.length());
			if(npos<0)
			{
				npos = tmpPath.rfind('\\',tmpPath.length());
			}
			 
			string tmpname2 =tmpPath.substr(0,npos);
			int npos2 = tmpname2.rfind('/',tmpname2.length());
			if(npos2<0)
			{
				npos2 = tmpname2.rfind('\\',tmpname2.length());
			}
			string tmpname3 =tmpname2.substr(0,npos2);
			int npos3 = tmpname3.rfind('/',tmpname3.length());
			if(npos3<0)
			{
				npos3 = tmpname3.rfind('\\',tmpname3.length());
			}
			string ss1 = tmpPath.substr(npos3+1,tmpPath.length()-npos3-1);
			string ffp_name = g_FFP_path+ss1;
			ffp_name = ffp_name.replace(ffp_name.length()-4,4,".mffp");

			ifstream ifs_mffp(ffp_name);
			if(!ifs_mffp.good())
				continue;
			vector<EyeArray> vec_ffp;
			int num_ffp ;
			ifs_mffp>>num_ffp;
			for (int mki = 0; mki < num_ffp; mki++)
			{
				EyeArray pt_ffp;
				ifs_mffp>>pt_ffp.x_left>>pt_ffp.y_left>>pt_ffp.x_right>>pt_ffp.y_right;
				vec_ffp.push_back(pt_ffp);
			}
			ifs_mffp.close();
			if (info.mFaceNum>0)
			{
				for (int km = 0; km < vec_ffp.size(); km++)
				{
					EyeArray pt_1 = vec_ffp[km];
					if(pt_1.x_right <pt_1.x_left)
					{
						int x_a = pt_1.x_right;
						int y_a = pt_1.y_right;

						pt_1.x_right =pt_1.x_left;
						pt_1.y_right =pt_1.y_left;
						pt_1.x_left = x_a;
						pt_1.y_left = y_a;
					}
					
					
					float NormDis =sqrtf( (pt_1.x_left - pt_1.x_right)*(pt_1.x_left - pt_1.x_right)
						+ (pt_1.y_left - pt_1.y_right)*(pt_1.y_left - pt_1.y_right) );

					Rect r1;
					int c_1_x = (pt_1.x_right+pt_1.x_left)/2;
					int c_1_y = (pt_1.y_right+pt_1.y_left)/2;
					r1.x = c_1_x-NormDis/2 ;
					r1.y = c_1_y-NormDis/2 ;
					r1.width = NormDis ;
					r1.height = r1.width;
					for (int kn = 0; kn < info.mFaceNum; kn++)
					{
						EyeArray pt_2;
						pt_2.x_left = info.mFaceInfo[kn].Eye_left_x;
						pt_2.y_left = info.mFaceInfo[kn].Eye_left_y;
						pt_2.x_right = info.mFaceInfo[kn].Eye_right_x;
						pt_2.y_right = info.mFaceInfo[kn].Eye_right_y;
						if(pt_2.x_right <pt_2.x_left)
						{
							int x_a = pt_2.x_right;
							int y_a = pt_2.y_right;

							pt_2.x_right =pt_2.x_left;
							pt_2.y_right =pt_2.y_left;
							pt_2.x_left = x_a;
							pt_2.y_left = y_a;
						}
						Rect r2;
						int c_2_x = (pt_2.x_right+pt_2.x_left)/2;
					   int c_2_y = (pt_2.y_right+pt_2.y_left)/2;
					   int NormDis_2 =sqrtf( (pt_2.x_left - pt_2.x_right)*(pt_2.x_left - pt_2.x_right)
						+ (pt_2.y_left - pt_2.y_right)*(pt_2.y_left - pt_2.y_right) );
					   r2.x = c_2_x-NormDis_2/2 ;
					r2.y = c_2_y-NormDis_2/2 ;
					r2.width = NormDis_2 ;
					r2.height = r2.width;
						int nres = OverLapTwoRect(r1,r2,0.5);
						if (nres==1)
						{
							float d1 = sqrtf ((pt_2.x_right-pt_1.x_right)*(pt_2.x_right-pt_1.x_right) +(pt_2.y_right-pt_1.y_right)*(pt_2.y_right-pt_1.y_right) );
							float d2 = sqrtf ((pt_2.x_left-pt_1.x_left)*(pt_2.x_left-pt_1.x_left) +(pt_2.y_left-pt_1.y_left)*(pt_2.y_left-pt_1.y_left) );
							float fError =MAX(d1,d2)/(NormDis+0.00001f);
							printf("The %d image  eye error is %f\n ",i,fError);
							eyeRoc.SetRocData(fError);
							break;
						}
					}
				}
			}

			
		}
		
	}
	/*
	double fNormalError = eyeRoc.GetRocStatiscal(ROC_EYE_SUM_NORMAL);
	double fPenalityError = eyeRoc.GetRocStatiscal(ROC_EYE_SUM_PENALITY);
	{
		char str[255];
		sprintf(str,"D:/log_new/%s",s_save_log.c_str());
		ofstream ofs_error(str,ios::trunc);
		if (!ofs_error.good())
		{
			printf("Error File open fail\n");
		}
		ofs_error<<eyeRoc.m_HistNum<<endl;
		ofs_error<<fNormalError<<endl;
		ofs_error<<fPenalityError<<endl;
		double ratio5=0;
		double ratio10=0;
		for (int mnk = 0; mnk < 5; mnk++)
		{
			ratio5 +=eyeRoc.m_HistBin[mnk];
		}
		for (int mnk = 0; mnk < 10; mnk++)
		{
			ratio10 +=eyeRoc.m_HistBin[mnk];
		}
		ofs_error<<ratio5*1.0f/eyeRoc.m_HistNum<<endl;
		ofs_error<<ratio10*1.0f/eyeRoc.m_HistNum<<endl;
		ofs_error.close();
	}
	{
        string FaceListPath = g_nativepath+"\\"+FaceDirName+".txt";

        ofstream fout;
        fout.open(FaceListPath.c_str(),ios::trunc);
        for (int i=0;i<FaceList.size();++i)
        {
            fout<<FaceList[i]<<endl;
        }
        fout.close();
        FaceList.clear();
    }
	*/
	
	
	
}

void processOneImage_emulateCloud(string userDir,string RelativePath)
{
	string imagePath = userDir + "\\" + RelativePath;
    double oneImgTime;
    int pos_start = RelativePath.rfind("\\");
    int pos_end = RelativePath.rfind(".");

    string imageName =RelativePath.substr(pos_start+1,pos_end - pos_start - 1);

	imagePath="M:/zhangtao/face0.jpg";
	//获取文件的大小
	ifstream in(imagePath.c_str());   
	in.seekg(0,ios::end);   
	int size = in.tellg();   
	in.close();   

	unsigned char *pFileBuf =new unsigned char [size];
    FILE *pFImg = fopen(imagePath.c_str(),"rb");
    if (!pFImg)	
    {
        return;
    }
	else
	{
		fread(pFileBuf,1,size,pFImg);
	}
	fclose(pFImg);
 
	
	gFaceAlbum.RunFaceTagging_ProcessExif( pFileBuf,size,2);

	ImageInfo info;
	gFaceAlbum.GetProcessResult(info);
	
	for (int i = 0; i < info.mFaceNum; i++)
	{
		static int k123=0;
		char str[255];
		sprintf(str,"C:/%d.txt",k123++);
		FILE *fd =fopen(str,"w");
		for (int k = 0; k < info.mFaceInfo[i].faceFeature.length(); k++)
		{
			fprintf(fd,"%c ",info.mFaceInfo[i].faceFeature[k]);
		}
		fclose(fd);
	}
	
	delete []pFileBuf;
    //Mat image = imread(imagePath);
	//Mat image = imread(imagePath);
	//
	//gFaceAlbum.RunFaceTaggingFromBuffer( image.data,image.cols,image.rows,image.step,image.channels(),2);
	
	static int kk=0;
	printf("%d finished processs\n",kk++);

}


void processOneImage(string userDir,string RelativePath)
{
    string imagePath = userDir + "\\" + RelativePath;
    double oneImgTime;
    int pos_start = RelativePath.rfind("\\");
    int pos_end = RelativePath.rfind(".");

    string imageName =RelativePath.substr(pos_start+1,pos_end - pos_start - 1);



	//imagePath
	
    Mat image = imread(imagePath);
	//Mat image = imread("E:/work/new_cluster/Linux/FaceTagging_Centos64/data/1.jpg");
	


    Mat srcImage = image;
    if (image.empty())
    {
        cout<<"load image error :"<<imagePath<<endl;
        return;
    }

	
	//获取文件的大小
	ifstream in(imagePath.c_str());   
	in.seekg(0,ios::end);   
	int size = in.tellg();   
	in.close();   

	unsigned char *pFileBuf =new unsigned char [size];

    Cexif loadExif;

	
    FILE *pFImg = fopen(imagePath.c_str(),"rb");
    if (!pFImg)	
    {
        //continue;
        return;
    }
	else
	{
		fread(pFileBuf,1,size,pFImg);
	}
	fclose(pFImg);
    bool isLoad = loadExif.DecodeExif(pFileBuf);

	gFaceAlbum.RunFaceTagging_ProcessExif( pFileBuf,size,2);
	

	delete []pFileBuf;
   
    cv::Mat imageGray ;

    if (image.channels() == 3)
    {
        cvtColor(image,imageGray,CV_BGR2GRAY);
    }
    else
    {
        imageGray=image.clone();
    }
     
    vector< EyeArray > vec_Eye;
    vector<XmRect>face_Rect;
	vector< vector<Point2f> >LandMarkpoints;

    string FaceDir = userDir+"\\"+FaceDirName+"\\";

    int width = image.cols;
    int height = image.rows;
#ifdef FaceDetect_FFP

    EyeArray eyePt;

    for (int ii=0;ii<30;++ii)
    {
        char buf[10];
        sprintf(buf,"_%d",ii);
        string imageName_Face = imageName+buf;
        string ffpPath =  gFaceInfo.FeaturePath+imageName_Face+".FFP";

#ifdef TEST_EYE_BY_FFPFace
		if( readFaceFFPFromFile(ffpPath,eyePt) != -1)
		{
			
			int nDis = sqrtf((eyePt.x_left - eyePt.x_right)*(eyePt.x_left - eyePt.x_right) +(eyePt.y_left - eyePt.y_right)*(eyePt.y_left - eyePt.y_right) );
			cv::Rect roi;
			int iy_c = (eyePt.y_left+eyePt.y_right)>>1;
			roi.x = MAX(eyePt.x_left-2*nDis,0);
			roi.y = MAX(iy_c-2*nDis,0);
			int irx = MIN(eyePt.x_right+2*nDis,image.cols-1);
			int iry = MIN(iy_c+4*nDis,image.rows-1);
			roi.width =irx-roi.x+1;
			roi.height =iry-roi.y+1;

			//check the face if match
			XmRect norm_Rect;
			XmRect detect_Rect;

			norm_Rect.x = MAX(eyePt.x_left-nDis,0);
			norm_Rect.y = MAX(iy_c-nDis/2,0);
			irx = MIN(eyePt.x_right+nDis,image.cols-1);
			iry = MIN(iy_c+3*nDis/2,image.rows-1);
			norm_Rect.width =irx-norm_Rect.x+1;
			norm_Rect.height =iry-norm_Rect.y+1;
			norm_Rect.facePose[0]=0;
			norm_Rect.facePose[1]=0;
			norm_Rect.facePose[2]=0;

			int nOverFlag;
			if (roi.width<=0 ||roi.height<=0)
			{
				printf("ndis=%d",nDis);
				nOverFlag =0;
				face_Rect.push_back(norm_Rect);
				vec_Eye.push_back(eyePt);
				continue;
			}
			cv::Mat roiImage =image(roi);
			
			vector<XmRect>face_Rect_tmp =  gFaceAlbum.FaceDetect_Image(roiImage);
			
			
			

			
			
			if (face_Rect_tmp.size()!=1)
			{
				nOverFlag =0;
			}
			else
			{
				cv::Rect r_det;
				r_det.x = face_Rect_tmp[0].x;
				r_det.y = face_Rect_tmp[0].y;
				r_det.width = face_Rect_tmp[0].width;
				r_det.height = face_Rect_tmp[0].height;
				cv::Mat mat_detect_second = roiImage(r_det);
// 				char buf[10];
// 				sprintf(buf,"_%d",ii);
// 				string s2 = gFaceDir+imageName+buf+"_detect.jpg";
// 				string s3 = gFaceDir+imageName+buf+"_roi.jpg";
// 				imwrite(s2,mat_detect_second);
// 				imwrite(s3,roiImage);

				detect_Rect.x =roi.x +face_Rect_tmp[0].x ;
				detect_Rect.y =roi.y +face_Rect_tmp[0].y ;
				detect_Rect.width =face_Rect_tmp[0].width ;
				detect_Rect.height =face_Rect_tmp[0].height ;

				nOverFlag = OverLapTwoRect(norm_Rect,detect_Rect,0.4);
			}
			if (nOverFlag == 1 )
			{
				face_Rect.push_back(detect_Rect);

				vector<XmRect> face_Rect_tmp ;
				face_Rect_tmp.push_back(detect_Rect);
				vector< vector<Point2f> > LandMarkpoints_tmp = gFaceAlbum.GetLandMarkPoints_Image(imageGray,face_Rect_tmp);
				vector< EyeArray > vec_Eye_tmp = gFaceAlbum.EyeDetect_Image(imageGray,LandMarkpoints_tmp);
				LandMarkpoints.push_back(LandMarkpoints_tmp[0]);
				if (vec_Eye_tmp.size()>0)
				{
					vec_Eye.push_back(vec_Eye_tmp[0]);
				}
			}
			else
			{
				face_Rect.push_back(norm_Rect);

				vector<XmRect> face_Rect_tmp ;
				face_Rect_tmp.push_back(norm_Rect);
				cv::Rect r_det;
				r_det.x = face_Rect_tmp[0].x;
				r_det.y = face_Rect_tmp[0].y;
				r_det.width = face_Rect_tmp[0].width;
				r_det.height = face_Rect_tmp[0].height;
				cv::Mat mat_i = imageGray(r_det);
 				char buf[10];
 				sprintf(buf,"_%d",ii);
 				string s2 = gFaceDir+imageName+buf+"_detect.jpg";
 				imwrite(s2,mat_i);

				vector< vector<Point2f> > LandMarkpoints_tmp = gFaceAlbum.GetLandMarkPoints_Image(imageGray,face_Rect_tmp);
				vector< EyeArray > vec_Eye_tmp = gFaceAlbum.EyeDetect_Image(imageGray,LandMarkpoints_tmp);
				LandMarkpoints.push_back(LandMarkpoints_tmp[0]);
				if (vec_Eye_tmp.size()>0)
				{
					vec_Eye.push_back(vec_Eye_tmp[0]);
				}
			}
			

			
				
		}
		else
		{
			break;
		}
		if (face_Rect.size()<=0)
		{
			return ;
		}
		
#else  //directly use the FFP loaded by the ffp file
		if( readFaceFFPFromFile(ffpPath,eyePt) != -1)
		{
			vec_Eye.push_back(eyePt);
			string FacePath = FaceDir+imageName+buf+".jpg";
			FaceList.push_back(FacePath); 
		}
		else
			break;
#endif
      }
	
#else

    {
        face_Rect =  gFaceAlbum.FaceDetect_Image(image);

        //cout<<face_Rect.size()<<endl;
        gFaceNum+=face_Rect.size();

    }

  //  if (gFaceInfo.nEyeFlag)
    {
		// if want to Get the eye points directly ,please call the function like this
        //vec_Eye =  gFaceAlbum.EyeDetect_Image(imageGray,face_Rect);

		//if want to get the extra  landmark points  please call the method like this
		LandMarkpoints = gFaceAlbum.GetLandMarkPoints_Image(imageGray,face_Rect);
		vec_Eye = gFaceAlbum.EyeDetect_Image(imageGray,LandMarkpoints);
    }

#endif

    for (int i=0;i<vec_Eye.size();++i)
    {
        char buf[10];
        sprintf(buf,"_%d",i);

        saveFaceImgWithEye(image,vec_Eye[i],gFaceDir+imageName+buf+".jpg");
		
       
        FaceList.push_back(gFaceDir+imageName+buf+".jpg");
        //saveFaceImgWithFaceRect(image,face_Rect[i],gFaceInfo.FeaturePath+imageName+buf+".jpg");

		// if angle != 0  需要旋转之后再保存
		{
			EyeArray  ptEye;
		//	switch (Angle)
			{
			/*case 90:
				{
					ptEye.x_left = vec_Eye[i].y_left;
					ptEye.y_left = vec_Eye[i].x_left;

					ptEye.x_right = vec_Eye[i].y_right;
					ptEye.y_right = vec_Eye[i].x_right;

					break;
				}
			case 180:
				{
					
					ptEye.x_left = image.cols -1- vec_Eye[i].x_left;
					ptEye.y_left = image.rows-1-vec_Eye[i].y_left;

					ptEye.x_right = image.cols -1- vec_Eye[i].x_right;
					ptEye.y_right = image.rows -1- vec_Eye[i].y_right;
					break;
				}
			case 270:
				{
					ptEye.x_left = image.rows-1- vec_Eye[i].y_left;
					ptEye.y_left = vec_Eye[i].x_left;

					ptEye.x_right = image.rows -1- vec_Eye[i].y_right;
					ptEye.y_right = vec_Eye[i].x_right;

					break;
				}
				*/
		//	default:
				ptEye.x_left = vec_Eye[i].x_left;
				ptEye.y_left = vec_Eye[i].y_left;

				ptEye.x_right = vec_Eye[i].x_right;
				ptEye.y_right = vec_Eye[i].y_right;

			//	break;
			}
			saveEyePtToFile(gFaceDir+imageName+buf+".FFP",ptEye);

// 			if (LandMarkpoints[i].size()>0)
// 			{
// 				saveLandMarkPtToFile(gFaceDir+imageName+buf+".pnt",LandMarkpoints[i]);
// 			}
			
			
		}
     //   saveEyePtToFile(gFaceDir+imageName+buf+".FFP",vec_Eye[i]);
     //   saveFaceExifToFile(gFaceDir+imageName+buf+".exif",vec_Eye[i],exif,RelativePath,width,height);
       
    }

    
	if(face_Rect.size()>0)
    {
		vector<string>fstr;
#ifdef CORRECT_3D
		if (LandMarkpoints.size()>0)
		{
			vector<Image3dWarping> image3dstr = gFaceAlbum.ImageCorrect3d(image,LandMarkpoints,face_Rect);
			fstr = gFaceAlbum.FeatureExtractor_Image(image,image3dstr,vec_Eye);
			for (int ikm=0; ikm<image3dstr.size();ikm++)
			{
				if (image3dstr[ikm].WarpingFlag==1)
				{
					char buf[10];
					sprintf(buf,"_%d",ikm);
 					string s2 = gFaceDir+imageName+buf+"_coorect.jpg";
					imwrite(s2,image3dstr[ikm].imageWarping);
					
				}
			}
			
			
		}
		else
		{
			fstr = gFaceAlbum.FeatureExtractor_Image(image,vec_Eye);
		}
		
		
#else
		fstr = gFaceAlbum.FeatureExtractor_Image(image,vec_Eye);
#endif
  
        for (int i=0;i<fstr.size();++i)
        {
            char buf[10];
            sprintf(buf,"_%d",i);

            saveFeatureToFile(gFaceDir+imageName+buf+".fea",fstr[i]);

            //Mat NorImg;
           // gFaceAlbum.GetColorRectFaceImg(image,vec_Eye[i],NorImg);
           // imwrite(gFaceInfo.FeaturePath+imageName+buf+"_color.jpg",NorImg);
        }

    }

}


int ProcessImgList()
{
    ifstream fileIn;
    string listPath =gFaceInfo.imageList;


    fileIn.open(listPath.c_str()); //打开文件夹路径下图片列表

    if (!fileIn.good())
    {
        cout<<"open Image List Error!! "<<listPath<<endl;
        return -2;
    }

    char imagePath[1024];

    
    int imgCount=0;

    string userAlbumDir ;
    while (fileIn.getline(imagePath,1024)!=NULL)
    {
        string tmpPath = imagePath;

        int pos1 = tmpPath.rfind("\\");
        string tmpPath1 = tmpPath.substr(0,pos1);
        pos1 = tmpPath1.rfind("\\");
        string uesrDir = tmpPath1.substr(0,pos1);
        if (userAlbumDir != uesrDir)
        {
           
            cout<<"user : "<<uesrDir<<endl;
            gFaceDir = uesrDir+"\\"+FaceDirName+"\\";
            _mkdir(gFaceDir.c_str());
            string resultDir = uesrDir+"\\results";
            _mkdir(resultDir.c_str());
            
            if (!userAlbumDir.empty())
            {
                
                string FaceListPath = userAlbumDir+"\\"+FaceDirName+".txt";
                ofstream fout;
                fout.open(FaceListPath.c_str(),ios::trunc);
                for (int i=0;i<FaceList.size();++i)
                {
                    fout<<FaceList[i]<<endl;
                }
                fout.close();
            }
            

            userAlbumDir = uesrDir;
            FaceList.clear();
        }
        

        string imageName = tmpPath.substr(pos1+1,tmpPath.size() - 1);

        //string absolutePath = gFaceInfo.ClusterAlbumDir+imagePath;
       // cout<<"image Path : "<<imageName<<endl;

    //    processOneImage(uesrDir,imageName);
		processOneImage_emulateCloud(uesrDir,imageName);
        //cout<<"Image Count:"<<imgCount++<<endl;
    }
    fileIn.close();


    if (!userAlbumDir.empty())
    {
        string FaceListPath = userAlbumDir+"\\"+FaceDirName+".txt";

        ofstream fout;
        fout.open(FaceListPath.c_str(),ios::trunc);
        for (int i=0;i<FaceList.size();++i)
        {
            fout<<FaceList[i]<<endl;
        }
        fout.close();
        FaceList.clear();
    }
}

void init_FaceInfo()
{

     gFaceInfo.ClusterAlbumDir = "M:\\zhijun\\Face\\FaceAlbum\\user1\\";


    gFaceInfo.FeaturePath =gFaceInfo.ClusterAlbumDir+FaceDirName+"\\";
    gFaceInfo.imageList=gFaceInfo.ClusterAlbumDir + "oriPhoto.list";
    gFaceInfo.SaveClusterDir =  gFaceInfo.ClusterAlbumDir+"\\results\\";
    gFaceInfo.SaveClusterPath =  gFaceInfo.ClusterAlbumDir+"\\results\\FaceClusters.list";
    gFaceInfo.FaceList = gFaceInfo.ClusterAlbumDir +"Face.list";
    gFaceInfo.ClusterGroundTruthFaceList = gFaceInfo.ClusterAlbumDir+"FaceGroundTruthList.txt";
   
   // gFaceInfo.ClssifierDir = "../../data/";

    gFaceInfo.modelPath ="../../data/models/";

    gFaceInfo.nEyeFlag = 1;
    gFaceInfo.nFeaFlag =1;

    gFaceInfo.nFaceDetect = 1;


}



int init_FaceTagging(int argc,char *argv[])
{
    

    if (argc == 1)
    {
        init_FaceInfo();

    }
    else
    {
        gFaceInfo.imageList=argv[1];
        gFaceInfo.modelPath = argv[2];

        if (argc > 3)
        {
            FaceDirName = argv[3];
            cout<<"FaceDirName : "<<FaceDirName<<endl;
        }
        
        
    }

    {
        if (XMFACE_NORMAL != gFaceAlbum.FaceDetect_Init(gFaceInfo.modelPath+"/FaceDetect_model/") )
        {
            cout<<"init detect error!!"<<endl;
            return XMFACE_ERR_INITIALIZE;
        }



    }
    {
        if (XMFACE_NORMAL != gFaceAlbum.EyeDetect_Init(gFaceInfo.modelPath+"/EyeDetect_model/") )
        {
            
            cout<<"init Eye detect error!!  "<<gFaceInfo.modelPath<<endl;
            return XMFACE_ERR_INITIALIZE;
        }
    }

   {
       if (XMFACE_NORMAL != gFaceAlbum.FeatureExtractor_Init(gFaceInfo.modelPath+"/feature_model/") )
       {
           cout<<"init Feature  error!!"<<endl;
           return XMFACE_ERR_INITIALIZE;
       }
   }

   for (int i = 0; i < 6; i++)
   {
	   {
        if (XMFACE_NORMAL != gFaceAlbum_testMul[i].FaceDetect_Init(gFaceInfo.modelPath+"/FaceDetect_model/") )
        {
            cout<<"init detect error!!"<<endl;
            return XMFACE_ERR_INITIALIZE;
        }



    }
    {
        if (XMFACE_NORMAL != gFaceAlbum_testMul[i].EyeDetect_Init(gFaceInfo.modelPath+"/EyeDetect_model/") )
        {
            
            cout<<"init Eye detect error!!  "<<gFaceInfo.modelPath<<endl;
            return XMFACE_ERR_INITIALIZE;
        }
    }

   {
       if (XMFACE_NORMAL != gFaceAlbum_testMul[i].FeatureExtractor_Init(gFaceInfo.modelPath+"/feature_model/") )
       {
           cout<<"init Feature  error!!"<<endl;
           return XMFACE_ERR_INITIALIZE;
       }
   }

   }
   
#ifdef CORRECT_3D
   {
	   if (XMFACE_NORMAL != gFaceAlbum.Correct3d_Init(gFaceInfo.modelPath+"/3d_model/") )
	   {
		   cout<<"init Feature  error!!"<<endl;
		   return XMFACE_ERR_INITIALIZE;
	   }
   }
#endif



    return 0;
}



//0
//1
//2
//3 是聚类
//4 是增量聚类
//5 是推荐聚类

// G:\Image_Data\Baby\test_samples\list.txt  E:\work\new_cluster\FaceTag\data\models
// M:\zhijun\Face\FaceAlbum\user1\oriPhoto.list
int main(int argc,char *argv[])
{
   // cout<<argv[1]<<endl;


    if( XMFACE_NORMAL != init_FaceTagging(argc,argv))
    {
        system("pause");
        return XMFACE_ERR_INITIALIZE;
    }
     if (!gFaceInfo.nClusterFlag)
     {
        //  ProcessImgList();
	//	 Process_MultiImgByMultiThread(argv[1]);

		 Process_MultiImgBySingleThread(argv[1]);
     }
  

   
   // cout<<"allFace :  "<<gFaceNum<<endl;
    gFaceAlbum.ReleaseFaceTagging();


}


#endif