
#include <string.h>
#include <malloc.h>
#include <math.h>

const int  EYE_DIS = 54 ; //标准化人脸坐标系下的双眼距离	// 
const int  NORMAL_EYE_XLEFT = 47;		// 标准化人脸坐标系下左眼的横坐标
const int  NORMAL_EYE_YLEFT = 62;		// 标准化人脸坐标系下左眼的纵坐标
const int  NORMAL_EYE_XRIGHT =100;	// 标准化人脸坐标系下右眼的横坐标
const int  NORMAL_EYE_YRIGHT =62;	// 标准化人脸坐标系下右眼的纵坐标
const int NORMAL_FACE_W =149;
const int NORMAL_FACE_H =149;

#define CLIP(a) ((a)>255?255:((a)<0?0:(a)))
#define ROUND(a) ((int)(a+0.5))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

typedef struct s_Point
{
	int x;
	int y;
}
s_Point;


typedef float MatrixType;

#ifndef MATRIX_ENUM
#define MATRIX_ENUM
typedef enum mul_ero{WandHnotmatch, CannotCreateMatHead, Success};
typedef enum Invert_type{Matrix_LU, Matrix_SUV, Matrix_SUV_SYM};
#endif

/*************************************************************************************************/
/* 矩阵加法 ,   dstMatrix =  srcMatrix1 + srcMatrix2,矩阵为width列，height行                     */
/*************************************************************************************************/
int MatrixAdd(  MatrixType *dstData, MatrixType *srcData1, MatrixType *srcData2,  int width,  int height)
{

	int i,j;
	for(i = 0;i < height; ++i)
		for(j = 0;j < width; ++j)
			dstData[i*width + j] = srcData1[i*width + j] + srcData2[i*width + j];

	return 0;

}



/*************************************************************************************************/
/* 矩阵减法 ,   dstMatrix =  srcMatrix1 + srcMatrix2,矩阵为width列，height行                     */
/*************************************************************************************************/
int MatrixSub( MatrixType *dstData,  MatrixType *srcData1, MatrixType *srcData2,  int width,  int height)
{

	int i,j;
	for(i = 0;i < height; ++i)
		for(j = 0;j < width; ++j)
			dstData[i*width + j] = srcData1[i*width + j] - srcData2[i*width + j];

	return 0;

}



int MatrixInvert( MatrixType *dstData, MatrixType *srcData,  int width,  int height)
{
	int i,j,k;
	MatrixType div,temp;
	MatrixType *out;
	int *is,*js;

	if(width != height)
		return -1;

	out = (MatrixType *)malloc(sizeof(MatrixType)*height*width);
	is = (int *)malloc(sizeof(int)*height);
	js = (int *)malloc(sizeof(int)*height);
	for(i = 0;i < height; ++i)
	{
		is[i] = i;
		js[i] = i;
	}

	// start from first column to the next 
	for(k = 0;k < height; ++k)
	{
		div = 0;
		for(i = k;i < height; ++i)
			for(j = k;j < height; ++j)
			{
				if(fabs(srcData[i*width + j]) > div)
				{
					div = fabs(srcData[i*width + j]);
					is[k] = i;
					js[k] = j;
				}
			}
			if(fabs(div) < 1e-10)
			{
				free( out );
				free( is );
				free( js );
				return -1;
			}
			if(is[k] != k)
			{
				for(j = 0;j < height; ++j)
				{
					temp = srcData[k*width + j];
					dstData[k*width + j] = srcData[is[k]*width + j];
					dstData[is[k]*width + j] = temp;
				}
			}
			if(js[k] != k)
			{
				for(i = 0;i < height; ++i)
				{
					temp = srcData[i*width + k];
					dstData[i*width + k] = srcData[i*width + js[k]];
					dstData[i*width + js[k]] = temp;
				}
			}
			dstData[k*width + k] = 1/srcData[k*width + k];
			for(j = 0;j < height; ++j)
			{
				if(j != k)
					dstData[k*width + j] = srcData[k*width + j]*srcData[k*width + k];
			}
			for(i = 0;i < height; ++i)
			{
				if(i != k)
				{
					for(j = 0;j < height; ++j)
					{
						if(j != k)
							dstData[i*width + j] -= srcData[i*width + k]*srcData[k*width + j];
					}
				}
			}
			for(i = 0;i < height; ++i)
			{
				if(i != k)
					dstData[i*width + k] = -srcData[i*width + k]*srcData[k*width + k];
			}							
	}

	for(k = height - 1;k >= 0; --k)
	{
		for(j = 0;j < height; ++j)
			if(js[k] != k)
			{
				temp = srcData[k*width + j];
				dstData[k*width + j] = srcData[js[k]*width + j];
				dstData[js[k]*width + j] = temp;
			}
			for(i = 0;i < height; ++i)
				if(is[k] != k)
				{
					temp = srcData[i*width + k];
					dstData[i*width + k] = srcData[i*width + is[k]];
					dstData[i*width + is[k]] = temp;
				}
	}
	free( is );
	free( js );
	free( out );
	return 0;
}



