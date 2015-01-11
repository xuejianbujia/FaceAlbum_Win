// AeyeFaceRecog.h

#ifndef _AeyeFaceRecog_H
#define _AeyeFaceRecog_H

#include "AEyeBaseFaceStruct.h"
#include "Face_MatchMx_Type.h"


#define  FALSE     0
#define  TRUE      1
#define  NULL      0
#define  IMGWIDTH   112
#define  IMGHEIGHT  144
#define  FEA_LEN_FX_GABOR    800
#define FEAT_GPOINT_MX 1

//初始化检测：
//返回值说明：返回0，成功初始化
//返回非零值，比如-1，未初始化
int  BeInitRight();

//初始化Gabor核，载入EnsemblePCALDA模型
//调用函数：    long InitFeatExtract(char *pModelDir, int *Parameters, char *Message)
//函数参数说明：char *pModelDir：模型所在文件夹，文件夹内应包含1.dat即模型文件。
//              int *Parameters：传入NULL
//              char *Message：  这里应传入NULL
//返回值说明： 如果成功初始化，则返回0
//返回非0，则出错
int  InitFeatExtract(MX_G_HANGLE_FR *pMxGHandleFR,char *pModelDir, int *Parameters, char *Message);

//释放内存
//释放初始化时所占用的内存
//函数参数说明：int *Parameters：
//              char *Message：  这里应传入NULL
//返回值说明： 如果原来已经成功初始化，则释放内存，并且返回0
//如果原来没有成功初始化，则不释放，并且返回1
int  UnInitialFeatExtract(MX_G_HANGLE_FR *pMxGHandleFR,int *Parameters, char *Message);


// 返回用于识别的人脸区域的宽度
int  GetNormFaceWd();

// 返回用于识别的人脸区域的宽度
int  GetNormFaceHt();


//计算特征
//对于输入图像，计算其DOG+Gabor特征，并计算其在PCALDA投影后的系数。
//调用函数：long ExtracFeature(OUT float *pFeat, byte *pImg_64_80, byte *pImg_112_144);
//函数参数说明：OUT float *pFeat：特征，其长度由GetFeatureLen()得出
//              byte *pImg_64_80：64*80的灰度图像数据
//              byte *pImg_112_144：112*144的图像数据，这里暂时不支持，应传入NULL
//返回值说明：  如果模型正确，特征计算正确，则返回0
//              返回-1，没有计算特征，即特征数目为0
//              返回-2，模型没有正确初始化
//			  特征组成说明：特征共20段，每段特征有LDADim个特征组成，并且，将其平方根计算保存在每段的尾部，用于快速计算cos距离
// 共20段，即把图像分为20个小区域
int  ExtracFeature(MX_G_HANGLE_FR *pMxGHandleFR,OUT float *pFeat, byte *pImg_64_80, byte *pImg_112_144);



//依据眼睛位置来提取特征 人脸归一化操作被封装在该函数里面
int  ExtracFeature_ByImageEye(MX_G_HANGLE_FR *pMxGHandleFR,OUT float *pFeat, byte *pImageOrg,int ColorBit,int iWidth,int iHeight,
	int xleft,int yleft,int xright,int yright);


// 得到所需内存长度，单位为sizeof(float)
// 如果返回为-1，则表示模型没有初始化，出错
// 返回正整数，为正确的特征长度
int  GetFeatureLen();

// 计算两个特征的相似度
// 调用函数：int ComputSimScore(OUT float *pScore, float *pFeatModel, float *pFeatMatch);
// 函数参数说明：float *pScore：输出两特种间的相似度（Cos距离）
//               float *pFeatModel：输入特征
// 			  float *pFeatMatch：输入特征
int  ComputSimScore(OUT float *pScore, float *pFeatModel, float *pFeatMatch);


#endif