/**
  DO NOT EDIT
  FILE auto-generated
  Module name:
    AutoGen.h
  Abstract:       Auto-generated AutoGen.h for building module or library.
**/

#ifndef _AUTOGENH_8802F36D_0078_4595_8124_13E13EFB8E5F
#define _AUTOGENH_8802F36D_0078_4595_8124_13E13EFB8E5F

#ifdef __cplusplus
extern "C" {
#endif

#include <Uefi.h>
#include <Library/PcdLib.h>

extern GUID  gEfiCallerIdGuid;
extern GUID  gEdkiiDscPlatformGuid;
extern CHAR8 *gEfiCallerBaseName;


// Guids
extern EFI_GUID gFdtTableGuid;
extern EFI_GUID gEfiFileInfoGuid;
extern EFI_GUID gUefiShellFileGuid;
extern EFI_GUID gEfiFileSystemVolumeLabelInfoIdGuid;
extern EFI_GUID gLinuxSimpleMassStorageGuid;
extern EFI_GUID gEfiAcpi10TableGuid;
extern EFI_GUID gEfiAcpi20TableGuid;
extern EFI_GUID gSimpleInitTokenSpaceGuid;

// Protocols
extern EFI_GUID gEfiAbsolutePointerProtocolGuid;
extern EFI_GUID gEfiSimplePointerProtocolGuid;
extern EFI_GUID gEfiSimpleTextInProtocolGuid;
extern EFI_GUID gEfiGraphicsOutputProtocolGuid;
extern EFI_GUID gEfiUgaDrawProtocolGuid;
extern EFI_GUID gEfiLoadedImageProtocolGuid;
extern EFI_GUID gEfiSimpleFileSystemProtocolGuid;
extern EFI_GUID gEfiTimestampProtocolGuid;
extern EFI_GUID gEfiAcpiTableProtocolGuid;

// Definition of SkuId Array
extern UINT64 _gPcd_SkuId_Array[];

// PCD definitions
#define _PCD_TOKEN_PcdGuiDefaultDPI  0U
extern const UINT16 _gPcd_FixedAtBuild_PcdGuiDefaultDPI;
#define _PCD_GET_MODE_16_PcdGuiDefaultDPI  _gPcd_FixedAtBuild_PcdGuiDefaultDPI
//#define _PCD_SET_MODE_16_PcdGuiDefaultDPI  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdGuiDefaultDPI 320
#define _PCD_SIZE_PcdGuiDefaultDPI 2
#define _PCD_GET_MODE_SIZE_PcdGuiDefaultDPI _PCD_SIZE_PcdGuiDefaultDPI
#define _PCD_TOKEN_PcdGuiDefaultRotate  0U
extern const UINT16 _gPcd_FixedAtBuild_PcdGuiDefaultRotate;
#define _PCD_GET_MODE_16_PcdGuiDefaultRotate  _gPcd_FixedAtBuild_PcdGuiDefaultRotate
//#define _PCD_SET_MODE_16_PcdGuiDefaultRotate  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdGuiDefaultRotate 0
#define _PCD_SIZE_PcdGuiDefaultRotate 2
#define _PCD_GET_MODE_SIZE_PcdGuiDefaultRotate _PCD_SIZE_PcdGuiDefaultRotate
#define _PCD_TOKEN_PcdGuiDefaultMouseScale  0U
extern const UINT8 _gPcd_FixedAtBuild_PcdGuiDefaultMouseScale;
#define _PCD_GET_MODE_8_PcdGuiDefaultMouseScale  _gPcd_FixedAtBuild_PcdGuiDefaultMouseScale
//#define _PCD_SET_MODE_8_PcdGuiDefaultMouseScale  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdGuiDefaultMouseScale 1
#define _PCD_SIZE_PcdGuiDefaultMouseScale 1
#define _PCD_GET_MODE_SIZE_PcdGuiDefaultMouseScale _PCD_SIZE_PcdGuiDefaultMouseScale
#define _PCD_TOKEN_PcdGuiDefaultMouseOnlyFirst  0U
extern const BOOLEAN _gPcd_FixedAtBuild_PcdGuiDefaultMouseOnlyFirst;
#define _PCD_GET_MODE_BOOL_PcdGuiDefaultMouseOnlyFirst  _gPcd_FixedAtBuild_PcdGuiDefaultMouseOnlyFirst
//#define _PCD_SET_MODE_BOOL_PcdGuiDefaultMouseOnlyFirst  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdGuiDefaultMouseOnlyFirst 1
#define _PCD_SIZE_PcdGuiDefaultMouseOnlyFirst 1
#define _PCD_GET_MODE_SIZE_PcdGuiDefaultMouseOnlyFirst _PCD_SIZE_PcdGuiDefaultMouseOnlyFirst


#ifdef __cplusplus
}
#endif

#endif
