#ifndef _AFBASE_H_
#define _AFBASE_H_

#include "amcomdef.h"
#include "al_aftypes.h"

#include "stdio.h"
#include "stdarg.h"

#ifndef AF_EXTERN_C
    #ifdef __cplusplus
		#ifdef  __SYMBIAN32__
			#define AF_EXTERN_C EXPORT_C
		#else
			#define AF_EXTERN_C extern "C"
		#endif
		#define AF_DEFAULT(val) = val
    #else
        #define AF_EXTERN_C
        #define AF_DEFAULT(val)
    #endif
#endif

#define AF_IMPL AF_EXTERN_C

#if defined WIN32 && defined AF_DLL
    #define AF_DLL_ENTRY __declspec(dllexport)
#else
    #define AF_DLL_ENTRY
#endif

#ifndef AFAPI
    #define AFAPI AF_EXTERN_C AF_DLL_ENTRY
#endif

#ifndef WIN32
    #define AF_CDECL
    #define AF_STDCALL
#else
    #define AF_CDECL __cdecl
    #define AF_STDCALL __stdcall
#endif

#ifndef AF_EXTERN_C_FUNCPTR
    #ifdef __cplusplus
		#define AF_EXTERN_C_FUNCPTR(x) extern "C" { typedef x; }
    #else
        #define AF_EXTERN_C_FUNCPTR(x) typedef x
    #endif
#endif

#define EYECORNERDETECTION
#define SKINTONEFILTER
#define SIXSEGFILTER
#define HAAREDGEFILTER

#define AF_HAAR_FEATURE_MAX  3

#define AF_DEPTH_MAX  6

#define ecl_cvDepthToType PRE_FUN(ecl_cvDepthToType)
#define ecl_cvPixSize PRE_FUN(ecl_cvPixSize)
#define ecl_af8x32fSqrTab PRE_FUN(ecl_af8x32fSqrTab)
#define ecl_afSquareRootTable PRE_FUN(ecl_afSquareRootTable)
#define ecl_ColorCvt PRE_FUN(ecl_ColorCvt)

#define ecl_icvGetMatSize PRE_FUN(ecl_icvGetMatSize)
#define ecl_iafAlignPtr PRE_FUN(ecl_iafAlignPtr)
#define ecl_iafAlign PRE_FUN(ecl_iafAlign)
#define ecl_afRound PRE_FUN(ecl_afRound)
#define ecl_afFloor PRE_FUN(ecl_afFloor)
#define ecl_iafGetMatSize PRE_FUN(ecl_iafGetMatSize)



