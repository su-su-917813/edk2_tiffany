/* SimpleFbDxe: Simple FrameBuffer */
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <PiDxe.h>
#include <Protocol/GraphicsOutput.h>
#include <Uefi.h>

/// Defines
#define VNBYTES(bpix) (1 << (bpix)) / 8
#define VNBITS(bpix) (1 << (bpix))

/* otm1911 panel is 24bpp RGB888 */
#define FB_BITS_PER_PIXEL (24)
#define FB_BYTES_PER_PIXEL (FB_BITS_PER_PIXEL / 8)

enum video_log2_bpp {
  VIDEO_BPP1 = 0,
  VIDEO_BPP2,
  VIDEO_BPP4,
  VIDEO_BPP8,
  VIDEO_BPP16,
  VIDEO_BPP32,
};

typedef struct {
  VENDOR_DEVICE_PATH DisplayDevicePath;
  EFI_DEVICE_PATH    EndDevicePath;
} DISPLAY_DEVICE_PATH;

DISPLAY_DEVICE_PATH mDisplayDevicePath = {
    {{HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
          (UINT8)(sizeof(VENDOR_DEVICE_PATH)),
          (UINT8)((sizeof(VENDOR_DEVICE_PATH)) >> 8),
      }},
     EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID},
    {END_DEVICE_PATH_TYPE,
     END_ENTIRE_DEVICE_PATH_SUBTYPE,
     {sizeof(EFI_DEVICE_PATH_PROTOCOL), 0}}};

/// Declares

STATIC
EFI_STATUS
EFIAPI
DisplayQueryMode(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This, IN UINT32 ModeNumber,
    OUT UINTN *SizeOfInfo, OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info);

STATIC
EFI_STATUS
EFIAPI
DisplaySetMode(IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This, IN UINT32 ModeNumber);

STATIC
EFI_STATUS
EFIAPI
DisplayBlt(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
    IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL *            BltBuffer,
    OPTIONAL IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation,
    IN UINTN SourceX, IN UINTN SourceY, IN UINTN DestinationX,
    IN UINTN DestinationY, IN UINTN Width, IN UINTN Height,
    IN UINTN Delta OPTIONAL);

STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL mDisplay = {
    DisplayQueryMode, DisplaySetMode, DisplayBlt, NULL};

STATIC
EFI_STATUS
EFIAPI
DisplayQueryMode(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This, IN UINT32 ModeNumber,
    OUT UINTN *SizeOfInfo, OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info)
{
  EFI_STATUS Status;
  Status = gBS->AllocatePool(
      EfiBootServicesData, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION),
      (VOID **)Info);

  ASSERT_EFI_ERROR(Status);

  *SizeOfInfo                   = sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
  (*Info)->Version              = This->Mode->Info->Version;
  (*Info)->HorizontalResolution = This->Mode->Info->HorizontalResolution;
  (*Info)->VerticalResolution   = This->Mode->Info->VerticalResolution;
  (*Info)->PixelFormat          = This->Mode->Info->PixelFormat;
  (*Info)->PixelsPerScanLine    = This->Mode->Info->PixelsPerScanLine;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
DisplaySetMode(IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This, IN UINT32 ModeNumber)
{
  return EFI_SUCCESS;
}

/*
 * The frame buffer exposed by lk2nd is 24bpp RGB888 (3 bytes per pixel),
 * while the UEFI GOP interface uses 32bpp BGRA BltBuffer. Convert here.
 */