/*************************************************************************************************/
/* 矩阵乘法 ,   dstMatrix为矩阵C， srcMatrix1为h1 * w1 维矩阵A，srcMatrix2为h2 *w2维矩阵B        */
/*       C = AB,C为h1 * w2维矩阵                                                                 */
/*************************************************************************************************/
int MatrixMul( MatrixType *dstData, MatrixType *srcData1,  int w1, int h1,
	MatrixType *srcData2,  int w2,  int h2)
{
	int i,j,k;
	if(w1 != h2)
	{
		return -1;
	}


	for(i = 0;i < h1; ++i)
	{
		for(j = 0;j < w2; ++j)
		{
			MatrixType sum = 0;
			for(k = 0;k < w1; ++k)
				sum += srcData1[i*w1 + k]*srcData2[k*w2 + j];
			dstData[i*w2 + j] = sum;
		}
	}

	return 0;

}



int MatrixTransPos( MatrixType *pDstData, MatrixType *pSrcData, int w, int h)
{

	int i,j;
	if(pDstData == NULL)//转置结果保留在原矩阵中
	{
		MatrixType *m3;

		m3 = (MatrixType *)malloc(sizeof(MatrixType)*h*w);
		for(i = 0;i < w; ++i)
			for(j = 0;j < h; ++j)
			{
				m3[i*h + j] = pSrcData[j*w + i];
			}
			for(i = 0;i < h; ++i)
				for(j = 0;j < w; ++j)
					pDstData[i*w + j] = m3[j*w + i];
			free( m3 );
	}
	else
	{
		for(i = 0;i < w; ++i)
			for(j = 0;j < h; ++j)
			{
				pDstData[i*h + j] = pSrcData[j*w + i];
			}
	}
	return 0;

}
// 确定点集origPtPos到点集desPtPos的仿射变换关系
// 此仿射变换关系共四个参数
void GetAffineMatrix_Pts(float* affineMatrix, float* origPtPos, float* desPtPos, int ptNum)
{
	float temp[4];
	float invers_a_at[4*4];

	int ptOrder;
	int arrayOrder;
	float x;
	float y;

	int ptCoordNum = (ptNum<<1);

	float* a = (float *)malloc(sizeof(float)*ptCoordNum*4);
	float* at = (float *)malloc(sizeof(float)*ptCoordNum*4);

	for(ptOrder = 0; ptOrder< ptNum; ptOrder ++)
	{
		x = origPtPos[(ptOrder<<1)];
		y = origPtPos[(ptOrder<<1)+1];

		arrayOrder = (ptOrder<<3);

		a[arrayOrder] = x;
		a[arrayOrder+1] = -y;
		a[arrayOrder+2] = 1;
		a[arrayOrder+3] = 0;

		a[arrayOrder+4] = y;
		a[arrayOrder+5] = x;
		a[arrayOrder+6] = 0;
		a[arrayOrder+7] = 1;
	}


	MatrixTransPos(at, a, 4, ptCoordNum);

	//MatrixMulti(at, 4, ptCoordNum, a, ptCoordNum, 4, invers_a_at);
	MatrixMul(invers_a_at, at, ptCoordNum, 4, a, 4, ptCoordNum);

	//MatrixInverse(invers_a_at, 4, 4);
	MatrixInvert(invers_a_at, invers_a_at, 4, 4);

	//MatrixMulti(at, 4, ptCoordNum, desPtPos, ptCoordNum, 1, temp);
	MatrixMul(temp, at, ptCoordNum, 4, desPtPos, 1, ptCoordNum);

	//MatrixMulti(invers_a_at, 4, 4, temp, 4, 1, affineMatrix);
	MatrixMul(affineMatrix, invers_a_at, 4, 4, temp, 1, 4);


	free(a);
	free(at);
}


