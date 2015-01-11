#include "amcomdef.h"
#include "ammem.h"


#ifndef _AFTYPES_H_
#define _AFTYPES_H_

#define PRE_FUN(fun) AGE_GENDER_##fun


#ifndef AF_INLINE
#ifdef WIN32
    #define AF_INLINE __inline
#elif defined __cplusplus
    #define AF_INLINE inline
#else
    #define AF_INLINE static
#endif
#endif /* AF_INLINE */

#define _AMINT64_

#ifdef _AMINT64_
#if defined _MSC_VER || defined __BORLANDC__
	#ifdef __SYMBIAN32__
		typedef __int64 int64;
		typedef unsigned __int64 uint64;
	#else
		typedef __int64 int64;
		typedef unsigned __int64 uint64;
	#endif
#elif defined _BREW_OS_
	#ifndef _WIN32
		typedef  signed long long   int64;       
		typedef  unsigned long long uint64;        
	#else
		typedef  __int64            int64;       
		typedef  unsigned __int64   uint64;      
	#endif 
#else
	typedef long long int64;
	typedef unsigned long long uint64;
#endif
#endif

typedef unsigned char uchar;

typedef unsigned int sizet;


/* AfArr is used to pass arbitrary array-like data structures
   into the functions and where the particular
   array type is recognized at runtime */ 
typedef void AfArr;


#define ecl_afRectA PRE_FUN(ecl_afRectA)
#define ecl_afRectB PRE_FUN(ecl_afRectB)
#define ecl_afRect PRE_FUN(ecl_afRect)
#define ecl_afPoint PRE_FUN(ecl_afPoint)
#define ecl_afSize PRE_FUN(ecl_afSize)

/*************************************** AfRect *****************************************/

typedef struct ecl_AfRectA
{
	unsigned char x;
	unsigned char y;
	unsigned char width;
	unsigned char height;
}
ecl_AfRectA;

AF_INLINE  ecl_AfRectA  ecl_afRectA( char x, char y, char width, char height )
{
    ecl_AfRectA r;

    r.x = x;
    r.y = y;
    r.width = width;
    r.height = height;

    return r;
}

typedef struct ecl_AfRectB
{
	MLong x;
	MLong y;
	MLong width;
	MLong height;
}
ecl_AfRectB;

AF_INLINE  ecl_AfRectB  ecl_afRectB( MLong x, MLong y, MLong width, MLong height )
{
    ecl_AfRectB r;

    r.x = x;
    r.y = y;
    r.width = width;
    r.height = height;

    return r;
}

typedef struct ecl_AfRect
{	
    MLong x;
    MLong y;
    MLong width;
    MLong height;	
	MLong neighbors;
	MLong conf;
}
ecl_AfRect;

AF_INLINE  ecl_AfRect  ecl_afRect( int x, int y, int width, int height, int conf )
{
    ecl_AfRect r;

    r.x = x;
    r.y = y;
    r.width = width;
    r.height = height;
	r.conf = conf;
	r.neighbors = 0;

    return r;
}

/******************************* AfPoint and variants ***********************************/

typedef struct ecl_AfPoint
{
    MLong x;
    MLong y;
}
ecl_AfPoint;



AF_INLINE  ecl_AfPoint  ecl_afPoint( int x, int y )
{
    ecl_AfPoint p;

    p.x = x;
    p.y = y;

    return p;
}

/******************************** AfSize's & AfBox **************************************/

typedef struct
{
    int width;
    int height;
}
ecl_AfSize;

AF_INLINE  ecl_AfSize  ecl_afSize( int width, int height )
{
    ecl_AfSize s;

    s.width = width;
    s.height = height;

    return s;
}

/****************************************************************************************\
*                                  Matrix type (AfMat)                                   *
\****************************************************************************************/

#define AF_8U   0
#define AF_8S   1
#define AF_16S  2
#define AF_32S  3
#define AF_32F  4
#define AF_64F  5
#define AF_64S	6

#define AF_USRTYPE1 6
#define AF_USRTYPE2 7

#define AF_8UC1 (AF_8U + 0*8)
#define AF_8UC2 (AF_8U + 1*8)
#define AF_8UC3 (AF_8U + 2*8)
#define AF_8UC4 (AF_8U + 3*8)

#define AF_8SC1 (AF_8S + 0*8)
#define AF_8SC2 (AF_8S + 1*8)
#define AF_8SC3 (AF_8S + 2*8)
#define AF_8SC4 (AF_8S + 3*8)

#define AF_16SC1 (AF_16S + 0*8)
#define AF_16SC2 (AF_16S + 1*8)
#define AF_16SC3 (AF_16S + 2*8)
#define AF_16SC4 (AF_16S + 3*8)

