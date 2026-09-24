[Defines]
  VENDOR_NAME                    = Xiaomi
  PLATFORM_NAME                  = tiffany
  DEVICE_DXE_FV_COMPONENTS       = Platform/Xiaomi/msm8953/tiffany.fdf.inc
  PLATFORM_GUID                  = 28f1a3bf-193a-47e3-a7b9-5a435eaab2ee
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010019
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Platform/Qualcomm/msm8953/msm8953.fdf
  #DEVICE_DXE_FV_COMPONENTS       = Platform/Xiaomi/sdm660/whyred.fdf.inc

!include Platform/Qualcomm/msm8953/msm8953.dsc

[BuildOptions.common]
  GCC:*_*_AARCH64_CC_FLAGS = -DENABLE_SIMPLE_INIT

[PcdsFixedAtBuild.common]
#  gQcomTokenSpaceGuid.PcdMipiFrameBufferWidth|1080
#  gQcomTokenSpaceGuid.PcdMipiFrameBufferHeight|2160

  # Simple Init
#  gSimpleInitTokenSpaceGuid.PcdGuiDefaultDPI|350

#  gRenegadePkgTokenSpaceGuid.PcdDeviceVendor|"Redmi"
#  gRenegadePkgTokenSpaceGuid.PcdDeviceProduct|"Note5"
#  gRenegadePkgTokenSpaceGuid.PcdDeviceCodeName|"whyred"
  gQcomTokenSpaceGuid.PcdMipiFrameBufferWidth|1080
  gQcomTokenSpaceGuid.PcdMipiFrameBufferHeight|1920
  gSimpleInitTokenSpaceGuid.PcdGuiDefaultDPI|320

  gRenegadePkgTokenSpaceGuid.PcdDeviceVendor|"Xiaomi"
  gRenegadePkgTokenSpaceGuid.PcdDeviceProduct|"Mi 5X"
  gRenegadePkgTokenSpaceGuid.PcdDeviceCodeName|"tiffany"
