#ifndef EDGEHISTOGRAM_H
#define EDGEHISTOGRAM_H

#include "GlobalFeatures.h"

class EdgeHistogram:public GlobalFeatures
{
	public:
		EdgeHistogram(IplImage* img=0);
		~EdgeHistogram();
		const vector<int>& ExtractFeatures();
		float distance(const vector<int> &query,const   vector<int> &test);
	private:
		vector<int> vec_ehd;
};

#endif
