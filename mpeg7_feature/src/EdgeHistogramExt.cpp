#include "EdgeHistogram.h"

EdgeHistogram::EdgeHistogram(IplImage* img):GlobalFeatures(img)
{
	//constructor
}

EdgeHistogram::~EdgeHistogram()
{
	//destructor
}

const vector<int>& EdgeHistogram::ExtractFeatures()
{
	vec_ehd.clear();

	if(!m_frame)
		return vec_ehd;

	XM::EdgeHistogramDescriptor* ehd=Feature::getEdgeHistogramD(m_frame);

	char* de=ehd->GetEdgeHistogramElement();

	for(unsigned int i=0;i<ehd->GetSize();i++)
	{
		vec_ehd.push_back((int)de[i]);
	}

	delete ehd;
	return vec_ehd;
}



float EdgeHistogram::distance(const vector<int> &query,const   vector<int> &test)
{


	int size = 80;

	float dist = 0.0f;
	int i, j;
	for( i = 0; i < size; i++ ){
		dist += (float)abs(query[i] - test[i]);
	}

	// global: 5 directions 4x4 = 16 blocks => 80
	int gvalues1[5];
	memset(gvalues1, 0, 5*sizeof(int));

	for( j = 0; j < 80; j += 5 ) {
		for( i = 0; i < 5; i++) {
			gvalues1[i] += query[ j + i ];
		}
	}
	for( i = 0; i < 5; i++) {
		gvalues1[i] = gvalues1[i]*5.0/16;
	}

	int gvalues2[5];
	memset(gvalues2, 0, 5*sizeof(int));
	for( j = 0; j < 80; j += 5 ) {
		for( i = 0; i < 5; i++) {
			gvalues2[i] += test[ j + i ];
		}
	}
	for( i = 0; i < 5; i++) {
		gvalues2[i] = gvalues2[i]*5.0f/16;
	}

	// add to distance
	for( i = 0; i < 5; i++) {
		dist += abs( gvalues1[i] - gvalues2[i] );
	}

	return dist;
}
// 
// #define EHD_MAX_DIST 554.0f
// float EdgeHistogram::distanceNorm(EdgeHistogramDescriptor* ehd)
// {
// 	return distance(ehd)/EHD_MAX_DIST;
// }