//获取指定位置的经过仿射变换后的人脸图像
// int leftShift, int topShift分别为目标图像左上点在仿射变换后目标坐标系中的横坐标和纵坐标
// gradNum为1或3，分别为灰度图和RGB三分量图
// 更快可以采用递推的方式进行
// 仿射变换参数为目标点到源点的变换参数
// 目标图像中只取变换后图像中leftShift, topShift的部分，即pDesImg中点(i,j)在转换目标图像中为(i+leftShift,j+topShift)
// 输入图像为1D的图像，可以为1个通道也可以为多个通道
// 采用目标点在原图中对应点的近似整点的值作为目标点的值
// 返回值为超出边界的点的数目，如果为0，表示没有点越界
int TransImage_Affine_1D_8u_Nearest(float rot_s_x, float rot_s_y, float move_x, float move_y, 
	unsigned char *pDesImg, int wd, int ht, unsigned char *pSrcImg, int oriwd, int oriht, 
	int leftShift, int topShift, int gradNum)
{
	int i;
	int j;
	int x1;
	int y1;
	int shifti;
	int shiftj;
	float *rc;
	float *rd;
	float tx1;
	float ty1;
	int t;
	int desIndex;

	int max_ht_wd = MAX(ht+topShift, wd+leftShift) + 1;
	int twd = oriwd;
	int tht = oriht;

	int overPointNum = 0;

	rc = (float *)malloc(sizeof(float)*max_ht_wd);
	rd = (float *)malloc(sizeof(float)*max_ht_wd);
	for(i = 0;i < max_ht_wd; ++i)
	{
		rc[i] = rot_s_x*i;
	}

	if(rot_s_y == 0)
	{
		memset(rd, 0, sizeof(float)*max_ht_wd);
	}
	else
	{
		for(i = 0;i < max_ht_wd; ++i)	
		{
			rd[i] = rot_s_y*i;
		}
	}

	for(i = 0; i < ht; ++i)
	{
		shifti = i+topShift;
		tx1 = -rd[shifti] + move_x;
		ty1 = rc[shifti] + move_y;
		for(j = 0; j < wd; ++j)
		{
			shiftj = j+leftShift;

			// 计算得到目标图中点(i, j)在原图中的对应整点坐标
			x1 = (int)(rc[shiftj] + tx1 + 0.5);
			y1 = (int)(rd[shiftj] + ty1 + 0.5);

			// 如果超出原图范围，则置为0
			if(x1 < 0 || y1 < 0 || x1 > (twd-1) || y1 > (tht-1))
			{
				desIndex = (i*wd+j)*gradNum;
				for(t = 0; t< gradNum; ++ t)
				{
					pDesImg[desIndex+t] = 0;
				}

				overPointNum ++;
			}
			else
			{
				// 取原图中对应整点作为目标点的值
				desIndex = (i*wd+j)*gradNum;
				for(t = 0; t< gradNum; ++ t)
				{
					pDesImg[desIndex+t] = pSrcImg[(y1*oriwd+x1)*gradNum+t];
				}
			}
		}
	}

	free(rc);
	free(rd);

	return(overPointNum);
}

