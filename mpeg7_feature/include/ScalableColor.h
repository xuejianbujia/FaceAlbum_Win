#ifndef SCALABLECOLOR_H
#define SCALABLECOLOR_H

#include "GlobalFeatures.h"

class ScalableColor:public GlobalFeatures
{
	public:
		ScalableColor(IplImage* img=0);
		~ScalableColor();
		void SetParams(bool flag=true,int size=128)
		{
			m_maskFlag=flag;
			m_descSize=size;
		}
		const vector<int>& ExtractFeatures();

		float distance(const vector<int>& data_query,const vector<int>& data_gallery);
	private:
		bool m_maskFlag;
		int m_descSize;
		vector<int> vec_scd;
};

#endif
