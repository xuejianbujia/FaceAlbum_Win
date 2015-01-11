#ifndef HOMOGENEOUSTEXTURE_H
#define HOMOGENEOUSTEXTURE_H

#include "GlobalFeatures.h"

class HomogeneousTexture:public GlobalFeatures
{
	public:
		HomogeneousTexture(IplImage* img=0);
		~HomogeneousTexture();
		void SetParams(int layerFlag=1)
		{
			m_layerFlag=layerFlag;
		}
		const vector<int>& ExtractFeatures();

		float distance(const vector<int>&data_query,const vector<int>&data_gallery);
	private:
		int m_layerFlag;
		vector<int> vec_htd;
};

#endif