#define ecl_afAlloc PRE_FUN(ecl_afAlloc)
#define ecl_afFree PRE_FUN(ecl_afFree)
#define ecl_afCreateMatHeader PRE_FUN(ecl_afCreateMatHeader)
#define ecl_afInitMatHeader PRE_FUN(ecl_afInitMatHeader)
#define ecl_afCreateImage PRE_FUN(ecl_afCreateImage)
#define ecl_afCreateImageHeader PRE_FUN(ecl_afCreateImageHeader)
#define ecl_afReleaseImage PRE_FUN(ecl_afReleaseImage)
#define ecl_afReleaseImageHeader PRE_FUN(ecl_afReleaseImageHeader)
#define ecl_afGetMat PRE_FUN(ecl_afGetMat)
#define ecl_afGetSize PRE_FUN(ecl_afGetSize)
#define ecl_afCreateMemStorage PRE_FUN(ecl_afCreateMemStorage)
#define ecl_afReleaseMemStorage PRE_FUN(ecl_afReleaseMemStorage)
#define ecl_afClearMemStorage PRE_FUN(ecl_afClearMemStorage)
#define ecl_afCreateChildMemStorage PRE_FUN(ecl_afCreateChildMemStorage)
#define ecl_afSaveMemStoragePos PRE_FUN(ecl_afSaveMemStoragePos)
#define ecl_afRestoreMemStoragePos PRE_FUN(ecl_afRestoreMemStoragePos)
#define ecl_afCreateSeq PRE_FUN(ecl_afCreateSeq)
#define ecl_afSetSeqBlockSize PRE_FUN(ecl_afSetSeqBlockSize)
#define ecl_afClearSeq PRE_FUN(ecl_afClearSeq)
#define ecl_afSeqPush PRE_FUN(ecl_afSeqPush)
#define ecl_afGetSeqElem PRE_FUN(ecl_afGetSeqElem)
#define ecl_afPartitionSeq PRE_FUN(ecl_afPartitionSeq)
#define ecl_afIntegral PRE_FUN(ecl_afIntegral)
#define ecl_afIntegralEx PRE_FUN(ecl_afIntegralEx)
#define ecl_afThreshold PRE_FUN(ecl_afThreshold)
#define afGetImageROI PRE_FUN(afGetImageROI)
#define afSmooth PRE_FUN(afSmooth)
#define afCornerMinEigenVal PRE_FUN(afCornerMinEigenVal)
#define iafFilterInitAlloc PRE_FUN(iafFilterInitAlloc)
#define iafFilterFree PRE_FUN(iafFilterFree)
#define afCreateStructuringElementEx PRE_FUN(afCreateStructuringElementEx)
#define afReleaseStructuringElement PRE_FUN(afReleaseStructuringElement)
#define afErode PRE_FUN(afErode)
#define afCreateMemManager PRE_FUN(afCreateMemManager)
#define afAllocMem PRE_FUN(afAllocMem)
#define afDestroyMemManager PRE_FUN(afDestroyMemManager)
#define afRewindMemManager PRE_FUN(afRewindMemManager)
//#define SQSUMMATTYPE AF_64FC1
//#define SQSUMIMGDEPTH AF_64FC1
#define SQSUMMATTYPE AF_32SC1
#define SQSUMIMGDEPTH AF_32SC1

#define INT64_MAX 9223372036854775807
#define FLT_MAX 3.402823466e+38F
#define SHRT_MAX 32767
#define SHRT_MIN    (-32768) 
#define INT_MAX	2147483647
#define INT_MIN     (-2147483647 - 1)
#define ULONG_MAX     0xffffffffUL  /* maximum unsigned long value */


#define DIFF_MAX INT_MAX


#define AF_DEPTH_SIGN 0x80000000

#define AF_DEPTH_1U     1
#define AF_DEPTH_8U     8
#define AF_DEPTH_16U   16
#define AF_DEPTH_32F   32
#define AF_DEPTH_64F   64

#define AF_DEPTH_8S  (AF_DEPTH_SIGN| 8)
#define AF_DEPTH_16S (AF_DEPTH_SIGN|16)
#define AF_DEPTH_32S (AF_DEPTH_SIGN|32)
#define AF_DEPTH_64S (AF_DEPTH_SIGN|64)

#define  AF_INTER_NN        0
#define  AF_INTER_LINEAR    1


#define  AF_SHAPE_RECT      0
#define  AF_SHAPE_CROSS     1
#define  AF_SHAPE_ELLIPSE   2
#define  AF_SHAPE_CUSTOM    100


#define AF_ORIGIN_TL  0
#define SMALL_GAUSSIAN_SIZE  7
#define AF_GAUSSIAN  2
#define AF_BLUR  1
#define AF_BLUR_NO_SCALE 0
#define AF_MEDIAN 3
#define AF_BILATERAL 4

#define AF_DEPTH_MAX  6

#define AF_8U   0
#define AF_8S   1
#define AF_32F  4

#define AF_WHOLE   0
#define AF_START   1
#define AF_END     2
#define AF_MIDDLE  4

#define AF_MAT_DEPTH_MASK       7
#define AF_MAT_DEPTH(flags)     ((flags) & AF_MAT_DEPTH_MASK)
#define AF_SCHARR -1

