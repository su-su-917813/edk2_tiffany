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
/* #include <Library/NonDiscoverableDeviceRegistrationLib.h> */  /* Device mode: not needed */
#include <Protocol/NonDiscoverableDevice.h>


#define DWC3_BASE                 0x07000000
#define DWC3_SIZE                 0x100000  
#define DWC3_GCTL_OFFSET          0xC110
#define DWC3_GCTL_PRTCAPDIR_MASK  0x00003000
#define DWC3_GCTL_PRTCAP_HOST     0x00001000


#define QSCRATCH_BASE             0x070F8800
#define QSCRATCH_GENERAL_CFG      (QSCRATCH_BASE + 0x08)
#define QSCRATCH_RAM1_REG         (QSCRATCH_BASE + 0x0C)
#define QSCRATCH_HS_PHY_CTRL_COMMON (QSCRATCH_BASE + 0xEC)


#define GENCFG_PIPE_UTMI_CLK_SEL_SHFT   0
#define GENCFG_DBM_EN_SHFT              1
#define GENCFG_PIPE3_PHYSTATUS_SW_SHFT  3
#define GENCFG_PIPE_UTMI_CLK_DIS_SHFT   8


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

/* === DWC3 全局寄存器（偏移以 DWC3_BASE=0x07000000 为基址）=== */
#define DWC3_GCTL           0xC110   /* 全局控制：模式切换 */
#define DWC3_GSNPSID        0xC120   /* 控制器 ID */
#define DWC3_GUSB2PHYCFG    0xC200   /* USB2 PHY 配置 */

/* === DWC3 设备模式寄存器 === */
#define DWC3_DCFG           0xC700   /* 设备配置 */
#define DWC3_DCTL           0xC704   /* 设备控制 */
#define DWC3_DEVTEN         0xC708   /* 设备事件使能 */
#define DWC3_DSTS           0xC70C   /* 设备状态 */
#define DWC3_DGCMDPAR0      0xC710   /* 全局命令参数0 */
#define DWC3_DGCMDPAR1      0xC714   /* 全局命令参数1 */
#define DWC3_DGCMDPAR2      0xC718   /* 全局命令参数2 */
#define DWC3_DGCMD          0xC71C   /* 全局命令 */

/* === 事件缓冲区 === */
#define DWC3_GEVNTADRLO     0xC400
#define DWC3_GEVNTADRHI     0xC404
#define DWC3_GEVNTSIZ       0xC408
#define DWC3_GEVNTCOUNT     0xC40C

/* === 端点 FIFO 大小 === */
#define DWC3_GTXFIFOSIZ(n)  (0xC300 + (n) * 0x04)
#define DWC3_GRXFIFOSIZ(n)  (0xC380 + (n) * 0x04)

/* === 端点命令寄存器（物理端点 n：0~31）=== */
#define DWC3_DEPCMDPAR0(n)  (0xC714 + (n) * 0x10)
#define DWC3_DEPCMDPAR1(n)  (0xC718 + (n) * 0x10)
#define DWC3_DEPCMDPAR2(n)  (0xC71C + (n) * 0x10)
#define DWC3_DEPCMD(n)      (0xC720 + (n) * 0x10)

/* === GCTL 位定义 === */
#define DWC3_GCTL_PRTCAPDIR_DEVICE   0x00000000
#define DWC3_GCTL_PRTCAPDIR_HOST     0x00001000
#define DWC3_GCTL_CORESOFTRESET      (1u << 11)

/* === DCFG 位定义 === */
#define DWC3_DCFG_DEVSPEED_HS        0x0
#define DWC3_DCFG_DEVSPEED_FS        0x1
#define DWC3_DCFG_DEVSPEED_SS        0x4
#define DWC3_DCFG_DEVADDR(a)         ((a) << 3)

/* === DCTL 位定义 === */
#define DWC3_DCTL_RUN_STOP           (1u << 31)
#define DWC3_DCTL_CSFTRST            (1u << 30)
#define DWC3_DCTL_KEEP_CONNECT       (1u << 19)

/* === DEPCMD 命令类型 === */
#define DWC3_DEPCMD_DEPCFG           0x1
#define DWC3_DEPCMD_DEPXFER          0x2
#define DWC3_DEPCMD_DEPSTRT          0x7

