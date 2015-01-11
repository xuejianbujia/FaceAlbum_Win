#include"Roca.h"

EyeRoc::EyeRoc()
{
	for(int i=0; i<100; i++)
	{
		m_HistBin[i]=0;
	}
	m_HistNum = 0;
}

EyeRoc::~EyeRoc()
{
}

int EyeRoc::SetRocData(float ferror)
{
	m_vec_error.push_back(ferror);

	int iBin = ferror*100;
	if(iBin >=100)
		iBin=99;
	m_HistBin[iBin] ++ ;
	m_HistNum++ ;
	return 0;
}


double EyeRoc::GetRocStatiscal(int Method)
{
	double fRoc = 0 ;
	if (m_vec_error.size()<=0)
	{
		return fRoc;

	}
	switch (Method)
	{
	case ROC_EYE_NORMAL:
		{
			break;
		}
	case ROC_EYE_SUM_PENALITY:
		{
			int num = m_vec_error.size();
			for (int i = 0; i < m_vec_error.size(); i++)
			{
				double ftmp =0;
				ftmp= 100*m_vec_error[i];
				fRoc+=(ftmp*ftmp) ;
			}

			fRoc /=num ;
			break;
		}
	case ROC_EYE_SUM_NORMAL:
		{
			int num = m_vec_error.size();
			for (int i = 0; i < m_vec_error.size(); i++)
			{
				double ftmp =0;
				
				{
					ftmp= 100*m_vec_error[i];
				}
				fRoc+=ftmp ;
			}

			fRoc /=num ;
			break;
		}
	default:
		break;
	}

}