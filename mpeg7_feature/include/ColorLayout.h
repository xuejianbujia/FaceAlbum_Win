#ifndef COLORLAYOUT_H
#define COLORLAYOUT_H

#include "GlobalFeatures.h"
#include"Extraction/ColorStructureExtraction.h"

class ColorLayout:public GlobalFeatures
{
	public:
		ColorLayout(IplImage* img=0);
		~ColorLayout();
		void SetParams(int numYCoef=64,int numCCoef=28)
		{
			m_numYCoef=numYCoef;
			m_numCCoef=numCCoef;
		}
		float distance(const vector<int>&vec_y_coeff1,const vector<int>&vec_cb_coeff1,const vector<int>&vec_cr_coeff1,
			const vector<int>&vec_y_coeff2,const vector<int>&vec_cb_coeff2,const vector<int>&vec_cr_coeff2  );

		const vector<int>& ExtractFeatures();
		int ExtractFeatures(vector<int>&vec_y_coeff,vector<int>&vec_cb_coeff,vector<int>&vec_cr_coeff);

		
	private:
		int m_numYCoef;
		int m_numCCoef;

		XM::ColorLayoutExtractionTool *m_pcldt;
public:
		int m_weightMatrix[3][64];
};

#endif
