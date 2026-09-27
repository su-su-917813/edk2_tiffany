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

  /* 8. DWC3 core reset 完整序列（参考 Linux dwc3_core_init） */
  {
    UINT32 V, Retry;
    /* 8.1 Global core soft reset */
    V = MmioRead32 (DWC3_BASE + DWC3_GCTL_OFFSET);
    V |= DWC3_GCTL_CORESOFTRESET;
    MmioWrite32 (DWC3_BASE + DWC3_GCTL_OFFSET, V);
    Udelay (200000);   /* 200ms */

    V = MmioRead32 (DWC3_BASE + DWC3_GCTL_OFFSET);
    V &= ~DWC3_GCTL_CORESOFTRESET;
    MmioWrite32 (DWC3_BASE + DWC3_GCTL_OFFSET, V);
    Udelay (200000);

    /* 8.2 等 DWC3 完成复位 —— 轮询 GSNPSID 确认非 0 */
    Retry = 100;
    while ((MmioRead32 (DWC3_BASE + 0xC120) == 0) && (Retry-- > 0)) {
      Udelay (10000);
    }

    /* 8.3 GCTL = Host + SCALEDOWN + DISSCRAMBLE + DSBLCLKGTNG */
    V  = 0;
    V |= (1u << 0);       /* DSBLCLKGTNG */
    V |= (1u << 2);       /* SOFITPSYNC */
    V |= (1u << 3);       /* DISSCRAMBLE */
    V |= (1u << 4);       /* SCALEDOWN[1:0] = 01 */
    V |= (1u << 12);      /* PRTCAPDIR[1:0] = 01 = Host */
    MmioWrite32 (DWC3_BASE + DWC3_GCTL_OFFSET, V);
    Udelay (10000);
  }
  After = MmioRead32 (DWC3_BASE + DWC3_GCTL_OFFSET);

  /* 8b. 使能所有 XHCI 端口 —— CONFIG.MaxPortsEn @ OpReg 0x38 */
  {
    UINT32 Cfg = MmioRead32 (DWC3_BASE + 0x4000 + 0x38);
    Cfg &= 0x00FFFFFF;
    Cfg |= (0x02 << 24);
    MmioWrite32 (DWC3_BASE + 0x4000 + 0x38, Cfg);
  }

  /* 8c. Host 模式 QUSB2 PHY 初始化 */
  {
    UINT32 PhyCfg;
    MmioWrite32 (DWC3_BASE + 0xC200, MmioRead32 (DWC3_BASE + 0xC200) | (1u << 31));
    Udelay (10000);
    MmioWrite32 (DWC3_BASE + 0xC200, MmioRead32 (DWC3_BASE + 0xC200) & ~(1u << 31));
    Udelay (10000);
    PhyCfg  = MmioRead32 (DWC3_BASE + 0xC200);
    PhyCfg &= ~0xF;
    PhyCfg &= ~(1u << 6);
    MmioWrite32 (DWC3_BASE + 0xC200, PhyCfg);
  }

  /* 8d. DWC3 RunStop 使能 */
  {
    UINT32 Dctl = MmioRead32 (DWC3_BASE + 0xC704);
    Dctl |=  0x1;
    MmioWrite32 (DWC3_BASE + 0xC704, Dctl);
    Udelay (10000);
  }

  /* 8e. USB2 PHY 完整配置（参考 Linux dwc3 驱动） */
  {
    UINT32 V;
    /* 1. PHY 软复位 */
    V = MmioRead32 (DWC3_BASE + 0xC200);
    MmioWrite32 (DWC3_BASE + 0xC200, V | (1u << 31));
    Udelay (10000);
    V = MmioRead32 (DWC3_BASE + 0xC200);
    MmioWrite32 (DWC3_BASE + 0xC200, V & ~(1u << 31));
    Udelay (10000);

    /* 2. GUSB2PHYCFG：30MHz、UTMI+、不挂起、8-bit UTMI */
    V  = MmioRead32 (DWC3_BASE + 0xC200);
    V &= ~0xF;               /* 清 PHY 时钟频率 */
    V |=  0x0;               /* 30MHz */
    V &= ~(1u << 4);         /* UTMI+ 而非 ULPI */
    V &= ~(1u << 6);         /* 清 SUSPHY */
    V &= ~(1u << 12);        /* 8-bit UTMI */
    V |=  (1u << 15);        /* ENAUTOSUSPEND */
    V &= ~(3u << 3);         /* 清 PHYIF */
    MmioWrite32 (DWC3_BASE + 0xC200, V);
    Udelay (10000);

    /* 3. GCTL：Host 模式 + 禁用时钟门控 + SOF ITP 同步 */
    V = MmioRead32 (DWC3_BASE + 0xC110);
    V |=  (1u << 0);         /* DSBLCLKGTNG */
    V |=  (1u << 2);         /* SOFITPSYNC */
    V &= ~(1u << 5);         /* 清 DCFG 上拉 */
    MmioWrite32 (DWC3_BASE + 0xC110, V);
    Udelay (1000);
  }

  /* 8f. 用正确偏移写 XHCI HCRST（XHCI OpReg 在 base + 0x40） */
  {
    UINT32 Cmd;
    MmioWrite32 (DWC3_BASE + 0x40, 0x2);   /* USBCMD.HCRST = 1 */
    Udelay (10000);
    for (Cmd = 0; Cmd < 100; Cmd++) {
      if ((MmioRead32 (DWC3_BASE + 0x40) & 0x2) == 0) break;
      Udelay (1000);
    }
  }

  /* 8e. 一次性测试所有可能的 XHCI OpReg 偏移 */
  {
    extern RETURN_STATUS EFIAPI SerialPortWrite (UINT8 *, UINTN);
    CONST CHAR8 Hx[] = "0123456789abcdef";
    UINT8  B00  = MmioRead8  (DWC3_BASE + 0x00);
    UINT16 H02  = MmioRead16 (DWC3_BASE + 0x02);
    UINT32 W40  = MmioRead32 (DWC3_BASE + 0x40);
    UINT32 W44  = MmioRead32 (DWC3_BASE + 0x44);
    UINT32 W4000= MmioRead32 (DWC3_BASE + 0x4000);
    UINT32 W440 = MmioRead32 (DWC3_BASE + 0x440);
    CHAR8  Mx[80];
    UINTN  K = 0;
    Mx[K++]='['; Mx[K++]='B'; Mx[K++]='0'; Mx[K++]='=';
    Mx[K++]=Hx[(B00>>4)&0xF]; Mx[K++]=Hx[B00&0xF]; Mx[K++]=' ';
    Mx[K++]='H'; Mx[K++]='0'; Mx[K++]='2'; Mx[K++]='=';
    Mx[K++]=Hx[(H02>>12)&0xF]; Mx[K++]=Hx[(H02>>8)&0xF];
    Mx[K++]=Hx[(H02>>4)&0xF]; Mx[K++]=Hx[H02&0xF]; Mx[K++]=' ';
    Mx[K++]='W'; Mx[K++]='4'; Mx[K++]='0'; Mx[K++]='=';
    Mx[K++]=Hx[(W40>>28)&0xF]; Mx[K++]=Hx[(W40>>24)&0xF];
    Mx[K++]=Hx[(W40>>20)&0xF]; Mx[K++]=Hx[(W40>>16)&0xF];
    Mx[K++]=Hx[(W40>>12)&0xF]; Mx[K++]=Hx[(W40>>8)&0xF];
    Mx[K++]=Hx[(W40>>4)&0xF]; Mx[K++]=Hx[W40&0xF]; Mx[K++]=' ';
    Mx[K++]='W'; Mx[K++]='4'; Mx[K++]='4'; Mx[K++]='=';
    Mx[K++]=Hx[(W44>>28)&0xF]; Mx[K++]=Hx[(W44>>24)&0xF];
    Mx[K++]=Hx[(W44>>20)&0xF]; Mx[K++]=Hx[(W44>>16)&0xF];
    Mx[K++]=Hx[(W44>>12)&0xF]; Mx[K++]=Hx[(W44>>8)&0xF];
    Mx[K++]=Hx[(W44>>4)&0xF]; Mx[K++]=Hx[W44&0xF]; Mx[K++]=' ';
    Mx[K++]='4'; Mx[K++]='k'; Mx[K++]='=';
    Mx[K++]=Hx[(W4000>>28)&0xF]; Mx[K++]=Hx[(W4000>>24)&0xF];
    Mx[K++]=Hx[(W4000>>20)&0xF]; Mx[K++]=Hx[(W4000>>16)&0xF];
    Mx[K++]=Hx[(W4000>>12)&0xF]; Mx[K++]=Hx[(W4000>>8)&0xF];
    Mx[K++]=Hx[(W4000>>4)&0xF]; Mx[K++]=Hx[W4000&0xF]; Mx[K++]=' ';
    Mx[K++]='4'; Mx[K++]='4'; Mx[K++]='0'; Mx[K++]='=';
    Mx[K++]=Hx[(W440>>28)&0xF]; Mx[K++]=Hx[(W440>>24)&0xF];
    Mx[K++]=Hx[(W440>>20)&0xF]; Mx[K++]=Hx[(W440>>16)&0xF];
    Mx[K++]=Hx[(W440>>12)&0xF]; Mx[K++]=Hx[(W440>>8)&0xF];
    Mx[K++]=Hx[(W440>>4)&0xF]; Mx[K++]=Hx[W440&0xF];
    Mx[K++]=0x0D; Mx[K++]=0x0A;
    SerialPortWrite ((UINT8 *)Mx, K);
  }

  /* 8g. 探测 XHCI CONFIG 寄存器在两个偏移的值 */
  {
    extern RETURN_STATUS EFIAPI SerialPortWrite (UINT8 *, UINTN);
    CONST CHAR8 Hx[] = "0123456789abcdef";
    UINT32 CfgA = MmioRead32 (DWC3_BASE + 0x78);     /* OpReg = 0x40 */
    UINT32 CfgB = MmioRead32 (DWC3_BASE + 0x4038);   /* OpReg = 0x4000 */
    UINT32 StsA = MmioRead32 (DWC3_BASE + 0x44);     /* USBSTS @ 0x40 */
    UINT32 StsB = MmioRead32 (DWC3_BASE + 0x4004);   /* USBSTS @ 0x4000 */
    CHAR8  Mx[60];
    UINTN  K = 0;
    Mx[K++]='[';
    Mx[K++]='C'; Mx[K++]='f'; Mx[K++]='g'; Mx[K++]='A'; Mx[K++]='=';
    Mx[K++]=Hx[(CfgA>>28)&0xF]; Mx[K++]=Hx[(CfgA>>24)&0xF];
    Mx[K++]=Hx[(CfgA>>20)&0xF]; Mx[K++]=Hx[(CfgA>>16)&0xF];
    Mx[K++]=Hx[(CfgA>>12)&0xF]; Mx[K++]=Hx[(CfgA>>8)&0xF];
    Mx[K++]=Hx[(CfgA>>4)&0xF]; Mx[K++]=Hx[CfgA&0xF]; Mx[K++]=' ';
    Mx[K++]='B'; Mx[K++]='=';
    Mx[K++]=Hx[(CfgB>>28)&0xF]; Mx[K++]=Hx[(CfgB>>24)&0xF];
    Mx[K++]=Hx[(CfgB>>20)&0xF]; Mx[K++]=Hx[(CfgB>>16)&0xF];
    Mx[K++]=Hx[(CfgB>>12)&0xF]; Mx[K++]=Hx[(CfgB>>8)&0xF];
    Mx[K++]=Hx[(CfgB>>4)&0xF]; Mx[K++]=Hx[CfgB&0xF]; Mx[K++]=' ';
    Mx[K++]='S'; Mx[K++]='A'; Mx[K++]='=';
    Mx[K++]=Hx[(StsA>>28)&0xF]; Mx[K++]=Hx[(StsA>>24)&0xF];
    Mx[K++]=Hx[(StsA>>20)&0xF]; Mx[K++]=Hx[(StsA>>16)&0xF];
    Mx[K++]=Hx[(StsA>>12)&0xF]; Mx[K++]=Hx[(StsA>>8)&0xF];
    Mx[K++]=Hx[(StsA>>4)&0xF]; Mx[K++]=Hx[StsA&0xF]; Mx[K++]=' ';
    Mx[K++]='S'; Mx[K++]='B'; Mx[K++]='=';
    Mx[K++]=Hx[(StsB>>28)&0xF]; Mx[K++]=Hx[(StsB>>24)&0xF];
    Mx[K++]=Hx[(StsB>>20)&0xF]; Mx[K++]=Hx[(StsB>>16)&0xF];
    Mx[K++]=Hx[(StsB>>12)&0xF]; Mx[K++]=Hx[(StsB>>8)&0xF];
    Mx[K++]=Hx[(StsB>>4)&0xF]; Mx[K++]=Hx[StsB&0xF];
    Mx[K++]=0x0D; Mx[K++]=0x0A;
    SerialPortWrite ((UINT8 *)Mx, K);
  }

  /* 8g. 探测 XHCI CONFIG 寄存器在两个偏移的值 */
  {
    extern RETURN_STATUS EFIAPI SerialPortWrite (UINT8 *, UINTN);
    CONST CHAR8 Hx[] = "0123456789abcdef";
    UINT32 CfgA = MmioRead32 (DWC3_BASE + 0x78);     /* OpReg = 0x40 */
    UINT32 CfgB = MmioRead32 (DWC3_BASE + 0x4038);   /* OpReg = 0x4000 */
    UINT32 StsA = MmioRead32 (DWC3_BASE + 0x44);     /* USBSTS @ 0x40 */
    UINT32 StsB = MmioRead32 (DWC3_BASE + 0x4004);   /* USBSTS @ 0x4000 */
    CHAR8  Mx[60];
    UINTN  K = 0;
    Mx[K++]='[';
    Mx[K++]='C'; Mx[K++]='f'; Mx[K++]='g'; Mx[K++]='A'; Mx[K++]='=';
    Mx[K++]=Hx[(CfgA>>28)&0xF]; Mx[K++]=Hx[(CfgA>>24)&0xF];
    Mx[K++]=Hx[(CfgA>>20)&0xF]; Mx[K++]=Hx[(CfgA>>16)&0xF];
    Mx[K++]=Hx[(CfgA>>12)&0xF]; Mx[K++]=Hx[(CfgA>>8)&0xF];
    Mx[K++]=Hx[(CfgA>>4)&0xF]; Mx[K++]=Hx[CfgA&0xF]; Mx[K++]=' ';
    Mx[K++]='B'; Mx[K++]='=';
    Mx[K++]=Hx[(CfgB>>28)&0xF]; Mx[K++]=Hx[(CfgB>>24)&0xF];
    Mx[K++]=Hx[(CfgB>>20)&0xF]; Mx[K++]=Hx[(CfgB>>16)&0xF];
    Mx[K++]=Hx[(CfgB>>12)&0xF]; Mx[K++]=Hx[(CfgB>>8)&0xF];
    Mx[K++]=Hx[(CfgB>>4)&0xF]; Mx[K++]=Hx[CfgB&0xF]; Mx[K++]=' ';
    Mx[K++]='S'; Mx[K++]='A'; Mx[K++]='=';
    Mx[K++]=Hx[(StsA>>28)&0xF]; Mx[K++]=Hx[(StsA>>24)&0xF];
    Mx[K++]=Hx[(StsA>>20)&0xF]; Mx[K++]=Hx[(StsA>>16)&0xF];
    Mx[K++]=Hx[(StsA>>12)&0xF]; Mx[K++]=Hx[(StsA>>8)&0xF];
    Mx[K++]=Hx[(StsA>>4)&0xF]; Mx[K++]=Hx[StsA&0xF]; Mx[K++]=' ';
    Mx[K++]='S'; Mx[K++]='B'; Mx[K++]='=';
    Mx[K++]=Hx[(StsB>>28)&0xF]; Mx[K++]=Hx[(StsB>>24)&0xF];
    Mx[K++]=Hx[(StsB>>20)&0xF]; Mx[K++]=Hx[(StsB>>16)&0xF];
    Mx[K++]=Hx[(StsB>>12)&0xF]; Mx[K++]=Hx[(StsB>>8)&0xF];
    Mx[K++]=Hx[(StsB>>4)&0xF]; Mx[K++]=Hx[StsB&0xF];
    Mx[K++]=0x0D; Mx[K++]=0x0A;
    SerialPortWrite ((UINT8 *)Mx, K);
  }

  /* 8h. 探测 XHCI Capability 是否可读 */
  {
    extern RETURN_STATUS EFIAPI SerialPortWrite (UINT8 *, UINTN);
    CONST CHAR8 Hb[] = "0123456789abcdef";
    UINT8  B0 = MmioRead8 (DWC3_BASE + 0x00);
    UINT8  B1 = MmioRead8 (DWC3_BASE + 0x01);
    UINT16 H02 = MmioRead16 (DWC3_BASE + 0x02);
    UINT32 HCSP1 = MmioRead32 (DWC3_BASE + 0x04);
    UINT32 HCCP = MmioRead32 (DWC3_BASE + 0x10);
    CHAR8  Mx[64];
    UINTN  K = 0;
    Mx[K++]='[';
    Mx[K++]='B'; Mx[K++]='0'; Mx[K++]='=';
    Mx[K++]=Hb[(B0>>4)&0xF]; Mx[K++]=Hb[B0&0xF]; Mx[K++]=' ';
    Mx[K++]='H'; Mx[K++]='0'; Mx[K++]='2'; Mx[K++]='=';
    Mx[K++]=Hb[(H02>>12)&0xF]; Mx[K++]=Hb[(H02>>8)&0xF];
    Mx[K++]=Hb[(H02>>4)&0xF]; Mx[K++]=Hb[H02&0xF]; Mx[K++]=' ';
    Mx[K++]='H'; Mx[K++]='C'; Mx[K++]='=';
    Mx[K++]=Hb[(HCCP>>28)&0xF]; Mx[K++]=Hb[(HCCP>>24)&0xF];
    Mx[K++]=Hb[(HCCP>>20)&0xF]; Mx[K++]=Hb[(HCCP>>16)&0xF];
    Mx[K++]=Hb[(HCCP>>12)&0xF]; Mx[K++]=Hb[(HCCP>>8)&0xF];
    Mx[K++]=Hb[(HCCP>>4)&0xF]; Mx[K++]=Hb[HCCP&0xF]; Mx[K++]=' ';
    Mx[K++]='H'; Mx[K++]='C'; Mx[K++]='S'; Mx[K++]='1'; Mx[K++]='=';
    Mx[K++]=Hb[(HCSP1>>28)&0xF]; Mx[K++]=Hb[(HCSP1>>24)&0xF];
    Mx[K++]=Hb[(HCSP1>>20)&0xF]; Mx[K++]=Hb[(HCSP1>>16)&0xF];
    Mx[K++]=Hb[(HCSP1>>12)&0xF]; Mx[K++]=Hb[(HCSP1>>8)&0xF];
    Mx[K++]=Hb[(HCSP1>>4)&0xF]; Mx[K++]=Hb[HCSP1&0xF];
    Mx[K++]=0x0D; Mx[K++]=0x0A;
    SerialPortWrite ((UINT8 *)Mx, K);
  }

  /* 8z. 扫描所有 PortSC 候选偏移（基于 CapLength=0x20） */
  {
    extern RETURN_STATUS EFIAPI SerialPortWrite (UINT8 *, UINTN);
    CONST CHAR8 Hs[] = "0123456789abcdef";
    /* 可能的 PortSC 偏移：CapReg 0x00 处的候选 + OpReg 基准候选 */
    UINT32 Offs[] = {
      0x20, 0x24, 0x28, 0x38, 0x3C,           /* 若 OpReg=base+0 附近 */
      0x420, 0x424, 0x438, 0x43C,             /* 若 OpReg=base+0x20 */
      0x800, 0x804, 0x838,                   /* 若 PortSC=base+0x800 */
      0x820, 0x824, 0x838, 0x83C,
      0x440, 0x444, 0x458, 0x45C,
      0x1800, 0x1810, 0x1820,
      0x4000, 0x4004, 0x4038, 0x4438,
      0x4400, 0x4420, 0x4430,
    };
    UINTN N = sizeof(Offs)/sizeof(Offs[0]);
    UINTN I;
    for (I = 0; I < N; I++) {
      UINT32 V = MmioRead32 (DWC3_BASE + Offs[I]);
      CHAR8  Ln[32];
      UINTN  K = 0;
      Ln[K++]='['; Ln[K++]='+';
      Ln[K++]=Hs[(Offs[I]>>12)&0xF]; Ln[K++]=Hs[(Offs[I]>>8)&0xF];
      Ln[K++]=Hs[(Offs[I]>>4)&0xF]; Ln[K++]=Hs[Offs[I]&0xF];
      Ln[K++]='=';
      Ln[K++]=Hs[(V>>28)&0xF]; Ln[K++]=Hs[(V>>24)&0xF];
      Ln[K++]=Hs[(V>>20)&0xF]; Ln[K++]=Hs[(V>>16)&0xF];
      Ln[K++]=Hs[(V>>12)&0xF]; Ln[K++]=Hs[(V>>8)&0xF];
      Ln[K++]=Hs[(V>>4)&0xF]; Ln[K++]=Hs[V&0xF];
      Ln[K++]=0x0D; Ln[K++]=0x0A;
      SerialPortWrite ((UINT8 *)Ln, K);
    }
  }

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
