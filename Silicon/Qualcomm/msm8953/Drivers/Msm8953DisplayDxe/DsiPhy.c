// DsiPhy.c - MSM8953 DSI PHY + PLL initialization
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include "DisplayRegs.h"

#define REG_WR(addr, val)  MmioWrite32((UINTN)(addr), (UINT32)(val))
#define REG_RD(addr)       MmioRead32((UINTN)(addr))
VOID
Mark (
  IN UINT32 Idx,
  IN UINT32 Color
  )
{
  volatile UINT32 *Fb = (volatile UINT32 *)0x90000000;
  UINT32 x, y;
  /* 80 像素宽、全屏高的竖条 */
  for (y = 0; y < 1920; y++) {
    for (x = 0; x < 80; x++) {
      Fb[y * 1080 + (Idx * 80 + x)] = Color;
    }
  }
}


// PHY calibration data from lk2nd target/msm8953/include/target/display.h
STATIC CONST UINT32 mStrengthCtrl[] = {
  0xFF, 0x06, 0xFF, 0x06, 0xFF, 0x06, 0xFF, 0x06, 0xFF, 0x00
};

STATIC CONST UINT32 mRegulatorSettings[] = {
  0x1d, 0x1d, 0x1d, 0x1d, 0x1d
};

STATIC CONST UINT8 mLaneConfig[] = {
  0x00, 0x00, 0x10, 0x0f,
  0x00, 0x00, 0x10, 0x0f,
  0x00, 0x00, 0x10, 0x0f,
  0x00, 0x00, 0x10, 0x0f,
  0x00, 0x00, 0x10, 0x8f,
};

// PHY timing from panel DTS (t-clk-pre=0x36, t-clk-post=0x0e)
// These are 8 values per lane
STATIC CONST UINT32 mPhyTiming[8] = {
  0x3F, 0x7F, 0x2A, 0x04, 0x00, 0x20, 0x3F, 0x0A
};

// PLL configuration structure (simplified from lk2nd dsi_pll_db)
typedef struct {
  UINT32 fref;
  UINT32 dsiclk_sel;
  UINT32 ssc_en;
  UINT32 ldo_en;
  UINT32 refclk_dbler_en;
  UINT32 vco_measure_time;
  UINT32 kvco_measure_time;
  UINT32 bandgap_timer;
  UINT32 pll_wakeup_timer;
  UINT32 plllock_cnt;
  UINT32 plllock_rng;
  UINT32 ssc_center_spread;
  UINT32 ssc_adj_per;
  UINT32 ssc_spread;
  UINT32 ssc_freq;
  UINT32 pll_ie_trim;
  UINT32 pll_ip_trim;
  UINT32 pll_iptat_trim;
  UINT32 pll_cpcset_cur;
  UINT32 pll_cpmset_cur;
  UINT32 pll_icpmset;
  UINT32 pll_icpcset;
  UINT32 pll_icpmset_p;
  UINT32 pll_icpmset_m;
  UINT32 pll_icpcset_p;
  UINT32 pll_icpcset_m;
  UINT32 pll_lpf_res1;
  UINT32 pll_lpf_cap1;
  UINT32 pll_lpf_cap2;
  UINT32 pll_c3ctrl;
  UINT32 pll_r3ctrl;
} DSI_PLL_INPUT;

typedef struct {
  UINT32 pll_txclk_en;
  UINT32 dec_start;
  UINT32 div_frac_start;
  UINT32 ssc_per;
  UINT32 ssc_step_size;
  UINT32 plllock_cmp;
  UINT32 pll_vco_div_ref;
  UINT32 pll_vco_count;
  UINT32 pll_kvco_div_ref;
  UINT32 pll_kvco_count;
  UINT32 pll_misc1;
  UINT32 pll_resetsm_cntrl;
  UINT32 pll_resetsm_cntrl2;
  UINT32 pll_resetsm_cntrl5;
  UINT32 pll_kvco_code;
  UINT32 cmn_ldo_cntrl;
  UINT32 pll_postdiv;
  UINT32 pll_n1div;
  UINT32 pll_n2div;
} DSI_PLL_OUTPUT;