STATIC
EFI_STATUS
EFIAPI
DisplayBlt(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
    IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL *            BltBuffer,
    OPTIONAL IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation,
    IN UINTN SourceX, IN UINTN SourceY, IN UINTN DestinationX,
    IN UINTN DestinationY, IN UINTN Width, IN UINTN Height,
    IN UINTN Delta OPTIONAL)
{
  EFI_TPL Tpl;
  UINT8  *Fb       = (UINT8 *)(UINTN)mDisplay.Mode->FrameBufferBase;
  UINTN   FbStride = mDisplay.Mode->Info->HorizontalResolution * 3;
  UINTN   SrcStride;
  UINT8  *Src;
  UINT8  *Dst;
  UINTN   X, Y;

  if (Width == 0 || Height == 0)
    return EFI_SUCCESS;

  Tpl = gBS->RaiseTPL(TPL_NOTIFY);

  switch (BltOperation) {
  case EfiBltVideoFill:
    for (Y = 0; Y < Height; Y++) {
      Dst = Fb + (DestinationY + Y) * FbStride + DestinationX * 3;
      for (X = 0; X < Width; X++) {
        Dst[X * 3 + 0] = BltBuffer->Red;
        Dst[X * 3 + 1] = BltBuffer->Green;
        Dst[X * 3 + 2] = BltBuffer->Blue;
      }
    }
    break;

  case EfiBltBufferToVideo:
    SrcStride = (Delta != 0) ? Delta : (Width * 4);
    for (Y = 0; Y < Height; Y++) {
      Src = (UINT8 *)BltBuffer + (SourceY + Y) * SrcStride + SourceX * 4;
      Dst = Fb + (DestinationY + Y) * FbStride + DestinationX * 3;
      for (X = 0; X < Width; X++) {
        Dst[X * 3 + 0] = Src[X * 4 + 2]; /* R */
        Dst[X * 3 + 1] = Src[X * 4 + 1]; /* G */
        Dst[X * 3 + 2] = Src[X * 4 + 0]; /* B */
      }
    }
    break;

  case EfiBltVideoToBltBuffer:
    SrcStride = (Delta != 0) ? Delta : (Width * 4);
    for (Y = 0; Y < Height; Y++) {
      Src = Fb + (SourceY + Y) * FbStride + SourceX * 3;
      Dst = (UINT8 *)BltBuffer + (DestinationY + Y) * SrcStride +
            DestinationX * 4;
      for (X = 0; X < Width; X++) {
        Dst[X * 4 + 0] = Src[X * 3 + 2]; /* B */
        Dst[X * 4 + 1] = Src[X * 3 + 1]; /* G */
        Dst[X * 4 + 2] = Src[X * 3 + 0]; /* R */
        Dst[X * 4 + 3] = 0;
      }
    }
    break;

  default:
    gBS->RestoreTPL(Tpl);
    return EFI_UNSUPPORTED;
  }

  WriteBackInvalidateDataCacheRange(
      (VOID *)(UINTN)mDisplay.Mode->FrameBufferBase,
      mDisplay.Mode->FrameBufferSize);

  gBS->RestoreTPL(Tpl);
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SimpleFbDxeInitialize(
    IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status             = EFI_SUCCESS;
  EFI_HANDLE hUEFIDisplayHandle = NULL;

  /* TEMP_SIMPLEFB_TEXT */
  {
    extern RETURN_STATUS EFIAPI SerialPortWrite(UINT8 *Buffer, UINTN NumberOfBytes);
    CHAR8 Msg[] = "\r\n[SIMPLEFB_DXE_TEXT_VISIBLE]\r\n";
    SerialPortWrite((UINT8 *)Msg, sizeof(Msg) - 1);
    while (1) { __asm__ volatile ("wfi"); }
  }



  /* Retrieve simple frame buffer from lk2nd */
  DEBUG(
      (EFI_D_ERROR,
       "SimpleFbDxe: Retrieve MIPI FrameBuffer parameters from PCD\n"));
  UINT32 MipiFrameBufferAddr   = FixedPcdGet32(PcdMipiFrameBufferAddress);
  UINT32 MipiFrameBufferWidth  = FixedPcdGet32(PcdMipiFrameBufferWidth);
  UINT32 MipiFrameBufferHeight = FixedPcdGet32(PcdMipiFrameBufferHeight);

  /* Sanity check */
  if (MipiFrameBufferAddr == 0 || MipiFrameBufferWidth == 0 ||
      MipiFrameBufferHeight == 0) {
    DEBUG((EFI_D_ERROR, "SimpleFbDxe: Invalid FrameBuffer parameters\n"));
    return EFI_DEVICE_ERROR;
  }

  /* Prepare struct */
  if (mDisplay.Mode == NULL) {
    Status = gBS->AllocatePool(
        EfiBootServicesData, sizeof(EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE),
        (VOID **)&mDisplay.Mode);

    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status))
      return Status;

    ZeroMem(mDisplay.Mode, sizeof(EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE));
  }

  if (mDisplay.Mode->Info == NULL) {
    Status = gBS->AllocatePool(
        EfiBootServicesData, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION),
        (VOID **)&mDisplay.Mode->Info);

    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status))
      return Status;

    ZeroMem(mDisplay.Mode->Info, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
  }

  /* Set information. GOP advertises 32bpp to upper layers, but the
   * physical frame buffer is 24bpp RGB888. */
  mDisplay.Mode->MaxMode       = 1;
  mDisplay.Mode->Mode          = 0;
  mDisplay.Mode->Info->Version = 0;

  mDisplay.Mode->Info->HorizontalResolution = MipiFrameBufferWidth;
  mDisplay.Mode->Info->VerticalResolution   = MipiFrameBufferHeight;

  UINT32               LineLength = MipiFrameBufferWidth * 3; /* 24bpp */
  UINT32               FrameBufferSize    = LineLength * MipiFrameBufferHeight;
  EFI_PHYSICAL_ADDRESS FrameBufferAddress = MipiFrameBufferAddr;

  mDisplay.Mode->Info->PixelsPerScanLine = MipiFrameBufferWidth;
  mDisplay.Mode->Info->PixelFormat = PixelBlueGreenRedReserved8BitPerColor;
  mDisplay.Mode->SizeOfInfo      = sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
  mDisplay.Mode->FrameBufferBase = FrameBufferAddress;
  mDisplay.Mode->FrameBufferSize = FrameBufferSize;

  /* Clear the screen to black */
  ZeroMem((void *)FrameBufferAddress, FrameBufferSize);
  WriteBackInvalidateDataCacheRange(
      (void *)FrameBufferAddress, FrameBufferSize);

  /* Register handle */
  Status = gBS->InstallMultipleProtocolInterfaces(
      &hUEFIDisplayHandle, &gEfiDevicePathProtocolGuid, &mDisplayDevicePath,
      &gEfiGraphicsOutputProtocolGuid, &mDisplay, NULL);

  ASSERT_EFI_ERROR(Status);

  return Status;
}
