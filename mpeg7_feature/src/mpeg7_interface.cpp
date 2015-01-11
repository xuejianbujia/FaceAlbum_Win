#include "mpeg7_interface.h"

using namespace std;


typedef struct ScoreStr
{
	int index_i ;
	int index_j ;
	float fScore;
}ScoreStr;

int cmp( const ScoreStr &a, const ScoreStr &b )
{
	if( a.fScore < b.fScore )
		return 1;
	else
		return 0;
}

 mpeg7_feature_color::mpeg7_feature_color()
{
	 m_y_coeff =64;
	 m_cb_coeff=28;
	 m_cr_coeff=28;
	 m_colorstructure=64;
}
 mpeg7_feature_color::~mpeg7_feature_color()
{

}


int mpeg7_feature_color::feature_get(cv::Mat &matColor,int feature_type)
{
    vec_y_coeff_Layout.clear();
    vec_cb_coeff_Layout.clear();
    vec_cr_coeff_Layout.clear();

	switch(feature_type)
	{
	case FEATURE_COLORLAYOUT:
		{
			ColorLayout ca(&(IplImage)(matColor));
			ca.ExtractFeatures(vec_y_coeff_Layout,vec_cb_coeff_Layout,vec_cr_coeff_Layout);	
		}
		break;
	case FEATURE_COLORSTRUCTURE:
		{
			ColorStructure colorstructure(&(IplImage)(matColor));
			vec_colorstructure =colorstructure.ExtractFeatures();
			break;
		}
		
	case FEATURE_COLORFUSION:
		{
			ColorStructure colorstructure(&(IplImage)(matColor));
			vec_colorstructure =colorstructure.ExtractFeatures();

			ColorLayout ca(&(IplImage)(matColor));
			ca.ExtractFeatures(vec_y_coeff_Layout,vec_cb_coeff_Layout,vec_cr_coeff_Layout);	
		}
		break;
	default:
		break;
	}
	
	return 0;
}