//获取指定位置的经过仿射变换后的人脸图像
// int leftShift, int topShift分别为目标图像左上点在仿射变换后目标坐标系中的横坐标和纵坐标
// gradNum为1或3，分别为灰度图和RGB三分量图
// 更快可以采用递推的方式进行
// 仿射变换参数为目标点到源点的变换参数
// 目标图像中只取变换后图像中leftShift, topShift的部分，即pDesImg中点(i,j)在转换目标图像中为(i+leftShift,j+topShift)
// 输入图像为1D的图像，可以为1个通道也可以为多个通道
// 采用目标点在原图中对应点附近多个点的均值作为目标点的值
// 返回值大于等于0时，为超出边界的点的数目，如果为0，表示没有点越界
int TransImage_Affine_1D_8u_Smooth(float rot_s_x, float rot_s_y, float move_x, float move_y, 
	unsigned char *pDesImg, int wd, int ht, unsigned char *pSrcImg, int oriwd, int oriht, 
	int leftShift, int topShift, int gradNum)
{
	int i;
	int j;
	int k;
	int l;
	int n;
	int x1;
	int y1;
	int shifti;
	int shiftj;
	float *rc;
	float *rd;
	int maxx;
	int maxy;
	int minx;
	int miny;
	float tx1;
	float ty1;
	int sht;
	int swd;
	int t;
	int desIndex;

	int sum[3];

	int max_ht_wd = MAX(ht+topShift, wd+leftShift) + 1;
	int twd = oriwd;
	int tht = oriht;

	int overPointNum = 0;

	const float smoothRatio = 1.0;
	swd = MAX((int)(sqrtf(rot_s_x*rot_s_x + rot_s_y*rot_s_y)*smoothRatio + 0.5), 1);
	sht = swd;

	rc = (float *)malloc(sizeof(float)*max_ht_wd);
	rd = (float *)malloc(sizeof(float)*max_ht_wd);
	for(i = 0;i < max_ht_wd; ++i)
	{
		rc[i] = rot_s_x*i;
	}

	if(rot_s_y == 0)
	{
		memset(rd, 0, sizeof(float)*max_ht_wd);
	}
	else
	{
		for(i = 0;i < max_ht_wd; ++i)	
		{
			rd[i] = rot_s_y*i;
		}
	}

	for(i = 0; i < ht; ++i)
	{
		shifti = i+topShift;
		tx1 = -rd[shifti] + move_x;
		ty1 = rc[shifti] + move_y;
		for(j = 0; j < wd; ++j)
		{
			shiftj = j+leftShift;

			// 计算目标点在源坐标系中对应点的坐标
			x1 = (int)(rc[shiftj] + tx1 + 0.5);
			y1 = (int)(rd[shiftj] + ty1 + 0.5);

			// 将目标点限制在源坐标系的范围内
			minx = MIN(MAX(x1, 0), twd);
			miny = MIN(MAX(y1, 0), tht);
			maxx = MIN(MAX(x1 + swd, 0), twd);
			maxy = MIN(MAX(y1 + sht, 0), tht);

			// 计算源坐标系中目标点的对应点附近的整点，根据所取范围内整点的均值作为目标点的值
			n = 0;
			memset(sum, 0x00, sizeof(int)*gradNum);

			for(k = miny;k < maxy; ++k)
			{
				for(l = minx;l < maxx; ++l)
				{
					for(t = 0; t< gradNum; ++ t)
					{
						sum[t] += pSrcImg[(k*oriwd+l)*gradNum+t];
					}

					n ++;
				}
			}

			if(!n) // 所有点都不在原图范围内，补0
			{
				desIndex = (i*wd+j)*gradNum;
				for(t = 0; t< gradNum; ++ t)
				{
					pDesImg[desIndex+t] = 0;
				}

				++ overPointNum;
			}
			else
			{
				// 将所有整点的均值作为目标点的值
				desIndex = (i*wd+j)*gradNum;
				for(t = 0; t< gradNum; ++ t)
				{
					pDesImg[desIndex+t] = (unsigned char)CLIP(sum[t]/n);
				}
			}
		}
	}

	free(rc);
	free(rd);

	return(overPointNum);
}

