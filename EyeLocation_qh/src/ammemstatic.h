

#ifndef _AMMEMSTATIC_H_
#define _AMMEMSTATIC_H_
#include "amcomdef.h"

typedef	struct __tag_code__
{
	MVoid*	p;
	MDWord	dwSize;
}CODE, *LPCODE;

#ifdef __cplusplus
extern "C" {
#endif

MVoid*	MMemAllocStatic(MHandle hContext, MDWord dwSize);
MVoid	MMemFreeStatic(MHandle hContext, MVoid* pMem);
MVoid*	MMemReallocStatic(MHandle hContext, MVoid* pMem, MDWord dwSize);


#ifdef __cplusplus
}
#endif

#endif//	_AMMEMSTATIC_H_
