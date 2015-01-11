#include "ScalableColor.h"

ScalableColor::ScalableColor(IplImage* img):GlobalFeatures(img)
{
	m_maskFlag=true;
	m_descSize=128;
};

ScalableColor::~ScalableColor()
{
	//destructor
}

const vector<int>& ScalableColor::ExtractFeatures()
{
	vec_scd.clear();
	if(!m_frame)
		return vec_scd;

	XM::ScalableColorDescriptor* scd=Feature::getScalableColorD(m_frame,m_maskFlag,m_descSize);
	for(unsigned int i=0;i<scd->GetNumberOfCoefficients();i++)
		vec_scd.push_back((int)scd->GetCoefficient(i));

	delete scd;
	return vec_scd;
}


float ScalableColor::distance(const vector<int>& data_query,const vector<int>& data_gallery)
{
	float dist = 0.0f;
	for( int i = 0; i < data_query.size(); i++ )
	{
		dist += abs(data_gallery[i]-data_query[i]);
	}
	return dist;
}