typedef struct {
  DSI_PLL_INPUT  In;
  DSI_PLL_OUTPUT Out;
} DSI_PLL_DB;

#define CEIL(x, y)  (((x) + ((y) - 1)) / (y))

STATIC
VOID
DsiPhySwReset (
  IN UINTN CtlBase
  )
{
  REG_WR(CtlBase + 0x012C, 0x0001);
  MicroSecondDelay(1000);
  REG_WR(CtlBase + 0x012C, 0x0000);
  MicroSecondDelay(100);
}

STATIC
VOID
DsiPhy14nmInit (
  IN UINTN PhyBase
  )
{
  UINT32 i, ln;
  UINTN  LaneBase;

  // Strength ctrl
  REG_WR(PhyBase + DSIPHY_CMN_LDO_CNTRL, 0x1c);
  REG_WR(PhyBase + DSIPHY_CMN_GLBL_TEST_CTRL, 0x1);

  // Configure 5 lanes (4 data + 1 clk)
  for (ln = 0; ln < 5; ln++) {
    LaneBase = PhyBase + DSIPHY_LANE_BASE + (ln * DSIPHY_LANE_SIZE);

    // Lane configuration: 4 registers
    for (i = 0; i < 4; i++) {
      REG_WR(LaneBase + (i * 4), mLaneConfig[ln * 4 + i]);
    }

    // Test strength
    REG_WR(LaneBase + 0x14, 0x88);

    // PHY timing: 8 registers
    for (i = 0; i < 8; i++) {
      REG_WR(LaneBase + 0x18 + (i * 4), mPhyTiming[i]);
    }

    // Strength: 2 registers
    REG_WR(LaneBase + 0x38, mStrengthCtrl[ln * 2]);
    REG_WR(LaneBase + 0x3C, mStrengthCtrl[ln * 2 + 1]);

    // Regulator
    REG_WR(LaneBase + 0x64, mRegulatorSettings[ln]);
  }

  // Reset digital block
  REG_WR(PhyBase + DSIPHY_CMN_CTRL_1, 0x80);
  MicroSecondDelay(100);
  REG_WR(PhyBase + DSIPHY_CMN_CTRL_1, 0x00);

  // Clock buffer
  REG_WR(PhyBase + DSIPHY_PLL_CLKBUFLR_EN, 0x1);
  {
    UINT32 data = REG_RD(PhyBase + DSIPHY_CMN_GLBL_TEST_CTRL);
    data &= ~BIT2;
    REG_WR(PhyBase + DSIPHY_CMN_GLBL_TEST_CTRL, data);
  }
}

STATIC
VOID
DsiPllInputInit (
  OUT DSI_PLL_DB *Pdb
  )
{
  ZeroMem(Pdb, sizeof(DSI_PLL_DB));

  Pdb->In.fref = 19200000;
  Pdb->In.dsiclk_sel = 1;
  Pdb->In.ssc_en = 1;
  Pdb->In.ldo_en = 0;
  Pdb->In.refclk_dbler_en = 0;
  Pdb->In.vco_measure_time = 5;
  Pdb->In.kvco_measure_time = 5;
  Pdb->In.bandgap_timer = 4;
  Pdb->In.pll_wakeup_timer = 5;
  Pdb->In.plllock_cnt = 1;
  Pdb->In.plllock_rng = 0;
  Pdb->In.ssc_center_spread = 0;
  Pdb->In.ssc_adj_per = 37;
  Pdb->In.ssc_spread = 5;
  Pdb->In.ssc_freq = 31500;
  Pdb->In.pll_ie_trim = 4;
  Pdb->In.pll_ip_trim = 4;
  Pdb->In.pll_iptat_trim = 7;
  Pdb->In.pll_cpcset_cur = 1;
  Pdb->In.pll_cpmset_cur = 1;
  Pdb->In.pll_icpmset = 7;
  Pdb->In.pll_icpcset = 7;
  Pdb->In.pll_icpmset_p = 0;
  Pdb->In.pll_icpmset_m = 0;
  Pdb->In.pll_icpcset_p = 0;
  Pdb->In.pll_icpcset_m = 0;
  Pdb->In.pll_lpf_res1 = 3;
  Pdb->In.pll_lpf_cap1 = 11;
  Pdb->In.pll_lpf_cap2 = 1;
  Pdb->In.pll_c3ctrl = 2;
  Pdb->In.pll_r3ctrl = 1;
}