//获取指定位置的经过仿射变换后的人脸图像
// int leftShift, int topShift分别为目标图像左上点在仿射变换后目标坐标系中的横坐标和纵坐标
// gradNum为1或3，分别为灰度图和RGB三分量图
// 更快可以采用递推的方式进行
// 仿射变换参数为目标点到源点的变换参数
// 目标图像中只取变换后图像中leftShift, topShift的部分，即pDesImg中点(i,j)在转换目标图像中为(i+leftShift,j+topShift)
// 如果原始图像分辨率大于目标图像，则根据比例进行平滑，否则采用最近的四个整点进行双线性插获得
// 返回值大于等于0时，为超出边界的点的数目，如果为0，表示没有点越界
int TransImage_Affine_1D_8u_Bilinear(float rot_s_x, float rot_s_y, float move_x, float move_y, 
	unsigned char *pDesImg, int wd, int ht, unsigned char *pSrcImg, int oriwd, int oriht, 
	int leftShift, int topShift, int gradNum)
{
	int i;
	int j;
	int x1;
	int y1;
	int x2;
	int y2;
	int shifti;
	int shiftj;
	float *rc;
	float *rd;
	float tx1;
	float ty1;
	float dy,dx;
	int t;
	int desIndex;
	int srcIndex;

	float area11;
	float area12;
	float area21;
	float area22;

	int max_ht_wd = MAX(ht+topShift, wd+leftShift) + 1;
	int twd = oriwd;
	int tht = oriht;

	float temp;

	int overPointNum = 0;

	rc = (float *)malloc(sizeof(float)*max_ht_wd);
	rd = (float *)malloc(sizeof(float)*max_ht_wd);
	for(i = 0;i < max_ht_wd; ++i)
	{
		rc[i] = rot_s_x*i;
	}

	if(rot_s_y == 0)
	{
		memset(rd, 0, sizeof(float)*max_ht_wd);
	}
	else
	{
		for(i = 0;i < max_ht_wd; ++i)	
		{
			rd[i] = rot_s_y*i;
		}
	}

	for(i = 0; i < ht; ++i)
	{
		shifti = i+topShift;
		tx1 = -rd[shifti] + move_x;
		ty1 = rc[shifti] + move_y;
		for(j = 0; j < wd; ++j)
		{
			shiftj = j+leftShift;

			// 计算目标图像中当前点在源图中对应点浮点坐标
			dx = rc[shiftj] + tx1;
			dy = rd[shiftj] + ty1;

			x1 = (int)(dx);
			y1 = (int)(dy);
			x2 = x1+1;
			y2 = y1+1;

			// 如果超出范围，则置为0
			if(x1 < 0 || y1 < 0 || x2 > (twd-1) || y2 > (tht-1))
			{
				desIndex = (i*wd+j)*gradNum;
				for(t = 0; t< gradNum; ++ t)
				{
					pDesImg[desIndex+t] = 0;
				}

				++ overPointNum;
			}
			else
			{
				// 计算各块面积
				area11 = (dx-x1)*(dy-y1); // 左上角点
				area12 = (x2-dx)*(dy-y1); // 右上角点
				area21 = (dx-x1)*(y2-dy); // 左下角点
				area22 = (x2-dx)*(y2-dy); // 右下角点

				srcIndex = (y1*oriwd+x1)*gradNum; // 源坐标系中左上角点序号
				desIndex = (i*wd+j)*gradNum; // 目标坐标系中当前点序号
				for(t = 0; t< gradNum; ++ t)
				{
					// 各点所占比重和对角位置面积成正比
					temp = pSrcImg[srcIndex+t]*area22+pSrcImg[srcIndex+1+t]*area21
						+pSrcImg[srcIndex+oriwd+t]*area12+pSrcImg[srcIndex+oriwd+1+t]*area11;

					pDesImg[desIndex+t] = CLIP(ROUND(temp));
				}
			}
		}
	}

	free(rc);
	free(rd);

	return(overPointNum);
}

