/**
  DO NOT EDIT
  FILE auto-generated
  Module name:
    AutoGen.h
  Abstract:       Auto-generated AutoGen.h for building module or library.
**/

#ifndef _AUTOGENH_88FE3206_389E_4737_B97F_0A029C5508D3
#define _AUTOGENH_88FE3206_389E_4737_B97F_0A029C5508D3

#ifdef __cplusplus
extern "C" {
#endif

#include <Uefi.h>
#include <Library/PcdLib.h>

extern GUID  gEfiCallerIdGuid;
extern GUID  gEdkiiDscPlatformGuid;
extern CHAR8 *gEfiCallerBaseName;


// Guids
extern EFI_GUID gEfiFileInfoGuid;
extern EFI_GUID gSimpleInitTokenSpaceGuid;

// Definition of SkuId Array
extern UINT64 _gPcd_SkuId_Array[];

// PCD definitions
#define _PCD_TOKEN_PcdLoggerdMinLevel  0U
extern const UINT32 _gPcd_FixedAtBuild_PcdLoggerdMinLevel;
#define _PCD_GET_MODE_32_PcdLoggerdMinLevel  _gPcd_FixedAtBuild_PcdLoggerdMinLevel
//#define _PCD_SET_MODE_32_PcdLoggerdMinLevel  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdLoggerdMinLevel 0xAE01
#define _PCD_SIZE_PcdLoggerdMinLevel 4
#define _PCD_GET_MODE_SIZE_PcdLoggerdMinLevel _PCD_SIZE_PcdLoggerdMinLevel


#ifdef __cplusplus
}
#endif

#endif
