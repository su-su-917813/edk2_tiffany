/**
  DO NOT EDIT
  FILE auto-generated
  Module name:
    AutoGen.h
  Abstract:       Auto-generated AutoGen.h for building module or library.
**/

#ifndef _AUTOGENH_87042C63_21D1_4A68_8C46_3AA49874B93A
#define _AUTOGENH_87042C63_21D1_4A68_8C46_3AA49874B93A

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
extern EFI_GUID gEfiFileSystemVolumeLabelInfoIdGuid;
extern EFI_GUID gLinuxEfiRandomSeedTableGuid;
extern EFI_GUID gSimpleInitTokenSpaceGuid;
extern EFI_GUID gArmTokenSpaceGuid;

// Protocols
extern EFI_GUID gEfiChipInfoProtocolGuid;
extern EFI_GUID gEfiPlatformInfoProtocolGuid;
extern EFI_GUID gEfiRamPartitionProtocolGuid;
extern EFI_GUID gEfiDDRGetInfoProtocolGuid;
extern EFI_GUID gEfiLoadedImageProtocolGuid;
extern EFI_GUID gEfiPartitionInfoProtocolGuid;
extern EFI_GUID gEfiSimpleFileSystemProtocolGuid;
extern EFI_GUID gEfiRngProtocolGuid;
extern EFI_GUID gKernelFdtProtocolGuid;

// Definition of SkuId Array
extern UINT64 _gPcd_SkuId_Array[];