#define AF_32SC1 (AF_32S + 0*8)
#define AF_32SC2 (AF_32S + 1*8)
#define AF_32SC3 (AF_32S + 2*8)
#define AF_32SC4 (AF_32S + 3*8)

#define AF_32FC1 (AF_32F + 0*8)
#define AF_32FC2 (AF_32F + 1*8)
#define AF_32FC3 (AF_32F + 2*8)
#define AF_32FC4 (AF_32F + 3*8)

#define AF_64FC1 (AF_64F + 0*8)
#define AF_64FC2 (AF_64F + 1*8)
#define AF_64FC3 (AF_64F + 2*8)
#define AF_64FC4 (AF_64F + 3*8)

#define AF_64SC1 (AF_64S + 0*8)

#define AF_AUTO_STEP  0x7fffffff
typedef struct ecl_AfMat
{
    int type;
    int step;

    /* for internal use only */
    int* refcount;

    union
    {
        uchar* ptr;
        short* s;
        int* i;
        float* fl;
        double* db;
    }data;

/*#ifdef __cplusplus
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
#else*/
    int rows;
    int cols;
/*#endif*/

} ecl_AfMat;


#define AF_MAT_CN_MASK          (3 << 3)
#define AF_MAT_CN(flags)        ((((flags) & AF_MAT_CN_MASK) >> 3) + 1)

#define AF_MAT_DEPTH_MASK       7
#define AF_MAT_DEPTH(flags)     ((flags) & AF_MAT_DEPTH_MASK)

#define AF_MAT_TYPE_MASK        31
#define AF_MAT_TYPE(flags)      ((flags) & AF_MAT_TYPE_MASK)

#define AF_MAT_CONT_FLAG_SHIFT  9
#define AF_MAT_CONT_FLAG        (1 << AF_MAT_CONT_FLAG_SHIFT)

#define AF_ARE_SIZES_EQ(mat1, mat2) \
    ((mat1)->rows == (mat2)->rows && (mat1)->cols == (mat2)->cols)

#define AF_ELEM_SIZE(type) \
    (AF_MAT_CN(type) << ((0xe90 >> AF_MAT_DEPTH(type)*2) & 3))

/* default image row align*/
#define  AF_DEFAULT_ROW_ALIGN  4

#define AF_IS_MAT_HDR(mat) \
    ((mat) != MNull && (((const ecl_AfMat*)(mat))->type & AF_MAGIC_MASK) == AF_MAT_MAGIC_VAL)

#define AF_IS_IMAGE_HDR(img) \
    ((img) != MNull && ((const ecl_AfImage*)(img))->size == sizeof(ecl_AfImage))
#define AF_MAT_MAGIC_VAL    0x42420000

#define AF_MAT_ELEM_PTR_FAST( mat, row, col, pix_size )  \
    (\
     (mat).data.ptr + (MDWord)(mat).step*(row) + (pix_size)*(col))

/******************************** Memory storage ****************************************/

typedef struct ecl_AfMemBlock
{
    struct ecl_AfMemBlock*  prev;
    struct ecl_AfMemBlock*  next;
}
ecl_AfMemBlock;

#define AF_STORAGE_MAGIC_VAL    0x42890000

typedef struct ecl_AfMemStorage
{
    int     signature;
    ecl_AfMemBlock* bottom;/* first allocated block */
    ecl_AfMemBlock* top;   /* current memory block - top of the stack */
    struct  ecl_AfMemStorage* parent; /* borrows new blocks from */
    int     block_size;  /* block size */
    int     free_space;  /* free space in the current block */
}
ecl_AfMemStorage;


typedef struct ecl_AfMemStoragePos
{
    ecl_AfMemBlock* top;
    int  free_space;
}
ecl_AfMemStoragePos;


/*********************************** Sequence *******************************************/

typedef struct ecl_AfSeqBlock
{
    struct ecl_AfSeqBlock*  prev; /* previous sequence block */
    struct ecl_AfSeqBlock*  next; /* next sequence block */
    int    start_index;       /* index of the first element in the block +
                                 sequence->first->start_index */
    int    count;             /* number of elements in the block */
    char*  data;              /* pointer to the first element of the block */
}
ecl_AfSeqBlock;


#define AF_TREE_NODE_FIELDS(node_type)                          \
    int       flags;         /* micsellaneous flags */          \
    int       header_size;   /* size of sequence header */      \
    struct    node_type* h_prev; /* previous sequence */        \
    struct    node_type* h_next; /* next sequence */            \
    struct    node_type* v_prev; /* 2nd previous sequence */    \
    struct    node_type* v_next; /* 2nd next sequence */

