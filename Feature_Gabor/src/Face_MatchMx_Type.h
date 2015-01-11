#ifndef _MX_FACE_TYPE_H_
#define _MX_FACE_TYPE_H_

 #if defined(__cplusplus)
 extern "C"
 {
 #endif


typedef struct MX_MatrixStruct 
{
	float *data;
	int height, width;
} Mx_Matrix;

typedef struct
{
	Mx_Matrix *RealPart[5][8];
	Mx_Matrix *ImagePart[5][8];

	int iKernelSize[5];
	int iScaleNum;
	int iThetaNum;
}GaborFilter;
	//	
	typedef struct _mx_size_ {
		int cx;
		int cy; 
	} MX_SIZE;


	typedef struct _mx_proj_ {
		int dimension;
		int length;
		float * mean;
		float * base;
	} MX_PROJ;

	typedef struct _mx_g_handle_fr
	{

		//
		MX_PROJ   gpointProjM,gpointProjMP1,gpointProjMP2,gpointProjMP3;


		//
		int gpointM_InitialFlag,gpointMP1_InitialFlag,gpointMP2_InitialFlag,gpointMP3_InitialFlag;

		int Face_MaskI[112*144], Face_MaskII[112*144];

		GaborFilter gfilter_MxHandle ;
	} MX_G_HANGLE_FR;
    

 #if defined(__cplusplus)
 }
 #endif
#endif
