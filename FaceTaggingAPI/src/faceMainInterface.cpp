#include "faceMainInterface.h"

void saveFeatureToFile(const string &saveFeaturePath,string featureStr)
{
     ofstream fout;
     fout.open(saveFeaturePath.c_str(),ios::trunc);
     fout<<featureStr;
     fout.close();
}
void saveEyePtToFile(const string &saveEyePath,EyeArray EyePt)
{
    ofstream fout;
    fout.open(saveEyePath.c_str(),ios::trunc);
    fout<<"LEX"<<" "<<EyePt.x_left <<endl;
    fout<<"LEY"<<" "<<EyePt.y_left <<endl;
    fout<<"REX"<<" "<<EyePt.x_right <<endl;
    fout<<"REY"<<" "<<EyePt.y_right <<endl;
    fout.close();
}
void saveLandMarkPtToFile(const string &saveEyePath,vector<Point2f> &LandMarkPt)
{
	ofstream fout;
	fout.open(saveEyePath.c_str(),ios::trunc);
	fout<<LandMarkPt.size()<<endl;
	for (int i=0;i<LandMarkPt.size();i++)
	{
		fout<<LandMarkPt[i].x<<" "<<LandMarkPt[i].y<<endl;
	}
	
	fout.close();
}
void saveFaceImgWithFaceRect(cv::Mat &image,EyeArray EyePt,Rect faceRect,const string &saveFacePath)
{
   
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

    Mat rsizeMat =Mat::zeros(480,480,image.type()); 


    resize(subImg,rsizeMat,cv::Size(480,480));

    imwrite(saveFacePath,rsizeMat);

}
void saveFaceImgWithEye(cv::Mat image,EyeArray EyePt,const string &saveFacePath)
{
    Rect faceRect;
    
    int midY = (EyePt.y_right + EyePt.y_left)/2;
    int midX = (EyePt.x_right + EyePt.x_left)/2;

    int ew = EyePt.x_right - EyePt.x_left;
    
    if (ew<0)
    {
        int tmpX = EyePt.x_right;
        EyePt.x_right = EyePt.x_left;
        EyePt.x_left = tmpX;
        ew = -ew;
    }

    faceRect.x = midX - 3*ew;
    faceRect.x = faceRect.x>=0?faceRect.x : 0;
    faceRect.y = midY - 2.5*ew;
    faceRect.y = faceRect.y>=0?faceRect.y : 0;

    faceRect.width  = (midX+3*ew) -  faceRect.x;
    faceRect.height = midY + 3.5*ew - faceRect.y;


    if (faceRect.width + faceRect.x >image.cols)
    {
        faceRect.width = image.cols - faceRect.x -1;
    }
    if (faceRect.height + faceRect.y >image.rows)
    {
        faceRect.height = image.rows - faceRect.y -1;
    }

    Mat subImg = ( image(faceRect)).clone();
    
#ifdef debug_test
    cvCircle( &((IplImage)subImg),cvPoint(EyePt.x_left - faceRect.x,EyePt.y_left - faceRect.y),4,CV_RGB(255,0,0),1,16 ); 
    cvCircle( &((IplImage)subImg),cvPoint(EyePt.x_right - faceRect.x,EyePt.y_right - faceRect.y),4,CV_RGB(255,0,0),1,16 ); 
	#endif
    
    Mat rsizeMat =Mat::zeros(480,480,image.type()); 
    

    resize(subImg,rsizeMat,cv::Size(480,480));

    imwrite(saveFacePath,rsizeMat);

}