//获取指定位置的经过仿射变换后的人脸图像
// int leftShift, int topShift分别为目标图像左上点在仿射变换后目标坐标系中的横坐标和纵坐标
// gradNum为1或3，分别为灰度图和RGB三分量图
// 更快可以采用递推的方式进行
// 仿射变换参数为目标点到源点的变换参数
// 目标图像中只取变换后图像中leftShift, topShift的部分，即pDesImg中点(i,j)在转换目标图像中为(i+leftShift,j+topShift)
// 如果原始图像分辨率大于目标图像，则根据比例进行平滑
// procCase用来指定处理的方式，如果为0，则表示采用最近邻法取点，如果为1，则表示双线性法取点，如果为2，则表示平均法取点
// 返回值大于等于0时，为超出边界的点的数目，如果为0，表示没有点越界

int TransImage_Affine_1D_8u(float rot_s_x, float rot_s_y, float move_x, float move_y, 
	unsigned char *pDesImg, int wd, int ht, unsigned char *pSrcImg, int oriwd, int oriht, 
	int leftShift, int topShift, int gradNum, int procCase)
{
	int ret=0;
	if(procCase == 0) // 直接最近邻法取点
	{
		ret= TransImage_Affine_1D_8u_Nearest(rot_s_x, rot_s_y, move_x, move_y, 
			pDesImg, wd, ht, pSrcImg, oriwd, oriht, leftShift, topShift, gradNum);
	}
	else if(procCase == 1)
	{
		ret= TransImage_Affine_1D_8u_Bilinear(rot_s_x, rot_s_y, move_x, move_y, 
			pDesImg, wd, ht, pSrcImg, oriwd, oriht, leftShift, topShift, gradNum);
	}
	else if(procCase == 2)
	{
		ret=  TransImage_Affine_1D_8u_Smooth(rot_s_x, rot_s_y, move_x, move_y, 
			pDesImg, wd, ht, pSrcImg, oriwd, oriht, leftShift, topShift, gradNum);
	}
	else
	{
		ret=  -1;
	}
	return ret;
}

// 根据目标坐标系和源坐标系的点对应关系，以及放缩尺度和旋转方向，得到变换后的目标图像
// sinAngle, cosAngle, sizeRatio 等仿射变换参数为目标点到源点的变换参数，角度在图像坐标系中顺时针为正
// srcReferPoint为源图像中参考点坐标，即以该点为中心进行旋转
// desReferPoint为目标图中参考点，即以该点为中心进行旋转
// procCase用来指定处理的方式，如果为0，则表示采用最近邻法取点，如果为1，则表示双线性法取点，如果为2，则表示平均法取点
// 如果gradNum=1，则表示一个通道，为灰度图，如果gradNum=3，则表示3个通道，为BGR或RGB图像，三个通道图像buffer大小均为srcWd，srcHt
// 返回值大于等于0时，为超出边界的点的数目，如果为0，表示没有点越界

