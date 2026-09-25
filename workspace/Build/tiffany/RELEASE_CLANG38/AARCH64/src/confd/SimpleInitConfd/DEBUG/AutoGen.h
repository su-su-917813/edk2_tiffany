/**
  DO NOT EDIT
  FILE auto-generated
  Module name:
    AutoGen.h
  Abstract:       Auto-generated AutoGen.h for building module or library.
**/

#ifndef _AUTOGENH_8BE9190F_4974_4625_8725_2F3913CCAE07
#define _AUTOGENH_8BE9190F_4974_4625_8725_2F3913CCAE07

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

// Protocols
extern EFI_GUID gEfiSimpleFileSystemProtocolGuid;
extern EFI_GUID gEfiPartitionInfoProtocolGuid;

// Definition of SkuId Array
extern UINT64 _gPcd_SkuId_Array[];

// PCD definitions
#define _PCD_TOKEN_PcdConfDefaultPrefix  0U
extern const UINT8 _gPcd_FixedAtBuild_PcdConfDefaultPrefix[];
#define _PCD_GET_MODE_PTR_PcdConfDefaultPrefix  _gPcd_FixedAtBuild_PcdConfDefaultPrefix
//#define _PCD_SET_MODE_PTR_PcdConfDefaultPrefix  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdConfDefaultPrefix _gPcd_FixedAtBuild_PcdConfDefaultPrefix
#define _PCD_SIZE_PcdConfDefaultPrefix 18
#define _PCD_GET_MODE_SIZE_PcdConfDefaultPrefix _PCD_SIZE_PcdConfDefaultPrefix
#define _PCD_TOKEN_PcdConfDefaultStaticPrefix  0U
extern const UINT8 _gPcd_FixedAtBuild_PcdConfDefaultStaticPrefix[];
#define _PCD_GET_MODE_PTR_PcdConfDefaultStaticPrefix  _gPcd_FixedAtBuild_PcdConfDefaultStaticPrefix
//#define _PCD_SET_MODE_PTR_PcdConfDefaultStaticPrefix  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdConfDefaultStaticPrefix _gPcd_FixedAtBuild_PcdConfDefaultStaticPrefix
#define _PCD_SIZE_PcdConfDefaultStaticPrefix 25
#define _PCD_GET_MODE_SIZE_PcdConfDefaultStaticPrefix _PCD_SIZE_PcdConfDefaultStaticPrefix


#ifdef __cplusplus
}
#endif

#endif