#define IAF_SYMMETRIC_KERNEL        1
#define IAF_ASYMMETRIC_KERNEL       2

#define IAF_1_2_1_KERNEL            (4*1+IAF_SYMMETRIC_KERNEL)
#define IAF_m1_0_1_KERNEL           (4*2+IAF_ASYMMETRIC_KERNEL)
#define IAF_1_m2_1_KERNEL           (4*3+IAF_SYMMETRIC_KERNEL)
#define IAF_3_10_3_KERNEL           (4*4+IAF_SYMMETRIC_KERNEL)


#define IAF_BINARY_KERNEL           (0<<16)
#define IAF_SEPARABLE_KERNEL        (1<<16)

#define IAF_KERNEL_TYPE_MASK        (15<<16)
#define IAF_KERNEL_TYPE(flags)      ((flags) & IAF_KERNEL_TYPE_MASK)

#define IAF_X_KERNEL_TYPE(flags)    ((flags) & 255)
#define IAF_Y_KERNEL_TYPE(flags)    (((flags) >> 8) & 255)

/* Constants for color conversion */
#define  AF_BGR2BGRA    0
#define  AF_RGB2RGBA    AF_BGR2BGRA

#define  AF_BGRA2BGR    1
#define  AF_RGBA2RGB    AF_BGRA2BGR

#define  AF_BGR2RGBA    2
#define  AF_RGB2BGRA    AF_BGR2RGBA

#define  AF_RGBA2BGR    3
#define  AF_BGRA2RGB    AF_RGBA2BGR

#define  AF_BGR2RGB     4
#define  AF_RGB2BGR     AF_BGR2RGB

#define  AF_BGRA2RGBA   5
#define  AF_RGBA2BGRA   AF_BGRA2RGBA

#define  AF_BGR2GRAY    6
#define  AF_RGB2GRAY    7
#define  AF_GRAY2BGR    8
#define  AF_GRAY2RGB    AF_GRAY2BGR
#define  AF_GRAY2BGRA   9
#define  AF_GRAY2RGBA   AF_GRAY2BGRA
#define  AF_BGRA2GRAY   10
#define  AF_RGBA2GRAY   11

#define  AF_BGR2BGR565  12
#define  AF_RGB2BGR565  13
#define  AF_BGR5652BGR  14
#define  AF_BGR5652RGB  15
#define  AF_BGRA2BGR565 16
#define  AF_RGBA2BGR565 17
#define  AF_BGR5652BGRA 18
#define  AF_BGR5652RGBA 19

#define  AF_GRAY2BGR565 20
#define  AF_BGR5652GRAY 21

#define  AF_BGR2XYZ     22
#define  AF_RGB2XYZ     23
#define  AF_XYZ2BGR     24
#define  AF_XYZ2RGB     25

#define  AF_BGR2YCrCb   26
#define  AF_RGB2YCrCb   27
#define  AF_YCrCb2BGR   28
#define  AF_YCrCb2RGB   29

#define  AF_BGR2HSV     30
#define  AF_RGB2HSV     31

#define  AF_BGR2Lab     34
#define  AF_RGB2Lab     35

#define  AF_BayerBG2BGR 40
#define  AF_BayerGB2BGR 41
#define  AF_BayerRG2BGR 42
#define  AF_BayerGR2BGR 43

#define  AF_BayerBG2RGB AF_BayerRG2BGR
#define  AF_BayerGB2RGB AF_BayerGR2BGR
#define  AF_BayerRG2RGB AF_BayerBG2BGR
#define  AF_BayerGR2RGB AF_BayerGB2BGR

#define  AF_COLORCVT_MAX  48


/* default storage block size*/
/*#define  AF_STORAGE_BLOCK_SIZE   ((1<<16) - 128)*/
#define  AF_STORAGE_BLOCK_SIZE   ((1<<13) - 128)

extern const char ecl_cvDepthToType[];