STATIC
VOID
DsiPllDecFracCalc (
  IN OUT DSI_PLL_DB *Pdb,
  IN UINT32 VcoClkRate,
  IN UINT32 Fref
  )
{
  UINT64 VcoClk;
  UINT64 Multiplier = BIT20;
  UINT64 DecStartMultiple, DecStart, PllCompVal;
  UINT32 Duration, DivFracStart;

  VcoClk = (UINT64)VcoClkRate * Multiplier;
  DecStartMultiple = VcoClk / Fref;
  DivFracStart = (UINT32)(DecStartMultiple % Multiplier);
  DecStart = DecStartMultiple / Multiplier;

  Pdb->Out.dec_start = (UINT32)DecStart;
  Pdb->Out.div_frac_start = DivFracStart;

  if (Pdb->In.plllock_cnt == 0) {
    Duration = 1024;
  } else if (Pdb->In.plllock_cnt == 1) {
    Duration = 256;
  } else if (Pdb->In.plllock_cnt == 2) {
    Duration = 128;
  } else {
    Duration = 32;
  }

  PllCompVal = (UINT64)Duration * DecStartMultiple;
  PllCompVal /= (Multiplier * 10);
  Pdb->Out.plllock_cmp = (UINT32)PllCompVal;

  Pdb->Out.pll_txclk_en = 1;
  Pdb->Out.cmn_ldo_cntrl = 0x3c;
}

STATIC
VOID
DsiPllSscCalc (
  IN OUT DSI_PLL_DB *Pdb,
  IN UINT32 VcoClkRate,
  IN UINT32 Fref
  )
{
  UINT32 Period, SscPeriod;
  UINT32 Ref, Rem;
  UINT64 StepSize;

  SscPeriod = Pdb->In.ssc_freq / 500;
  Period = Fref / 1000;
  SscPeriod = CEIL(Period, SscPeriod);
  SscPeriod -= 1;
  Pdb->Out.ssc_per = SscPeriod;

  StepSize = VcoClkRate;
  Ref = Fref / 1000;
  StepSize /= Ref;
  StepSize <<= 20;
  StepSize /= 1000;
  StepSize *= Pdb->In.ssc_spread;
  StepSize /= 1000;
  StepSize *= (Pdb->In.ssc_adj_per + 1);

  Rem = StepSize % (SscPeriod + 1);
  if (Rem) {
    StepSize++;
  }

  Pdb->Out.ssc_step_size = (UINT32)(StepSize & 0xFFFF);
}

STATIC
UINT32
DsiPllKvcoSlop (
  IN UINT32 Vrate
  )
{
  if (Vrate > 1300000000 && Vrate <= 1800000000) {
    return 600;
  } else if (Vrate > 1800000000 && Vrate < 2300000000) {
    return 400;
  } else if (Vrate > 2300000000 && Vrate < 2600000000) {
    return 280;
  }
  return 0;
}

STATIC
UINT32
DsiPllCalcKvcoCode (
  IN UINT32 VcoClkRate
  )
{
  if (VcoClkRate >= 2300000000ULL && VcoClkRate <= 2600000000ULL) {
    return 0x2f;
  } else if (VcoClkRate >= 1800000000ULL && VcoClkRate < 2300000000ULL) {
    return 0x2c;
  }
  return 0x28;
}

