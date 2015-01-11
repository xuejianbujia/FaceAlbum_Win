#ifndef _ECL_HAAR_H
#define _ECL_HAAR_H

#if _MSC_VER >= 1200
#pragma warning( disable: 4711 ) /* function AAA selected for automatic inline expansion */
#endif

#include "al_afbase.h"
#include "al_aftypes.h"
#include "al_eyecenter_location.h"
#include "ammem.h"

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#define ecl_LoadLUTHidCascade PRE_FUN(ecl_LoadLUTHidCascade)
#define ecl_HaarDetectObjects PRE_FUN(ecl_HaarDetectObjects)

/****************************************************************************************
                             Haar-like object detection                                *
****************************************************************************************/

#define AF_HAAR_FEATURE_MAX  3
#define _SINGLE_ORIENT_
#define NBINS	48
#define IBINS	47

#define SCALE_FACTOR 1.15
#define ECL_PI  3.1415926

#define MIN_OPEATION_NSCALE 2

#define AF_ABS(x) ((x)>0 ? (x) :-(x))
#define AF_CLIP(x) (MByte)(((x)&(~255)) ? ((-(x))>>31):(x))
#define LINE_BYTES(Width, BitCt)    (((MLong)(Width) * (BitCt) + 31) / 32 * 4)

#define ECL_PARAM_ASSERT(x) if(!(x)) return MERR_INVALID_PARAM

typedef MLong sumtype;
typedef MLong sqsumtype;
//typedef int64 sqsumtype;

#ifdef _SINGLE_ORIENT_
#define ORIENT_NUM 1
#else
#define ORIENT_NUM 3
#endif

#define AF_PARAM_ASSERT(PARAM)	\
		if(!(PARAM))				\
		{						\
			return MERR_INVALID_PARAM;	\
		}	

#define AF_MEM_ALLOC_ASSERT(PARAM)	\
		if(!(PARAM))				\
		{						\
			return MERR_NO_MEMORY;	\
		}

/***************** eye center location associated but not for customers *************************/ 

//typedef struct 
//{
//	ASFL_Rect *pEyesRectArray;         /* The bounding box of single eyes with same length as pFaceRectArray in ECL_FACEINPUT.
//	                                   * Only rect information with flag equal to "1" is valid.*/
//	MLong *pFlagArray;                /* The location result flag array with same length as pFaceRectArray in ECL_FACEINPUT.
//	                                   * "1" representing two eyes are located, otherwise the flag is set to "0" */
//	MLong lFaceNumber;
//	MLong lEyesNumber;
//}ASFL_SINGLEEYERES, *LPASFL_SINGLEEYERES;

/***************** eye center location associated but not for customers *************************/ 


typedef struct ecl_AfRECT
{
	MLong x;
	MLong y;
	MLong width;
	MLong height;
}ecl_AfRECT;

typedef struct ecl_FeatureRect
{
	ecl_AfRectA r;
	MLong weight;
}ecl_FeatureRect;

typedef struct ecl_AfHaarFeature
{
 	ecl_FeatureRect rect[AF_HAAR_FEATURE_MAX];
}
ecl_AfHaarFeature;


typedef MLong GFIXED;

typedef struct ecl_HidFeatureRect
{
    sumtype *p0, *p1, *p2, *p3;

}ecl_HidFeatureRect;

typedef struct ecl_AfHidHaarFeature
{
	ecl_HidFeatureRect * rect;
}
ecl_AfHidHaarFeature;

typedef struct ecl_AfHidScaledClassifier
{
    ecl_AfHidHaarFeature* feature;
	ecl_AfHaarFeature origFeature;	
    MLong min;
    MLong r;
	
	MLong LUT[NBINS];	
}
ecl_AfHidScaledClassifier;

typedef struct ecl_AfHidScaledStage
{
	MLong threshold;	
	MLong  count;
	MLong n2h,n2v,n3h,n3v,n4,nds,ndd,na3h,na3v,na4h,na4v;
	ecl_AfHaarFeature * origFeature;
	
	sumtype ***ptr[ORIENT_NUM];
	MLong * rmin;
	MLong * LUT;
}
ecl_AfHidScaledStage;

typedef struct ecl_AfHidScaledCascade
{
	MLong  count;
	MLong nscales;
		
	ecl_AfSize origWindowSize;	
#ifdef _AMINT64_
	int64 * invWindowArea;
#else
	MLong * invWindowArea;
#endif
	
	ecl_AfHidScaledStage * stageClassifier;

	ecl_AfMat sum;
	ecl_AfMat sqsum;

	sqsumtype *pq0, *pq1, *pq2, *pq3;
	sumtype *p0, *p1, *p2, *p3;
}
ecl_AfHidScaledCascade;

typedef struct ecl_AfAvgComp
{
    ecl_AfRect rect;
    MLong neighbors;
	MLong conf;
}
ecl_AfAvgComp;


typedef struct ecl_AfEngine
{
	MHandle hMemMgr;
	ecl_AfHidScaledCascade *phid_cascade;
	ASEL_Eyes *pCoupleEyesRectArray;
	MLong *pCoupleEyesFlagArray;
//	ASFL_Rect *pSingleEyesRectArray;
//	MLong *pSingleEyesFlagArray;
}ecl_AfEngine;

ecl_AfHidScaledCascade* ecl_LoadLUTHidCascade(MHandle hMemMgr,const MByte *pModelBuf,MLong nScales,int Model);
ecl_AfSeq* ecl_HaarDetectObjects(MVoid* srcimg,
							 ecl_AfHidScaledCascade* hid_cascade,
							 ecl_AfMemStorage * storage ,
							 MHandle hMemMgr,
							 MLong  min_neighbors);	
#endif