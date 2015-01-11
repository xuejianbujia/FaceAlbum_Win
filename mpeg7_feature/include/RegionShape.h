#ifndef REGIONSHAPE_H
#define REGIONSHAPE_H

#include "GlobalFeatures.h"

class RegionShape:public GlobalFeatures
{
	public:
		RegionShape(IplImage* img=0);
		~RegionShape();
		const vector<int>& ExtractFeatures();
	private:
		vector<int> vec_rsd;
};

#endif
