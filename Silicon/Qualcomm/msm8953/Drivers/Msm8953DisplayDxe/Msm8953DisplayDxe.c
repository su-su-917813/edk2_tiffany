// Msm8953DisplayDxe.c - Phase 1b
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>

EFI_STATUS EFIAPI DsiPhyInit(VOID);

EFI_STATUS
EFIAPI
Msm8953DisplayDxeInitialize(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  DEBUG((DEBUG_ERROR, "Msm8953DisplayDxe: Phase 1b - DSI PHY init\n"));

  Status = DsiPhyInit();
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Msm8953DisplayDxe: DSI PHY init failed: %r\n", Status));
    // 不返回错误，让系统继续启动
  }

  return EFI_SUCCESS;
}
