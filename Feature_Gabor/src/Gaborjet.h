#ifndef _GABORJET_FACE_GABOR_H_
#define _GABORJET_FACE_GABOR_H_
// #ifdef __cplusplus
// extern "C" {
// #endif

//
#define FREQUENCY     5
#define ORIENTATION   8

#include "Face_MatchMx_Type.h"



int Mx_GaborFilter_Initial(GaborFilter &gfilter_Mx);
int Mx_GaborFilter_UnInitial(GaborFilter &gfilter_Mx);

/*
int Mx_GaborFilter_GetExtImage(unsigned char *srcImage,int iHeight,int iWidth,unsigned char *srcImageExt,int *iHeightExt,int *iWidthExt,int scale);
int Mx_Point_CalGaborMagFeature(unsigned char *srcImage, int iWidth, int iHeight,
									   unsigned char *srcImageExt, int iWidthExt, int iHeightExt,
									   int x_pos, int y_pos,
									   int scale,int theta,float*desMag);
int TGaborPixel(unsigned char *srcImage, int iWidth, int iHeight,int x_pos, int y_pos,float*desMag);
*/
int GaborPoints(GaborFilter &gfilter_Mx,unsigned char * pImage, const int w, const int h, int block_h,int block_w,int iMPRI[4], float * MPRIFeature);

// 	#ifdef __cplusplus
// 	}
// 	#endif

	#endif
