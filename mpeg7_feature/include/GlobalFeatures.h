#ifndef GLOBALFEATURES_H
#define GLOBALFEATURES_H

#include <vector>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include "cv.h"
//#include "cxcore.h"
#include <opencv2/opencv.hpp>

#include "Feature.h"

using namespace std;

class GlobalFeatures
{
	public:
		GlobalFeatures(IplImage* img=NULL);
		~GlobalFeatures();
		void SetImage(IplImage* img);
		virtual const vector<int>& ExtractFeatures()=0;
	protected:
		Frame* m_frame;
};

#endif