// PCD definitions
#define _PCD_TOKEN_PcdBootCustomLoadAddress  0U
extern const BOOLEAN _gPcd_FixedAtBuild_PcdBootCustomLoadAddress;
#define _PCD_GET_MODE_BOOL_PcdBootCustomLoadAddress  _gPcd_FixedAtBuild_PcdBootCustomLoadAddress
//#define _PCD_SET_MODE_BOOL_PcdBootCustomLoadAddress  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdBootCustomLoadAddress 0
#define _PCD_SIZE_PcdBootCustomLoadAddress 1
#define _PCD_GET_MODE_SIZE_PcdBootCustomLoadAddress _PCD_SIZE_PcdBootCustomLoadAddress
#define _PCD_TOKEN_PcdBootLoadAddressStart  0U
extern const UINT64 _gPcd_FixedAtBuild_PcdBootLoadAddressStart;
#define _PCD_GET_MODE_64_PcdBootLoadAddressStart  _gPcd_FixedAtBuild_PcdBootLoadAddressStart
//#define _PCD_SET_MODE_64_PcdBootLoadAddressStart  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdBootLoadAddressStart 0x0
#define _PCD_SIZE_PcdBootLoadAddressStart 8
#define _PCD_GET_MODE_SIZE_PcdBootLoadAddressStart _PCD_SIZE_PcdBootLoadAddressStart
#define _PCD_TOKEN_PcdBootLoadAddressEnd  0U
extern const UINT64 _gPcd_FixedAtBuild_PcdBootLoadAddressEnd;
#define _PCD_GET_MODE_64_PcdBootLoadAddressEnd  _gPcd_FixedAtBuild_PcdBootLoadAddressEnd
//#define _PCD_SET_MODE_64_PcdBootLoadAddressEnd  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdBootLoadAddressEnd 0x0
#define _PCD_SIZE_PcdBootLoadAddressEnd 8
#define _PCD_GET_MODE_SIZE_PcdBootLoadAddressEnd _PCD_SIZE_PcdBootLoadAddressEnd
#define _PCD_TOKEN_PcdBootKernelAddressStart  0U
extern const UINT64 _gPcd_FixedAtBuild_PcdBootKernelAddressStart;
#define _PCD_GET_MODE_64_PcdBootKernelAddressStart  _gPcd_FixedAtBuild_PcdBootKernelAddressStart
//#define _PCD_SET_MODE_64_PcdBootKernelAddressStart  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdBootKernelAddressStart 0x0
#define _PCD_SIZE_PcdBootKernelAddressStart 8
#define _PCD_GET_MODE_SIZE_PcdBootKernelAddressStart _PCD_SIZE_PcdBootKernelAddressStart
#define _PCD_TOKEN_PcdBootKernelAddressEnd  0U
extern const UINT64 _gPcd_FixedAtBuild_PcdBootKernelAddressEnd;
#define _PCD_GET_MODE_64_PcdBootKernelAddressEnd  _gPcd_FixedAtBuild_PcdBootKernelAddressEnd
//#define _PCD_SET_MODE_64_PcdBootKernelAddressEnd  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdBootKernelAddressEnd 0x0
#define _PCD_SIZE_PcdBootKernelAddressEnd 8
#define _PCD_GET_MODE_SIZE_PcdBootKernelAddressEnd _PCD_SIZE_PcdBootKernelAddressEnd
#define _PCD_TOKEN_PcdBootRamdiskAddressStart  0U
extern const UINT64 _gPcd_FixedAtBuild_PcdBootRamdiskAddressStart;
#define _PCD_GET_MODE_64_PcdBootRamdiskAddressStart  _gPcd_FixedAtBuild_PcdBootRamdiskAddressStart
//#define _PCD_SET_MODE_64_PcdBootRamdiskAddressStart  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdBootRamdiskAddressStart 0x0
#define _PCD_SIZE_PcdBootRamdiskAddressStart 8
#define _PCD_GET_MODE_SIZE_PcdBootRamdiskAddressStart _PCD_SIZE_PcdBootRamdiskAddressStart
#define _PCD_TOKEN_PcdBootRamdiskAddressEnd  0U
extern const UINT64 _gPcd_FixedAtBuild_PcdBootRamdiskAddressEnd;
#define _PCD_GET_MODE_64_PcdBootRamdiskAddressEnd  _gPcd_FixedAtBuild_PcdBootRamdiskAddressEnd
//#define _PCD_SET_MODE_64_PcdBootRamdiskAddressEnd  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdBootRamdiskAddressEnd 0x0
#define _PCD_SIZE_PcdBootRamdiskAddressEnd 8
#define _PCD_GET_MODE_SIZE_PcdBootRamdiskAddressEnd _PCD_SIZE_PcdBootRamdiskAddressEnd
#define _PCD_TOKEN_PcdBootFdtAddressStart  0U
extern const UINT64 _gPcd_FixedAtBuild_PcdBootFdtAddressStart;
#define _PCD_GET_MODE_64_PcdBootFdtAddressStart  _gPcd_FixedAtBuild_PcdBootFdtAddressStart
//#define _PCD_SET_MODE_64_PcdBootFdtAddressStart  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdBootFdtAddressStart 0x0
#define _PCD_SIZE_PcdBootFdtAddressStart 8
#define _PCD_GET_MODE_SIZE_PcdBootFdtAddressStart _PCD_SIZE_PcdBootFdtAddressStart
#define _PCD_TOKEN_PcdBootFdtAddressEnd  0U
extern const UINT64 _gPcd_FixedAtBuild_PcdBootFdtAddressEnd;
#define _PCD_GET_MODE_64_PcdBootFdtAddressEnd  _gPcd_FixedAtBuild_PcdBootFdtAddressEnd
//#define _PCD_SET_MODE_64_PcdBootFdtAddressEnd  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdBootFdtAddressEnd 0x0
#define _PCD_SIZE_PcdBootFdtAddressEnd 8
#define _PCD_GET_MODE_SIZE_PcdBootFdtAddressEnd _PCD_SIZE_PcdBootFdtAddressEnd
#define _PCD_TOKEN_PcdGicDistributorBase  0U
extern const UINT64 _gPcd_FixedAtBuild_PcdGicDistributorBase;
#define _PCD_GET_MODE_64_PcdGicDistributorBase  _gPcd_FixedAtBuild_PcdGicDistributorBase
//#define _PCD_SET_MODE_64_PcdGicDistributorBase  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdGicDistributorBase 0x17a00000
#define _PCD_SIZE_PcdGicDistributorBase 8
#define _PCD_GET_MODE_SIZE_PcdGicDistributorBase _PCD_SIZE_PcdGicDistributorBase
#define _PCD_TOKEN_PcdGicSgiIntId  0U
extern const UINT32 _gPcd_FixedAtBuild_PcdGicSgiIntId;
#define _PCD_GET_MODE_32_PcdGicSgiIntId  _gPcd_FixedAtBuild_PcdGicSgiIntId
//#define _PCD_SET_MODE_32_PcdGicSgiIntId  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdGicSgiIntId 0
#define _PCD_SIZE_PcdGicSgiIntId 4
#define _PCD_GET_MODE_SIZE_PcdGicSgiIntId _PCD_SIZE_PcdGicSgiIntId


#ifdef __cplusplus
}
#endif

#endif
