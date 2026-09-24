/**
  DO NOT EDIT
  FILE auto-generated
  Module name:
    AutoGen.h
  Abstract:       Auto-generated AutoGen.h for building module or library.
**/

#ifndef _AUTOGENH_33220897_2430_1CD5_EA89_CB01B138CF39
#define _AUTOGENH_33220897_2430_1CD5_EA89_CB01B138CF39

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
#define _PCD_TOKEN_PcdMipiFrameBufferAddress  0U
extern const UINT32 _gPcd_FixedAtBuild_PcdMipiFrameBufferAddress;
#define _PCD_GET_MODE_32_PcdMipiFrameBufferAddress  _gPcd_FixedAtBuild_PcdMipiFrameBufferAddress
//#define _PCD_SET_MODE_32_PcdMipiFrameBufferAddress  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdMipiFrameBufferAddress 0x90000000
#define _PCD_SIZE_PcdMipiFrameBufferAddress 4
#define _PCD_GET_MODE_SIZE_PcdMipiFrameBufferAddress _PCD_SIZE_PcdMipiFrameBufferAddress
#define _PCD_TOKEN_PcdMipiFrameBufferWidth  0U
extern const UINT32 _gPcd_FixedAtBuild_PcdMipiFrameBufferWidth;
#define _PCD_GET_MODE_32_PcdMipiFrameBufferWidth  _gPcd_FixedAtBuild_PcdMipiFrameBufferWidth
//#define _PCD_SET_MODE_32_PcdMipiFrameBufferWidth  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdMipiFrameBufferWidth 1080
#define _PCD_SIZE_PcdMipiFrameBufferWidth 4
#define _PCD_GET_MODE_SIZE_PcdMipiFrameBufferWidth _PCD_SIZE_PcdMipiFrameBufferWidth
#define _PCD_TOKEN_PcdMipiFrameBufferHeight  0U
extern const UINT32 _gPcd_FixedAtBuild_PcdMipiFrameBufferHeight;
#define _PCD_GET_MODE_32_PcdMipiFrameBufferHeight  _gPcd_FixedAtBuild_PcdMipiFrameBufferHeight
//#define _PCD_SET_MODE_32_PcdMipiFrameBufferHeight  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdMipiFrameBufferHeight 1920
#define _PCD_SIZE_PcdMipiFrameBufferHeight 4
#define _PCD_GET_MODE_SIZE_PcdMipiFrameBufferHeight _PCD_SIZE_PcdMipiFrameBufferHeight
#define _PCD_TOKEN_PcdMipiFrameBufferPixelBpp  0U
extern const UINT32 _gPcd_FixedAtBuild_PcdMipiFrameBufferPixelBpp;
#define _PCD_GET_MODE_32_PcdMipiFrameBufferPixelBpp  _gPcd_FixedAtBuild_PcdMipiFrameBufferPixelBpp
//#define _PCD_SET_MODE_32_PcdMipiFrameBufferPixelBpp  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdMipiFrameBufferPixelBpp 32
#define _PCD_SIZE_PcdMipiFrameBufferPixelBpp 4
#define _PCD_GET_MODE_SIZE_PcdMipiFrameBufferPixelBpp _PCD_SIZE_PcdMipiFrameBufferPixelBpp
#define _PCD_TOKEN_PcdMipiFrameBufferDelay  0U
extern const UINT32 _gPcd_FixedAtBuild_PcdMipiFrameBufferDelay;
#define _PCD_GET_MODE_32_PcdMipiFrameBufferDelay  _gPcd_FixedAtBuild_PcdMipiFrameBufferDelay
//#define _PCD_SET_MODE_32_PcdMipiFrameBufferDelay  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdMipiFrameBufferDelay 1
#define _PCD_SIZE_PcdMipiFrameBufferDelay 4
#define _PCD_GET_MODE_SIZE_PcdMipiFrameBufferDelay _PCD_SIZE_PcdMipiFrameBufferDelay


#ifdef __cplusplus
}
#endif

#endif