/* === DEPCFG param0 字段 === */
#define DWC3_DEPCFG_ACTION_INIT      (0 << 30)
#define DWC3_DEPCFG_ACTION_MODIFY    (2 << 30)

/* === DEPCFG param1 字段 === */
#define DWC3_DEPCFG_EPTYPE_CONTROL   (0 << 0)
#define DWC3_DEPCFG_EPTYPE_ISOC      (1 << 0)
#define DWC3_DEPCFG_EPTYPE_BULK      (2 << 0)
#define DWC3_DEPCFG_EPTYPE_INT       (3 << 0)
#define DWC3_DEPCFG_MAXPKT(n)        (((n) & 0x7FF) << 3)
#define DWC3_DEPCFG_FIFO(n)          (((n) & 0xFF) << 17)

/* === DEPCFG param2 字段 === */
#define DWC3_DEPCFG_XFER_COMPLETE_EN (1 << 0)
#define DWC3_DEPCFG_XFER_IN_PROGRESS_EN (1 << 1)
#define DWC3_DEPCFG_XFER_NOT_READY_EN   (1 << 2)
#define DWC3_DEPCFG_EP_NUMBER(n)     (((n) & 0x1F) << 25)
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

  /* 强制使能 REFCLK_EN (bit2)，否则 PHY 无参考时钟 */
  V = MmioRead32 (QSCRATCH_GENERAL_CFG);
  V |= (1u << 2);
  MmioWrite32 (QSCRATCH_GENERAL_CFG, V);
  gBS->Stall (10000);
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

  /* 6c. QSCRATCH 寄存器快照 */
  {
    extern RETURN_STATUS EFIAPI SerialPortWrite (UINT8 *, UINTN);
    CONST CHAR8 Hs[] = "0123456789abcdef";
    UINT32 QRegs[] = { 0x070F8800, 0x070F8808, 0x070F88EC };
    UINTN  I;
    for (I = 0; I < 3; I++) {
      UINT32 V = MmioRead32 (QRegs[I]);
      CHAR8  Ln[20];
      UINTN  K = 0;
      Ln[K++]='['; Ln[K++]='R';
      Ln[K++]=Hs[(QRegs[I]>>4)&0xF]; Ln[K++]=Hs[QRegs[I]&0xF];
      Ln[K++]='=';
      Ln[K++]=Hs[(V>>28)&0xF]; Ln[K++]=Hs[(V>>24)&0xF];
      Ln[K++]=Hs[(V>>20)&0xF]; Ln[K++]=Hs[(V>>16)&0xF];
      Ln[K++]=Hs[(V>>12)&0xF]; Ln[K++]=Hs[(V>>8)&0xF];
      Ln[K++]=Hs[(V>>4)&0xF]; Ln[K++]=Hs[V&0xF];
      Ln[K++]=0x0D; Ln[K++]=0x0A;
      SerialPortWrite ((UINT8 *)Ln, K);
    }
  }

  /* 6d. QUSB2 PHY 其余寄存器快照 */
  {
    extern RETURN_STATUS EFIAPI SerialPortWrite (UINT8 *, UINTN);
    CONST CHAR8 Hs[] = "0123456789abcdef";
    UINT32 PRegs[] = { 0x00, 0x04, 0x08, 0x10, 0x14, 0x3C, 0x40, 0x44, 0x8C, 0x90, 0x94, 0xB8 };
    UINTN  I;
    for (I = 0; I < sizeof(PRegs)/sizeof(PRegs[0]); I++) {
      UINT32 V = MmioRead32 (0x00079000 + PRegs[I]);
      CHAR8  Ln[20];
      UINTN  K = 0;
      Ln[K++]='['; Ln[K++]='X';
      Ln[K++]=Hs[(PRegs[I]>>4)&0xF]; Ln[K++]=Hs[PRegs[I]&0xF];
      Ln[K++]='=';
      Ln[K++]=Hs[(V>>28)&0xF]; Ln[K++]=Hs[(V>>24)&0xF];
      Ln[K++]=Hs[(V>>20)&0xF]; Ln[K++]=Hs[(V>>16)&0xF];
      Ln[K++]=Hs[(V>>12)&0xF]; Ln[K++]=Hs[(V>>8)&0xF];
      Ln[K++]=Hs[(V>>4)&0xF]; Ln[K++]=Hs[V&0xF];
      Ln[K++]=0x0D; Ln[K++]=0x0A;
      SerialPortWrite ((UINT8 *)Ln, K);
    }
  }

  /* 6c. 释放 QSCRATCH PHY 复位 + 使能参考时钟 */
  {
    UINT32 V;

    /* QSCRATCH_PHY_CFG (0x070F8800): 释放 PHY 软复位 */
    V = MmioRead32 (0x070F8800);
    V |= 0x1;   /* PHY_SW_RESET = 1（释放） */
    MmioWrite32 (0x070F8800, V);
    Udelay (10000);

    /* QSCRATCH_GENERAL_CFG: 保留已配置的位，只置位 REFCLK_EN */
    V = MmioRead32 (0x070F8808);
    V |= (1u << 2);   /* REFCLK_EN */
    MmioWrite32 (0x070F8808, V);
    Udelay (10000);
  }

  /* 6d. QSCRATCH + QUSB2 PHY 寄存器快照（诊断用） */
  {
    extern RETURN_STATUS EFIAPI SerialPortWrite (UINT8 *, UINTN);
    CONST CHAR8 Hs[] = "0123456789abcdef";
    UINT32 QRegs[] = { 0x070F8800, 0x070F8808, 0x070F88EC };
    UINT32 PRegs[] = { 0x00, 0x04, 0x08, 0x10, 0x14, 0x3C, 0x40, 0x44, 0x8C, 0x90, 0x94, 0xB8 };
    UINTN  I;
    for (I = 0; I < sizeof(QRegs)/sizeof(QRegs[0]); I++) {
      UINT32 V = MmioRead32 (QRegs[I]);
      CHAR8  Ln[20]; UINTN K = 0;
      Ln[K++]='['; Ln[K++]='R';
      Ln[K++]=Hs[(QRegs[I]>>4)&0xF]; Ln[K++]=Hs[QRegs[I]&0xF];
      Ln[K++]='=';
      Ln[K++]=Hs[(V>>28)&0xF]; Ln[K++]=Hs[(V>>24)&0xF];
      Ln[K++]=Hs[(V>>20)&0xF]; Ln[K++]=Hs[(V>>16)&0xF];
      Ln[K++]=Hs[(V>>12)&0xF]; Ln[K++]=Hs[(V>>8)&0xF];
      Ln[K++]=Hs[(V>>4)&0xF];  Ln[K++]=Hs[V&0xF];
      Ln[K++]=0x0D; Ln[K++]=0x0A;
      SerialPortWrite ((UINT8 *)Ln, K);
    }
    for (I = 0; I < sizeof(PRegs)/sizeof(PRegs[0]); I++) {
      UINT32 V = MmioRead32 (0x00079000 + PRegs[I]);
      CHAR8  Ln[20]; UINTN K = 0;
      Ln[K++]='['; Ln[K++]='X';
      Ln[K++]=Hs[(PRegs[I]>>4)&0xF]; Ln[K++]=Hs[PRegs[I]&0xF];
      Ln[K++]='=';
      Ln[K++]=Hs[(V>>28)&0xF]; Ln[K++]=Hs[(V>>24)&0xF];
      Ln[K++]=Hs[(V>>20)&0xF]; Ln[K++]=Hs[(V>>16)&0xF];
      Ln[K++]=Hs[(V>>12)&0xF]; Ln[K++]=Hs[(V>>8)&0xF];
      Ln[K++]=Hs[(V>>4)&0xF];  Ln[K++]=Hs[V&0xF];
      Ln[K++]=0x0D; Ln[K++]=0x0A;
      SerialPortWrite ((UINT8 *)Ln, K);
    }
  }

  /* ============================================================
   * 7. 切换到 Device 模式（GCTL.PRTCAPDIR = 00）
   * ============================================================ */
  {
    UINT32 V = MmioRead32 (DWC3_BASE + DWC3_GCTL);
    V &= ~(0x3u << 12);        /* PRTCAPDIR = 00 = Device */
    MmioWrite32 (DWC3_BASE + DWC3_GCTL, V);
    Udelay (10000);
  }

  /* ============================================================
   * 8. DWC3 核心软复位
   * ============================================================ */
  {
    UINT32 V, Retry;
    V = MmioRead32 (DWC3_BASE + DWC3_GCTL);
    V |= DWC3_GCTL_CORESOFTRESET;
    MmioWrite32 (DWC3_BASE + DWC3_GCTL, V);
    Udelay (200000);

    V &= ~DWC3_GCTL_CORESOFTRESET;
    MmioWrite32 (DWC3_BASE + DWC3_GCTL, V);
    Udelay (200000);

    Retry = 100;
    while ((MmioRead32 (DWC3_BASE + DWC3_GSNPSID) == 0) && (Retry-- > 0)) {
      Udelay (10000);
    }
  }

  /* ============================================================
   * 9. GCTL 最终配置（Device 模式）
   * ============================================================ */
  {
    UINT32 V  = 0;
    V |= (1u << 0);       /* DSBLCLKGTNG */
    V |= (1u << 2);       /* SOFITPSYNC */
    V |= (1u << 3);       /* DISSCRAMBLE */
    V |= (1u << 4);       /* SCALEDOWN[1:0] = 01 */
    V &= ~(0x3u << 12);   /* PRTCAPDIR = 00 = Device */
    MmioWrite32 (DWC3_BASE + DWC3_GCTL, V);
    Udelay (10000);
    After = MmioRead32 (DWC3_BASE + DWC3_GCTL);
  }

  /* ============================================================
   * 10. 配置端点 FIFO 大小
   *     TX FIFO0(EP0)=64, TX FIFO1(EP1 IN)=512, TX FIFO2(EP2 OUT)=512
   *     RX FIFO0 = 1024
   * ============================================================ */
  MmioWrite32 (DWC3_BASE + DWC3_GTXFIFOSIZ (0), 0x00000040);
  MmioWrite32 (DWC3_BASE + DWC3_GTXFIFOSIZ (1), 0x00400080);
  MmioWrite32 (DWC3_BASE + DWC3_GTXFIFOSIZ (2), 0x00800100);
  MmioWrite32 (DWC3_BASE + DWC3_GRXFIFOSIZ (0), 0x01800000);
  Udelay (10000);

  /* ============================================================
   * 11. 分配事件缓冲区
   * ============================================================ */
  {
    EFI_PHYSICAL_ADDRESS  EventBufPhys = 0;
    UINTN                 EventBufPages = 1;   /* 4KB */

    Status = gBS->AllocatePages (
                    AllocateAnyPages,
                    EfiBootServicesData,
                    EventBufPages,
                    &EventBufPhys
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    gBS->SetMem ((VOID *)(UINTN)EventBufPhys, EventBufPages * 4096, 0);

    MmioWrite32 (DWC3_BASE + DWC3_GEVNTADRLO, (UINT32)(EventBufPhys & 0xFFFFFFFF));
    MmioWrite32 (DWC3_BASE + DWC3_GEVNTADRHI, (UINT32)((EventBufPhys >> 32) & 0xFFFFFFFF));
    MmioWrite32 (DWC3_BASE + DWC3_GEVNTSIZ,   (UINT32)(EventBufPages * 4096));
    Udelay (10000);
  }

  /* ============================================================
   * 12. DCFG：高速 + 设备地址 0
   * ============================================================ */
  {
    UINT32 V = 0;
    V |= DWC3_DCFG_DEVSPEED_HS;
    V |= DWC3_DCFG_DEVADDR (0);
    MmioWrite32 (DWC3_BASE + DWC3_DCFG, V);
    Udelay (10000);
  }

  /* ============================================================
   * 13. PHY 配置 + DCTL 启动
   * ============================================================ */
  {
    UINT32 V, Retry;


    /* 13.2 KEEP_CONNECT */
    V = MmioRead32 (DWC3_BASE + DWC3_DCTL);
    V |= DWC3_DCTL_KEEP_CONNECT;
    MmioWrite32 (DWC3_BASE + DWC3_DCTL, V);
    Udelay (10000);

    /* 13.3 CSFTRST */
    V = MmioRead32 (DWC3_BASE + DWC3_DCTL);
    V |= DWC3_DCTL_CSFTRST;
    MmioWrite32 (DWC3_BASE + DWC3_DCTL, V);
    Udelay (10000);

    /* 13.4 等 CSFTRST 清零 */
    Retry = 100;
    while (((MmioRead32 (DWC3_BASE + DWC3_DCTL) & DWC3_DCTL_CSFTRST) != 0) && (Retry-- > 0)) {
      Udelay (10000);
    }

    /* 13.4a 重新初始化 QUSB2 PHY（CSFTRST 会复位 PHY） */
    /* PHY Power-on */
    MmioWrite32 (0x00079000 + 0x18, 0x00);   /* PLL_PWR_CTL */
    Udelay (10000);
    V = MmioRead32 (0x00079000 + 0xB4);
    MmioWrite32 (0x00079000 + 0xB4, V & ~0x1);  /* 释放 Powerdown */
    Udelay (10000);

    /* PHY PLL 配置（从 Host 模式复用） */
    MmioWrite32 (0x00079000 + 0x0C, 0x79);   /* PLL_USER_CTL1 */
    MmioWrite32 (0x00079000 + 0x1C, 0x9F);   /* PLL_AUTOPGM_CTL1 */
    MmioWrite32 (0x00079000 + 0x80, 0xF8);   /* PORT_TUNE1 */
    MmioWrite32 (0x00079000 + 0x84, 0xB3);   /* PORT_TUNE2 */
    MmioWrite32 (0x00079000 + 0x88, 0x83);   /* PORT_TUNE3 */
    MmioWrite32 (0x00079000 + 0x9C, 0x14);   /* PORT_TEST2 */
    Udelay (10000);

    /* 等 PLL 锁定（最多 500ms） */
    Retry = 500;
    while (((MmioRead32 (0x00079000 + 0x38) & 0x20) == 0) && (Retry-- > 0)) {
      Udelay (1000);
    }

    /* 13.4b CSFTRST 之后配置 GUSB2PHYCFG */
    V = MmioRead32 (DWC3_BASE + DWC3_GUSB2PHYCFG);
    V |= (1u << 31);                        /* PHYSOFTRST */
    MmioWrite32 (DWC3_BASE + DWC3_GUSB2PHYCFG, V);
    Udelay (10000);

    V = MmioRead32 (DWC3_BASE + DWC3_GUSB2PHYCFG);
    V &= ~(1u << 31);                       /* 解除 PHYSOFTRST */
    V &= ~((1u << 4) | (1u << 6) | (1u << 8));
    V |= (1u << 0);                         /* VBUSVLDEXT = 1 */
    MmioWrite32 (DWC3_BASE + DWC3_GUSB2PHYCFG, V);
    Udelay (10000);


    /* 13.5 等 DSTS.Connected，最多 500ms */
    Retry = 500;
    while (((MmioRead32 (DWC3_BASE + DWC3_DSTS) & 0x1) == 0) && (Retry-- > 0)) {
      Udelay (1000);
    }

    /* 13.6 使能 RUN_STOP + KEEP_CONNECT + U2 电源管理 */
    V = MmioRead32 (DWC3_BASE + DWC3_DCTL);
    V |= DWC3_DCTL_RUN_STOP;
    V |= DWC3_DCTL_KEEP_CONNECT;
    V |= (1u << 12);   /* INITU2ENA */
    V |= (1u << 11);   /* ACCEPTU2ENA */
    MmioWrite32 (DWC3_BASE + DWC3_DCTL, V);
    Udelay (10000);

    /* 13.7 打印 GUSB2PHYCFG 和 DSTS */
    {
      CONST CHAR8 Hd[] = "0123456789abcdef";
      UINT32 Regs[] = { DWC3_GUSB2PHYCFG, DWC3_DSTS };
      UINTN i;
      for (i = 0; i < 2; i++) {
        UINT32 Rv = MmioRead32 (DWC3_BASE + Regs[i]);
        CHAR8 Ln[20]; UINTN K = 0;
        Ln[K++]='['; Ln[K++]='P'; Ln[K++]='H'; Ln[K++]='Y'; Ln[K++]='=';
        Ln[K++]=Hd[(Rv>>28)&0xF]; Ln[K++]=Hd[(Rv>>24)&0xF];
        Ln[K++]=Hd[(Rv>>20)&0xF]; Ln[K++]=Hd[(Rv>>16)&0xF];
        Ln[K++]=Hd[(Rv>>12)&0xF]; Ln[K++]=Hd[(Rv>>8)&0xF];
        Ln[K++]=Hd[(Rv>>4)&0xF];  Ln[K++]=Hd[Rv&0xF];
        Ln[K++]=0x0D; Ln[K++]=0x0A;
        SerialPortWrite ((UINT8 *)Ln, K);
      }
    }
  }

  /* ============================================================
   * 14. 配置 EP0（控制端点）
   * ============================================================ */
  {
    UINT32 Param0, Param1, Param2, Retry;

    Param0  = DWC3_DEPCFG_ACTION_INIT;
    Param0 |= 0;                                /* EP number = 0 */

    Param1  = DWC3_DEPCFG_EPTYPE_CONTROL;
    Param1 |= DWC3_DEPCFG_MAXPKT (64);
    Param1 |= DWC3_DEPCFG_FIFO (0);

    Param2  = DWC3_DEPCFG_XFER_COMPLETE_EN;
    Param2 |= DWC3_DEPCFG_XFER_IN_PROGRESS_EN;
    Param2 |= DWC3_DEPCFG_XFER_NOT_READY_EN;

    MmioWrite32 (DWC3_BASE + DWC3_DEPCMDPAR0 (0), Param0);
    MmioWrite32 (DWC3_BASE + DWC3_DEPCMDPAR1 (0), Param1);
    MmioWrite32 (DWC3_BASE + DWC3_DEPCMDPAR2 (0), Param2);
    MmioWrite32 (DWC3_BASE + DWC3_DEPCMD (0), DWC3_DEPCMD_DEPCFG);
    Udelay (10000);

    Retry = 100;
    while (((MmioRead32 (DWC3_BASE + DWC3_DEPCMD (0)) & 0xC00) != 0) && (Retry-- > 0)) {
      Udelay (1000);
    }
  }

  /* ============================================================
   * 15. 启动 EP0（DEPSTRT）
   * ============================================================ */
  {
    MmioWrite32 (DWC3_BASE + DWC3_DEPCMDPAR0 (0), 0);
    MmioWrite32 (DWC3_BASE + DWC3_DEPCMDPAR1 (0), 0);
    MmioWrite32 (DWC3_BASE + DWC3_DEPCMDPAR2 (0), 0);
    MmioWrite32 (DWC3_BASE + DWC3_DEPCMD (0), DWC3_DEPCMD_DEPSTRT);
    Udelay (10000);
  }

  /* ============================================================
   * 16. 使能设备事件
   * ============================================================ */
  {
    UINT32 V  = 0;
    V |= (1u << 0);    /* DisconnEvtEn */
    V |= (1u << 1);    /* USBRstEvtEn */
    V |= (1u << 2);    /* ConnectDoneEvtEn */
    V |= (1u << 3);    /* ULStEvtEn */
    V |= (1u << 4);    /* WkUpEvtEn */
    V |= (1u << 5);    /* ErTyEvtEn */
    MmioWrite32 (DWC3_BASE + DWC3_DEVTEN, V);
  }

  /* ============================================================
   * 17. 打印关键寄存器（GCTL / DCFG / DCTL / DSTS）
   * ============================================================ */
  {
    UINT32 Regs[] = { DWC3_GCTL, DWC3_DCFG, DWC3_DCTL, DWC3_DSTS };
    UINTN  I;
    for (I = 0; I < 4; I++) {
      UINT32 Rv = MmioRead32 (DWC3_BASE + Regs[I]);
      UINTN  K = 0;
      CHAR8  Ln[20];
      Ln[K++]='['; Ln[K++]='D';
      Ln[K++]=Hex[(Regs[I]>>4)&0xF]; Ln[K++]=Hex[Regs[I]&0xF];
      Ln[K++]='=';
      Ln[K++]=Hex[(Rv>>28)&0xF]; Ln[K++]=Hex[(Rv>>24)&0xF];
      Ln[K++]=Hex[(Rv>>20)&0xF]; Ln[K++]=Hex[(Rv>>16)&0xF];
      Ln[K++]=Hex[(Rv>>12)&0xF]; Ln[K++]=Hex[(Rv>>8)&0xF];
      Ln[K++]=Hex[(Rv>>4)&0xF];  Ln[K++]=Hex[Rv&0xF];
      Ln[K++]=0x0D; Ln[K++]=0x0A;
      SerialPortWrite ((UINT8 *)Ln, K);
    }
  }

  return Status;
}
