/**
  DO NOT EDIT
  FILE auto-generated
  Module name:
    AutoGen.h
  Abstract:       Auto-generated AutoGen.h for building module or library.
**/

#ifndef _AUTOGENH_f0d87c5e_4d5d_4fb5_939f_a6768ae8a310
#define _AUTOGENH_f0d87c5e_4d5d_4fb5_939f_a6768ae8a310

#ifdef __cplusplus
extern "C" {
#endif

#include <PiDxe.h>
#include <Library/PcdLib.h>

extern GUID  gEfiCallerIdGuid;
extern GUID  gEdkiiDscPlatformGuid;
extern CHAR8 *gEfiCallerBaseName;

#define EFI_CALLER_ID_GUID \
  {0xf0d87c5e, 0x4d5d, 0x4fb5, {0x93, 0x9f, 0xa6, 0x76, 0x8a, 0xe8, 0xa3, 0x10}}
#define EDKII_DSC_PLATFORM_GUID \
  {0x28f1a3bf, 0x193a, 0x47e3, {0xa7, 0xb9, 0x5a, 0x43, 0x5e, 0xaa, 0xb2, 0xee}}

// Guids
extern EFI_GUID gQcomTokenSpaceGuid;
extern EFI_GUID gEfiMdePkgTokenSpaceGuid;
extern EFI_GUID gEfiVTUTF8Guid;
extern EFI_GUID gEfiVT100Guid;
extern EFI_GUID gEfiVT100PlusGuid;
extern EFI_GUID gEfiPcAnsiGuid;
extern EFI_GUID gEfiUartDevicePathGuid;
extern EFI_GUID gEfiSasDevicePathGuid;
extern EFI_GUID gEfiVirtualDiskGuid;
extern EFI_GUID gEfiVirtualCdGuid;
extern EFI_GUID gEfiPersistentVirtualDiskGuid;
extern EFI_GUID gEfiPersistentVirtualCdGuid;
extern EFI_GUID gEfiEventReadyToBootGuid;
extern EFI_GUID gEfiEventLegacyBootGuid;
extern EFI_GUID gEfiGlobalVariableGuid;
extern EFI_GUID gEfiAcpi20TableGuid;
extern EFI_GUID gEfiAcpi10TableGuid;
extern EFI_GUID gEfiHobListGuid;
extern EFI_GUID gArmTokenSpaceGuid;

// Protocols
extern EFI_GUID gQcomI2cProtocolGuid;
extern EFI_GUID gQcomTlmmProtocolGuid;
extern EFI_GUID gHardwareInterruptProtocolGuid;
extern EFI_GUID gSynapticsTouchDeviceProtocolGuid;
extern EFI_GUID gEfiDevicePathProtocolGuid;
extern EFI_GUID gPcdProtocolGuid;
extern EFI_GUID gEfiPcdProtocolGuid;
extern EFI_GUID gGetPcdInfoProtocolGuid;
extern EFI_GUID gEfiGetPcdInfoProtocolGuid;
extern EFI_GUID gEfiDebugPortProtocolGuid;
extern EFI_GUID gEfiDriverBindingProtocolGuid;
extern EFI_GUID gEfiSimpleTextOutProtocolGuid;
extern EFI_GUID gEfiGraphicsOutputProtocolGuid;
extern EFI_GUID gEfiHiiFontProtocolGuid;
extern EFI_GUID gEfiSimpleFileSystemProtocolGuid;
extern EFI_GUID gEfiUgaDrawProtocolGuid;
extern EFI_GUID gEfiComponentNameProtocolGuid;
extern EFI_GUID gEfiComponentName2ProtocolGuid;
extern EFI_GUID gEfiDriverConfigurationProtocolGuid;
extern EFI_GUID gEfiDriverConfiguration2ProtocolGuid;
extern EFI_GUID gEfiDriverDiagnosticsProtocolGuid;
extern EFI_GUID gEfiDriverDiagnostics2ProtocolGuid;
extern EFI_GUID gEfiLoadedImageProtocolGuid;

// Definition of SkuId Array
extern UINT64 _gPcd_SkuId_Array[];

// Definition of PCDs used in this module

#define _PCD_TOKEN_PcdMipiFrameBufferWidth  0U
#define _PCD_SIZE_PcdMipiFrameBufferWidth 4
#define _PCD_GET_MODE_SIZE_PcdMipiFrameBufferWidth  _PCD_SIZE_PcdMipiFrameBufferWidth 
#define _PCD_VALUE_PcdMipiFrameBufferWidth  1080U
extern const  UINT32  _gPcd_FixedAtBuild_PcdMipiFrameBufferWidth;
#define _PCD_GET_MODE_32_PcdMipiFrameBufferWidth  _gPcd_FixedAtBuild_PcdMipiFrameBufferWidth
//#define _PCD_SET_MODE_32_PcdMipiFrameBufferWidth  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdMipiFrameBufferHeight  0U
#define _PCD_SIZE_PcdMipiFrameBufferHeight 4
#define _PCD_GET_MODE_SIZE_PcdMipiFrameBufferHeight  _PCD_SIZE_PcdMipiFrameBufferHeight 
#define _PCD_VALUE_PcdMipiFrameBufferHeight  1920U
extern const  UINT32  _gPcd_FixedAtBuild_PcdMipiFrameBufferHeight;
#define _PCD_GET_MODE_32_PcdMipiFrameBufferHeight  _gPcd_FixedAtBuild_PcdMipiFrameBufferHeight
//#define _PCD_SET_MODE_32_PcdMipiFrameBufferHeight  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdTouchCtlrAddress  0U
#define _PCD_SIZE_PcdTouchCtlrAddress 2
#define _PCD_GET_MODE_SIZE_PcdTouchCtlrAddress  _PCD_SIZE_PcdTouchCtlrAddress 
#define _PCD_VALUE_PcdTouchCtlrAddress  0U
extern const  UINT16  _gPcd_FixedAtBuild_PcdTouchCtlrAddress;
#define _PCD_GET_MODE_16_PcdTouchCtlrAddress  _gPcd_FixedAtBuild_PcdTouchCtlrAddress
//#define _PCD_SET_MODE_16_PcdTouchCtlrAddress  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdTouchCtlrResetPin  0U
#define _PCD_SIZE_PcdTouchCtlrResetPin 4
#define _PCD_GET_MODE_SIZE_PcdTouchCtlrResetPin  _PCD_SIZE_PcdTouchCtlrResetPin 
#define _PCD_VALUE_PcdTouchCtlrResetPin  0U
extern const  UINT32  _gPcd_FixedAtBuild_PcdTouchCtlrResetPin;
#define _PCD_GET_MODE_32_PcdTouchCtlrResetPin  _gPcd_FixedAtBuild_PcdTouchCtlrResetPin
//#define _PCD_SET_MODE_32_PcdTouchCtlrResetPin  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdTouchCtlrIntPin  0U
#define _PCD_SIZE_PcdTouchCtlrIntPin 4
#define _PCD_GET_MODE_SIZE_PcdTouchCtlrIntPin  _PCD_SIZE_PcdTouchCtlrIntPin 
#define _PCD_VALUE_PcdTouchCtlrIntPin  0U
extern const  UINT32  _gPcd_FixedAtBuild_PcdTouchCtlrIntPin;
#define _PCD_GET_MODE_32_PcdTouchCtlrIntPin  _gPcd_FixedAtBuild_PcdTouchCtlrIntPin
//#define _PCD_SET_MODE_32_PcdTouchCtlrIntPin  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdTouchCtlrI2cDevice  0U
#define _PCD_SIZE_PcdTouchCtlrI2cDevice 4
#define _PCD_GET_MODE_SIZE_PcdTouchCtlrI2cDevice  _PCD_SIZE_PcdTouchCtlrI2cDevice 
#define _PCD_VALUE_PcdTouchCtlrI2cDevice  0U
extern const  UINT32  _gPcd_FixedAtBuild_PcdTouchCtlrI2cDevice;
#define _PCD_GET_MODE_32_PcdTouchCtlrI2cDevice  _gPcd_FixedAtBuild_PcdTouchCtlrI2cDevice
//#define _PCD_SET_MODE_32_PcdTouchCtlrI2cDevice  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdTouchMinX  0U
#define _PCD_SIZE_PcdTouchMinX 4
#define _PCD_GET_MODE_SIZE_PcdTouchMinX  _PCD_SIZE_PcdTouchMinX 
#define _PCD_VALUE_PcdTouchMinX  0U
extern const  UINT32  _gPcd_FixedAtBuild_PcdTouchMinX;
#define _PCD_GET_MODE_32_PcdTouchMinX  _gPcd_FixedAtBuild_PcdTouchMinX
//#define _PCD_SET_MODE_32_PcdTouchMinX  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdTouchMaxX  0U
#define _PCD_SIZE_PcdTouchMaxX 4
#define _PCD_GET_MODE_SIZE_PcdTouchMaxX  _PCD_SIZE_PcdTouchMaxX 
#define _PCD_VALUE_PcdTouchMaxX  0U
extern const  UINT32  _gPcd_FixedAtBuild_PcdTouchMaxX;
#define _PCD_GET_MODE_32_PcdTouchMaxX  _gPcd_FixedAtBuild_PcdTouchMaxX
//#define _PCD_SET_MODE_32_PcdTouchMaxX  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdTouchInvertedX  0U
#define _PCD_SIZE_PcdTouchInvertedX 1
#define _PCD_GET_MODE_SIZE_PcdTouchInvertedX  _PCD_SIZE_PcdTouchInvertedX 
#define _PCD_VALUE_PcdTouchInvertedX  0U
extern const  BOOLEAN  _gPcd_FixedAtBuild_PcdTouchInvertedX;
#define _PCD_GET_MODE_BOOL_PcdTouchInvertedX  _gPcd_FixedAtBuild_PcdTouchInvertedX
//#define _PCD_SET_MODE_BOOL_PcdTouchInvertedX  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdTouchMinY  0U
#define _PCD_SIZE_PcdTouchMinY 4
#define _PCD_GET_MODE_SIZE_PcdTouchMinY  _PCD_SIZE_PcdTouchMinY 
#define _PCD_VALUE_PcdTouchMinY  0U
extern const  UINT32  _gPcd_FixedAtBuild_PcdTouchMinY;
#define _PCD_GET_MODE_32_PcdTouchMinY  _gPcd_FixedAtBuild_PcdTouchMinY
//#define _PCD_SET_MODE_32_PcdTouchMinY  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdTouchMaxY  0U
#define _PCD_SIZE_PcdTouchMaxY 4
#define _PCD_GET_MODE_SIZE_PcdTouchMaxY  _PCD_SIZE_PcdTouchMaxY 
#define _PCD_VALUE_PcdTouchMaxY  0U
extern const  UINT32  _gPcd_FixedAtBuild_PcdTouchMaxY;
#define _PCD_GET_MODE_32_PcdTouchMaxY  _gPcd_FixedAtBuild_PcdTouchMaxY
//#define _PCD_SET_MODE_32_PcdTouchMaxY  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdTouchInvertedY  0U
#define _PCD_SIZE_PcdTouchInvertedY 1
#define _PCD_GET_MODE_SIZE_PcdTouchInvertedY  _PCD_SIZE_PcdTouchInvertedY 
#define _PCD_VALUE_PcdTouchInvertedY  0U
extern const  BOOLEAN  _gPcd_FixedAtBuild_PcdTouchInvertedY;
#define _PCD_GET_MODE_BOOL_PcdTouchInvertedY  _gPcd_FixedAtBuild_PcdTouchInvertedY
//#define _PCD_SET_MODE_BOOL_PcdTouchInvertedY  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdTouchCtlrVddPin  0U
#define _PCD_SIZE_PcdTouchCtlrVddPin 4
#define _PCD_GET_MODE_SIZE_PcdTouchCtlrVddPin  _PCD_SIZE_PcdTouchCtlrVddPin 
#define _PCD_VALUE_PcdTouchCtlrVddPin  0U
extern const  UINT32  _gPcd_FixedAtBuild_PcdTouchCtlrVddPin;
#define _PCD_GET_MODE_32_PcdTouchCtlrVddPin  _gPcd_FixedAtBuild_PcdTouchCtlrVddPin
//#define _PCD_SET_MODE_32_PcdTouchCtlrVddPin  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdTouchCtlrVddIoPin  0U
#define _PCD_SIZE_PcdTouchCtlrVddIoPin 4
#define _PCD_GET_MODE_SIZE_PcdTouchCtlrVddIoPin  _PCD_SIZE_PcdTouchCtlrVddIoPin 
#define _PCD_VALUE_PcdTouchCtlrVddIoPin  0U
extern const  UINT32  _gPcd_FixedAtBuild_PcdTouchCtlrVddIoPin;
#define _PCD_GET_MODE_32_PcdTouchCtlrVddIoPin  _gPcd_FixedAtBuild_PcdTouchCtlrVddIoPin
//#define _PCD_SET_MODE_32_PcdTouchCtlrVddIoPin  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdTouchCtlrVddPinActiveLow  0U
#define _PCD_SIZE_PcdTouchCtlrVddPinActiveLow 1
#define _PCD_GET_MODE_SIZE_PcdTouchCtlrVddPinActiveLow  _PCD_SIZE_PcdTouchCtlrVddPinActiveLow 
#define _PCD_VALUE_PcdTouchCtlrVddPinActiveLow  0U
extern const  BOOLEAN  _gPcd_FixedAtBuild_PcdTouchCtlrVddPinActiveLow;
#define _PCD_GET_MODE_BOOL_PcdTouchCtlrVddPinActiveLow  _gPcd_FixedAtBuild_PcdTouchCtlrVddPinActiveLow
//#define _PCD_SET_MODE_BOOL_PcdTouchCtlrVddPinActiveLow  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

// Definition of PCDs used in libraries is in AutoGen.c


EFI_STATUS
EFIAPI
SynaDeviceInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );





#ifdef __cplusplus
}
#endif

#endif
