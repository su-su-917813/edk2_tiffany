#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/NonDiscoverableDeviceRegistrationLib.h>
#include <Protocol/NonDiscoverableDevice.h>

#define TLMM_BASE        0x01000000
#define TLMM_SIZE        0x00300000
#define GCC_BASE         0x00100000
#define GCC_SIZE         0x000B0000
#define SDCC2_CORE_BASE  0x07864000
#define SDCC2_CORE_SIZE  0x1900

#define SDCC2_BCR        (GCC_BASE + 0x43000)
#define SDCC2_APPS_CBCR  (GCC_BASE + 0x43018)
#define SDCC2_AHB_CBCR   (GCC_BASE + 0x4301C)

STATIC
EFI_STATUS
MapMmio (
  IN EFI_PHYSICAL_ADDRESS  Base,
  IN UINT64                Size
  )
{
  EFI_STATUS  Status;

  // 加 GCD 描述符
  Status = gDS->AddMemorySpace (
                   EfiGcdMemoryTypeMemoryMappedIo,
                   Base, Size,
                   EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
                   );
  if (EFI_ERROR (Status) && Status != EFI_ALREADY_STARTED) {
    DEBUG ((DEBUG_ERROR, "MsmSdcc: AddMemorySpace %llx fail %r\n", Base, Status));
    return Status;
  }

  // 设置属性 → 触发 CpuSetMemoryAttributes → ArmSetMemoryAttributes 建立 MMU 映射
  Status = gDS->SetMemorySpaceAttributes (Base, Size, EFI_MEMORY_UC);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "MsmSdcc: SetAttr %llx fail %r\n", Base, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
VOID
MsmSdcc2GpioInit (
  VOID
  )
{
  UINTN  Gpio;
  MmioWrite32 (TLMM_BASE + 152 * 0x1000, (0 << 0) | (1 << 2) | (7 << 6));  // CLK
  MmioWrite32 (TLMM_BASE + 153 * 0x1000, (3 << 0) | (1 << 2) | (5 << 6));  // CMD
  for (Gpio = 154; Gpio <= 157; Gpio++) {
    MmioWrite32 (TLMM_BASE + Gpio * 0x1000, (3 << 0) | (1 << 2) | (5 << 6));
  }
}

STATIC
VOID
MsmSdcc2ClockInit (
  VOID
  )
{
  UINT32  Val;
  MmioWrite32 (SDCC2_BCR, 0x0);
  { volatile UINTN _d; for (_d=0; _d<1000000; _d++) {} }
  Val = MmioRead32 (SDCC2_AHB_CBCR);
  MmioWrite32 (SDCC2_AHB_CBCR, Val | 0x1);
  Val = MmioRead32 (SDCC2_APPS_CBCR);
  MmioWrite32 (SDCC2_APPS_CBCR, Val | 0x1);
}

EFI_STATUS
EFIAPI
InitializeMsmSdccPlatform (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_ERROR, "MsmSdcc: map MMIO\n"));

  // 关键步骤：动态映射 MMIO 到 DXE MMU
  Status = MapMmio (TLMM_BASE, TLMM_SIZE);
  if (EFI_ERROR (Status)) return Status;

  Status = MapMmio (GCC_BASE, GCC_SIZE);
  if (EFI_ERROR (Status)) return Status;

  Status = MapMmio (SDCC2_CORE_BASE, SDCC2_CORE_SIZE);
  if (EFI_ERROR (Status)) return Status;

  DEBUG ((DEBUG_ERROR, "MsmSdcc: gpio init\n"));
  MsmSdcc2GpioInit ();
  DEBUG ((DEBUG_ERROR, "MsmSdcc: clock init\n"));
  MsmSdcc2ClockInit ();

  DEBUG ((DEBUG_ERROR, "MsmSdcc: register non-discoverable\n"));
  Status = RegisterNonDiscoverableMmioDevice (
             NonDiscoverableDeviceTypeSdhci,
             NonDiscoverableDeviceDmaTypeCoherent,
             NULL, NULL, 1,
             SDCC2_CORE_BASE, SDCC2_CORE_SIZE
             );
  DEBUG ((DEBUG_ERROR, "MsmSdcc: register = %r\n", Status));
  return Status;
}