void saveFaceImgWithEyeAndLandMark(cv::Mat image,cv::Rect rc, EyeArray EyePt,vector <Point2f> mkeyPointsLandMark,const string &saveFacePath)
{
    Rect faceRect;
    
    int midY = (EyePt.y_right + EyePt.y_left)/2;
    int midX = (EyePt.x_right + EyePt.x_left)/2;

    int ew = sqrtf((EyePt.x_right - EyePt.x_left)*(EyePt.x_right - EyePt.x_left)+(EyePt.y_right - EyePt.y_left)*(EyePt.y_right - EyePt.y_left));
    
    if (ew<0)
    {
        int tmpX = EyePt.x_right;
        EyePt.x_right = EyePt.x_left;
        EyePt.x_left = tmpX;
        ew = -ew;
    }

    faceRect.x = midX - 3*ew;
    faceRect.x = faceRect.x>=0?faceRect.x : 0;
    faceRect.y = midY - 2.5*ew;
    faceRect.y = faceRect.y>=0?faceRect.y : 0;

    faceRect.width  = (midX+3*ew) -  faceRect.x;
    faceRect.height = midY + 3.5*ew - faceRect.y;


    if (faceRect.width + faceRect.x >image.cols)
    {
        faceRect.width = image.cols - faceRect.x -1;
    }
    if (faceRect.height + faceRect.y >image.rows)
    {
        faceRect.height = image.rows - faceRect.y -1;
    }

#ifdef debug_test

	for (int ki = 0; ki < mkeyPointsLandMark.size(); ki++)
	{
		cv::circle(image,cvPoint(mkeyPointsLandMark[ki].x ,mkeyPointsLandMark[ki].y ),4,CV_RGB(0,0,255),1,16 );
	}
	 cv::circle( image,cvPoint(EyePt.x_left ,EyePt.y_left),4,CV_RGB(255,0,0),1,16 ); 
     cv::circle( image,cvPoint(EyePt.x_right ,EyePt.y_right ),4,CV_RGB(255,0,0),1,16 ); 

	 cv::rectangle(image, rc,CV_RGB(0,255,0),1,16);
#endif
	 imwrite(saveFacePath,image);
//    Mat subImg = ( image(faceRect)).clone();
//#ifdef debug_test
//    cvCircle( &((IplImage)subImg),cvPoint(EyePt.x_left - faceRect.x,EyePt.y_left - faceRect.y),4,CV_RGB(255,0,0),1,16 ); 
//    cvCircle( &((IplImage)subImg),cvPoint(EyePt.x_right - faceRect.x,EyePt.y_right - faceRect.y),4,CV_RGB(255,0,0),1,16 ); 
//	
//
//#endif
//
//    
//    Mat rsizeMat =Mat::zeros(480,480,image.type()); 
//    
//
//    resize(subImg,rsizeMat,cv::Size(480,480));
//
//    imwrite(saveFacePath,rsizeMat);

}


void saveFaceExifToFile( const string saveExifPath,EyeArray eyePt,EXIFINFO *FaceExif,const string fromPhotoPath ,int width ,int height )
{
    ofstream fout;
    fout.open(saveExifPath.c_str(),ios::trunc);
    fout<<"SourcePhotoPath#"<<fromPhotoPath<<endl;
    if (FaceExif->IsExif)
    {
        fout<<"DataTime#"<<FaceExif->DateTime<<endl;
        if (FaceExif->isHaveGPSInfo)
        {
            fout<<"GPS#";
            double lat,lon;
            lat =FaceExif->GPSLattitude[0]+FaceExif->GPSLattitude[1]/60+FaceExif->GPSLattitude[2]/3600;
            lon = FaceExif->GPSLongitude[0]+FaceExif->GPSLongitude[1]/60+FaceExif->GPSLongitude[2]/3600;
            fout<<lat<<" "<<lon<<endl;
           // fout<<FaceExif->GPSLattitude[0]<<" "<<FaceExif->GPSLattitude[1]<<" "<<FaceExif->GPSLattitude[2]<<" "<<FaceExif->GPSLattitudeRef<<" ";
           // fout<<FaceExif->GPSLongitude[0]<<" "<<FaceExif->GPSLongitude[1]<<" "<<FaceExif->GPSLongitude[2]<<" "<<FaceExif->GPSLongitudeRef<<endl;
        }
        
    }
   
    fout<<"LEYE#"<<eyePt.x_left<<" "<<eyePt.y_left<<endl;
    fout<<"REYE#"<<eyePt.x_right<<" "<<eyePt.y_right<<endl;
    
    fout<<"Width#"<<width<<endl;
    fout<<"Height#"<<height<<endl;

    

    fout.close();
}

 int readFaceFFPFromFile( const string ffpPath ,EyeArray &EyePt)
{
    ifstream fin;
    
    fin.open(ffpPath.c_str());
    if (!fin)
    {
        cout<<"open file :"<<ffpPath<<"  ERROR!!"<<endl;
        return -1;
    }
    string str;
    fin>>str>>EyePt.x_left;
    fin>>str>>EyePt.y_left;
    fin>>str>>EyePt.x_right;
    fin>>str>>EyePt.y_right;

    fin.close();
}

 int rotationImageWithOrientation( Mat &Image,int orientation )
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
         Angle = 270;
         break;
     case 3:
         Angle = 180;
         break;
     case 6:
         Angle = 90;
         break;
     default:
         return 0;
     }
     int flag = 1;
     

     int angle = Angle;


    
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

