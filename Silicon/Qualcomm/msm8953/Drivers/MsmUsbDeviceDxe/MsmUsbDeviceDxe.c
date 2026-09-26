/** @file
  MSM8953 DWC3 USB controller — Host mode init for USB keyboard support.
  Based on lk2nd usb30_wrapper.c initialization sequence.
**/

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/NonDiscoverableDeviceRegistrationLib.h>
#include <Protocol/NonDiscoverableDevice.h>

/* ==== DWC3 ==== */
#define DWC3_BASE                 0x07000000
#define DWC3_SIZE                 0x100000    /* 覆盖 DWC3 + QSCRATCH */
#define DWC3_GCTL_OFFSET          0xC110
#define DWC3_GCTL_PRTCAPDIR_MASK  0x00003000
#define DWC3_GCTL_PRTCAP_HOST     0x00001000
#define DWC3_GCTL_CORESOFTRESET   (1 << 11)

/* ==== QSCRATCH wrapper ==== */
#define QSCRATCH_BASE             0x070F8800
#define QSCRATCH_GENERAL_CFG      (QSCRATCH_BASE + 0x08)
#define QSCRATCH_RAM1_REG         (QSCRATCH_BASE + 0x0C)
#define QSCRATCH_HS_PHY_CTRL_COMMON (QSCRATCH_BASE + 0xEC)

/* GENERAL_CFG bit positions */
#define GENCFG_PIPE_UTMI_CLK_SEL_SHFT   0
#define GENCFG_DBM_EN_SHFT              1
#define GENCFG_PIPE3_PHYSTATUS_SW_SHFT  3
#define GENCFG_PIPE_UTMI_CLK_DIS_SHFT   8

/* ==== GCC (CLK_CTL_BASE) ==== */
#define GCC_BASE                 0x01800000
#define GCC_SIZE                 0x00100000

#define USB30_MASTER_CBCR        (GCC_BASE + 0x3F000)
#define USB30_SLEEP_CBCR         (GCC_BASE + 0x3F004)
#define USB30_MOCK_UTMI_CBCR     (GCC_BASE + 0x3F008)
#define USB30_BCR                (GCC_BASE + 0x3F070)
#define GCC_USB30_GDSCR          (GCC_BASE + 0x3F078)
#define USB30_MASTER_CMD_RCGR    (GCC_BASE + 0x3F00C)
#define USB30_MASTER_CFG_RCGR    (GCC_BASE + 0x3F010)
#define USB30_MASTER_M           (GCC_BASE + 0x3F014)
#define USB30_MASTER_N           (GCC_BASE + 0x3F018)
#define USB30_MASTER_D           (GCC_BASE + 0x3F01C)
#define PC_NOC_USB3_AXI_CBCR     (GCC_BASE + 0x3F038)
#define USB_PHY_CFG_AHB_CBCR     (GCC_BASE + 0x3F080)
#define GCC_QUSB2_PHY_BCR        (GCC_BASE + 0x4103C)

STATIC
EFI_STATUS
MapMmio (IN EFI_PHYSICAL_ADDRESS Base, IN UINT64 Size)
{
  EFI_STATUS  Status;
  Status = gDS->AddMemorySpace (EfiGcdMemoryTypeMemoryMappedIo, Base, Size,
                                EFI_MEMORY_UC | EFI_MEMORY_RUNTIME);
  if (EFI_ERROR (Status) && Status != EFI_ALREADY_STARTED) return Status;
  return gDS->SetMemorySpaceAttributes (Base, Size, EFI_MEMORY_UC);
}

STATIC
VOID
EnableClock (IN UINT32 CbcrAddr)
{
  UINT32 V = MmioRead32 (CbcrAddr);
  V &= ~0x80000000;
  V |=  0x1;
  MmioWrite32 (CbcrAddr, V);
}

STATIC
VOID
Udelay (IN UINT32 Us)
{
  volatile UINTN d;
  while (Us--) {
    for (d = 0; d < 20; d++) { __asm__ volatile ("nop"); }
  }
}

STATIC
VOID
QscratchInit (VOID)
{
  UINT32 V;

  /* 1. DBM bypass */
  V = MmioRead32 (QSCRATCH_GENERAL_CFG);
  V &= ~(1 << GENCFG_DBM_EN_SHFT);
  MmioWrite32 (QSCRATCH_GENERAL_CFG, V);

  /* 2. RAM config */
  MmioWrite32 (QSCRATCH_RAM1_REG, 0x0);

  /* 3. HS PHY ctrl */
  MmioWrite32 (QSCRATCH_HS_PHY_CTRL_COMMON, 0x00001CB8);

  /* 4. HS-only 模式 */
  V = MmioRead32 (QSCRATCH_GENERAL_CFG);
  V |=  (1 << GENCFG_PIPE_UTMI_CLK_DIS_SHFT);
  MmioWrite32 (QSCRATCH_GENERAL_CFG, V);
  Udelay (1);

  V = MmioRead32 (QSCRATCH_GENERAL_CFG);
  V |=  (1 << GENCFG_PIPE_UTMI_CLK_SEL_SHFT) |
        (1 << GENCFG_PIPE3_PHYSTATUS_SW_SHFT);
  MmioWrite32 (QSCRATCH_GENERAL_CFG, V);
  Udelay (1);

  V = MmioRead32 (QSCRATCH_GENERAL_CFG);
  V &= ~(1 << GENCFG_PIPE_UTMI_CLK_DIS_SHFT);
  MmioWrite32 (QSCRATCH_GENERAL_CFG, V);
}

