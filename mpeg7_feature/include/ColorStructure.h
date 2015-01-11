#ifndef COLORSTRUCTURE_H
#define COLORSTRUCTURE_H

#include "GlobalFeatures.h"
#include"Extraction/ColorStructureExtraction.h"

class ColorStructure:public GlobalFeatures
{
	public:
		ColorStructure(IplImage* img=0);
		~ColorStructure();
		void SetParams(int size=64)
		{
			m_descSize=size;
		}
		const vector<int>& ExtractFeatures();

		float distance(const vector<int> &data_query,const vector<int> &data_gallery);
	private:
		int m_descSize;
		vector<int> vec_csd;

		XM::ColorStructureExtractionTool* m_csdt;
};

#endif
