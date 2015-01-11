#ifndef __XIAOMI_TYPES_DETECT_H__
#define __XIAOMI_TYPES_DETECT_H__




#define MI_MAT_ELEM_PTR_FAST( mat, row, col, pix_size )  \
    (assert( (unsigned)(row) < (unsigned)(mat).rows &&   \
    (unsigned)(col) < (unsigned)(mat).cols ),   \
    (mat).data.ptr + (size_t)(mat).step*(row) + (pix_size)*(col))

/* min & max without jumps */
#define  MI_IMIN(a, b)  ((a) ^ (((a)^(b)) & (((a) < (b)) - 1)))

#define  MI_IMAX(a, b)  ((a) ^ (((a)^(b)) & (((a) > (b)) - 1)))

inline int Round(float value)    //负数的话是-0.5
{
    return (int)(value + (value >= 0 ? 0.5 : -0.5));

}
inline float Miabs(float value)
{
    return value>0?value:-value;
}

typedef struct _MiPoint
{
    int x;
    int y;
    _MiPoint(int _x,int _y):x(_x),y(_y)
    {
    }
    _MiPoint(){

    }
}
MiPoint;

typedef struct XmMat
{
    int type;
    int step;

    /* for internal use only */

    union
    {
        unsigned char* ptr;
        short* s;
        int* i;
        float* fl;
        double* db;
    } data;
    union
    {
        int rows;
        int height;
    };

    union
    {
        int cols;
        int width;
    };

}
XmMat;
typedef struct _XMSize
{
    int width;
    int height;
    _XMSize():width(0),height(0){}
    _XMSize(int _width,int _height):width(_width),height(_height)
    {}
}
XmSize;
typedef struct _XMRect
{
    int x;
    int y;
    int width;
    int height;

    int facePose[3]; //人脸检测专用,0为正面，1为左侧面 2为右侧面

    float disB;  //这个是人脸的置信度
    int weights;
    _XMRect():x(0),y(0),width(0),height(0),disB(0.0),weights(0){}
    _XMRect(int _x,int _y,int _width,int _height,float _disB=0.0,int _weights=0):x(_x),y(_y),width(_width),height(_height),disB(_disB),weights(_weights)
    {

    }

}
XmRect;

typedef void XmArr;

#define XM_MAT_CN_MASK          (3 << 3)
#define XM_MAT_CN(flags)        ((((flags) & XM_MAT_CN_MASK) >> 3) + 1)

#define XM_MAT_DEPTH_MASK       7
#define XM_MAT_DEPTH(flags)     ((flags) & XM_MAT_DEPTH_MASK)

#define XM_MAT_TYPE_MASK        31
#define XM_MAT_TYPE(flags)      ((flags) & XM_MAT_TYPE_MASK)

#define XM_MAT_CONT_FLAG_SHIFT  9
#define XM_MAT_CONT_FLAG        (1 << XM_MAT_CONT_FLAG_SHIFT)

#define XM_ARE_SIZES_EQ(mat1, mat2) \
    ((mat1)->rows == (mat2)->rows && (mat1)->cols == (mat2)->cols)


#endif