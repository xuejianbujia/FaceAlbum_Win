#include "ContourShape.h"

ContourShape::ContourShape(IplImage* img):GlobalFeatures(img)
{
	//constructor
}

ContourShape::~ContourShape()
{
	//destructor
}

const vector<int>& ContourShape::ExtractFeatures()
{
	vec_csd.clear();

	if(!m_frame)
		return vec_csd; 

	XM::ContourShapeDescriptor* csd=Feature::getContourShapeD(m_frame);

	unsigned long lgcv[2];
	csd->GetGlobalCurvature(lgcv[0], lgcv[1]);
	vec_csd.push_back((int)lgcv[0]);
	vec_csd.push_back((int)lgcv[1]);
	unsigned int num = csd->GetNoOfPeaks();

	if (num > 0)
    {
		unsigned long lpcv[2];
		csd->GetPrototypeCurvature(lpcv[0], lpcv[1]);
		vec_csd.push_back((int)lpcv[0]);
		vec_csd.push_back((int)lpcv[1]);
    }
	vec_csd.push_back((int)(csd->GetHighestPeakY()));

    for (unsigned int i = 1; i < num; i++)
	{
		unsigned short xp, yp;
		csd->GetPeak(i, xp, yp);
		vec_csd.push_back((int)xp);
		vec_csd.push_back((int)yp);
    }

	delete csd;
	return vec_csd;
}
