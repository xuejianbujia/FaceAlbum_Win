

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#include <stdio.h>

#ifdef WIN32
#include <direct.h>
#endif

#include "ColorStructure.h"
#include "ColorLayout.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;


enum
{
	FEATURE_COLORLAYOUT = 1,
	FEATURE_COLORSTRUCTURE = 2,
	FEATURE_COLORFUSION = 3
};

class mpeg7_feature_color
{
public:
	 mpeg7_feature_color();
	 ~mpeg7_feature_color();

	 int feature_get(cv::Mat &matColor,int feature_type=FEATURE_COLORFUSION);

	
	 //score compare  during the feature: color structure
	 float ScoreCompare(vector<int> &vec_fea_A,vector<int> &vec_fea_B,int feature_type=FEATURE_COLORFUSION);

	  //score compare  by type. don't need to give the parameters
	  float ScoreCompare(mpeg7_feature_color &mObjectB, int feature_type);
public:
	vector<int> vec_y_coeff_Layout;
	vector<int> vec_cb_coeff_Layout;
	vector<int> vec_cr_coeff_Layout;
	vector<int> vec_colorstructure;
	vector<int> vec_fea_total;
	int m_y_coeff;
	int m_cb_coeff;
	int m_cr_coeff;
	int m_colorstructure;
};

