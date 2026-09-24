/**
  DO NOT EDIT
  FILE auto-generated
  Module name:
    AutoGen.h
  Abstract:       Auto-generated AutoGen.h for building module or library.
**/

#ifndef _AUTOGENH_59C11815_F8DA_4F49_B4FB_EC1E41ED1F07
#define _AUTOGENH_59C11815_F8DA_4F49_B4FB_EC1E41ED1F07

#ifdef __cplusplus
extern "C" {
#endif

#include <PiPei.h>
#include <Library/PcdLib.h>

extern GUID  gEfiCallerIdGuid;
extern GUID  gEdkiiDscPlatformGuid;
extern CHAR8 *gEfiCallerBaseName;


// Guids
extern EFI_GUID gQcomTokenSpaceGuid;
extern EFI_GUID gEfiMdeModulePkgTokenSpaceGuid;

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
#define _PCD_TOKEN_PcdHallSensorPin  0U
extern const UINT32 _gPcd_FixedAtBuild_PcdHallSensorPin;
#define _PCD_GET_MODE_32_PcdHallSensorPin  _gPcd_FixedAtBuild_PcdHallSensorPin
//#define _PCD_SET_MODE_32_PcdHallSensorPin  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdHallSensorPin 0
#define _PCD_SIZE_PcdHallSensorPin 4
#define _PCD_GET_MODE_SIZE_PcdHallSensorPin _PCD_SIZE_PcdHallSensorPin
#define _PCD_TOKEN_PcdHallSensorActiveLow  0U
extern const BOOLEAN _gPcd_FixedAtBuild_PcdHallSensorActiveLow;
#define _PCD_GET_MODE_BOOL_PcdHallSensorActiveLow  _gPcd_FixedAtBuild_PcdHallSensorActiveLow
//#define _PCD_SET_MODE_BOOL_PcdHallSensorActiveLow  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdHallSensorActiveLow 1
#define _PCD_SIZE_PcdHallSensorActiveLow 1
#define _PCD_GET_MODE_SIZE_PcdHallSensorActiveLow _PCD_SIZE_PcdHallSensorActiveLow
#define _PCD_TOKEN_PcdFirmwareVersionString  0U
extern const UINT16 _gPcd_FixedAtBuild_PcdFirmwareVersionString[];
#define _PCD_GET_MODE_PTR_PcdFirmwareVersionString  _gPcd_FixedAtBuild_PcdFirmwareVersionString
//#define _PCD_SET_MODE_PTR_PcdFirmwareVersionString  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_VALUE_PcdFirmwareVersionString _gPcd_FixedAtBuild_PcdFirmwareVersionString
#define _PCD_SIZE_PcdFirmwareVersionString 52
#define _PCD_GET_MODE_SIZE_PcdFirmwareVersionString _PCD_SIZE_PcdFirmwareVersionString


#ifdef __cplusplus
}
#endif

#endif