#define icvIplToAfDepth( depth ) \
    ecl_cvDepthToType[(((depth) & 255) >> 2) + ((depth) < 0)]

#define ALIGN(size) (((size)+3) & -4)

/* helper tables */
extern const MLong ecl_cvPixSize[];

typedef struct ecl_AfBigFuncTable
{
    MVoid*   fn_2d[AF_MAT_TYPE_MASK+1];
}
ecl_AfBigFuncTable;

typedef struct _ecl_AfConvKernel
{
    MLong  nCols;
    MLong  nRows;
    MLong  anchorX;
    MLong  anchorY;
    MLong *values;
    MLong  nShiftR;
}
ecl_AfConvKernel;

#define AF_MORPH_ALIGN  4

typedef struct ecl_AfFuncTable
{
    MVoid*   fn_2d[AF_DEPTH_MAX + 1];
}
ecl_AfFuncTable;

typedef struct
{
    float  b, g, r;
}
ecl_AfRGB32f;
typedef struct
{
    float  b, g, r, a;
}
ecl_AfRGBA32f;

typedef struct
{
    MByte  b, g, r;
}
ecl_AfRGB8u;


typedef struct
{
    MByte  b, g, r, a;
}
ecl_AfRGBA8u;

typedef enum {
   cv1u,
   cv8u, cv8s,
   cv16u, cv16s, cv16sc,
   cv32u, cv32s, cv32sc,
   cv32f, cv32fc,
   cv64u, cv64s, cv64sc,
   cv64f, cv64fc
} ecl_AfDataType;

typedef struct 
{
    MLong delta, divisor;
}ecl_AfFastDiv;

typedef struct ecl_AfFilterState
{
    /* kernel data */
    MLong ker_width;
    MLong ker_height;
    MLong ker_x;
    MLong ker_y;
    MLong kerType;
    MByte *ker0;
    MByte *ker1;
    MDouble divisor;

    /* image data */
    MLong max_width;
    ecl_AfDataType dataType;
    MLong channels;
    MLong origin;

    /* cyclic buffer */
    char *buffer;
    MLong buffer_step;
    MLong crows;
    char **rows;
    char *tbuf;
}
ecl_AfFilterState;

#define _AfConvState ecl_AfFilterState

typedef struct ecl_ArcSoft_Face_Tracking_Version
{
	MLong lCodebase;	/* Codebase version number */
	MLong lMajor;		/* major version number */
	MLong lMinor;		/* minor version number*/
	MLong lBuild;		/*Build version number, increasable only */
	const MTChar *Version;		/* version in string form */
	const MTChar *BuildDate;	/* latest build Date */
	const MTChar *CopyRight;	/* copyright */
} ecl_ArcSoft_Face_Tracking_Version;


AF_INLINE  ecl_AfSize  ecl_icvGetMatSize( const ecl_AfMat* mat )
{
    ecl_AfSize size;
	size.width = mat->cols;
	size.height= mat->rows;
    return size;
}

AF_INLINE MVoid* ecl_iafAlignPtr( MVoid* ptr, MLong align )
{
    return (MVoid*)( ((MUInt64)ptr + align - 1) & -align );
}

AF_INLINE MLong ecl_iafAlign( MLong size, MLong align )
{
    return (size + align - 1) & -align;
}

AF_INLINE MLong  ecl_afRound( MDouble val )
{
    /*MDouble temp = val + 6755399441055744.0;*/
   /* return (MLong)*((uint64*)&temp);*/
	return (MLong)(val+0.5);
}
AF_INLINE MLong  ecl_afFloor( MDouble val )
{
   /*MDouble temp = val + 6755399441055744.0;*/
    /*float diff = (float)(val - (MLong)*((uint64*)&temp));*/

   /* return (MLong)*((uint64*)&temp) - (*(MLong*)&diff < 0);*/
	return (MLong)(val);
}
AF_INLINE ecl_AfSize  ecl_iafGetMatSize( const ecl_AfMat* mat )
{
    ecl_AfSize size;
    size.width= mat->cols;
    size.height=mat->rows;
    return size;
}
/* maximum size of dynamic memory buffer*/
#define  AF_MAX_ALLOC_SIZE    (((MDWord)1 << (sizeof(MDWord)*8-2)))

