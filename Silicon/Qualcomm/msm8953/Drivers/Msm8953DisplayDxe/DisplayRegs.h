// DisplayRegs.h - MSM8953 Display register definitions
#ifndef _MSM8953_DISPLAY_REGS_H_
#define _MSM8953_DISPLAY_REGS_H_

// Base addresses
#define MSM8953_MDP5_BASE        0x01A00000
#define MSM8953_DSI0_CTRL_BASE   0x01A94000
#define MSM8953_DSI0_PHY_BASE    0x01A94400
#define MSM8953_DSI0_PLL_BASE    0x01A94800
#define MSM8953_MMSS_CC_BASE     0x01AB0000
#define MSM8953_GCC_BASE         0x01800000
#define MSM8953_TLMM_BASE        0x01000000

// DSI PHY 14nm (thulium) - Common registers
#define DSIPHY_CMN_CLK_CFG0              0x0010
#define DSIPHY_CMN_CLK_CFG1              0x0014
#define DSIPHY_CMN_GLBL_TEST_CTRL        0x0018
#define DSIPHY_CMN_CTRL_0                0x001C
#define DSIPHY_CMN_CTRL_1                0x0020
#define DSIPHY_CMN_PLL_CNTRL             0x0048
#define DSIPHY_CMN_LDO_CNTRL             0x004C

// DSI PHY PLL registers
#define DSIPHY_PLL_IE_TRIM               0x0400
#define DSIPHY_PLL_IP_TRIM               0x0404
#define DSIPHY_PLL_IPTAT_TRIM            0x0410
#define DSIPHY_PLL_CLKBUFLR_EN           0x041C
#define DSIPHY_PLL_SYSCLK_EN_RESET       0x0428
#define DSIPHY_PLL_RESETSM_CNTRL         0x042C
#define DSIPHY_PLL_RESETSM_CNTRL2        0x0430
#define DSIPHY_PLL_RESETSM_CNTRL5        0x043C
#define DSIPHY_PLL_KVCO_DIV_REF1         0x0440
#define DSIPHY_PLL_KVCO_DIV_REF2         0x0444
#define DSIPHY_PLL_KVCO_COUNT1           0x0448
#define DSIPHY_PLL_KVCO_COUNT2           0x044C
#define DSIPHY_PLL_KVCO_CODE             0x0458
#define DSIPHY_PLL_VCO_DIV_REF1          0x046C
#define DSIPHY_PLL_VCO_DIV_REF2          0x0470
#define DSIPHY_PLL_VCO_COUNT1            0x0474
#define DSIPHY_PLL_VCO_COUNT2            0x0478
#define DSIPHY_PLL_PLLLOCK_CMP1          0x047C
#define DSIPHY_PLL_PLLLOCK_CMP2          0x0480
#define DSIPHY_PLL_PLLLOCK_CMP3          0x0484
#define DSIPHY_PLL_PLLLOCK_CMP_EN        0x0488
#define DSIPHY_PLL_DEC_START             0x0490
#define DSIPHY_PLL_SSC_EN_CENTER         0x0494
#define DSIPHY_PLL_SSC_ADJ_PER1          0x0498
#define DSIPHY_PLL_SSC_ADJ_PER2          0x049C
#define DSIPHY_PLL_SSC_PER1              0x04A0
#define DSIPHY_PLL_SSC_PER2              0x04A4
#define DSIPHY_PLL_SSC_STEP_SIZE1        0x04A8
#define DSIPHY_PLL_SSC_STEP_SIZE2        0x04AC
#define DSIPHY_PLL_DIV_FRAC_START1       0x04B4
#define DSIPHY_PLL_DIV_FRAC_START2       0x04B8
#define DSIPHY_PLL_DIV_FRAC_START3       0x04BC
#define DSIPHY_PLL_TXCLK_EN              0x04C0
#define DSIPHY_PLL_PLL_CRCTRL            0x04C4
#define DSIPHY_PLL_RESET_SM_READY_STATUS 0x04CC
#define DSIPHY_PLL_PLL_MISC1             0x04E8
#define DSIPHY_PLL_CP_SET_CUR            0x04F0
#define DSIPHY_PLL_PLL_ICPMSET           0x04F4
#define DSIPHY_PLL_PLL_ICPCSET           0x04F8
#define DSIPHY_PLL_PLL_ICP_SET           0x04FC
#define DSIPHY_PLL_PLL_LPF1              0x0500
#define DSIPHY_PLL_PLL_LPF2_POSTDIV      0x0504
#define DSIPHY_PLL_PLL_BANDGAP           0x0508

// DSI controller registers (offset from DSI0_CTRL_BASE)
#define DSI_SOFT_RESET                   0x0118
#define DSI_CLK_CTRL                     0x011C
#define DSI_CTRL                         0x0004
#define DSI_TRIG_CTRL                    0x0084
#define DSI_ERR_INT_MASK0                0x010C
#define DSI_INT_CTRL                     0x0110
#define DSI_LANE_CTL                     0x00AC
#define DSI_LANE_SWAP_CTL                0x00B0
#define DSI_TIMING_CTL                   0x00C4
#define DSI_VIDEO_MODE_ACTIVE_H          0x0024
#define DSI_VIDEO_MODE_ACTIVE_V          0x0028
#define DSI_VIDEO_MODE_TOTAL             0x002C
#define DSI_VIDEO_MODE_HSYNC             0x0030
#define DSI_VIDEO_MODE_VSYNC             0x0034
#define DSI_VIDEO_MODE_VSYNC_VPOS        0x0038
#define DSI_VIDEO_MODE_CTRL              0x0010
#define DSI_TIMING_DB_MODE               0x01E8
#define DSI_TIMING_FLUSH                 0x01E4
#define DSI_HS_TIMER_CTRL                0x00BC
#define DSI_MISR_VIDEO_CTRL              0x00A4
#define DSI_DMA_CMD_OFFSET               0x0048
#define DSI_DMA_CMD_LENGTH               0x004C
#define DSI_CMD_MODE_DMA_SW_TRIGGER      0x0090

// PHY 14nm lane base offsets
#define DSIPHY_LANE_BASE                 0x0100
#define DSIPHY_LANE_SIZE                 0x0080

// VCO reference clock (19.2 MHz)
#define VCO_REF_CLK_RATE                 19200000

// Panel parameters (otm1911)
#define PANEL_WIDTH                      1080
#define PANEL_HEIGHT                     1920
#define PANEL_HFP                        24
#define PANEL_HBP                        24
#define PANEL_HPW                        20
#define PANEL_VFP                        14
#define PANEL_VBP                        6
#define PANEL_VPW                        2

// Calculated pixel clock for 1080x1920@60Hz
// (1080+24+20+24) * (1920+14+2+6) * 60 = 1148 * 1942 * 60 = 133,764,960 Hz
#define PANEL_PIXEL_CLOCK                133764960

// VCO clock for thulium PLL (postdiv=2, n1div=7, n2div=12)
// vco = 19.2 MHz * (7*12*2)/2 = 1612.8 MHz
#define PANEL_VCO_CLOCK                  1612800000

#endif