STATIC
VOID
DsiPllCalcVcoCount (
  IN OUT DSI_PLL_DB *Pdb,
  IN UINT32 VcoClkRate,
  IN UINT32 Fref
  )
{
  UINT64 Data;
  UINT32 Cnt;

  Data = Fref * Pdb->In.vco_measure_time;
  Data /= 1000000;
  Data &= 0x03FF;
  Data -= 2;
  Pdb->Out.pll_vco_div_ref = (UINT32)Data;

  Data = VcoClkRate / 1000000;
  Data *= Pdb->In.vco_measure_time;
  Data /= 10;
  Pdb->Out.pll_vco_count = (UINT32)Data;

  Data = Fref * Pdb->In.kvco_measure_time;
  Data /= 1000000;
  Data &= 0x03FF;
  Data -= 1;
  Pdb->Out.pll_kvco_div_ref = (UINT32)Data;

  Cnt = DsiPllKvcoSlop(VcoClkRate);
  Cnt *= 2;
  Cnt /= 100;
  Cnt *= Pdb->In.kvco_measure_time;
  Pdb->Out.pll_kvco_count = Cnt;

  Pdb->Out.pll_misc1 = 16;
  Pdb->Out.pll_resetsm_cntrl = 48;
  Pdb->Out.pll_resetsm_cntrl2 = Pdb->In.bandgap_timer << 3;
  Pdb->Out.pll_resetsm_cntrl5 = Pdb->In.pll_wakeup_timer;
  Pdb->Out.pll_kvco_code = DsiPllCalcKvcoCode(VcoClkRate);
}

STATIC
VOID
DsiPllAssertAndDivCfg (
  IN UINTN PhyBase,
  IN DSI_PLL_DB *Pdb
  )
{
  UINT32 N2div;

  N2div = REG_RD(PhyBase + DSIPHY_CMN_CLK_CFG0);
  N2div &= ~0xF0;
  N2div |= (Pdb->Out.pll_n2div << 4);
  REG_WR(PhyBase + DSIPHY_CMN_CLK_CFG0, N2div);
}

STATIC
VOID
DsiPllNonFreqConfig (
  IN UINTN PhyBase,
  IN DSI_PLL_DB *Pdb
  )
{
  UINT32 Data;

  REG_WR(PhyBase + DSIPHY_CMN_LDO_CNTRL, Pdb->Out.cmn_ldo_cntrl);
  REG_WR(PhyBase + DSIPHY_PLL_SYSCLK_EN_RESET, 0x0);
  REG_WR(PhyBase + DSIPHY_PLL_TXCLK_EN, Pdb->Out.pll_txclk_en);
  REG_WR(PhyBase + DSIPHY_PLL_RESETSM_CNTRL, Pdb->Out.pll_resetsm_cntrl);
  REG_WR(PhyBase + DSIPHY_PLL_RESETSM_CNTRL2, Pdb->Out.pll_resetsm_cntrl2);
  REG_WR(PhyBase + DSIPHY_PLL_RESETSM_CNTRL5, Pdb->Out.pll_resetsm_cntrl5);

  Data = Pdb->Out.pll_vco_div_ref & 0xFF;
  REG_WR(PhyBase + DSIPHY_PLL_VCO_DIV_REF1, Data);
  Data = (Pdb->Out.pll_vco_div_ref >> 8) & 0x03;
  REG_WR(PhyBase + DSIPHY_PLL_VCO_DIV_REF2, Data);

  Data = Pdb->Out.pll_kvco_div_ref & 0xFF;
  REG_WR(PhyBase + DSIPHY_PLL_KVCO_DIV_REF1, Data);
  Data = (Pdb->Out.pll_kvco_div_ref >> 8) & 0x03;
  REG_WR(PhyBase + DSIPHY_PLL_KVCO_DIV_REF2, Data);

  REG_WR(PhyBase + DSIPHY_PLL_PLL_MISC1, Pdb->Out.pll_misc1);
  REG_WR(PhyBase + DSIPHY_PLL_IE_TRIM, Pdb->In.pll_ie_trim);
  REG_WR(PhyBase + DSIPHY_PLL_IP_TRIM, Pdb->In.pll_ip_trim);

  Data = (Pdb->In.pll_cpmset_cur << 3) | Pdb->In.pll_cpcset_cur;
  REG_WR(PhyBase + DSIPHY_PLL_CP_SET_CUR, Data);

  Data = (Pdb->In.pll_icpcset_p << 3) | Pdb->In.pll_icpcset_m;
  REG_WR(PhyBase + DSIPHY_PLL_PLL_ICPCSET, Data);

  Data = (Pdb->In.pll_icpmset_p << 3) | Pdb->In.pll_icpcset_m;
  REG_WR(PhyBase + DSIPHY_PLL_PLL_ICPMSET, Data);

  Data = (Pdb->In.pll_icpmset << 3) | Pdb->In.pll_icpcset;
  REG_WR(PhyBase + DSIPHY_PLL_PLL_ICP_SET, Data);

  Data = (Pdb->In.pll_lpf_cap2 << 4) | Pdb->In.pll_lpf_cap1;
  REG_WR(PhyBase + DSIPHY_PLL_PLL_LPF1, Data);

  REG_WR(PhyBase + DSIPHY_PLL_IPTAT_TRIM, Pdb->In.pll_iptat_trim);

  Data = Pdb->In.pll_c3ctrl | (Pdb->In.pll_r3ctrl << 4);
  REG_WR(PhyBase + DSIPHY_PLL_PLL_CRCTRL, Data);
}