/* default alignment for dynamic data strucutures, resided in storages.*/
#define  AF_STRUCT_ALIGN  ((MLong)sizeof(MDouble))

/* boundary all the allocated buffers are aligned by*/
#define  AF_MALLOC_ALIGN  32

#define _AF_GET_LAST_ELEM( seq, block ) \
    ((block)->data + ((block)->count - 1)*((seq)->elem_size))

#define  AF_COPY( dst, src, len, idx ) \
    for( (idx) = 0; (idx) < (len); (idx)++) (dst)[idx] = (src)[idx]

#define  AF_SET( dst, val, len, idx )  \
    for( (idx) = 0; (idx) < (len); (idx)++) (dst)[idx] = (val)

#define  AF_IMIN(a, b)  ((a) ^ (((a)^(b)) & (((a) < (b)) - 1)))

#define  AF_IMAX(a, b)  ((a) ^ (((a)^(b)) & (((a) > (b)) - 1)))

#define  AF_CALC_MIN(a, b) (a) = AF_IMIN((a),(b))
#define  AF_CALC_MAX(a, b) (a) = AF_IMAX((a),(b))

typedef int ( * ecl_AfFilterFunc )( const MVoid* src, MLong src_step,
                           MVoid* dst, MLong dst_step,
                            ecl_AfSize* size, struct ecl_AfFilterState * state,
                            MLong stage );

#define AfElementShape MLong

#define AfMorphFunc ecl_AfFilterFunc
#define AfMorphState ecl_AfFilterState

/* a < b ? -1 : a > b ? 1 : 0 */
AF_EXTERN_C_FUNCPTR( MLong (AF_CDECL* ecl_AfCmpFunc)
                     (const MVoid* a, const MVoid* b, MVoid* userdata ));

/****************************************************************************************\
*                                     Allocation/deallocation                            *
\****************************************************************************************/
/* <malloc> wrapper.
   If there is no enough memory the function
   (as well as other OpenCV functions that call afAlloc)
   raises an error. */
AFAPI  MVoid*  ecl_afAlloc( MDWord size,MHandle hMemMgr);

/* <free> wrapper.
   Here and further all the memory releasing functions
   (that all call afFree) take MDouble pointer which is used
   to clear user pointer to the data after releasing it.
   Passing pointer to NULL pointer is Ok: nothing happens in this case
*/
AFAPI  MVoid   ecl_afFree( MVoid** ptr ,MHandle hMemMgr);

AF_IMPL ecl_AfMat* ecl_afCreateMatHeader( MLong rows, MLong cols, MLong type,MHandle hMemMgr );
AF_IMPL ecl_AfMat* ecl_afInitMatHeader( ecl_AfMat* arr, MLong rows, MLong cols,
                                MLong type, MVoid* data, MLong step );

/* Creates IPL image (header and data) */
AFAPI  ecl_AfImage*  ecl_afCreateImage( ecl_AfSize size, MLong depth, MLong channels,
							    MHandle pMemM);


AFAPI ecl_AfImage * ecl_afCreateImageHeader( ecl_AfSize size, MLong depth, MLong channels,
									MByte * pData,MHandle pMemM );
/* Releases IPL image header and data */
AFAPI  MVoid  ecl_afReleaseImage( ecl_AfImage** image,MHandle hMemMgr );

AFAPI MVoid ecl_afReleaseImageHeader( ecl_AfImage ** image,MHandle hMemMgr );
/* Converts AfArr (IplImage or AfMat,...) to AfMat.
   If the last parameter is non-zero, function can
   convert multi(>2)-dimensional array to AfMat as long as
   the last array's dimension is continous. The resultant
   matrix will be have appropriate (a huge) number of rows */
