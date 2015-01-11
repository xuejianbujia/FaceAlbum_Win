#include "RegionShape.h"
#include <iostream>

RegionShape::RegionShape(IplImage* img):GlobalFeatures(img)
{
	//constructor
//	std::cout<<"RegionShape::RegionShape(IplImage* img):GlobalFeatures(img)"<<endl;
}

RegionShape::~RegionShape()
{
	//destructor
}

const vector<int>& RegionShape::ExtractFeatures()
{
	vec_rsd.clear();

	if(!m_frame)
		return vec_rsd; 
	
	if(m_frame->mask)
	{
//		m_frame->setMask((Mat&)m_frame->mask, 1, 255, 0);
	}
	//std::cout<<"step: "<<m_frame->image->step<<std::endl;
	//std::cout<<"rows: "<<m_frame->image->rows<<std::endl;
	//std::cout<<"cols: "<<m_frame->image->cols<<std::endl;

	// std::cout<<"step: "<<m_frame->mask->step<<std::endl;
	// std::cout<<"rows: "<<m_frame->mask->rows<<std::endl;
	// std::cout<<"cols: "<<m_frame->mask->cols<<std::endl;
	XM::RegionShapeDescriptor* rsd=Feature::getRegionShapeD(m_frame);

	int i,j;

	for(i=0;i<ART_ANGULAR;i++)
		for(j=0;j<ART_RADIAL;j++)
			if(i!=0 || j!=0)
				vec_rsd.push_back((int)rsd->GetElement(i,j));

	delete rsd;
	return vec_rsd;
}
