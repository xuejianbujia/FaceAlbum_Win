#include "ColorStructure.h"



ColorStructure::ColorStructure(IplImage* img):GlobalFeatures(img)
{
	//constructor
	m_descSize=64;
}

ColorStructure::~ColorStructure()
{
	//destructor
}

const vector<int>& ColorStructure::ExtractFeatures()
{
	vec_csd.clear();
	if(!m_frame)
		return vec_csd;

	//XM::ColorStructureDescriptor* csd=Feature::getColorStructureD(m_frame,m_descSize);
	m_csdt = new  XM::ColorStructureExtractionTool();

	XM::ColorStructureDescriptor* csd = m_csdt->GetDescriptor();
	m_csdt->extract(m_frame->image, m_descSize);


	for(unsigned int i=0;i<csd->GetSize();i++)
		vec_csd.push_back((int)csd->GetElement(i));

	delete csd;
	delete m_csdt;
	return vec_csd;
}


#define CSD_MAX_DIST 6701.0f
float ColorStructure::distance(const vector<int> &data_query,const vector<int> &data_gallery)
{
	float dist = 0.0f;
	//    int size = this->GetSize();
	//	for( int i = 0; i < size; i ++)
	//		dist += abs( this->GetElement(i) - csd->GetElement(i));

	for( int i = 0; i < data_query.size(); i ++)
		dist += abs( (int)data_query[i] - (int)data_gallery[i]);

	return dist/CSD_MAX_DIST;

}