AFAPI ecl_AfMat* ecl_afGetMat( const AfArr* src,
					  ecl_AfMat* header,
					  MLong* coi );

/* Returns width and height of array in elements */
AFAPI  ecl_AfSize ecl_afGetSize( const AfArr* arr );

#define AF_AUTOSTEP  0x7fffffff

/****************************************************************************************\
*                              Dynamic data structures                                   *
\****************************************************************************************/

/* Creates new memory storage.
   block_size == 0 means that default,
   somewhat optimal size, is used (currently, it is 64K) */
AFAPI  ecl_AfMemStorage*  ecl_afCreateMemStorage( MLong block_size,MHandle hMemMgr);


/* Releases memory storage. All the children of a parent must be released before
   the parent. A child storage returns all the blocks to parent when it is released */
AFAPI  MVoid  ecl_afReleaseMemStorage( ecl_AfMemStorage** storage,MHandle hMemMgr );


/* Clears memory storage. This is the only way(!!!) (besides afRestoreMemStoragePos)
   to reuse memory allocated for the storage - afClearSeq,afClearSet ...
   do not free any memory.
   A child storage returns all the blocks to the parent when it is cleared */
AFAPI  MVoid  ecl_afClearMemStorage( ecl_AfMemStorage* storage,MHandle hMemMgr );

/* Creates a memory storage that will borrow memory blocks from parent storage */
AFAPI  ecl_AfMemStorage*  ecl_afCreateChildMemStorage( ecl_AfMemStorage* parent,MHandle hMemMgr );


/* Remember a storage "free memory" position */
AFAPI  MVoid  ecl_afSaveMemStoragePos( const ecl_AfMemStorage* storage, ecl_AfMemStoragePos* pos );

/* Restore a storage "free memory" position */
AFAPI  MVoid  ecl_afRestoreMemStoragePos( ecl_AfMemStorage* storage, ecl_AfMemStoragePos* pos );

/* Creates new empty sequence that will reside in the specified storage */
AFAPI  ecl_AfSeq*  ecl_afCreateSeq( MLong seq_flags, MLong header_size,
                             MLong elem_size, ecl_AfMemStorage* storage,MHandle hMemMgr );

/* Changes default size (granularity) of sequence blocks.
   The default size is ~1Kbyte */
AFAPI  MVoid  ecl_afSetSeqBlockSize( ecl_AfSeq* seq, MLong delta_elements );

/* Removes all the elements from the sequence. The freed memory
   can be reused later only by the same sequence unless afClearMemStorage
   or afRestoreMemStoragePos is called */
AFAPI  MVoid  ecl_afClearSeq( ecl_AfSeq* seq );

/* Adds new element to the end of sequence. Returns pointer to the element */
AFAPI  char*  ecl_afSeqPush( ecl_AfSeq* seq,
						MVoid* element,
						MHandle hMemMgr);

/* Retrives pointer to specified sequence element.
   Negative indices are supported and mean counting from the end
   (e.g -1 means the last sequence element) */
AFAPI  char*  ecl_afGetSeqElem( ecl_AfSeq* seq, 
						   MLong index,
						   ecl_AfSeqBlock** block);

/* Splits sequence into set of equivalency classes
   using specified equivalency criteria */
AFAPI  MLong  ecl_afPartitionSeq(MHandle hMemMgr, ecl_AfSeq* seq, ecl_AfMemStorage* storage, ecl_AfSeq** comps,
                             ecl_AfCmpFunc is_equal, MVoid* userdata, MLong is_set );

/************ Internal sequence functions ************/

AFAPI MVoid ecl_afIntegral( const AfArr* image,
					   AfArr* sumImage,
					   AfArr* sumSqImage);

