#ifndef _FACE_FEATURE_MX_H_
#define _FACE_FEATURE_MX_H_

// #if defined(__cplusplus)
// extern "C"
// {
// #endif

	/**************************************************************************************************
	 * DESCRIPTION
	 *	 This function does the necessary initializations for face operations. 
	 *   allocate memories, and load .dat files.
	 * Input :
	 *	 DataPath: the path for all data file 
	 *	 Parameters:	control the initialization method.
	 * Output :
	 *	 Message     Error message, the size is 256 bytes.
	 * Return : 
	 *   0 success
	 *	 -1 failed the data  file can't open
	 *	 -2 memory allocate is failed
	 *	 -3 initialization has been done!
	 **************************************************************************************************/
	 int Face_FeatureExtractor_Init_MX(MX_G_HANGLE_FR *pMxGHandleFR,const char* DataPath, int MethodType1,int MethodType2, char* Message);

	
   /**************************************************************************************************
	* DESCRIPTION
	* Input :
	*   
	* Output :
	*   Error message, the size is 256 bytes.
	* Return :
	*    0          success.
	*   -1          error.
	*****************************************************************************************************/
	int Face_FeatureExtractor_Destroy_MX(MX_G_HANGLE_FR *pMxGHandleFR,char* Message);


   /**************************************************************************************************
	* DESCRIPTION
	*  
	* Input :
	*   type the type of extracted feature
	* Output :
	*   size is the feature length
	* Return :
	*    0          success.
	*   -1          no initialization.
	*****************************************************************************************************/
	int Face_GetFeatureInfo_MX(MX_G_HANGLE_FR *pMxGHandleFR,int MethodType1,int MethodType2, int *size,  char *Message);

	/**************************************************************************************************
	* DESCRIPTION
	*   The function only fulfill feature extraction 
	* Input :
	*   pNormFace    The normalized gray face image, image width = 112, image_height = 144;
	*   int method1: method type.
	*   int method2: preprocess type.
	* Output :
	*   pFeature     The feature templates.
	* Return :
	*    0          success.
	*   -1          no initialization.
	*****************************************************************************************************/
    int Face_FeatureExtractor_Norm_MX(MX_G_HANGLE_FR *pMxGHandleFR,unsigned char *pNormFace, int iWidth, int iHeight, 
	int MethodType1,int MethodType2, float *pFeature,  char *Message);


	/**************************************************************************************************
	* DESCRIPTION
	*   This function is to verify two input template and output the match score; 
	* Input :
	*   pProbe, pGallery input two feature template;
	*   fea_len the template length;
	*	Parameters[256] not use presently, it can be set to zero;
	*   PNorm GNorm are the Norms of probe and gallary features (<0: not input; >0: input)
	* Output :
	*   pScore  match score, the range is [0 1];
	* Return :
	*    0          success.
	*   -1          error.
	*****************************************************************************************************/
	int Face_FeaturVerify_MX(const float *pProbe, const float *pGallery, int fea_len,  float *pScore, char *Message);
	
// #if defined(__cplusplus)
// }
// #endif
#endif