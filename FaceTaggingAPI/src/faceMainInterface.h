/*************************************************
Copyright:1.0.0.0
Author: 陈志军
Date:2014-3-5
Description: 人脸相册各个算法对外开放的接口封装
**************************************************/

#ifndef __FACE_MAIN_INTERFACE_H__
#define __FACE_MAIN_INTERFACE_H__
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <fstream>
#include "EXIF.H"

//#define  Location_test

#include "opencv/cv.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

#define SaveFacePhotoExif

#define  debug_test

#include "AL_SDMModelRunner.h"
#include "FaceFeatureExtractor.h"



void saveFeatureToFile(const string &saveFeatureDir,string featureStr);
void saveFaceImgWithEye(cv::Mat image,EyeArray EyePt,const string &saveFacePath);
void saveFaceImgWithEyeAndLandMark(cv::Mat image,cv::Rect rc, EyeArray EyePt,vector <Point2f> mkeyPointsLandMark,const string &saveFacePath);
void saveFaceImgWithFaceRect(cv::Mat &image,EyeArray EyePt,Rect faceRect,const string &saveFacePath);
void saveEyePtToFile(const string &saveEyePath,EyeArray EyePt);
void saveLandMarkPtToFile(const string &saveEyePath,vector<Point2f> &LandMarkPt);
void saveFaceExifToFile(const string saveExifPath,EyeArray eyePt,EXIFINFO *FaceExif,const string fromPhotoPath,int width ,int height );
int readFaceFFPFromFile( const string ffpPath ,EyeArray &EyePt);

int rotationImageWithOrientation(Mat &Image,int orientation);

#endif