AFAPI MVoid ecl_afIntegralEx(MHandle hMemMgr,
						   const AfArr* image,
						   AfArr* sumImage,
					       AfArr* sumSqImage,
						   AfArr* sumTiltedImage);

//AF_IMPL MVoid  ecl_afResize(MHandle hMemMgr,
//						const AfArr* srcarr,
//						AfArr* dstarr,
//						MLong method);

AF_IMPL MVoid ecl_afThreshold(ecl_AfImage * src,
						  ecl_AfImage * dst,
						  MLong threshold,
						  MByte val);

extern const MLong ecl_af8x32fSqrTab[];
extern const MLong afLogTab[];
extern const MLong af_hist_bin_array[];

/*
#ifdef _DOUBLE_RANGE_
#define AF_UV_BINS 16
#define AF_MEAN_SHIFT_BIN_NUM	(AF_UV_BINS*AF_UV_BINS*AF_UV_BINS)
#define AF_HISTO_SHIFT_BITS 4
#define AF_HIST_BIN(x) (x>>AF_HISTO_SHIFT_BITS)
#else*/

#define AF_UV_BINS 11
#define AF_MEAN_SHIFT_BIN_NUM	(AF_UV_BINS*AF_UV_BINS*AF_UV_BINS)
#define AF_HISTO_SHIFT_BITS 3
#define AF_HIST_BIN(x) (af_hist_bin_array[(x>>AF_HISTO_SHIFT_BITS)])
/*#endif*/

#define AF_SUMBG_FIX 1024
#define AF_SUMBG_FIX_SHIFT 10

#define AF_8TO32F_SQR(x)  ecl_af8x32fSqrTab[x]
#define ROUND(a) ((MLong)(a+0.5))
#define FLOOR(a) ((MLong)a)


AF_IMPL ecl_AfImage * afGetImageROI(ecl_AfImage * src,
								MRECT *rt,
								Pecl_AfMemManager pMemM);

AF_IMPL MVoid afSmooth(MHandle hMemMgr,
					   const MVoid* srcarr, 
					   MVoid* dstarr,
					   MLong smoothtype,
					   MLong param1,
					   MLong param2 );
AF_IMPL MVoid afCornerMinEigenVal(MHandle hMemMgr,
								  const MVoid* srcarr, 
								  MVoid* eigenvarr,
								  MLong block_size, 
								  MLong aperture_size );

AF_IMPL MVoid iafFilterInitAlloc(MHandle hMemMgr,
								 MLong roiWidth, 
								 ecl_AfDataType dataType,
								 MLong channels,
								 ecl_AfSize elSize,
								 ecl_AfPoint elAnchor,
								 const MVoid* elData,
								 MLong elementType,
								 struct ecl_AfFilterState ** filterState );

AF_IMPL MVoid iafFilterFree(ecl_AfFilterState ** filterState,MHandle hMemMgr) ;

AF_IMPL ecl_AfConvKernel *
afCreateStructuringElementEx(MHandle hMemMgr,
							 MLong cols,
							 MLong rows,
							 MLong anchorX,
							 MLong anchorY,
							 AfElementShape shape,
							 MLong *values );
AF_IMPL MVoid
afReleaseStructuringElement( ecl_AfConvKernel ** element ,MHandle hMemMgr);

AF_IMPL MVoid
afErode(MHandle hMemMgr,
		const MVoid* src,
		MVoid* dst, 
		ecl_AfConvKernel* element,
		MLong iterations );


Pecl_AfMemManager afCreateMemManager(MLong totalsize,MHandle hMemMgr);
MByte * afAllocMem(MLong size,Pecl_AfMemManager pMemM);
MVoid afDestroyMemManager(Pecl_AfMemManager * ppMemM);
MVoid afRewindMemManager(Pecl_AfMemManager pMem);

void ecl_log_to_file(MChar *fmt,...);
void ecl_dump_to_file(const MChar *fname,MByte *buf, MLong size);

#endif/*_AFBASE_H_*/

