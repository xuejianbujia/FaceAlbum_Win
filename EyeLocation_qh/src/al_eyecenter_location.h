
#ifndef _EYE_CENTER_LOCAL_H
#define _EYE_CENTER_LOCAL_H

#include "amcomdef.h"
#include "asvloffscreen.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef MVoid*         ASEL_ENGINE;

//face orientation code
enum ASEL_OrientCode{
	ASEL_FOC_0			= 0x1,		/* 0 degree */
	ASEL_FOC_90			= 0x2,		/* 90 degree */	
	ASEL_FOC_270		= 0x3,		/* 270 degree */
	ASEL_FOC_180        = 0x4,      /* 180 degree */
	ASEL_FOC_30			= 0x5,		/* 30 degree*/
	ASEL_FOC_60			= 0x6,		/* 60 degree*/
	ASEL_FOC_120		= 0x7,		/* 120 degree*/
	ASEL_FOC_150		= 0x8,		/* 150 degree*/
	ASEL_FOC_210		= 0x9,		/* 210 degree*/
	ASEL_FOC_240		= 0xa,		/* 240 degree*/
	ASEL_FOC_300		= 0xb,		/* 300 degree*/
	ASEL_FOC_330		= 0xc		/* 330 degree*/
};

typedef struct
{
    MLong      x;                       //x position
    MLong      y;                       //y position
    MLong      width;                   //width
    MLong      height ;                 //height  
}ASEL_Rect;

typedef struct
{
  ASEL_Rect    eye_rect[2];             //the bounding box of the eye
  MLong       total ;                  //the number of eyes detected
}ASEL_Eyes ;

typedef struct
{ 
	MRECT 		*pFaceRectArray;		/* The bounding box of face */
	MLong       *pFaceOrientArray;   /* The orientation of face */
	MLong		 lFaceNumber;			/* number of faces detected	*/
}ASEL_FACEINPUT, *LPASEL_FACEINPUT;

typedef struct 
{
	ASEL_Eyes *pEyesRectArray;         /* The bounding box of eyes with same length as pFaceRectArray in ECL_FACEINPUT.
	                                   * Only rect information with flag equal to "1" is valid.*/
	MLong *pFlagArray;                /* The location result flag array with same length as pFaceRectArray in ECL_FACEINPUT.
	                                   * "1" representing two eyes are located, otherwise the flag is set to "0" */
	MLong lFaceNumber;
	MLong lEyesNumber;
}ASEL_EYESRES, *LPASEL_EYESRES;

typedef MRESULT (*ASEL_FNPROGRESS) (
	MVoid		*pParam1,				/*[in] Not used*/
	MVoid		*pParam2				/*[in] Caller-defined data*/
);

/************************************************************************
 * The function will be used to Initialize the eye center location engine. 
 ************************************************************************/
MRESULT ASEL_InitialEngine(
	 MHandle      hMemMgr,				// [in] User defined memory manager
	 ASEL_ENGINE   *pEngine				// [out] Pointer pointing to an eye center location engine
);

/************************************************************************
 * The function used to locate the eyes' centers automatically.  
 *
 * Comment:
 *  The input image should contain faces whose positions and sizes are saved in pFaceRes
 *  The result will be  loaded to pEyesRes.
 ************************************************************************/
MRESULT ASEL_EyeCenterLocation(
	 ASEL_ENGINE         Engine,		/* [in]  The input eye center location engine. */
	 LPASVLOFFSCREEN	 pImgInfo,		/* [in]  The original image data*/
	 LPASEL_FACEINPUT    pFaceRes,		/* [in]  The face position*/
	 LPASEL_EYESRES      pEyesRes,		/* [out] eyes' centers location result */
	 ASEL_FNPROGRESS fnCallback,	/* [in]  The callback function */
	 MVoid*				 pParam			/* [in]  Caller-specific data that will be passed into the callback function */
);

/************************************************************************
 * The function used to Uninitialize the eye center location module. 
 ************************************************************************/
MRESULT ASEL_UnInitialEngine(
	 ASEL_ENGINE   Engine  // [in] The input eye center location engine.
);

#ifdef __cplusplus
}
#endif

#endif //_EYE_CENTER_LOCAL_H