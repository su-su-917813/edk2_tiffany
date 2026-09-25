/**
  DO NOT EDIT
  FILE auto-generated
  Module name:
    AutoGen.h
  Abstract:       Auto-generated AutoGen.h for building module or library.
**/

#ifndef _AUTOGENH_775732DA_086B_4416_8CA9_C284E4585EA7
#define _AUTOGENH_775732DA_086B_4416_8CA9_C284E4585EA7

#ifdef __cplusplus
extern "C" {
#endif

#include <Base.h>
#include <Library/PcdLib.h>

extern GUID  gEfiCallerIdGuid;
extern GUID  gEdkiiDscPlatformGuid;
extern CHAR8 *gEfiCallerBaseName;


// Guids
extern GUID gQcomTokenSpaceGuid;

// Definition of SkuId Array
extern UINT64 _gPcd_SkuId_Array[];

// PCD definitions
#define _PCD_TOKEN_PcdBootShimInfo1  0U
extern const UINT64 _gPcd_FixedAtBuild_PcdBootShimInfo1;
#define _PCD_GET_MODE_64_PcdBootShimInfo1  _gPcd_FixedAtBuild_PcdBootShimInfo1
//#define _PCD_SET_MODE_64_PcdBootShimInfo1  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdBootShimInfo1 0xb0000000
#define _PCD_SIZE_PcdBootShimInfo1 8
#define _PCD_GET_MODE_SIZE_PcdBootShimInfo1 _PCD_SIZE_PcdBootShimInfo1


#ifdef __cplusplus
}
#endif

#endif