/*
   Read/Write sequence.
   Elements can be dynamically inserted to or deleted from the sequence.
*/
#define AF_SEQUENCE_FIELDS()                                            \
    AF_TREE_NODE_FIELDS(ecl_AfSeq)                                          \
    int       total;          /* total number of elements */            \
    int       elem_size;      /* size of sequence element in bytes */   \
    char*     block_max;      /* maximal bound of the last block */     \
    char*     ptr;            /* current write pointer */               \
    int       delta_elems;    /* how many elements allocated when the seq grows */  \
    ecl_AfMemStorage* storage;    /* where the seq is stored */             \
    ecl_AfSeqBlock* free_blocks;  /* free blocks list */                    \
    ecl_AfSeqBlock* first; /* pointer to the first sequence block */


typedef struct ecl_AfSeq
{
    AF_SEQUENCE_FIELDS()
}
ecl_AfSeq;

/****************************************************************************************\
*                                    Sequence types                                      *
\****************************************************************************************/

#define AF_SEQ_MAGIC_VAL             0x42990000

/*************** Utility definitions, macros and inline functions ***********************/

#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif

/* min & max without jumps */
#define  AF_IMIN(a, b)  ((a) ^ (((a)^(b)) & (((a) < (b)) - 1)))

#define  AF_IMAX(a, b)  ((a) ^ (((a)^(b)) & (((a) > (b)) - 1)))

/****************************************************************************************/
/*                            Sequence writer & reader                                  */
/****************************************************************************************/

typedef struct ecl_AfSeqWriter
{
    int          header_size;                                      \
    ecl_AfSeq*       seq;        /* the sequence written */            \
    ecl_AfSeqBlock*  block;      /* current block */                   \
    char*        ptr;        /* pointer to free space */           \
    char*        block_min;  /* pointer to the beginning of block*/\
    char*        block_max;  /* pointer to the end of block */
    int  reserved[4]; /* some reserved fields */
}
ecl_AfSeqWriter;

typedef struct ecl_AfSeqReader
{
    int          header_size;                                       \
    ecl_AfSeq*       seq;        /* sequence, beign read */             \
    ecl_AfSeqBlock*  block;      /* current block */                    \
    char*        ptr;        /* pointer to element be read next */  \
    char*        block_min;  /* pointer to the beginning of block */\
    char*        block_max;  /* pointer to the end of block */      \
    int          delta_index;/* = seq->first->start_index   */      \
    char*        prev_elem;  /* pointer to previous element */
    int  reserved[4];
}
ecl_AfSeqReader;

/****************************************************************************************/
/*                                Operations on sequences                               */
/****************************************************************************************/


#define AF_WRITE_SEQ_ELEM( elem, writer )             \
{                                                     \
    if( (writer).ptr >= (writer).block_max )          \
    {                                                 \
        ecl_afCreateSeqBlock( &writer, hMemMgr );                   \
    }                                                 \
    MMemCpy((writer).ptr, &elem, sizeof(elem));        \
    (writer).ptr += sizeof(elem);                     \
}

/* move reader position forward */
#define AF_NEXT_SEQ_ELEM( elem_size, reader )                 \
{                                                             \
    if( ((reader).ptr += (elem_size)) >= (reader).block_max ) \
    {                                                         \
        ecl_afChangeSeqBlock( &(reader), 1 );                     \
    }                                                         \
}

/*************************************** Set ********************************************/
/*
  Set.
  Order isn't keeped. There can be gaps between sequence elements.
  After the element has been inserted it stays on the same place all the time.
  The MSB(most-significant or sign bit) of the first field is 0 iff the element exists.
*/
#define AF_SET_ELEM_FIELDS(elem_type)   \
    int  flags;                         \
    struct elem_type* next_free;

typedef struct ecl_AfSetElem
{
    AF_SET_ELEM_FIELDS(ecl_AfSetElem)
}
ecl_AfSetElem;

#define AF_SET_FIELDS()      \
    AF_SEQUENCE_FIELDS()     \
    ecl_AfSetElem* free_elems;

typedef struct ecl_AfSet
{
    AF_SET_FIELDS()
}
ecl_AfSet;

/* Checks whether the element pointed by ptr belongs to a set or not */
#define AF_IS_SET_ELEM( ptr )  (((ecl_AfSetElem*)(ptr))->flags >= 0)

#define AF_MAGIC_MASK       0xFFFF0000

typedef struct tag_ecl_AfImage
{
	MLong		size;
	MLong		width;
	MLong		height;
	MLong		depth;
	MLong		channel;
	MLong		linebytes;
	MByte	*data;
}ecl_AfImage, *Pecl_AfImage;

typedef struct tag_ecl_AfMemManager
{
	MByte * head;
	MHandle hMemMgr;
	MLong consumed;
	MLong ntotal;
}ecl_AfMemManager, * Pecl_AfMemManager;

#endif/*_AFTYPES_H_*/
