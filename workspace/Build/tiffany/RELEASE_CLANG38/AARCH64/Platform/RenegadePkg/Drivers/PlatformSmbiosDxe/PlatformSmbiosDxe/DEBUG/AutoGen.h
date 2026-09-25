/**
  DO NOT EDIT
  FILE auto-generated
  Module name:
    AutoGen.h
  Abstract:       Auto-generated AutoGen.h for building module or library.
**/

#ifndef _AUTOGENH_BAD0554E_22E9_4D83_9AFD_CC87727A1A45
#define _AUTOGENH_BAD0554E_22E9_4D83_9AFD_CC87727A1A45

#ifdef __cplusplus
extern "C" {
#endif

#include <PiDxe.h>
#include <Library/PcdLib.h>

extern GUID  gEfiCallerIdGuid;
extern GUID  gEdkiiDscPlatformGuid;
extern CHAR8 *gEfiCallerBaseName;

#define EFI_CALLER_ID_GUID \
  {0xBAD0554E, 0x22E9, 0x4D83, {0x9A, 0xFD, 0xCC, 0x87, 0x72, 0x7A, 0x1A, 0x45}}
#define EDKII_DSC_PLATFORM_GUID \
  {0x28f1a3bf, 0x193a, 0x47e3, {0xa7, 0xb9, 0x5a, 0x43, 0x5e, 0xaa, 0xb2, 0xee}}

// Guids
extern EFI_GUID gEfiGlobalVariableGuid;
extern EFI_GUID gArmTokenSpaceGuid;
extern EFI_GUID gEfiMdeModulePkgTokenSpaceGuid;
extern EFI_GUID gRenegadePkgTokenSpaceGuid;
extern EFI_GUID gEfiMdePkgTokenSpaceGuid;
extern EFI_GUID gQcomTokenSpaceGuid;
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
extern EFI_GUID gEfiAcpi20TableGuid;
extern EFI_GUID gEfiAcpi10TableGuid;
extern EFI_GUID gEfiHobListGuid;
extern EFI_GUID gSimpleInitFileGuid;

// Protocols
extern EFI_GUID gEfiSmbiosProtocolGuid;
extern EFI_GUID gKernelFdtProtocolGuid;
extern EFI_GUID gPcdProtocolGuid;
extern EFI_GUID gEfiPcdProtocolGuid;
extern EFI_GUID gGetPcdInfoProtocolGuid;
extern EFI_GUID gEfiGetPcdInfoProtocolGuid;
extern EFI_GUID gEfiDevicePathProtocolGuid;
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

#define _PCD_TOKEN_PcdSystemMemoryBase  0U
#define _PCD_SIZE_PcdSystemMemoryBase 8
#define _PCD_GET_MODE_SIZE_PcdSystemMemoryBase  _PCD_SIZE_PcdSystemMemoryBase 
#define _PCD_VALUE_PcdSystemMemoryBase  0x80000000ULL
extern const  UINT64  _gPcd_FixedAtBuild_PcdSystemMemoryBase;
#define _PCD_GET_MODE_64_PcdSystemMemoryBase  _gPcd_FixedAtBuild_PcdSystemMemoryBase
//#define _PCD_SET_MODE_64_PcdSystemMemoryBase  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdFirmwareRevision  0U
#define _PCD_SIZE_PcdFirmwareRevision 4
#define _PCD_GET_MODE_SIZE_PcdFirmwareRevision  _PCD_SIZE_PcdFirmwareRevision 
#define _PCD_VALUE_PcdFirmwareRevision  0x00010000U
extern const  UINT32  _gPcd_FixedAtBuild_PcdFirmwareRevision;
#define _PCD_GET_MODE_32_PcdFirmwareRevision  _gPcd_FixedAtBuild_PcdFirmwareRevision
//#define _PCD_SET_MODE_32_PcdFirmwareRevision  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdFirmwareVersionString  0U
#define _PCD_VALUE_PcdFirmwareVersionString  _gPcd_FixedAtBuild_PcdFirmwareVersionString
extern const UINT16 _gPcd_FixedAtBuild_PcdFirmwareVersionString[9];
#define _PCD_GET_MODE_PTR_PcdFirmwareVersionString  _gPcd_FixedAtBuild_PcdFirmwareVersionString
#define _PCD_SIZE_PcdFirmwareVersionString 18
#define _PCD_GET_MODE_SIZE_PcdFirmwareVersionString  _PCD_SIZE_PcdFirmwareVersionString 
//#define _PCD_SET_MODE_PTR_PcdFirmwareVersionString  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdDeviceVendor  0U
#define _PCD_VALUE_PcdDeviceVendor  _gPcd_FixedAtBuild_PcdDeviceVendor
extern const UINT8 _gPcd_FixedAtBuild_PcdDeviceVendor[7];
#define _PCD_GET_MODE_PTR_PcdDeviceVendor  _gPcd_FixedAtBuild_PcdDeviceVendor
#define _PCD_SIZE_PcdDeviceVendor 7
#define _PCD_GET_MODE_SIZE_PcdDeviceVendor  _PCD_SIZE_PcdDeviceVendor 
//#define _PCD_SET_MODE_PTR_PcdDeviceVendor  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdDeviceProduct  0U
#define _PCD_VALUE_PcdDeviceProduct  _gPcd_FixedAtBuild_PcdDeviceProduct
extern const UINT8 _gPcd_FixedAtBuild_PcdDeviceProduct[6];
#define _PCD_GET_MODE_PTR_PcdDeviceProduct  _gPcd_FixedAtBuild_PcdDeviceProduct
#define _PCD_SIZE_PcdDeviceProduct 6
#define _PCD_GET_MODE_SIZE_PcdDeviceProduct  _PCD_SIZE_PcdDeviceProduct 
//#define _PCD_SET_MODE_PTR_PcdDeviceProduct  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdDeviceCodeName  0U
#define _PCD_VALUE_PcdDeviceCodeName  _gPcd_FixedAtBuild_PcdDeviceCodeName
extern const UINT8 _gPcd_FixedAtBuild_PcdDeviceCodeName[8];
#define _PCD_GET_MODE_PTR_PcdDeviceCodeName  _gPcd_FixedAtBuild_PcdDeviceCodeName
#define _PCD_SIZE_PcdDeviceCodeName 8
#define _PCD_GET_MODE_SIZE_PcdDeviceCodeName  _PCD_SIZE_PcdDeviceCodeName 
//#define _PCD_SET_MODE_PTR_PcdDeviceCodeName  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

// Definition of PCDs used in libraries is in AutoGen.c


EFI_STATUS
EFIAPI
PlatformSmbiosDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );





#ifdef __cplusplus
}
#endif

#endif