//score compare  during the feature: color structure
float mpeg7_feature_color::ScoreCompare(vector<int> &vec_fea_A,vector<int> &vec_fea_B, int feature_type)
{
	int i,j,k;
	float fScore_re=0;
	ColorStructure colorstructure;
	ColorLayout colorLayOut;

	switch(feature_type)
	{
	case FEATURE_COLORLAYOUT:
		{
			vector<int> vec_y_coeff_Layout_A;
			vector<int> vec_cb_coeff_Layout_A;
			vector<int> vec_cr_coeff_Layout_A;
			vector<int> vec_y_coeff_Layout_B;
			vector<int> vec_cb_coeff_Layout_B;
			vector<int> vec_cr_coeff_Layout_B;
			if (vec_fea_A.size() != (m_y_coeff+m_cb_coeff+m_cr_coeff) ||vec_fea_B.size() != (m_y_coeff+m_cb_coeff+m_cr_coeff))
			{
				printf("the feature size is not match with the feature type\n");
				return 10000;
			}
			for (i=0; i<m_y_coeff; i++)
			{
				vec_y_coeff_Layout_A.push_back(vec_fea_A[i]);
				vec_y_coeff_Layout_B.push_back(vec_fea_B[i]);
			}
			for (i=0; i<m_cb_coeff; i++)
			{
				vec_cb_coeff_Layout_A.push_back(vec_fea_A[i+m_y_coeff]);
				vec_cb_coeff_Layout_B.push_back(vec_fea_B[i+m_y_coeff]);
			}
			for (i=0; i<m_cr_coeff; i++)
			{
				vec_cr_coeff_Layout_A.push_back(vec_fea_A[i+m_y_coeff+m_cb_coeff]);
				vec_cr_coeff_Layout_B.push_back(vec_fea_B[i+m_y_coeff+m_cb_coeff]);
			}
			

			float fScore_layout = colorLayOut.distance(vec_y_coeff_Layout_A,vec_cb_coeff_Layout_A,vec_cr_coeff_Layout_A,vec_y_coeff_Layout_B,vec_cb_coeff_Layout_B,vec_cr_coeff_Layout_B);
			fScore_re = fScore_layout;
			break;
		}

	case FEATURE_COLORSTRUCTURE:
		{
			if (vec_fea_A.size() != m_colorstructure ||vec_fea_B.size() != m_colorstructure)
			{
				printf("the feature size is not match with the feature type\n");
				return 10000;
			}
			
			float fScore_cs = colorstructure.distance(vec_fea_A,vec_fea_B);
			fScore_cs =200*fScore_cs;
			fScore_re = fScore_cs;
			break;
		}

	case FEATURE_COLORFUSION:
		{
			vector<int> vec_y_coeff_Layout_A;
			vector<int> vec_cb_coeff_Layout_A;
			vector<int> vec_cr_coeff_Layout_A;
			vector<int> vec_y_coeff_Layout_B;
			vector<int> vec_cb_coeff_Layout_B;
			vector<int> vec_cr_coeff_Layout_B;

			vector<int> vec_colorstructure_A;
			vector<int> vec_colorstructure_B;
			if (vec_fea_A.size() != (m_y_coeff+m_cb_coeff+m_cr_coeff+m_colorstructure) ||vec_fea_B.size() != (m_y_coeff+m_cb_coeff+m_cr_coeff+m_colorstructure))
			{
				printf("the feature size is not match with the feature type\n");
				return 10000;
			}
			for (i=0; i<m_y_coeff; i++)
			{
				vec_y_coeff_Layout_A.push_back(vec_fea_A[i]);
				vec_y_coeff_Layout_B.push_back(vec_fea_B[i]);
			}
			for (i=0; i<m_cb_coeff; i++)
			{
				vec_cb_coeff_Layout_A.push_back(vec_fea_A[i+m_y_coeff]);
				vec_cb_coeff_Layout_B.push_back(vec_fea_B[i+m_y_coeff]);
			}
			for (i=0; i<m_cr_coeff; i++)
			{
				vec_cr_coeff_Layout_A.push_back(vec_fea_A[i+m_y_coeff+m_cb_coeff]);
				vec_cr_coeff_Layout_B.push_back(vec_fea_B[i+m_y_coeff+m_cb_coeff]);
			}
			for (i=0; i<m_colorstructure; i++)
			{
				vec_colorstructure_A.push_back(vec_fea_A[i+m_y_coeff+m_cb_coeff+m_cr_coeff]);
				vec_colorstructure_B.push_back(vec_fea_B[i+m_y_coeff+m_cb_coeff+m_cr_coeff]);
			}
		


			float fScore_layout = colorLayOut.distance(vec_y_coeff_Layout_A,vec_cb_coeff_Layout_A,vec_cr_coeff_Layout_A,vec_y_coeff_Layout_B,vec_cb_coeff_Layout_B,vec_cr_coeff_Layout_B);
			float fScore_cs = colorstructure.distance(vec_colorstructure_A,vec_colorstructure_B);
			fScore_cs =200*fScore_cs;

			fScore_re = (fScore_layout+fScore_cs)/2;
		}
		break;
	default:
		break;
	}

	return fScore_re;
}

//score compare  by type. don't need to give the parameters
float mpeg7_feature_color::ScoreCompare(mpeg7_feature_color &mObjectB, int feature_type)
{
	float fScore_re=0;
	ColorStructure colorstructure;
	ColorLayout colorLayOut;

	switch(feature_type)
	{
	case FEATURE_COLORLAYOUT:
		{
			float fScore_layout = colorLayOut.distance(this->vec_y_coeff_Layout,this->vec_cb_coeff_Layout,this->vec_cr_coeff_Layout,mObjectB.vec_y_coeff_Layout,mObjectB.vec_cb_coeff_Layout,mObjectB.vec_cr_coeff_Layout);
			
			fScore_re = fScore_layout;
			break;
		}
		
	case FEATURE_COLORSTRUCTURE:
		{
			float fScore_cs = colorstructure.distance(this->vec_colorstructure,mObjectB.vec_colorstructure);
			fScore_cs =200*fScore_cs;
			fScore_re = fScore_cs;
			break;
		}
		
	case FEATURE_COLORFUSION:
		{
			float fScore_layout = colorLayOut.distance(this->vec_y_coeff_Layout,this->vec_cb_coeff_Layout,this->vec_cr_coeff_Layout,mObjectB.vec_y_coeff_Layout,mObjectB.vec_cb_coeff_Layout,mObjectB.vec_cr_coeff_Layout);
			float fScore_cs = colorstructure.distance(this->vec_colorstructure,mObjectB.vec_colorstructure);
			fScore_cs =200*fScore_cs;

			fScore_re = (fScore_cs+fScore_layout)/2;
		}
		break;
	default:
		break;
	}

	return fScore_re;
}