STATIC
VOID
DsiPllFreqConfig (
  IN UINTN PhyBase,
  IN DSI_PLL_DB *Pdb
  )
{
  UINT32 Data;

  REG_WR(PhyBase + DSIPHY_CMN_PLL_CNTRL, 0x0);
  REG_WR(PhyBase + DSIPHY_CMN_CTRL_1, 0x20);
  MicroSecondDelay(10);
  REG_WR(PhyBase + DSIPHY_CMN_CTRL_1, 0x00);

  REG_WR(PhyBase + DSIPHY_CMN_CLK_CFG1, Pdb->In.dsiclk_sel);
  REG_WR(PhyBase + DSIPHY_CMN_CTRL_0, 0xFF);
  REG_WR(PhyBase + DSIPHY_PLL_DEC_START, Pdb->Out.dec_start);

  Data = Pdb->Out.div_frac_start & 0xFF;
  REG_WR(PhyBase + DSIPHY_PLL_DIV_FRAC_START1, Data);
  Data = (Pdb->Out.div_frac_start >> 8) & 0xFF;
  REG_WR(PhyBase + DSIPHY_PLL_DIV_FRAC_START2, Data);
  Data = (Pdb->Out.div_frac_start >> 16) & 0x0F;
  REG_WR(PhyBase + DSIPHY_PLL_DIV_FRAC_START3, Data);

  Data = Pdb->Out.plllock_cmp & 0xFF;
  REG_WR(PhyBase + DSIPHY_PLL_PLLLOCK_CMP1, Data);
  Data = (Pdb->Out.plllock_cmp >> 8) & 0xFF;
  REG_WR(PhyBase + DSIPHY_PLL_PLLLOCK_CMP2, Data);
  Data = (Pdb->Out.plllock_cmp >> 16) & 0x03;
  REG_WR(PhyBase + DSIPHY_PLL_PLLLOCK_CMP3, Data);

  Data = (Pdb->In.plllock_cnt << 1) | (Pdb->In.plllock_rng << 3);
  REG_WR(PhyBase + DSIPHY_PLL_PLLLOCK_CMP_EN, Data);

  Data = Pdb->Out.pll_vco_count & 0xFF;
  REG_WR(PhyBase + DSIPHY_PLL_VCO_COUNT1, Data);
  Data = (Pdb->Out.pll_vco_count >> 8) & 0xFF;
  REG_WR(PhyBase + DSIPHY_PLL_VCO_COUNT2, Data);

  Data = Pdb->Out.pll_kvco_count & 0xFF;
  REG_WR(PhyBase + DSIPHY_PLL_KVCO_COUNT1, Data);
  Data = (Pdb->Out.pll_kvco_count >> 8) & 0x03;
  REG_WR(PhyBase + DSIPHY_PLL_KVCO_COUNT2, Data);

  REG_WR(PhyBase + DSIPHY_PLL_KVCO_CODE, Pdb->Out.pll_kvco_code);

  if (Pdb->Out.pll_postdiv) {
    Data = ((Pdb->Out.pll_postdiv - 1) << 4) | Pdb->In.pll_lpf_res1;
  } else {
    Data = Pdb->In.pll_lpf_res1;
  }
  REG_WR(PhyBase + DSIPHY_PLL_PLL_LPF2_POSTDIV, Data);

  Data = Pdb->Out.pll_n1div | (Pdb->Out.pll_n2div << 4);
  REG_WR(PhyBase + DSIPHY_CMN_CLK_CFG0, Data);
}

