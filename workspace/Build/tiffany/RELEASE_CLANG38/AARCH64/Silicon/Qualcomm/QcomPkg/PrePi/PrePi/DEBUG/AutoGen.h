/**
  DO NOT EDIT
  FILE auto-generated
  Module name:
    AutoGen.h
  Abstract:       Auto-generated AutoGen.h for building module or library.
**/

#ifndef _AUTOGENH_8AF09F13_44C5_96EC_1437_DD899CB5EE5D
#define _AUTOGENH_8AF09F13_44C5_96EC_1437_DD899CB5EE5D

#ifdef __cplusplus
extern "C" {
#endif

#include <PiPei.h>
#include <Library/PcdLib.h>

extern GUID  gEfiCallerIdGuid;
extern GUID  gEdkiiDscPlatformGuid;
extern CHAR8 *gEfiCallerBaseName;

#define EFI_CALLER_ID_GUID \
  {0x8AF09F13, 0x44C5, 0x96EC, {0x14, 0x37, 0xDD, 0x89, 0x9C, 0xB5, 0xEE, 0x5D}}
#define EDKII_DSC_PLATFORM_GUID \
  {0x28f1a3bf, 0x193a, 0x47e3, {0xa7, 0xb9, 0x5a, 0x43, 0x5e, 0xaa, 0xb2, 0xee}}

// Guids
extern EFI_GUID gEfiSystemNvDataFvGuid;
extern EFI_GUID gEfiVariableGuid;
extern EFI_GUID gEfiFirmwarePerformanceGuid;
extern EFI_GUID gArmTokenSpaceGuid;
extern EFI_GUID gEmbeddedTokenSpaceGuid;
extern EFI_GUID gQcomTokenSpaceGuid;
extern EFI_GUID gSimpleInitTokenSpaceGuid;
extern EFI_GUID gEfiMdeModulePkgTokenSpaceGuid;
extern EFI_GUID gSimpleInitFileGuid;
extern EFI_GUID gEfiMdePkgTokenSpaceGuid;
extern EFI_GUID gEfiMemoryTypeInformationGuid;
extern EFI_GUID gEfiHobMemoryAllocModuleGuid;
extern EFI_GUID gEfiHobMemoryAllocStackGuid;
extern EFI_GUID gArmMmuReplaceLiveTranslationEntryFuncGuid;
extern EFI_GUID gLzmaCustomDecompressGuid;

// Protocols
extern EFI_GUID gPeCoffLoaderProtocolGuid;

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

#define _PCD_TOKEN_PcdSystemMemorySize  0U
#define _PCD_SIZE_PcdSystemMemorySize 8
#define _PCD_GET_MODE_SIZE_PcdSystemMemorySize  _PCD_SIZE_PcdSystemMemorySize 
#define _PCD_VALUE_PcdSystemMemorySize  0xFDFA0000ULL
extern const  UINT64  _gPcd_FixedAtBuild_PcdSystemMemorySize;
#define _PCD_GET_MODE_64_PcdSystemMemorySize  _gPcd_FixedAtBuild_PcdSystemMemorySize
//#define _PCD_SET_MODE_64_PcdSystemMemorySize  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdCpuVectorBaseAddress  0U
#define _PCD_SIZE_PcdCpuVectorBaseAddress 8
#define _PCD_GET_MODE_SIZE_PcdCpuVectorBaseAddress  _PCD_SIZE_PcdCpuVectorBaseAddress 
#define _PCD_VALUE_PcdCpuVectorBaseAddress  0x9ff8c000ULL
extern const  UINT64  _gPcd_FixedAtBuild_PcdCpuVectorBaseAddress;
#define _PCD_GET_MODE_64_PcdCpuVectorBaseAddress  _gPcd_FixedAtBuild_PcdCpuVectorBaseAddress
//#define _PCD_SET_MODE_64_PcdCpuVectorBaseAddress  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdFdBaseAddress  0U
#define _PCD_SIZE_PcdFdBaseAddress 8
#define _PCD_GET_MODE_SIZE_PcdFdBaseAddress  _PCD_SIZE_PcdFdBaseAddress 
#define _PCD_VALUE_PcdFdBaseAddress  0x8F600000ULL
extern const  UINT64  _gPcd_FixedAtBuild_PcdFdBaseAddress;
#define _PCD_GET_MODE_64_PcdFdBaseAddress  _gPcd_FixedAtBuild_PcdFdBaseAddress
//#define _PCD_SET_MODE_64_PcdFdBaseAddress  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdFdSize  0U
#define _PCD_SIZE_PcdFdSize 4
#define _PCD_GET_MODE_SIZE_PcdFdSize  _PCD_SIZE_PcdFdSize 
#define _PCD_VALUE_PcdFdSize  0x02000000U
extern const  UINT32  _gPcd_FixedAtBuild_PcdFdSize;
#define _PCD_GET_MODE_32_PcdFdSize  _gPcd_FixedAtBuild_PcdFdSize
//#define _PCD_SET_MODE_32_PcdFdSize  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdPrePiStackBase  0U
#define _PCD_SIZE_PcdPrePiStackBase 4
#define _PCD_GET_MODE_SIZE_PcdPrePiStackBase  _PCD_SIZE_PcdPrePiStackBase 
#define _PCD_VALUE_PcdPrePiStackBase  0x9FF90000U
extern const  UINT32  _gPcd_FixedAtBuild_PcdPrePiStackBase;
#define _PCD_GET_MODE_32_PcdPrePiStackBase  _gPcd_FixedAtBuild_PcdPrePiStackBase
//#define _PCD_SET_MODE_32_PcdPrePiStackBase  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdPrePiStackSize  0U
#define _PCD_SIZE_PcdPrePiStackSize 4
#define _PCD_GET_MODE_SIZE_PcdPrePiStackSize  _PCD_SIZE_PcdPrePiStackSize 
#define _PCD_VALUE_PcdPrePiStackSize  0x00040000U
extern const  UINT32  _gPcd_FixedAtBuild_PcdPrePiStackSize;
#define _PCD_GET_MODE_32_PcdPrePiStackSize  _gPcd_FixedAtBuild_PcdPrePiStackSize
//#define _PCD_SET_MODE_32_PcdPrePiStackSize  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdPrePiCpuIoSize  0U
#define _PCD_SIZE_PcdPrePiCpuIoSize 1
#define _PCD_GET_MODE_SIZE_PcdPrePiCpuIoSize  _PCD_SIZE_PcdPrePiCpuIoSize 
#define _PCD_VALUE_PcdPrePiCpuIoSize  44U
extern const  UINT8  _gPcd_FixedAtBuild_PcdPrePiCpuIoSize;
#define _PCD_GET_MODE_8_PcdPrePiCpuIoSize  _gPcd_FixedAtBuild_PcdPrePiCpuIoSize
//#define _PCD_SET_MODE_8_PcdPrePiCpuIoSize  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdUefiMemPoolBase  0U
#define _PCD_SIZE_PcdUefiMemPoolBase 8
#define _PCD_GET_MODE_SIZE_PcdUefiMemPoolBase  _PCD_SIZE_PcdUefiMemPoolBase 
#define _PCD_VALUE_PcdUefiMemPoolBase  0xA0000000ULL
extern const  UINT64  _gPcd_FixedAtBuild_PcdUefiMemPoolBase;
#define _PCD_GET_MODE_64_PcdUefiMemPoolBase  _gPcd_FixedAtBuild_PcdUefiMemPoolBase
//#define _PCD_SET_MODE_64_PcdUefiMemPoolBase  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdUefiMemPoolSize  0U
#define _PCD_SIZE_PcdUefiMemPoolSize 4
#define _PCD_GET_MODE_SIZE_PcdUefiMemPoolSize  _PCD_SIZE_PcdUefiMemPoolSize 
#define _PCD_VALUE_PcdUefiMemPoolSize  0x2E000000U
extern const  UINT32  _gPcd_FixedAtBuild_PcdUefiMemPoolSize;
#define _PCD_GET_MODE_32_PcdUefiMemPoolSize  _gPcd_FixedAtBuild_PcdUefiMemPoolSize
//#define _PCD_SET_MODE_32_PcdUefiMemPoolSize  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdDeviceTreeStore  0U
#define _PCD_SIZE_PcdDeviceTreeStore 8
#define _PCD_GET_MODE_SIZE_PcdDeviceTreeStore  _PCD_SIZE_PcdDeviceTreeStore 
#define _PCD_VALUE_PcdDeviceTreeStore  0x83300000ULL
extern const  UINT64  _gPcd_FixedAtBuild_PcdDeviceTreeStore;
#define _PCD_GET_MODE_64_PcdDeviceTreeStore  _gPcd_FixedAtBuild_PcdDeviceTreeStore
//#define _PCD_SET_MODE_64_PcdDeviceTreeStore  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdLinuxUtilsCheckHallOnly  0U
#define _PCD_SIZE_PcdLinuxUtilsCheckHallOnly 1
#define _PCD_GET_MODE_SIZE_PcdLinuxUtilsCheckHallOnly  _PCD_SIZE_PcdLinuxUtilsCheckHallOnly 
#define _PCD_VALUE_PcdLinuxUtilsCheckHallOnly  0U
extern const  BOOLEAN  _gPcd_FixedAtBuild_PcdLinuxUtilsCheckHallOnly;
#define _PCD_GET_MODE_BOOL_PcdLinuxUtilsCheckHallOnly  _gPcd_FixedAtBuild_PcdLinuxUtilsCheckHallOnly
//#define _PCD_SET_MODE_BOOL_PcdLinuxUtilsCheckHallOnly  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdPrePiProduceMemoryTypeInformationHob  0U
#define _PCD_SIZE_PcdPrePiProduceMemoryTypeInformationHob 1
#define _PCD_GET_MODE_SIZE_PcdPrePiProduceMemoryTypeInformationHob  _PCD_SIZE_PcdPrePiProduceMemoryTypeInformationHob 
#define _PCD_VALUE_PcdPrePiProduceMemoryTypeInformationHob  ((BOOLEAN)1U)
extern const  BOOLEAN  _gPcd_FixedAtBuild_PcdPrePiProduceMemoryTypeInformationHob;
#define _PCD_GET_MODE_BOOL_PcdPrePiProduceMemoryTypeInformationHob  _gPcd_FixedAtBuild_PcdPrePiProduceMemoryTypeInformationHob
//#define _PCD_SET_MODE_BOOL_PcdPrePiProduceMemoryTypeInformationHob  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

#define _PCD_TOKEN_PcdFirmwareVersionString  0U
#define _PCD_VALUE_PcdFirmwareVersionString  _gPcd_FixedAtBuild_PcdFirmwareVersionString
extern const UINT16 _gPcd_FixedAtBuild_PcdFirmwareVersionString[26];
#define _PCD_GET_MODE_PTR_PcdFirmwareVersionString  _gPcd_FixedAtBuild_PcdFirmwareVersionString
#define _PCD_SIZE_PcdFirmwareVersionString 52
#define _PCD_GET_MODE_SIZE_PcdFirmwareVersionString  _PCD_SIZE_PcdFirmwareVersionString 
//#define _PCD_SET_MODE_PTR_PcdFirmwareVersionString  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD

// Definition of PCDs used in libraries is in AutoGen.c


#ifdef __cplusplus
}
#endif

#endif
