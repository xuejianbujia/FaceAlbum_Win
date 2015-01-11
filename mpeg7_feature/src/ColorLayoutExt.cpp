#include "ColorLayout.h"
#include"Feature.h"



ColorLayout::ColorLayout(IplImage* img):GlobalFeatures(img)
{


		m_weightMatrix[0][0] = 2;
		m_weightMatrix[0][1] = m_weightMatrix[0][2] = 2;
		m_weightMatrix[1][0] = 2;
		m_weightMatrix[1][1] = m_weightMatrix[1][2] = 1;
		m_weightMatrix[2][0] = 4;
		m_weightMatrix[2][1] = m_weightMatrix[2][2] = 2;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 3; j < 64; j++)
				m_weightMatrix[i][j] = 1;
		}

}

ColorLayout::~ColorLayout()
{
	//destructor
}

const vector<int>& ColorLayout::ExtractFeatures()
{
	vector<int> atm;
	return atm;
}


int ColorLayout::ExtractFeatures(vector<int>&vec_y_coeff,vector<int>&vec_cb_coeff,vector<int>&vec_cr_coeff)
{


	if(!m_frame)
		return 0;

	SetParams();
	//XM::ColorLayoutDescriptor* cld=Feature::getColorLayoutD(m_frame,m_numYCoef,m_numCCoef);

	m_pcldt = new  XM::ColorLayoutExtractionTool(m_numYCoef,m_numCCoef);
	XM::ColorLayoutDescriptor* cld = m_pcldt->GetDescriptor();
	m_pcldt->extract( m_frame->image, m_numYCoef,m_numCCoef);




	int numberOfYCoeff=cld->GetNumberOfYCoeff();
	int numberOfCCoeff=cld->GetNumberOfCCoeff();


	int *y_coeff,*cb_coeff,*cr_coeff;
	y_coeff=cld->GetYCoeff();
	cb_coeff=cld->GetCbCoeff();
	cr_coeff=cld->GetCrCoeff();

	int i;
	for(i=0;i<numberOfYCoeff;i++)
		vec_y_coeff.push_back(y_coeff[i]);
	for(i=0;i<numberOfCCoeff;i++)
		vec_cb_coeff.push_back(cb_coeff[i]);
	for(i=0;i<numberOfCCoeff;i++)
		vec_cr_coeff.push_back(cr_coeff[i]);
	delete cld;
	delete m_pcldt;

	
	return 0;
}


/// compute the distance between 2 CLDs
float ColorLayout::distance(const vector<int>&vec_y_coeff1,const vector<int>&vec_cb_coeff1,const vector<int>&vec_cr_coeff1,
	                        const vector<int>&vec_y_coeff2,const vector<int>&vec_cb_coeff2,const vector<int>&vec_cr_coeff2  )
{
	int numYCoeff1, numYCoeff2, CCoeff1, CCoeff2, YCoeff, CCoeff;
	//Numbers of the Coefficients of two descriptor values.
	numYCoeff1 = vec_y_coeff1.size();
	numYCoeff2 = vec_y_coeff2.size();
	CCoeff1 = vec_cb_coeff1.size();
	CCoeff2 = vec_cb_coeff2.size();
	//take the Minimal Coeff-number
	YCoeff = MIN(numYCoeff1, numYCoeff2);
	CCoeff = MIN(CCoeff1, CCoeff2);
	
	int j;
	int sum[3];
	int diff;
	sum[0] = 0;
	for (j = 0; j < YCoeff; j++)
	{
		diff = (vec_y_coeff1[j] - vec_y_coeff2[j]);
		sum[0] += (m_weightMatrix[0][j] * diff * diff);
	}
	sum[1] = 0;
	for (j = 0; j < CCoeff; j++)
	{
		diff = (vec_cb_coeff1[j] - vec_cb_coeff2[j]);
		sum[1] += (m_weightMatrix[1][j] * diff * diff);
	}
	sum[2] = 0;
	for (j = 0; j < CCoeff; j++)
	{
		diff = (vec_cr_coeff1[j] - vec_cr_coeff2[j]);
		sum[2] += (m_weightMatrix[2][j] * diff * diff);
	}
	//returns the distance between the two desciptor values
	return sqrt(sum[0] * 1.0) + sqrt(sum[1] * 1.0) + sqrt(sum[2] * 1.0);
}

