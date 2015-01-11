#include "GlobalFeatures.h"
#include <iostream>

GlobalFeatures::GlobalFeatures(IplImage* img)
{
	if(!img)
	{
		m_frame=NULL;
	}
	else
	{
		Mat image=cvarrToMat(img);
		Mat image1 = cvarrToMat(img);
		//std::cout<<"step: "<<image.step<<std::endl;
		m_frame=new Frame(image.cols,image.rows,true,true,true);
		//std::cout<<"arbitrary: "<<m_frame->image->arbitrary_shape<<std::endl;
		//std::cout<<"rows: "<<m_frame->image->height<<std::endl;
		//std::cout<<"cols: "<<m_frame->image->width<<std::endl;
		 
		//std::cout<<"arbitrary: "<<m_frame->mask->arbitrary_shape<<std::endl;
		//std::cout<<"rows: "<<m_frame->mask->height<<std::endl;
		//std::cout<<"cols: "<<m_frame->mask->width<<std::endl;
		
		m_frame->setImage(image);
		m_frame->setMask(image1,1,255,0);
		//m_frame->setMaskValue(255);

		//std::cout<<"GlobalFeatures::GlobalFeatures(IplImage* img)"<<std::endl;
	}
}

GlobalFeatures::~GlobalFeatures()
{
	if(m_frame)
		delete m_frame;
}

void GlobalFeatures::SetImage(IplImage* img)
{
	if(!img)
	{
		m_frame=NULL;
	}
	else
	{
		Mat image=cvarrToMat(img);
		if(m_frame)
		{
			delete m_frame;
			m_frame=NULL;
		}
		m_frame=new Frame(image.cols,image.rows,true,true,true);
		m_frame->setImage(image);
	}
}
