#ifndef _AEYE_BASE_FACE_STRUCT_
#define _AEYE_BASE_FACE_STRUCT_

// 定义函数返回结果
#define  AEYE_SUCCESS 0
#define  AEYE_FAIL    -1

// 如果使用了，则表示为输出参数
#define OUT

// 定义函数调用方式
#define STD_CALL __stdcall

typedef void * HANDLE;

typedef unsigned char byte;
#define INT_MAX0 2147483647
#define INT_MIN0 (-2147483647-1)
#define ROUND_int32(f) ((f)>0 ? ( (int)((f)+0.5) ) : ( (int)((f)-0.5) ))
#define ROUND_uint32(f) ((int)((f)+0.5))

typedef struct s_POINT
{
	int x;
	int y;
}
s_AEPoint;


typedef struct s_RECT
{
	int left;
	int top;
	int right;
	int bottom;
}
s_AERect;

// 人脸检测用结构体
typedef struct _AE_face_
{
	s_AERect faceRect;	// 人脸框定义，分别为人脸区域的左右上下坐标
	int confidence;		// confidence 人脸置信度,从0到100,0最低,100最高
	int quality;		// quality 保留字,暂时不用
}
s_AEFace;

// 人脸跟踪用结构体
typedef struct
{
	int faceID;			// 目标的唯一标示，用来区分不同的目标
	s_AERect faceRect;	// 该跟踪目标的坐标位置
	double confidence;	// 跟踪的置信度，用来衡量跟踪目标一直是同一个目标的程度
	int state;          // 保留字，用于便于扩展，比如用于输出跟踪目标的状态：被遮挡，暂时丢失等
}
s_AETrackFace;

// 眼睛定位用结构体，包括左眼和右眼
typedef struct _AE_eye_
{
	int xLeft;			// xLeft左眼中心点横坐标
	int yLeft;			// yLeft 左眼中心点纵坐标
	int leftWd;			// leftWd 左眼宽度
	int leftHt;			// leftHt 左眼高度
	int xRight;			// xRight 右眼中心点横坐标
	int yRight;			// yRight 右眼中心点纵坐标
	int rightWd;		// rightWd 右眼宽度
	int rightHt;		// rightHt 右眼高度
	int confidence;		// confidence 左眼和右眼检测结果的置信度,从0到100,0最低,100最高
	int lQuality;		// lQuality rQuality 保留字,暂时不用
	int rQuality;
}
s_AEEye;

#endif