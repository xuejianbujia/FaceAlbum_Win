#include "DominantColor.h"

DominantColor::DominantColor(IplImage* img):GlobalFeatures(img)
{
	m_normalize=true;
	m_variance=true;
	m_spatial=true;
	m_bin1=32;
	m_bin2=32;
	m_bin3=32;
}

DominantColor::~DominantColor()
{
	//destructor
}

const vector<int>& DominantColor::ExtractFeatures()
{
	vec_dcd.clear();

	if(!m_frame)
		return vec_dcd;

	XM::DominantColorDescriptor* dcd=Feature::getDominantColorD(m_frame,m_normalize,m_variance,m_spatial,m_bin1,m_bin2,m_bin3);

	int ndc=dcd->GetDominantColorsNumber();

	if(m_spatial)
		vec_dcd.push_back(dcd->GetSpatialCoherency());

	XM::DOMCOL* domcol=dcd->GetDominantColors();

	for(int i=0;i<ndc;i++)
	{
		vec_dcd.push_back(domcol[i].m_Percentage);
		vec_dcd.push_back(domcol[i].m_ColorValue[0]);
		vec_dcd.push_back(domcol[i].m_ColorValue[1]);
		vec_dcd.push_back(domcol[i].m_ColorValue[2]);
		if(m_variance)
		{
			vec_dcd.push_back(domcol[i].m_ColorVariance[0]);
			vec_dcd.push_back(domcol[i].m_ColorVariance[1]);
			vec_dcd.push_back(domcol[i].m_ColorVariance[2]);
		}
	}

	delete dcd;
	return vec_dcd;
}