EFI_STATUS
EFIAPI
InitializeMsmUsbDevice (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  UINT32      Before, After;
  volatile UINT8 *Fb;
  UINT32      x, y, Color, R, G, B;
  extern RETURN_STATUS EFIAPI SerialPortWrite (UINT8 *, UINTN);
  CHAR8       Line[64];
  UINTN       I;
  CONST CHAR8 Hex[] = "0123456789abcdef";

  /* 1. MMU 映射 */
  MapMmio (DWC3_BASE, DWC3_SIZE);
  MapMmio (GCC_BASE, GCC_SIZE);

  /* 2. 释放复位 */
  MmioWrite32 (USB30_BCR, 0x0);
  MmioWrite32 (GCC_QUSB2_PHY_BCR, 0x0);
  Udelay (100);

  /* 3. 使能 USB30 电源域 */
  {
    UINT32 G = MmioRead32 (GCC_USB30_GDSCR);
    G &= ~0x1;
    MmioWrite32 (GCC_USB30_GDSCR, G);
  }
  Udelay (100);

  /* 4. 配置 USB30 master 时钟源：gpll0 / 6 = 133.33MHz */
  MmioWrite32 (USB30_MASTER_M, 0x0);
  MmioWrite32 (USB30_MASTER_N, 0x0);
  MmioWrite32 (USB30_MASTER_D, 0x0);
  MmioWrite32 (USB30_MASTER_CFG_RCGR, (0 << 8) | 6);
  MmioWrite32 (USB30_MASTER_CMD_RCGR, 0x3);
  Udelay (1000);

  /* 5. 使能所有 USB 时钟分支 */
  EnableClock (USB30_MASTER_CBCR);
  EnableClock (USB30_SLEEP_CBCR);
  EnableClock (USB30_MOCK_UTMI_CBCR);
  EnableClock (USB_PHY_CFG_AHB_CBCR);
  EnableClock (PC_NOC_USB3_AXI_CBCR);
  Udelay (100);

  /* 6. QSCRATCH 初始化 */
  QscratchInit ();

  /* 7. DWC3 core soft reset */
  Before = MmioRead32 (DWC3_BASE + DWC3_GCTL_OFFSET);
  MmioWrite32 (DWC3_BASE + DWC3_GCTL_OFFSET, Before | DWC3_GCTL_CORESOFTRESET);
  Udelay (100000);
  MmioWrite32 (DWC3_BASE + DWC3_GCTL_OFFSET,
               MmioRead32 (DWC3_BASE + DWC3_GCTL_OFFSET) & ~DWC3_GCTL_CORESOFTRESET);
  Udelay (100000);

  /* 8. 配置 GCTL 为 Host 模式 */
  MmioWrite32 (DWC3_BASE + DWC3_GCTL_OFFSET, DWC3_GCTL_PRTCAP_HOST);
  Udelay (10000);
  After = MmioRead32 (DWC3_BASE + DWC3_GCTL_OFFSET);

  /* 9. 注册 XHCI 设备 */
  Status = RegisterNonDiscoverableMmioDevice (
             NonDiscoverableDeviceTypeXhci,
             NonDiscoverableDeviceDmaTypeCoherent,
             NULL, NULL, 1,
             DWC3_BASE, DWC3_SIZE);

  /* 10. 屏幕色块 + 串口输出 */
  Color = (After == 0xFFFFFFFF) ? 0xFF0000 :
          (After == 0x00000000) ? 0x00FF00 : 0x0000FF;
  R = (Color >> 16) & 0xFF; G = (Color >> 8) & 0xFF; B = Color & 0xFF;
  Fb = (volatile UINT8 *)0x90000000;
  for (y = 100; y < 400; y++) {
    for (x = 100; x < 400; x++) {
      Fb[(y * 1080 + x) * 3 + 0] = R;
      Fb[(y * 1080 + x) * 3 + 1] = G;
      Fb[(y * 1080 + x) * 3 + 2] = B;
    }
  }

  Line[0]='['; Line[1]='G'; Line[2]='C'; Line[3]='T';
  Line[4]='L'; Line[5]=']'; Line[6]=' ';
  for (I = 0; I < 8; I++) Line[7+I] = Hex[(After >> (28 - I*4)) & 0xF];
  Line[15]=' '; Line[16]='R'; Line[17]='=';
  for (I = 0; I < 8; I++) Line[18+I] = Hex[((UINT32)Status >> (28 - I*4)) & 0xF];
  Line[26]=0x0D; Line[27]=0x0A;
  SerialPortWrite ((UINT8 *)Line, 28);

  { volatile UINTN d; for (d = 0; d < 800000000; d++) { __asm__ volatile ("nop"); } }
  return Status;
}