#ifdef WIN32
int main_test(int argc,char *argv[])
{
	int  m,n,k;
	string list_file=argv[1];
	string out_file=list_file;
	int pos =out_file.rfind('/',out_file.length());
	if (pos<0)
	{
		 pos =out_file.rfind('\\',out_file.length());
	}
	string sa =out_file.substr(0,pos)+"/reslut.txt";
	
	ofstream ofs(sa);
	if (!ofs.good())
	{
		cout<<out_file<<" load fail\n";
	}
	ifstream ifs(list_file);
	if (!ifs.good())
	{
		cout<<list_file<<" load fail\n";
	}

	string img_name;
	
	vector<cv::Mat> arr_img;
	while(ifs>>img_name)
	{
		cv::Mat img = cv::imread(img_name);
		if (img.data == NULL)
		{
			cout<<img_name<<" read fail\n";
		}
		arr_img.push_back(img);
	}

	for (k=0; k<arr_img.size(); k++)
	{
		ostringstream ostr;
		ostr<<"D:/save/ColorLaout/"<<k<<"/";
		string sname= ostr.str();
		_mkdir(sname.c_str());
		ostr.str("");
	
	}

	int i,j;
	for (i=0; i<arr_img.size(); i++)
	{
		float fScore;
		vector< ScoreStr > vecScore;
		int nTestMethod =2 ;

		vector<int> vec_y_coeff_A,vec_y_coeff_B;
		vector<int> vec_cb_coeff_A,vec_cb_coeff_B;
		vector<int> vec_cr_coeff_A,vec_cr_coeff_B;

		switch(nTestMethod)
		{
		case 1:
			{
				ColorStructure colorstructureA(&(IplImage)(arr_img[i]));
				vec_y_coeff_A =colorstructureA.ExtractFeatures();
			}

			break;
		case 2:
			{
				ColorLayout ca(&(IplImage)(arr_img[i]));

				ca.ExtractFeatures(vec_y_coeff_A,vec_cb_coeff_A,vec_cr_coeff_A);

			}

			break;
		default:
			break;
		}

		for (j=0; j<arr_img.size();j++)
		{
			switch(nTestMethod)
			{
				case 1:
			       {
				      ColorStructure colorstructureB(&(IplImage)(arr_img[j]));
				      vec_y_coeff_B =colorstructureB.ExtractFeatures();
					  fScore = colorstructureB.distance(vec_y_coeff_A,vec_y_coeff_B);
			       }
					
					break;
				case 2:
					{
						ColorLayout colorLayOutB(&(IplImage)(arr_img[j]));

						colorLayOutB.ExtractFeatures(vec_y_coeff_B,vec_cb_coeff_B,vec_cr_coeff_B);
						
						fScore = colorLayOutB.distance(vec_y_coeff_A,vec_cb_coeff_A,vec_cr_coeff_A,vec_y_coeff_B,vec_cb_coeff_B,vec_cr_coeff_B);

						vec_y_coeff_B.clear();
						vec_cb_coeff_B.clear();
						vec_cr_coeff_B.clear();
					}

					break;
				default:
					break;
			}


			ScoreStr strScoreA;
			strScoreA.index_i =i;
			strScoreA.index_j=j;
			strScoreA.fScore = fScore;

			vecScore.push_back(strScoreA);

			cout<<i<<"__"<<j<<" Match score is "<<fScore<<endl;
			ofs<<i<<"__"<<j<<" Match score is "<<fScore<<endl;
		}	

		// here  sort

		sort(vecScore.begin(),vecScore.end(),cmp);



		int ik=0;
		for (k=0; k<vecScore.size(); k++)
		{
			ostringstream ostr;

			ostr<<"D:/save/ColorLaout/"<<i<<"/00000"<<ik++<<"_"<<vecScore[k].index_i<<"_"<<vecScore[k].index_j<<"_"<<vecScore[k].fScore<<".jpg";
			string s_name = ostr.str();
			printf("%s\n",s_name.c_str());
			imwrite(s_name.c_str(),arr_img[vecScore[k].index_j]);

			ostr.str("");
		}
		
		vecScore.clear();

	}

	ifs.close();
	ofs.close();
	
	//IplImage* img=cvLoadImage(argv[1],CV_LOAD_IMAGE_ANYCOLOR);
	//std::cout<<"img depth = "<<img->depth<<endl;
	//ColorStructure colorstructure(img);
// 	ColorLayout colorlayout(img);
// 	DominantColor dominantcolor(img);
// 	EdgeHistogram histogram(img);
// 	HomogeneousTexture homogeneoustexture(img);
// 	RegionShape regionshape(img);
// 	ContourShape contourshape(img);
// 	ScalableColor scalablecolor(img);

	
//	vector<int> feature=colorstructure.ExtractFeatures();
	/*
	for(i=0;i<feature.size();i++)
		std::cout<<feature[i]<<" ";
	std::cout<<std::endl;
	std::cout<<std::endl;

	feature=colorlayout.ExtractFeatures();
	for(i=0;i<feature.size();i++)
		std::cout<<feature[i]<<" ";
	std::cout<<std::endl;
	std::cout<<std::endl;


    vector<int>	feature=dominantcolor.ExtractFeatures();
	std::cout<<feature.size()<<std::endl;
	std::cout<<std::endl;
	unsigned int i;
	for(i=0;i<feature.size();i++)
		std::cout<<feature[i]<<" ";
	std::cout<<std::endl;
	std::cout<<std::endl;

	
	feature=histogram.ExtractFeatures();
	for(i=0;i<feature.size();i++)
		std::cout<<feature[i]<<" ";
	std::cout<<std::endl;
	std::cout<<std::endl;

	feature=homogeneoustexture.ExtractFeatures();
	for(i=0;i<feature.size();i++)
		std::cout<<feature[i]<<" ";
	std::cout<<std::endl;
	std::cout<<std::endl;
*/
	/*
	vector<int> feature=regionshape.ExtractFeatures();
	unsigned int i;
	for(i=0;i<feature.size();i++)
		std::cout<<feature[i]<<" ";
	std::cout<<std::endl;
	std::cout<<std::endl;
	*/

	/*
	vector<int> feature=contourshape.ExtractFeatures();
	unsigned int i;
	for(i=0;i<feature.size();i++)
        std::cout<<feature[i]<<" ";
	std::cout<<std::endl;
	std::cout<<std::endl;
	*/

/*
	feature=scalablecolor.ExtractFeatures();
	for(i=0;i<feature.size();i++)
		std::cout<<feature[i]<<" ";
	std::cout<<std::endl;
	std::cout<<std::endl;
	*/
	return 0;
}
#endif
// 
// int main( int argc, char** argv )
// {
// 	CvPoint pt1, pt2;
// 	double d0, d1;
// 	int n1, n2, k, i, m = 0;
// 
// 	cv::Mat img1 = cv::imread( img1_file );
//  	if( img1.data == NULL )
//  		printf( "unable to load image from %s", img1_file );
// 	cv::Mat img2 = cv::imread( img2_file );
// 	if( img2.data == NULL )
// 		printf( "unable to load image from %s", img2_file );
// 
// 	int descSize_A ;
// 	Frame  fA(img1);
// 	
// 	XM::ColorStructureExtractionTool* csdt = new  XM::ColorStructureExtractionTool();
// 	XM::ColorStructureDescriptor* csd = csdt->GetDescriptor();
// 	csdt->extract(fA.image, descSize_A);
// 
// 
// 	fprintf( stderr, "Found %d total matches\n", m );
// 	cvNamedWindow( "Matches", 1 );
// 	cvWaitKey( 0 );
// 	return 0;
// }
