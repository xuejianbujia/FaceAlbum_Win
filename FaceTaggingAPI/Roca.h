#ifndef ROCA_H
#define ROCA_H

#include<vector>
using namespace std;


enum
{
      ROC_EYE_NORMAL        =0,
	  ROC_EYE_SUM_NORMAL    =1,
	  ROC_EYE_SUM_PENALITY  =2
};

class EyeRoc
{
public:
	EyeRoc();
	~EyeRoc();

	int SetRocData(float ferror);

	double GetRocStatiscal(int Method=ROC_EYE_NORMAL);
public:
	vector<float> m_vec_error;
	int m_HistBin[100];
	int m_HistNum;
};

#endif