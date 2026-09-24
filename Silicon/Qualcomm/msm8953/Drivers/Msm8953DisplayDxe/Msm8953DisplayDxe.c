// Msm8953DisplayDxe.c - Phase 1b
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>

EFI_STATUS EFIAPI DsiPhyInit(VOID);

STATIC
VOID
MarkDirect (
  IN UINT32 Idx,
  IN UINT32 Color
  )
{
  volatile UINT32 *Fb = (volatile UINT32 *)0x90000000;
  UINT32 x, y;
  for (y = 0; y < 20; y++) {
    for (x = 0; x < 20; x++) {
      Fb[y * 1080 + (Idx * 20 + x)] = Color;
    }
  }
}

EFI_STATUS
EFIAPI
Msm8953DisplayDxeInitialize(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  MarkDirect(0, 0x00FF0000);   // 红

  Status = DsiPhyInit();
  if (EFI_ERROR(Status)) {
    MarkDirect(1, 0x0000FF00); // 绿 = 失败
  } else {
    MarkDirect(1, 0x000000FF); // 蓝 = 成功
  }

  MarkDirect(2, 0x00FFFFFF);   // 白 = 入口执行完毕

  return EFI_SUCCESS;
}