int AffineTranImg_CenterSizeAngle_1D(unsigned char *pDesImg, int desWd, int desHt, s_Point desReferPoint, 
	unsigned char *pSrcImg, int srcWd, int srcHt, s_Point srcReferPoint, 
	float sinAngle, float cosAngle, float sizeRatio, int gradNum, int procCase)
{
	// 得到由目标坐标系到源坐标系的变换关系
	int ret;
	float temp0,temp1;
	float mTrans[4];
	mTrans[0] = cosAngle*sizeRatio;
	mTrans[1] = sinAngle*sizeRatio;
	temp0 = desReferPoint.x;
	temp1 = desReferPoint.y;
	mTrans[2] = (float)(-mTrans[0]*temp0+mTrans[1]*temp1+srcReferPoint.x);
	mTrans[3] = (float)(-mTrans[0]*temp1-mTrans[1]*temp0+srcReferPoint.y);

	ret =TransImage_Affine_1D_8u(mTrans[0], mTrans[1], mTrans[2], mTrans[3], 
		pDesImg, desWd, desHt, pSrcImg, srcWd, srcHt, 0, 0, gradNum, procCase);

	return ret;
}

/************************************************************************/
// 函数功能：根据眼睛定位结果将图像感兴趣区域仿射变换到eye距离为60的标准坐标系（lefteye(60,60),righteye(120,69),w:180;h:180）;
//			并拷贝感兴趣区域，仿射变换参数存入pdA
// 参数说明：
//			byte* pbClipNormalFaceImgROI,	OUT 输出的ROI图像
//			float* pdA,					OUT 输出从标准人脸坐标到输入图像坐标的仿射变换参数
//          float	pdEyes[4],					OUT 眼睛定位结果
//			int nImgW,						IN	图像宽度
//			int nImgH,						IN  图像高度
// 返 回 值：
/************************************************************************/
int	ClipNormalFaceImgROI(unsigned char* pbClipNormalFaceImgROI,  float* pdA, float* pdEyes,
	unsigned char* pbFrameGray, int nImgW, int nImgH)
{
	s_Point dstReferPt; 
	s_Point srcReferPt;
	float dEyeDis;
	float dScale;
	float dR;
	float origPtPos[4];
	float* dstPtPos;

	float cosAngle,sinAngle;

	if ( (!pbClipNormalFaceImgROI) || (!pdA) || (!pbFrameGray) || (nImgW<0) || (nImgH<0) )
	{
		return -1;
	}

	//////////////////////////////////////根据眼睛定位结果将图像缩放到眼睛距离为60////////////////////////////////////
	// 计算眼睛距离
	dEyeDis = sqrtf ((float)( (pdEyes[0]-pdEyes[2]) *(pdEyes[0]-pdEyes[2]) + (pdEyes[1]-pdEyes[3]) * (pdEyes[1]-pdEyes[3]) ) );
	// 尺度变化参数
	dScale = EYE_DIS / dEyeDis;

	dstPtPos = pdEyes;

	origPtPos[0] = NORMAL_EYE_XLEFT;
	origPtPos[1] = NORMAL_EYE_YLEFT;
	origPtPos[2] = NORMAL_EYE_XRIGHT;
	origPtPos[3] = NORMAL_EYE_YRIGHT;

	// 计算从标准人脸坐标到输入图像坐标的变换参数
	GetAffineMatrix_Pts(pdA, origPtPos, dstPtPos, 2); // 算出的结果与OptimizeA相同
	//	 dR = sqrt(pdA[0] * pdA[0] + pdA[1] * pdA[1]); // pdA计算正确的话，dR应等于dScale

	dR= pdA[0];
	// 以标准人脸左眼中心点为参考点

	dstReferPt.x = NORMAL_EYE_XLEFT;
	dstReferPt.y = NORMAL_EYE_YLEFT;
	// 以当前人脸左眼中心点为参考点

	srcReferPt.x = (int)(pdEyes[0]);
	srcReferPt.y = (int)(pdEyes[1]);

	// 割取ROI图像，并进行仿射变换
	cosAngle = (float)(pdA[0] / dR); // c/r
	sinAngle = (float)(pdA[1] / dR); // d/r
	AffineTranImg_CenterSizeAngle_1D(pbClipNormalFaceImgROI, NORMAL_FACE_W, NORMAL_FACE_H,
		dstReferPt, pbFrameGray, nImgW, nImgH, srcReferPt, sinAngle, cosAngle, (float)dR, 1, 2);

	return 0;
}
