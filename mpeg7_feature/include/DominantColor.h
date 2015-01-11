#ifndef DOMINANTCOLOR_H
#define DOMINANTCOLOR_H

#include "GlobalFeatures.h"

class DominantColor:public GlobalFeatures
{
	public:
		DominantColor(IplImage* img=0);
		~DominantColor();
		void SetParams(bool normalize=true,bool variance=true,bool spatial=true,int bin1=32,int bin2=32,int bin3=32)
		{
			m_normalize=normalize;
			m_variance=variance;
			m_spatial=spatial;
			m_bin1=bin1;
			m_bin2=bin2;
			m_bin3=bin3;
		}
		const vector<int>& ExtractFeatures();
	private:
		bool m_normalize;
		bool m_variance;
		bool m_spatial;
		int m_bin1;
		int m_bin2;
		int m_bin3;
		vector<int> vec_dcd;
};

#endif