STATIC
VOID
DsiPllSscConfig (
  IN UINTN PhyBase,
  IN DSI_PLL_DB *Pdb
  )
{
  UINT32 Data;

  Data = Pdb->In.ssc_adj_per & 0xFF;
  REG_WR(PhyBase + DSIPHY_PLL_SSC_ADJ_PER1, Data);
  Data = (Pdb->In.ssc_adj_per >> 8) & 0x03;
  REG_WR(PhyBase + DSIPHY_PLL_SSC_ADJ_PER2, Data);

  Data = Pdb->Out.ssc_per & 0xFF;
  REG_WR(PhyBase + DSIPHY_PLL_SSC_PER1, Data);
  Data = (Pdb->Out.ssc_per >> 8) & 0xFF;
  REG_WR(PhyBase + DSIPHY_PLL_SSC_PER2, Data);

  Data = Pdb->Out.ssc_step_size & 0xFF;
  REG_WR(PhyBase + DSIPHY_PLL_SSC_STEP_SIZE1, Data);
  Data = (Pdb->Out.ssc_step_size >> 8) & 0xFF;
  REG_WR(PhyBase + DSIPHY_PLL_SSC_STEP_SIZE2, Data);

  Data = (Pdb->In.ssc_center_spread & 0x01);
  Data <<= 1;
  Data |= 0x01;
  REG_WR(PhyBase + DSIPHY_PLL_SSC_EN_CENTER, Data);
}

EFI_STATUS
EFIAPI
DsiPhyInit (
  VOID
  )
{
  UINTN      PhyBase = MSM8953_DSI0_PHY_BASE;
  UINTN      CtlBase = MSM8953_DSI0_CTRL_BASE;
  DSI_PLL_DB Pdb;
  UINT32     St;
  UINTN      i;

  Mark(0,  0x00FF0000);

  DsiPhySwReset(CtlBase);
  Mark(1,  0x0000FF00);

  DsiPhy14nmInit(PhyBase);
  Mark(2,  0x000000FF);

  DsiPllInputInit(&Pdb);
  Mark(3,  0x00FFFF00);

  Pdb.Out.pll_postdiv = 2;
  Pdb.Out.pll_n1div = 7;
  Pdb.Out.pll_n2div = 12;

  DsiPllDecFracCalc(&Pdb, PANEL_VCO_CLOCK, VCO_REF_CLK_RATE);
  Mark(4,  0x0000FFFF);

  if (Pdb.In.ssc_en) {
    DsiPllSscCalc(&Pdb, PANEL_VCO_CLOCK, VCO_REF_CLK_RATE);
  }
  DsiPllCalcVcoCount(&Pdb, PANEL_VCO_CLOCK, VCO_REF_CLK_RATE);
  Mark(5,  0x00FF00FF);

  DsiPllAssertAndDivCfg(PhyBase, &Pdb);
  REG_WR(PhyBase + DSIPHY_CMN_CLK_CFG1, Pdb.In.dsiclk_sel);
  Mark(6,  0x00FFFFFF);

  DsiPllNonFreqConfig(PhyBase, &Pdb);
  Mark(7,  0x00808080);

  DsiPllFreqConfig(PhyBase, &Pdb);
  Mark(8,  0x00800000);

  if (Pdb.In.ssc_en) {
    DsiPllSscConfig(PhyBase, &Pdb);
  }
  Mark(9,  0x00008000);

  REG_WR(PhyBase + 0x045C, 0x10);
  REG_WR(PhyBase + DSIPHY_CMN_PLL_CNTRL, 0x01);
  Mark(10, 0x00000080);

  St = 0;
  for (i = 0; i < 15; i++) {
    St = REG_RD(PhyBase + DSIPHY_PLL_RESET_SM_READY_STATUS);
    if (St & BIT5) break;
    MicroSecondDelay(1000);
  }
  if (St & BIT5) {
    Mark(11, 0x0000FF00);
  } else {
    Mark(11, 0x00FF0000);
  }

  Mark(12, 0x00FFFFFF);

  return EFI_SUCCESS;
}
