#include "HomogeneousTexture.h"

HomogeneousTexture::HomogeneousTexture(IplImage* img):GlobalFeatures(img)
{
	m_layerFlag=1;
}

HomogeneousTexture::~HomogeneousTexture()
{
	//destructor
}

 const vector<int>& HomogeneousTexture::ExtractFeatures()
{
	vec_htd.clear();
	
	if(!m_frame)
		return vec_htd;

	XM::HomogeneousTextureDescriptor* htd=Feature::getHomogeneousTextureD(m_frame,m_layerFlag);

	int* values=htd->GetHomogeneousTextureFeature();

	int i;
	for(i=0;i<32;i++)
		vec_htd.push_back(values[i]);
	if(m_layerFlag)
		for(i=32;i<62;i++)
			vec_htd.push_back(values[i]);

	delete htd;
	return vec_htd;
}


#define RadialDivision  5
#define AngularDivision 6
#define HTD_FULL_MAX_DIST	9237.0f
float HomogeneousTexture::distance(const vector<int>&data_query,const vector<int>&data_gallery)
{
	float dist = 0.0f;
	static double wm[RadialDivision] = {0.42,1.00,1.00,0.08,1.00};
	static double wd[RadialDivision] = {0.32,1.00,1.00,1.00,1.00};
	//double wdc=0.28;
	//double wstd=0.22;

	int m, n;
	for( n = 0; n < RadialDivision; n++ )
		for( m = 0; m < AngularDivision; m++ ) {
			dist += (float)( wm[n]*abs( data_query[n*AngularDivision+m+2] - data_gallery[n*AngularDivision+m+2] ) )
				+ ( wd[n]*abs( data_query[n*AngularDivision+m+30+2] - data_gallery[n*AngularDivision+m+30+2] ) );
		}

		return dist/HTD_FULL_MAX_DIST;

}