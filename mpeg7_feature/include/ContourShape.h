#ifndef CONTOURSHAPE_H
#define CONTOURSHAPE_H

#include "GlobalFeatures.h"

class ContourShape:public GlobalFeatures
{
	public:
		ContourShape(IplImage* img=0);
		~ContourShape();
		const vector<int>& ExtractFeatures();
	private:
		vector<int> vec_csd;
};

#endif
