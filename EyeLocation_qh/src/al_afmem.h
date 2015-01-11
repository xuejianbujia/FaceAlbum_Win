

/*
 * afmem.h
 * Used for memory management for funnybase lib
 * 
 * History
 * 
 * 05-10-2005 Huaqi Zhang (hzhang@arcsoft.com.cn)
 * - initial version 
 * 
 *
 */

#ifndef _AFMEM_H_
#define _AFMEM_H_

#include "ammem.h"

#ifdef _USE_EXTERN_MEMMGR_

#define AFMemAlloc		MMemAlloc
#define AFMemFree		MMemFree
#define AFMemSet		MMemSet
#define	AFMemCpy		MMemCpy

#else

/**
 * Notes:	
 *	In the FunnyBase lib you CAN ONLY use AFMem* functions to memory 
 *	management. You SHOULD NOT use platform lib's memory management
 */

#if !defined(_USE_STATIC_MEM_) && !defined(_USE_DYNAMIC_MEM_)
	#error Please specify memory management type (staic or dynamic)!
#endif

#if defined(_USE_STATIC_MEM_) && defined(__WIN32__)  && defined(_DEBUG)
	#define	_CALC_PEAK_MEMORY_
#endif

#define ALIGN_SIZE				4
#define MEMORY_START_OFFSET		64
#define MEMORY_END_OFFSET		64
/*#define MEMORY_LIST_MIN_NUM	1024	// The memory used for memory manager: 12280 Bytes*/
#define MEMORY_LIST_MIN_NUM		128		/* The memory used for memory manager: 768 Bytes*/

typedef struct _tag_MEMLIST		AMMemList;
typedef struct _tag_MEMLIST*	PAMMemList;

struct _tag_MEMLIST { 
  MDWord	dwStartOffset;
  MDWord	dwSize;
  AMMemList	*pNext; 
};

typedef struct {
	PAMMemList	pHead;
	PAMMemList	pCurt;

#ifdef _CALC_PEAK_MEMORY_
	MLong		nPeakMem;
	MLong		nUsedMem;
	MLong		nPeakBlock;
	MLong		nUsedBlock;
#endif	/* _CALC_PEAK_MEMORY_*/

#ifdef _SAMSUNG_MSG_OUT_
	MLong		nErrMagic;
	MLong		nReqMem;
	MLong		nFreeMem;
#endif /* _SAMSUNG_MSG_OUT_*/

	MByte		*pMem;
	MLong		nMemSize;
	MLong		nMemList;
} AMMemMgr, *PAMMemMgr;

#if defined(_USE_STATIC_MEM_)

MVoid*	AMFB_MemAllocStatic(MHandle hContext, MDWord dwSize);
MVoid	AMFB_MemFreeStatic(MHandle hContext, MVoid* pMem);

#if defined(_SAMSUNG_MSG_OUT_)
MVoid AMFB_MemErrReport(MHandle hContext, MLong *nReqMem, MLong *nFreeMem, MLong *nErrMagic);
#endif /* _SAMSUNG_MSG_OUT_*/

#endif /* _USE_STATIC_MEM_*/


#if defined(_USE_STATIC_MEM_)
	#define AFMemAlloc		AMFB_MemAllocStatic
	#define AFMemFree		AMFB_MemFreeStatic
#else	/* _USE_DYNAMIC_MEM_*/
	#define AFMemAlloc		MMemAlloc
	#define AFMemFree		MMemFree
#endif /* _USE_STATIC_MEM_*/

#define AFMemSet		MMemSet
#define	AFMemCpy		MMemCpy


#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 * The function used to create memory manager. The handle will passed to
 * FunnyBase engine for memory management.
 ************************************************************************/
MHandle AMMemMgrCreate(	/* Return memory manager handle*/
	MVoid	*pMem,		/* The alloced memory. Should align memory*/
	MLong	nMemSize	/* The memory size*/
);

/************************************************************************
 * The function destroy the memory manager handle.
 ************************************************************************/
MVoid AMMemMgrDestroy(
	MHandle hMemMgr		/* The hangle of memory manager*/
);

#ifdef __cplusplus
}
#endif

#endif /* _USE_EXTERN_MEMMGR_*/

#endif /* _AFMEM_H_*/
