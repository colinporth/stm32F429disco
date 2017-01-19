// main.cpp
//{{{  includes
#include "cLcd.h"
#include "ps2.h"

#include "stm32f429i_discovery.h"

#include "fatFs.h"
#include "diskio.h"

#include "cDecodePic.h"
//}}}
//{{{  clock defines
#define HSE_VALUE        ((uint32_t)8000000)
#define HSI_VALUE        ((uint32_t)16000000)
#define VECT_TAB_OFFSET  0x00
//}}}
//{{{  sdram defines
#define SDRAM_BANK1_ADDR  ((uint32_t)0xC0000000)
#define SDRAM_BANK1_LEN   ((uint32_t)0x01000000)
#define SDRAM_BANK2_ADDR  ((uint32_t)0xD0000000)
#define SDRAM_BANK2_LEN   ((uint32_t)0x00800000)

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)
//}}}
//{{{  trace defines
#define ITM_LAR_KEY   0xC5ACCE55
//{{{  ITM defines
/* ITM Trace Privilege Register Definitions */
#define ITM1_TPR_PRIVMASK_Pos                0U                                            /*!< ITM TPR: PRIVMASK Position */
#define ITM1_TPR_PRIVMASK_Msk               (0xFUL /*<< ITM1_TPR_PRIVMASK_Pos*/)            /*!< ITM TPR: PRIVMASK Mask */

/* ITM Trace Control Register Definitions */
#define ITM1_TCR_BUSY_Pos                   23U                                            /*!< ITM TCR: BUSY Position */
#define ITM1_TCR_BUSY_Msk                   (1UL << ITM1_TCR_BUSY_Pos)                      /*!< ITM TCR: BUSY Mask */

#define ITM1_TCR_TraceBusID_Pos             16U                                            /*!< ITM TCR: ATBID Position */
#define ITM1_TCR_TraceBusID_Msk             (0x7FUL << ITM1_TCR_TraceBusID_Pos)             /*!< ITM TCR: ATBID Mask */

#define ITM1_TCR_GTSFREQ_Pos                10U                                            /*!< ITM TCR: Global timestamp frequency Position */
#define ITM1_TCR_GTSFREQ_Msk                (3UL << ITM1_TCR_GTSFREQ_Pos)                   /*!< ITM TCR: Global timestamp frequency Mask */

#define ITM1_TCR_TSPrescale_Pos              8U                                            /*!< ITM TCR: TSPrescale Position */
#define ITM1_TCR_TSPrescale_Msk             (3UL << ITM1_TCR_TSPrescale_Pos)                /*!< ITM TCR: TSPrescale Mask */

#define ITM1_TCR_SWOENA_Pos                  4U                                            /*!< ITM TCR: SWOENA Position */
#define ITM1_TCR_SWOENA_Msk                 (1UL << ITM1_TCR_SWOENA_Pos)                    /*!< ITM TCR: SWOENA Mask */

#define ITM1_TCR_DWTENA_Pos                  3U                                            /*!< ITM TCR: DWTENA Position */
#define ITM1_TCR_DWTENA_Msk                 (1UL << ITM1_TCR_DWTENA_Pos)                    /*!< ITM TCR: DWTENA Mask */

#define ITM1_TCR_SYNCENA_Pos                 2U                                            /*!< ITM TCR: SYNCENA Position */
#define ITM1_TCR_SYNCENA_Msk                (1UL << ITM1_TCR_SYNCENA_Pos)                   /*!< ITM TCR: SYNCENA Mask */

#define ITM1_TCR_TSENA_Pos                   1U                                            /*!< ITM TCR: TSENA Position */
#define ITM1_TCR_TSENA_Msk                  (1UL << ITM1_TCR_TSENA_Pos)                     /*!< ITM TCR: TSENA Mask */

#define ITM1_TCR_ITMENA_Pos                  0U                                            /*!< ITM TCR: ITM Enable bit Position */
#define ITM1_TCR_ITMENA_Msk                 (1UL /*<< ITM1_TCR_ITMENA_Pos*/)                /*!< ITM TCR: ITM Enable bit Mask */

/* ITM Integration Write Register Definitions */
#define ITM1_IWR_ATVALIDM_Pos                0U                                            /*!< ITM IWR: ATVALIDM Position */
#define ITM1_IWR_ATVALIDM_Msk               (1UL /*<< ITM1_IWR_ATVALIDM_Pos*/)              /*!< ITM IWR: ATVALIDM Mask */

/* ITM Integration Read Register Definitions */
#define ITM1_IRR_ATREADYM_Pos                0U                                            /*!< ITM IRR: ATREADYM Position */
#define ITM1_IRR_ATREADYM_Msk               (1UL /*<< ITM1_IRR_ATREADYM_Pos*/)              /*!< ITM IRR: ATREADYM Mask */

/* ITM Integration Mode Control Register Definitions */
#define ITM1_IMCR_INTEGRATION_Pos            0U                                            /*!< ITM IMCR: INTEGRATION Position */
#define ITM1_IMCR_INTEGRATION_Msk           (1UL /*<< ITM1_IMCR_INTEGRATION_Pos*/)          /*!< ITM IMCR: INTEGRATION Mask */

/* ITM Lock Status Register Definitions */
#define ITM1_LSR_ByteAcc_Pos                 2U                                            /*!< ITM LSR: ByteAcc Position */
#define ITM1_LSR_ByteAcc_Msk                (1UL << ITM1_LSR_ByteAcc_Pos)                   /*!< ITM LSR: ByteAcc Mask */

#define ITM1_LSR_Access_Pos                  1U                                            /*!< ITM LSR: Access Position */
#define ITM1_LSR_Access_Msk                 (1UL << ITM1_LSR_Access_Pos)                    /*!< ITM LSR: Access Mask */

#define ITM1_LSR_Present_Pos                 0U                                            /*!< ITM LSR: Present Position */
#define ITM1_LSR_Present_Msk                (1UL /*<< ITM1_LSR_Present_Pos*/)               /*!< ITM LSR: Present Mask */
//}}}
//{{{  ITM_Type       0xE0000000
typedef struct {
__OM  union {
    __OM  uint8_t    u8;                 /*!< Offset: 0x000 ( /W)  ITM Stimulus Port 8-bit */
    __OM  uint16_t   u16;                /*!< Offset: 0x000 ( /W)  ITM Stimulus Port 16-bit */
    __OM  uint32_t   u32;                /*!< Offset: 0x000 ( /W)  ITM Stimulus Port 32-bit */
  }  PORT [32U];                         /*!< Offset: 0x000 ( /W)  ITM Stimulus Port Registers */

        uint32_t RESERVED0[864U];
  __IOM uint32_t TER;                    /*!< Offset: 0xE00 (R/W)  ITM Trace Enable Register */
        uint32_t RESERVED1[15U];
  __IOM uint32_t TPR;                    /*!< Offset: 0xE40 (R/W)  ITM Trace Privilege Register */
        uint32_t RESERVED2[15U];
  __IOM uint32_t TCR;                    /*!< Offset: 0xE80 (R/W)  ITM Trace Control Register */
        uint32_t RESERVED3[29U];
  __OM  uint32_t IWR;                    /*!< Offset: 0xEF8 ( /W)  ITM Integration Write Register */
  __IM  uint32_t IRR;                    /*!< Offset: 0xEFC (R/ )  ITM Integration Read Register */
  __IOM uint32_t IMCR;                   /*!< Offset: 0xF00 (R/W)  ITM Integration Mode Control Register */
        uint32_t RESERVED4[43U];
  __OM  uint32_t LAR;                    /*!< Offset: 0xFB0 ( /W)  ITM Lock Access Register */
  __IM  uint32_t LSR;                    /*!< Offset: 0xFB4 (R/ )  ITM Lock Status Register */
        uint32_t RESERVED5[6U];
  __IM  uint32_t PID4;                   /*!< Offset: 0xFD0 (R/ )  ITM Peripheral Identification Register #4 */
  __IM  uint32_t PID5;                   /*!< Offset: 0xFD4 (R/ )  ITM Peripheral Identification Register #5 */
  __IM  uint32_t PID6;                   /*!< Offset: 0xFD8 (R/ )  ITM Peripheral Identification Register #6 */
  __IM  uint32_t PID7;                   /*!< Offset: 0xFDC (R/ )  ITM Peripheral Identification Register #7 */
  __IM  uint32_t PID0;                   /*!< Offset: 0xFE0 (R/ )  ITM Peripheral Identification Register #0 */
  __IM  uint32_t PID1;                   /*!< Offset: 0xFE4 (R/ )  ITM Peripheral Identification Register #1 */
  __IM  uint32_t PID2;                   /*!< Offset: 0xFE8 (R/ )  ITM Peripheral Identification Register #2 */
  __IM  uint32_t PID3;                   /*!< Offset: 0xFEC (R/ )  ITM Peripheral Identification Register #3 */
  __IM  uint32_t CID0;                   /*!< Offset: 0xFF0 (R/ )  ITM Component  Identification Register #0 */
  __IM  uint32_t CID1;                   /*!< Offset: 0xFF4 (R/ )  ITM Component  Identification Register #1 */
  __IM  uint32_t CID2;                   /*!< Offset: 0xFF8 (R/ )  ITM Component  Identification Register #2 */
  __IM  uint32_t CID3;                   /*!< Offset: 0xFFC (R/ )  ITM Component  Identification Register #3 */
} ITM1_Type1;

#define ITM1_BASE       (0xE0000000UL)                            /*!< ITM Base Address */
#define ITM1            ((ITM1_Type*)       ITM_BASE)   /*!< ITM configuration struct */
//}}}

//{{{  DWT defines
/* DWT Control Register Definitions */
#define DWt1_CTRL_NUMCOMP_Pos               28U                                         /*!< DWT CTRL: NUMCOMP Position */
#define DWt1_CTRL_NUMCOMP_Msk               (0xFUL << DWt1_CTRL_NUMCOMP_Pos)             /*!< DWT CTRL: NUMCOMP Mask */

#define DWt1_CTRL_NOTRCPKT_Pos              27U                                         /*!< DWT CTRL: NOTRCPKT Position */
#define DWt1_CTRL_NOTRCPKT_Msk              (0x1UL << DWt1_CTRL_NOTRCPKT_Pos)            /*!< DWT CTRL: NOTRCPKT Mask */

#define DWt1_CTRL_NOEXTTRIG_Pos             26U                                         /*!< DWT CTRL: NOEXTTRIG Position */
#define DWt1_CTRL_NOEXTTRIG_Msk             (0x1UL << DWt1_CTRL_NOEXTTRIG_Pos)           /*!< DWT CTRL: NOEXTTRIG Mask */

#define DWt1_CTRL_NOCYCCNT_Pos              25U                                         /*!< DWT CTRL: NOCYCCNT Position */
#define DWt1_CTRL_NOCYCCNT_Msk              (0x1UL << DWt1_CTRL_NOCYCCNT_Pos)            /*!< DWT CTRL: NOCYCCNT Mask */

#define DWt1_CTRL_NOPRFCNT_Pos              24U                                         /*!< DWT CTRL: NOPRFCNT Position */
#define DWt1_CTRL_NOPRFCNT_Msk              (0x1UL << DWt1_CTRL_NOPRFCNT_Pos)            /*!< DWT CTRL: NOPRFCNT Mask */

#define DWt1_CTRL_CYCEVTENA_Pos             22U                                         /*!< DWT CTRL: CYCEVTENA Position */
#define DWt1_CTRL_CYCEVTENA_Msk             (0x1UL << DWt1_CTRL_CYCEVTENA_Pos)           /*!< DWT CTRL: CYCEVTENA Mask */

#define DWt1_CTRL_FOLDEVTENA_Pos            21U                                         /*!< DWT CTRL: FOLDEVTENA Position */
#define DWt1_CTRL_FOLDEVTENA_Msk            (0x1UL << DWt1_CTRL_FOLDEVTENA_Pos)          /*!< DWT CTRL: FOLDEVTENA Mask */

#define DWt1_CTRL_LSUEVTENA_Pos             20U                                         /*!< DWT CTRL: LSUEVTENA Position */
#define DWt1_CTRL_LSUEVTENA_Msk             (0x1UL << DWt1_CTRL_LSUEVTENA_Pos)           /*!< DWT CTRL: LSUEVTENA Mask */

#define DWt1_CTRL_SLEEPEVTENA_Pos           19U                                         /*!< DWT CTRL: SLEEPEVTENA Position */
#define DWt1_CTRL_SLEEPEVTENA_Msk           (0x1UL << DWt1_CTRL_SLEEPEVTENA_Pos)         /*!< DWT CTRL: SLEEPEVTENA Mask */

#define DWt1_CTRL_EXCEVTENA_Pos             18U                                         /*!< DWT CTRL: EXCEVTENA Position */
#define DWt1_CTRL_EXCEVTENA_Msk             (0x1UL << DWt1_CTRL_EXCEVTENA_Pos)           /*!< DWT CTRL: EXCEVTENA Mask */

#define DWt1_CTRL_CPIEVTENA_Pos             17U                                         /*!< DWT CTRL: CPIEVTENA Position */
#define DWt1_CTRL_CPIEVTENA_Msk             (0x1UL << DWt1_CTRL_CPIEVTENA_Pos)           /*!< DWT CTRL: CPIEVTENA Mask */

#define DWt1_CTRL_EXCTRCENA_Pos             16U                                         /*!< DWT CTRL: EXCTRCENA Position */
#define DWt1_CTRL_EXCTRCENA_Msk             (0x1UL << DWt1_CTRL_EXCTRCENA_Pos)           /*!< DWT CTRL: EXCTRCENA Mask */

#define DWt1_CTRL_PCSAMPLENA_Pos            12U                                         /*!< DWT CTRL: PCSAMPLENA Position */
#define DWt1_CTRL_PCSAMPLENA_Msk            (0x1UL << DWt1_CTRL_PCSAMPLENA_Pos)          /*!< DWT CTRL: PCSAMPLENA Mask */

#define DWt1_CTRL_SYNCTAP_Pos               10U                                         /*!< DWT CTRL: SYNCTAP Position */
#define DWt1_CTRL_SYNCTAP_Msk               (0x3UL << DWt1_CTRL_SYNCTAP_Pos)             /*!< DWT CTRL: SYNCTAP Mask */

#define DWt1_CTRL_CYCTAP_Pos                 9U                                         /*!< DWT CTRL: CYCTAP Position */
#define DWt1_CTRL_CYCTAP_Msk                (0x1UL << DWt1_CTRL_CYCTAP_Pos)              /*!< DWT CTRL: CYCTAP Mask */

#define DWt1_CTRL_POSTINIT_Pos               5U                                         /*!< DWT CTRL: POSTINIT Position */
#define DWt1_CTRL_POSTINIT_Msk              (0xFUL << DWt1_CTRL_POSTINIT_Pos)            /*!< DWT CTRL: POSTINIT Mask */

#define DWt1_CTRL_POSTPRESET_Pos             1U                                         /*!< DWT CTRL: POSTPRESET Position */
#define DWt1_CTRL_POSTPRESET_Msk            (0xFUL << DWt1_CTRL_POSTPRESET_Pos)          /*!< DWT CTRL: POSTPRESET Mask */

#define DWt1_CTRL_CYCCNTENA_Pos              0U                                         /*!< DWT CTRL: CYCCNTENA Position */
#define DWt1_CTRL_CYCCNTENA_Msk             (0x1UL /*<< DWt1_CTRL_CYCCNTENA_Pos*/)       /*!< DWT CTRL: CYCCNTENA Mask */

/* DWT CPI Count Register Definitions */
#define DWt1_CPICNT_CPICNT_Pos               0U                                         /*!< DWT CPICNT: CPICNT Position */
#define DWt1_CPICNT_CPICNT_Msk              (0xFFUL /*<< DWt1_CPICNT_CPICNT_Pos*/)       /*!< DWT CPICNT: CPICNT Mask */

/* DWT Exception Overhead Count Register Definitions */
#define DWt1_EXCCNT_EXCCNT_Pos               0U                                         /*!< DWT EXCCNT: EXCCNT Position */
#define DWt1_EXCCNT_EXCCNT_Msk              (0xFFUL /*<< DWt1_EXCCNT_EXCCNT_Pos*/)       /*!< DWT EXCCNT: EXCCNT Mask */

/* DWT Sleep Count Register Definitions */
#define DWt1_SLEEPCNT_SLEEPCNT_Pos           0U                                         /*!< DWT SLEEPCNT: SLEEPCNT Position */
#define DWt1_SLEEPCNT_SLEEPCNT_Msk          (0xFFUL /*<< DWt1_SLEEPCNT_SLEEPCNT_Pos*/)   /*!< DWT SLEEPCNT: SLEEPCNT Mask */

/* DWT LSU Count Register Definitions */
#define DWt1_LSUCNT_LSUCNT_Pos               0U                                         /*!< DWT LSUCNT: LSUCNT Position */
#define DWt1_LSUCNT_LSUCNT_Msk              (0xFFUL /*<< DWt1_LSUCNT_LSUCNT_Pos*/)       /*!< DWT LSUCNT: LSUCNT Mask */

/* DWT Folded-instruction Count Register Definitions */
#define DWt1_FOLDCNT_FOLDCNT_Pos             0U                                         /*!< DWT FOLDCNT: FOLDCNT Position */
#define DWt1_FOLDCNT_FOLDCNT_Msk            (0xFFUL /*<< DWt1_FOLDCNT_FOLDCNT_Pos*/)     /*!< DWT FOLDCNT: FOLDCNT Mask */

/* DWT Comparator Mask Register Definitions */
#define DWt1_MASK_MASK_Pos                   0U                                         /*!< DWT MASK: MASK Position */
#define DWt1_MASK_MASK_Msk                  (0x1FUL /*<< DWt1_MASK_MASK_Pos*/)           /*!< DWT MASK: MASK Mask */

/* DWT Comparator Function Register Definitions */
#define DWt1_FUNCTION_MATCHED_Pos           24U                                         /*!< DWT FUNCTION: MATCHED Position */
#define DWt1_FUNCTION_MATCHED_Msk           (0x1UL << DWt1_FUNCTION_MATCHED_Pos)         /*!< DWT FUNCTION: MATCHED Mask */

#define DWt1_FUNCTION_DATAVADDR1_Pos        16U                                         /*!< DWT FUNCTION: DATAVADDR1 Position */
#define DWt1_FUNCTION_DATAVADDR1_Msk        (0xFUL << DWt1_FUNCTION_DATAVADDR1_Pos)      /*!< DWT FUNCTION: DATAVADDR1 Mask */

#define DWt1_FUNCTION_DATAVADDR0_Pos        12U                                         /*!< DWT FUNCTION: DATAVADDR0 Position */
#define DWt1_FUNCTION_DATAVADDR0_Msk        (0xFUL << DWt1_FUNCTION_DATAVADDR0_Pos)      /*!< DWT FUNCTION: DATAVADDR0 Mask */

#define DWt1_FUNCTION_DATAVSIZE_Pos         10U                                         /*!< DWT FUNCTION: DATAVSIZE Position */
#define DWt1_FUNCTION_DATAVSIZE_Msk         (0x3UL << DWt1_FUNCTION_DATAVSIZE_Pos)       /*!< DWT FUNCTION: DATAVSIZE Mask */

#define DWt1_FUNCTION_LNK1ENA_Pos            9U                                         /*!< DWT FUNCTION: LNK1ENA Position */
#define DWt1_FUNCTION_LNK1ENA_Msk           (0x1UL << DWt1_FUNCTION_LNK1ENA_Pos)         /*!< DWT FUNCTION: LNK1ENA Mask */

#define DWt1_FUNCTION_DATAVMATCH_Pos         8U                                         /*!< DWT FUNCTION: DATAVMATCH Position */
#define DWt1_FUNCTION_DATAVMATCH_Msk        (0x1UL << DWt1_FUNCTION_DATAVMATCH_Pos)      /*!< DWT FUNCTION: DATAVMATCH Mask */

#define DWt1_FUNCTION_CYCMATCH_Pos           7U                                         /*!< DWT FUNCTION: CYCMATCH Position */
#define DWt1_FUNCTION_CYCMATCH_Msk          (0x1UL << DWt1_FUNCTION_CYCMATCH_Pos)        /*!< DWT FUNCTION: CYCMATCH Mask */

#define DWt1_FUNCTION_EMITRANGE_Pos          5U                                         /*!< DWT FUNCTION: EMITRANGE Position */
#define DWt1_FUNCTION_EMITRANGE_Msk         (0x1UL << DWt1_FUNCTION_EMITRANGE_Pos)       /*!< DWT FUNCTION: EMITRANGE Mask */

#define DWt1_FUNCTION_FUNCTION_Pos           0U                                         /*!< DWT FUNCTION: FUNCTION Position */
#define DWt1_FUNCTION_FUNCTION_Msk          (0xFUL /*<< DWt1_FUNCTION_FUNCTION_Pos*/)    /*!< DWT FUNCTION: FUNCTION Mask */
//}}}
//{{{  DWT_Type       0xE0001000
typedef struct {
  __IOM uint32_t CTRL;                   /*!< Offset: 0x000 (R/W)  Control Register */
  __IOM uint32_t CYCCNT;                 /*!< Offset: 0x004 (R/W)  Cycle Count Register */
  __IOM uint32_t CPICNT;                 /*!< Offset: 0x008 (R/W)  CPI Count Register */
  __IOM uint32_t EXCCNT;                 /*!< Offset: 0x00C (R/W)  Exception Overhead Count Register */
  __IOM uint32_t SLEEPCNT;               /*!< Offset: 0x010 (R/W)  Sleep Count Register */
  __IOM uint32_t LSUCNT;                 /*!< Offset: 0x014 (R/W)  LSU Count Register */
  __IOM uint32_t FOLDCNT;                /*!< Offset: 0x018 (R/W)  Folded-instruction Count Register */
  __IM  uint32_t PCSR;                   /*!< Offset: 0x01C (R/ )  Program Counter Sample Register */
  __IOM uint32_t COMP0;                  /*!< Offset: 0x020 (R/W)  Comparator Register 0 */
  __IOM uint32_t MASK0;                  /*!< Offset: 0x024 (R/W)  Mask Register 0 */
  __IOM uint32_t FUNCTION0;              /*!< Offset: 0x028 (R/W)  Function Register 0 */
        uint32_t RESERVED0[1U];
  __IOM uint32_t COMP1;                  /*!< Offset: 0x030 (R/W)  Comparator Register 1 */
  __IOM uint32_t MASK1;                  /*!< Offset: 0x034 (R/W)  Mask Register 1 */
  __IOM uint32_t FUNCTION1;              /*!< Offset: 0x038 (R/W)  Function Register 1 */
        uint32_t RESERVED1[1U];
  __IOM uint32_t COMP2;                  /*!< Offset: 0x040 (R/W)  Comparator Register 2 */
  __IOM uint32_t MASK2;                  /*!< Offset: 0x044 (R/W)  Mask Register 2 */
  __IOM uint32_t FUNCTION2;              /*!< Offset: 0x048 (R/W)  Function Register 2 */
        uint32_t RESERVED2[1U];
  __IOM uint32_t COMP3;                  /*!< Offset: 0x050 (R/W)  Comparator Register 3 */
  __IOM uint32_t MASK3;                  /*!< Offset: 0x054 (R/W)  Mask Register 3 */
  __IOM uint32_t FUNCTION3;              /*!< Offset: 0x058 (R/W)  Function Register 3 */
  } DWt1_Type1;

#define DWt1_BASE       (0xE0001000UL)                            /*!< DWT Base Address */
#define DWT1            ((DWt1_Type*)       DWt1_BASE)   /*!< DWT configuration struct */
//}}}

//{{{  CoreDebug defines
/* Debug Halting Control and Status Register Definitions */
#define CoreDebug1_DHCSR_DBGKEY_Pos         16U                                            /*!< CoreDebug DHCSR: DBGKEY Position */
#define CoreDebug1_DHCSR_DBGKEY_Msk         (0xFFFFUL << CoreDebug1_DHCSR_DBGKEY_Pos)       /*!< CoreDebug DHCSR: DBGKEY Mask */

#define CoreDebug1_DHCSR_S_RESET_ST_Pos     25U                                            /*!< CoreDebug DHCSR: S_RESET_ST Position */
#define CoreDebug1_DHCSR_S_RESET_ST_Msk     (1UL << CoreDebug1_DHCSR_S_RESET_ST_Pos)        /*!< CoreDebug DHCSR: S_RESET_ST Mask */

#define CoreDebug1_DHCSR_S_RETIRE_ST_Pos    24U                                            /*!< CoreDebug DHCSR: S_RETIRE_ST Position */
#define CoreDebug1_DHCSR_S_RETIRE_ST_Msk    (1UL << CoreDebug1_DHCSR_S_RETIRE_ST_Pos)       /*!< CoreDebug DHCSR: S_RETIRE_ST Mask */

#define CoreDebug1_DHCSR_S_LOCKUP_Pos       19U                                            /*!< CoreDebug DHCSR: S_LOCKUP Position */
#define CoreDebug1_DHCSR_S_LOCKUP_Msk       (1UL << CoreDebug1_DHCSR_S_LOCKUP_Pos)          /*!< CoreDebug DHCSR: S_LOCKUP Mask */

#define CoreDebug1_DHCSR_S_SLEEP_Pos        18U                                            /*!< CoreDebug DHCSR: S_SLEEP Position */
#define CoreDebug1_DHCSR_S_SLEEP_Msk        (1UL << CoreDebug1_DHCSR_S_SLEEP_Pos)           /*!< CoreDebug DHCSR: S_SLEEP Mask */

#define CoreDebug1_DHCSR_S_HALT_Pos         17U                                            /*!< CoreDebug DHCSR: S_HALT Position */
#define CoreDebug1_DHCSR_S_HALT_Msk         (1UL << CoreDebug1_DHCSR_S_HALT_Pos)            /*!< CoreDebug DHCSR: S_HALT Mask */

#define CoreDebug1_DHCSR_S_REGRDY_Pos       16U                                            /*!< CoreDebug DHCSR: S_REGRDY Position */
#define CoreDebug1_DHCSR_S_REGRDY_Msk       (1UL << CoreDebug1_DHCSR_S_REGRDY_Pos)          /*!< CoreDebug DHCSR: S_REGRDY Mask */

#define CoreDebug1_DHCSR_C_SNAPSTALL_Pos     5U                                            /*!< CoreDebug DHCSR: C_SNAPSTALL Position */
#define CoreDebug1_DHCSR_C_SNAPSTALL_Msk    (1UL << CoreDebug1_DHCSR_C_SNAPSTALL_Pos)       /*!< CoreDebug DHCSR: C_SNAPSTALL Mask */

#define CoreDebug1_DHCSR_C_MASKINTS_Pos      3U                                            /*!< CoreDebug DHCSR: C_MASKINTS Position */
#define CoreDebug1_DHCSR_C_MASKINTS_Msk     (1UL << CoreDebug1_DHCSR_C_MASKINTS_Pos)        /*!< CoreDebug DHCSR: C_MASKINTS Mask */

#define CoreDebug1_DHCSR_C_STEP_Pos          2U                                            /*!< CoreDebug DHCSR: C_STEP Position */
#define CoreDebug1_DHCSR_C_STEP_Msk         (1UL << CoreDebug1_DHCSR_C_STEP_Pos)            /*!< CoreDebug DHCSR: C_STEP Mask */

#define CoreDebug1_DHCSR_C_HALT_Pos          1U                                            /*!< CoreDebug DHCSR: C_HALT Position */
#define CoreDebug1_DHCSR_C_HALT_Msk         (1UL << CoreDebug1_DHCSR_C_HALT_Pos)            /*!< CoreDebug DHCSR: C_HALT Mask */

#define CoreDebug1_DHCSR_C_DEBUGEN_Pos       0U                                            /*!< CoreDebug DHCSR: C_DEBUGEN Position */
#define CoreDebug1_DHCSR_C_DEBUGEN_Msk      (1UL /*<< CoreDebug1_DHCSR_C_DEBUGEN_Pos*/)     /*!< CoreDebug DHCSR: C_DEBUGEN Mask */

/* Debug Core Register Selector Register Definitions */
#define CoreDebug1_DCRSR_REGWnR_Pos         16U                                            /*!< CoreDebug DCRSR: REGWnR Position */
#define CoreDebug1_DCRSR_REGWnR_Msk         (1UL << CoreDebug1_DCRSR_REGWnR_Pos)            /*!< CoreDebug DCRSR: REGWnR Mask */

#define CoreDebug1_DCRSR_REGSEL_Pos          0U                                            /*!< CoreDebug DCRSR: REGSEL Position */
#define CoreDebug1_DCRSR_REGSEL_Msk         (0x1FUL /*<< CoreDebug1_DCRSR_REGSEL_Pos*/)     /*!< CoreDebug DCRSR: REGSEL Mask */

/* Debug Exception and Monitor Control Register Definitions */
#define CoreDebug1_DEMCR_TRCENA_Pos         24U                                            /*!< CoreDebug DEMCR: TRCENA Position */
#define CoreDebug1_DEMCR_TRCENA_Msk         (1UL << CoreDebug1_DEMCR_TRCENA_Pos)            /*!< CoreDebug DEMCR: TRCENA Mask */

#define CoreDebug1_DEMCR_MON_REQ_Pos        19U                                            /*!< CoreDebug DEMCR: MON_REQ Position */
#define CoreDebug1_DEMCR_MON_REQ_Msk        (1UL << CoreDebug1_DEMCR_MON_REQ_Pos)           /*!< CoreDebug DEMCR: MON_REQ Mask */

#define CoreDebug1_DEMCR_MON_STEP_Pos       18U                                            /*!< CoreDebug DEMCR: MON_STEP Position */
#define CoreDebug1_DEMCR_MON_STEP_Msk       (1UL << CoreDebug1_DEMCR_MON_STEP_Pos)          /*!< CoreDebug DEMCR: MON_STEP Mask */

#define CoreDebug1_DEMCR_MON_PEND_Pos       17U                                            /*!< CoreDebug DEMCR: MON_PEND Position */
#define CoreDebug1_DEMCR_MON_PEND_Msk       (1UL << CoreDebug1_DEMCR_MON_PEND_Pos)          /*!< CoreDebug DEMCR: MON_PEND Mask */

#define CoreDebug1_DEMCR_MON_EN_Pos         16U                                            /*!< CoreDebug DEMCR: MON_EN Position */
#define CoreDebug1_DEMCR_MON_EN_Msk         (1UL << CoreDebug1_DEMCR_MON_EN_Pos)            /*!< CoreDebug DEMCR: MON_EN Mask */

#define CoreDebug1_DEMCR_VC_HARDERR_Pos     10U                                            /*!< CoreDebug DEMCR: VC_HARDERR Position */
#define CoreDebug1_DEMCR_VC_HARDERR_Msk     (1UL << CoreDebug1_DEMCR_VC_HARDERR_Pos)        /*!< CoreDebug DEMCR: VC_HARDERR Mask */

#define CoreDebug1_DEMCR_VC_INTERR_Pos       9U                                            /*!< CoreDebug DEMCR: VC_INTERR Position */
#define CoreDebug1_DEMCR_VC_INTERR_Msk      (1UL << CoreDebug1_DEMCR_VC_INTERR_Pos)         /*!< CoreDebug DEMCR: VC_INTERR Mask */

#define CoreDebug1_DEMCR_VC_BUSERR_Pos       8U                                            /*!< CoreDebug DEMCR: VC_BUSERR Position */
#define CoreDebug1_DEMCR_VC_BUSERR_Msk      (1UL << CoreDebug1_DEMCR_VC_BUSERR_Pos)         /*!< CoreDebug DEMCR: VC_BUSERR Mask */

#define CoreDebug1_DEMCR_VC_STATERR_Pos      7U                                            /*!< CoreDebug DEMCR: VC_STATERR Position */
#define CoreDebug1_DEMCR_VC_STATERR_Msk     (1UL << CoreDebug1_DEMCR_VC_STATERR_Pos)        /*!< CoreDebug DEMCR: VC_STATERR Mask */

#define CoreDebug1_DEMCR_VC_CHKERR_Pos       6U                                            /*!< CoreDebug DEMCR: VC_CHKERR Position */
#define CoreDebug1_DEMCR_VC_CHKERR_Msk      (1UL << CoreDebug1_DEMCR_VC_CHKERR_Pos)         /*!< CoreDebug DEMCR: VC_CHKERR Mask */

#define CoreDebug1_DEMCR_VC_NOCPERR_Pos      5U                                            /*!< CoreDebug DEMCR: VC_NOCPERR Position */
#define CoreDebug1_DEMCR_VC_NOCPERR_Msk     (1UL << CoreDebug1_DEMCR_VC_NOCPERR_Pos)        /*!< CoreDebug DEMCR: VC_NOCPERR Mask */

#define CoreDebug1_DEMCR_VC_MMERR_Pos        4U                                            /*!< CoreDebug DEMCR: VC_MMERR Position */
#define CoreDebug1_DEMCR_VC_MMERR_Msk       (1UL << CoreDebug1_DEMCR_VC_MMERR_Pos)          /*!< CoreDebug DEMCR: VC_MMERR Mask */

#define CoreDebug1_DEMCR_VC_CORERESET_Pos    0U                                            /*!< CoreDebug DEMCR: VC_CORERESET Position */
#define CoreDebug1_DEMCR_VC_CORERESET_Msk   (1UL /*<< CoreDebug1_DEMCR_VC_CORERESET_Pos*/)  /*!< CoreDebug DEMCR: VC_CORERESET Mask */
//}}}
//{{{  CoreDebug_Type 0xE000EDF0
typedef struct {
  __IOM uint32_t DHCSR;                  /*!< Offset: 0x000 (R/W)  Debug Halting Control and Status Register */
  __OM  uint32_t DCRSR;                  /*!< Offset: 0x004 ( /W)  Debug Core Register Selector Register */
  __IOM uint32_t DCRDR;                  /*!< Offset: 0x008 (R/W)  Debug Core Register Data Register */
  __IOM uint32_t DEMCR;                  /*!< Offset: 0x00C (R/W)  Debug Exception and Monitor Control Register */
  } CoreDebug_Type1;

#define CoreDebug1_BASE (0xE000EDF0UL)                            /*!< Core Debug Base Address */
#define CoreDebug1      ((CoreDebug1_Type*) CoreDebug_BASE)   /*!< Core Debug configuration struct */
//}}}

//{{{  TPI defines
/* TPI Asynchronous Clock Prescaler Register Definitions */
#define TPI1_ACPR_PRESCALER_Pos              0U                                         /*!< TPI ACPR: PRESCALER Position */
#define TPI1_ACPR_PRESCALER_Msk             (0x1FFFUL /*<< TPI1_ACPR_PRESCALER_Pos*/)    /*!< TPI ACPR: PRESCALER Mask */

/* TPI Selected Pin Protocol Register Definitions */
#define TPI1_SPPR_TXMODE_Pos                 0U                                         /*!< TPI SPPR: TXMODE Position */
#define TPI1_SPPR_TXMODE_Msk                (0x3UL /*<< TPI1_SPPR_TXMODE_Pos*/)          /*!< TPI SPPR: TXMODE Mask */

/* TPI Formatter and Flush Status Register Definitions */
#define TPI1_FFSR_FtNonStop_Pos              3U                                         /*!< TPI FFSR: FtNonStop Position */
#define TPI1_FFSR_FtNonStop_Msk             (0x1UL << TPI1_FFSR_FtNonStop_Pos)           /*!< TPI FFSR: FtNonStop Mask */

#define TPI1_FFSR_TCPresent_Pos              2U                                         /*!< TPI FFSR: TCPresent Position */
#define TPI1_FFSR_TCPresent_Msk             (0x1UL << TPI1_FFSR_TCPresent_Pos)           /*!< TPI FFSR: TCPresent Mask */

#define TPI_FFSR_FtStopped_Pos              1U                                         /*!< TPI FFSR: FtStopped Position */
#define TPI1_FFSR_FtStopped_Msk             (0x1UL << TPI1_FFSR_FtStopped_Pos)           /*!< TPI FFSR: FtStopped Mask */

#define TPI1_FFSR_FlInProg_Pos               0U                                         /*!< TPI FFSR: FlInProg Position */
#define TPI1_FFSR_FlInProg_Msk              (0x1UL /*<< TPI1_FFSR_FlInProg_Pos*/)        /*!< TPI FFSR: FlInProg Mask */

/* TPI Formatter and Flush Control Register Definitions */
#define TPI1_FFCR_TrigIn_Pos                 8U                                         /*!< TPI FFCR: TrigIn Position */
#define TPI1_FFCR_TrigIn_Msk                (0x1UL << TPI1_FFCR_TrigIn_Pos)              /*!< TPI FFCR: TrigIn Mask */

#define TPI1_FFCR_EnFCont_Pos                1U                                         /*!< TPI FFCR: EnFCont Position */
#define TPI1_FFCR_EnFCont_Msk               (0x1UL << TPI1_FFCR_EnFCont_Pos)             /*!< TPI FFCR: EnFCont Mask */

/* TPI TRIGGER Register Definitions */
#define TPI1_TRIGGER_TRIGGER_Pos             0U                                         /*!< TPI TRIGGER: TRIGGER Position */
#define TPI1_TRIGGER_TRIGGER_Msk            (0x1UL /*<< TPI1_TRIGGER_TRIGGER_Pos*/)      /*!< TPI TRIGGER: TRIGGER Mask */

/* TPI Integration ETM Data Register Definitions (FIFO0) */
#define TPI1_FIFO0_ITM_ATVALID_Pos          29U                                         /*!< TPI FIFO0: ITM_ATVALID Position */
#define TPI1_FIFO0_ITM_ATVALID_Msk          (0x3UL << TPI1_FIFO0_ITM_ATVALID_Pos)        /*!< TPI FIFO0: ITM_ATVALID Mask */

#define TPI1_FIFO0_ITM_bytecount_Pos        27U                                         /*!< TPI FIFO0: ITM_bytecount Position */
#define TPI1_FIFO0_ITM_bytecount_Msk        (0x3UL << TPI1_FIFO0_ITM_bytecount_Pos)      /*!< TPI FIFO0: ITM_bytecount Mask */

#define TPI1_FIFO0_ETM_ATVALID_Pos          26U                                         /*!< TPI FIFO0: ETM_ATVALID Position */
#define TPI1_FIFO0_ETM_ATVALID_Msk          (0x3UL << TPI1_FIFO0_ETM_ATVALID_Pos)        /*!< TPI FIFO0: ETM_ATVALID Mask */

#define TPI1_FIFO0_ETM_bytecount_Pos        24U                                         /*!< TPI FIFO0: ETM_bytecount Position */
#define TPI1_FIFO0_ETM_bytecount_Msk        (0x3UL << TPI1_FIFO0_ETM_bytecount_Pos)      /*!< TPI FIFO0: ETM_bytecount Mask */

#define TPI1_FIFO0_ETM2_Pos                 16U                                         /*!< TPI FIFO0: ETM2 Position */
#define TPI1_FIFO0_ETM2_Msk                 (0xFFUL << TPI1_FIFO0_ETM2_Pos)              /*!< TPI FIFO0: ETM2 Mask */

#define TPI1_FIFO0_ETM1_Pos                  8U                                         /*!< TPI FIFO0: ETM1 Position */
#define TPI1_FIFO0_ETM1_Msk                 (0xFFUL << TPI1_FIFO0_ETM1_Pos)              /*!< TPI FIFO0: ETM1 Mask */

#define TPI1_FIFO0_ETM0_Pos                  0U                                         /*!< TPI FIFO0: ETM0 Position */
#define TPI1_FIFO0_ETM0_Msk                 (0xFFUL /*<< TPI1_FIFO0_ETM0_Pos*/)          /*!< TPI FIFO0: ETM0 Mask */

/* TPI ITATBCTR2 Register Definitions */
#define TPI1_ITATBCTR2_ATREADY_Pos           0U                                         /*!< TPI ITATBCTR2: ATREADY Position */
#define TPI1_ITATBCTR2_ATREADY_Msk          (0x1UL /*<< TPI1_ITATBCTR2_ATREADY_Pos*/)    /*!< TPI ITATBCTR2: ATREADY Mask */

/* TPI Integration ITM Data Register Definitions (FIFO1) */
#define TPI1_FIFO1_ITM_ATVALID_Pos          29U                                         /*!< TPI FIFO1: ITM_ATVALID Position */
#define TPI1_FIFO1_ITM_ATVALID_Msk          (0x3UL << TPI1_FIFO1_ITM_ATVALID_Pos)        /*!< TPI FIFO1: ITM_ATVALID Mask */

#define TPI1_FIFO1_ITM_bytecount_Pos        27U                                         /*!< TPI FIFO1: ITM_bytecount Position */
#define TPI1_FIFO1_ITM_bytecount_Msk        (0x3UL << TPI1_FIFO1_ITM_bytecount_Pos)      /*!< TPI FIFO1: ITM_bytecount Mask */

#define TPI1_FIFO1_ETM_ATVALID_Pos          26U                                         /*!< TPI FIFO1: ETM_ATVALID Position */
#define TPI1_FIFO1_ETM_ATVALID_Msk          (0x3UL << TPI1_FIFO1_ETM_ATVALID_Pos)        /*!< TPI FIFO1: ETM_ATVALID Mask */

#define TPI1_FIFO1_ETM_bytecount_Pos        24U                                         /*!< TPI FIFO1: ETM_bytecount Position */
#define TPI1_FIFO1_ETM_bytecount_Msk        (0x3UL << TPI1_FIFO1_ETM_bytecount_Pos)      /*!< TPI FIFO1: ETM_bytecount Mask */

#define TPI1_FIFO1_ITM2_Pos                 16U                                         /*!< TPI FIFO1: ITM2 Position */
#define TPI1_FIFO1_ITM2_Msk                 (0xFFUL << TPI1_FIFO1_ITM2_Pos)              /*!< TPI FIFO1: ITM2 Mask */

#define TPI1_FIFO1_ITM1_Pos                  8U                                         /*!< TPI FIFO1: ITM1 Position */
#define TPI1_FIFO1_ITM1_Msk                 (0xFFUL << TPI1_FIFO1_ITM1_Pos)              /*!< TPI FIFO1: ITM1 Mask */

#define TPI1_FIFO1_ITM0_Pos                  0U                                         /*!< TPI FIFO1: ITM0 Position */
#define TPI1_FIFO1_ITM0_Msk                 (0xFFUL /*<< TPI1_FIFO1_ITM0_Pos*/)          /*!< TPI FIFO1: ITM0 Mask */

/* TPI ITATBCTR0 Register Definitions */
#define TPI1_ITATBCTR0_ATREADY_Pos           0U                                         /*!< TPI ITATBCTR0: ATREADY Position */
#define TPI1_ITATBCTR0_ATREADY_Msk          (0x1UL /*<< TPI1_ITATBCTR0_ATREADY_Pos*/)    /*!< TPI ITATBCTR0: ATREADY Mask */

/* TPI Integration Mode Control Register Definitions */
#define TPI1_ITCTRL_Mode_Pos                 0U                                         /*!< TPI ITCTRL: Mode Position */
#define TPI1_ITCTRL_Mode_Msk                (0x1UL /*<< TPI1_ITCTRL_Mode_Pos*/)          /*!< TPI ITCTRL: Mode Mask */

/* TPI DEVID Register Definitions */
#define TPI1_DEVID_NRZVALID_Pos             11U                                         /*!< TPI DEVID: NRZVALID Position */
#define TPI1_DEVID_NRZVALID_Msk             (0x1UL << TPI1_DEVID_NRZVALID_Pos)           /*!< TPI DEVID: NRZVALID Mask */

#define TPI1_DEVID_MANCVALID_Pos            10U                                         /*!< TPI DEVID: MANCVALID Position */
#define TPI1_DEVID_MANCVALID_Msk            (0x1UL << TPI1_DEVID_MANCVALID_Pos)          /*!< TPI DEVID: MANCVALID Mask */

#define TPI1_DEVID_PTINVALID_Pos             9U                                         /*!< TPI DEVID: PTINVALID Position */
#define TPI1_DEVID_PTINVALID_Msk            (0x1UL << TPI1_DEVID_PTINVALID_Pos)          /*!< TPI DEVID: PTINVALID Mask */

#define TPI1_DEVID_MinBufSz_Pos              6U                                         /*!< TPI DEVID: MinBufSz Position */
#define TPI1_DEVID_MinBufSz_Msk             (0x7UL << TPI1_DEVID_MinBufSz_Pos)           /*!< TPI DEVID: MinBufSz Mask */

#define TPI1_DEVID_AsynClkIn_Pos             5U                                         /*!< TPI DEVID: AsynClkIn Position */
#define TPI1_DEVID_AsynClkIn_Msk            (0x1UL << TPI1_DEVID_AsynClkIn_Pos)          /*!< TPI DEVID: AsynClkIn Mask */

#define TPI1_DEVID_NrTraceInput_Pos          0U                                         /*!< TPI DEVID: NrTraceInput Position */
#define TPI1_DEVID_NrTraceInput_Msk         (0x1FUL /*<< TPI1_DEVID_NrTraceInput_Pos*/)  /*!< TPI DEVID: NrTraceInput Mask */

/* TPI DEVTYPE Register Definitions */
#define TPI1_DEVTYPE_MajorType_Pos           4U                                         /*!< TPI DEVTYPE: MajorType Position */
#define TPI1_DEVTYPE_MajorType_Msk          (0xFUL << TPI1_DEVTYPE_MajorType_Pos)        /*!< TPI DEVTYPE: MajorType Mask */

#define TPI1_DEVTYPE_SubType_Pos             0U                                         /*!< TPI DEVTYPE: SubType Position */
#define TPI1_DEVTYPE_SubType_Msk            (0xFUL /*<< TPI1_DEVTYPE_SubType_Pos*/)      /*!< TPI DEVTYPE: SubType Mask */
//}}}
//{{{  TPI1_Type      0xE0040000
typedef struct {
  __IOM uint32_t SSPSR;                  /*!< Offset: 0x000 (R/ )  Supported Parallel Port Size Register */
  __IOM uint32_t CSPSR;                  /*!< Offset: 0x004 (R/W)  Current Parallel Port Size Register */
        uint32_t RESERVED0[2U];
  __IOM uint32_t ACPR;                   /*!< Offset: 0x010 (R/W)  Asynchronous Clock Prescaler Register */
        uint32_t RESERVED1[55U];
  __IOM uint32_t SPPR;                   /*!< Offset: 0x0F0 (R/W)  Selected Pin Protocol Register */
        uint32_t RESERVED2[131U];
  __IM  uint32_t FFSR;                   /*!< Offset: 0x300 (R/ )  Formatter and Flush Status Register */
  __IOM uint32_t FFCR;                   /*!< Offset: 0x304 (R/W)  Formatter and Flush Control Register */
  __IM  uint32_t FSCR;                   /*!< Offset: 0x308 (R/ )  Formatter Synchronization Counter Register */
        uint32_t RESERVED3[759U];
  __IM  uint32_t TRIGGER;                /*!< Offset: 0xEE8 (R/ )  TRIGGER */
  __IM  uint32_t FIFO0;                  /*!< Offset: 0xEEC (R/ )  Integration ETM Data */
  __IM  uint32_t ITATBCTR2;              /*!< Offset: 0xEF0 (R/ )  ITATBCTR2 */
        uint32_t RESERVED4[1U];
  __IM  uint32_t ITATBCTR0;              /*!< Offset: 0xEF8 (R/ )  ITATBCTR0 */
  __IM  uint32_t FIFO1;                  /*!< Offset: 0xEFC (R/ )  Integration ITM Data */
  __IOM uint32_t ITCTRL;                 /*!< Offset: 0xF00 (R/W)  Integration Mode Control */
        uint32_t RESERVED5[39U];
  __IOM uint32_t CLAIMSET;               /*!< Offset: 0xFA0 (R/W)  Claim tag set */
  __IOM uint32_t CLAIMCLR;               /*!< Offset: 0xFA4 (R/W)  Claim tag clear */
        uint32_t RESERVED7[8U];
  __IM  uint32_t DEVID;                  /*!< Offset: 0xFC8 (R/ )  TPIU_DEVID */
  __IM  uint32_t DEVTYPE;                /*!< Offset: 0xFCC (R/ )  TPIU_DEVTYPE */
  } TPI1_Type1;

#define TPI1_BASE       (0xE0040000UL)                            /*!< TPI Base Address */
#define TPI1            ((TPI1_Type*)       TPI_BASE)   /*!< TPI configuration struct */
//}}}

//{{{  ETM defines
#define ETM_CR_POWERDOWN                0x00000001
#define ETM_CR_MONITORCPRT              0x00000002
#define ETM_CR_TRACE_DATA               0x00000004
#define ETM_CR_TRACE_ADDR               0x00000008
#define ETM_CR_PORTSIZE_1BIT            0x00200000
#define ETM_CR_PORTSIZE_2BIT            0x00200010
#define ETM_CR_PORTSIZE_4BIT            0x00000000
#define ETM_CR_PORTSIZE_8BIT            0x00000010
#define ETM_CR_PORTSIZE_16BIT           0x00000020
#define ETM_CR_STALL_PROCESSOR          0x00000080
#define ETM_CR_BRANCH_OUTPUT            0x00000100
#define ETM_CR_DEBUGREQ                 0x00000200
#define ETM_CR_PROGRAMMING              0x00000400
#define ETM_CR_ETMEN                    0x00000800
#define ETM_CR_CYCLETRACE               0x00001000
#define ETM_CR_CONTEXTID_8BIT           0x00004000
#define ETM_CR_CONTEXTID_16BIT          0x00008000
#define ETM_CR_CONTEXTID_32BIT          0x0000C000
#define ETM_CR_CONTEXTID_8BIT           0x00004000
#define ETM_CR_PORTMODE_ONCHIP          0x00000000
#define ETM_CR_PORTMODE_2_1             0x00010000
#define ETM_CR_PORTMODE_IMPL            0x00030000
#define ETM_CR_PORTMODE_1_1             0x00002000
#define ETM_CR_PORTMODE_1_2             0x00022000
#define ETM_CR_PORTMODE_1_3             0x00012000
#define ETM_CR_PORTMODE_1_4             0x00032000
#define ETM_CR_SUPPRESS_DATA            0x00040000
#define ETM_CR_FILTER_CPRT              0x00080000
#define ETM_CR_DATA_ONLY                0x00100000
#define ETM_CR_BLOCK_DEBUGGER           0x00400000
#define ETM_CR_BLOCK_SOFTWARE           0x00800000
#define ETM_CR_ACCESS                   0x01000000
#define ETM_CR_PROCSEL_Pos              25
#define ETM_CR_TIMESTAMP                0x10000000
#define ETM_CR_VMID                     0x40000000

#define ETM_SR_PROGSTATUS               0x00000002
#define ETM_SR_TRIGSTATUS               0x00000008

#define ETM_TECR1_EXCLUDE               0x01000000
#define ETM_TECR1_TSSEN                 0x02000000

#define ETM_FFRR_EXCLUDE                0x01000000

#define ETM_LAR_KEY                     0xC5ACCE55

#define ETM_TraceMode() ETM->CR &= ~ETM_CR_PROGRAMMING
#define ETM_SetupMode() ETM->CR |= ETM_CR_PROGRAMMING
//}}}
//{{{  ETM_Type       0xE0041000
// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ihi0014q/Chdfiagc.html
// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0337i/CHDBGEED.html
// - LAR:       Allow write access to other ETM registers
// - CR:        Enable/disable tracing
// - TRIGGER:   Select tracing trigger event
// - SR:        Current status
// - TECR1      Select areas of code where to enable trace
// - TECR2      Select comparator for trace enable
// - TEEVR      Select event for trace enable

typedef struct {
    __IO uint32_t CR;           /* Main Control Register */
    __IO uint32_t CCR;          /* Configuration Code Register */
    __IO uint32_t TRIGGER;      /* Trigger Event */
    __IO uint32_t ASICCR;       /* ASIC Control Register */
    __IO uint32_t SR;           /* ETM Status Register */
    __IO uint32_t SCR;          /* System Configuration Register */
    __IO uint32_t TSSCR;        /* TraceEnable Start/Stop Control Register */
    __IO uint32_t TECR2;        /* TraceEnable Control 2 */
    __IO uint32_t TEEVR;        /* TraceEnable Event Register */
    __IO uint32_t TECR1;        /* TraceEnable Control 1 */
    __IO uint32_t FFRR;         /* FIFOFULL Region Register */
    __IO uint32_t FFLR;         /* FIFOFULL Level Register */
    __IO uint32_t VDEVR;        /* ViewData Event Register */
    __IO uint32_t VDCR1;        /* ViewData Control 1 */
    __IO uint32_t VDCR2;        /* ViewData Control 2 */
    __IO uint32_t VDCR3;        /* ViewData Control 3 */
    __IO uint32_t ACVR[16];     /* Address Comparator Value Registers */
    __IO uint32_t ACTR[16];     /* Address Comparator Access Type Registers */
    __IO uint32_t DCVR[16];     /* Data Comparator Value Registers */
    __IO uint32_t DCMR[16];     /* Data Comparator Mask Registers */
    __IO uint32_t CNTRLDVR[4];  /* Counter Reload Value Registers */
    __IO uint32_t CNTENR[4];    /* Counter Enable Registers */
    __IO uint32_t CNTRLDEVR[4]; /* Counter Reload Event Registers */
    __IO uint32_t CNTVR[4];     /* Counter Value Registers */
    __IO uint32_t SQabEVR[6];   /* Sequencer State Transition Event Registers */
    __IO uint32_t RESERVED0;
    __IO uint32_t SQR;          /* Current Sequencer State Register */
    __IO uint32_t EXTOUTEVR[4]; /* External Output Event Registers */
    __IO uint32_t CIDCVR[3];    /* Context ID Comparator Value Registers */
    __IO uint32_t CIDCMR;       /* Context ID Comparator Mask Register */
    __IO uint32_t IMPL[8];      /* Implementation specific registers */
    __IO uint32_t SYNCFR;       /* Synchronization Frequency Register */
    __IO uint32_t IDR;          /* ETM ID Register */
    __IO uint32_t CCER;         /* Configuration Code Extension Register */
    __IO uint32_t EXTINSELR;    /* Extended External Input Selection Register */
    __IO uint32_t TESSEICR;     /* TraceEnable Start/Stop EmbeddedICE Control Register */
    __IO uint32_t EIBCR;        /* EmbeddedICE Behavior Control Register */
    __IO uint32_t TSEVR;        /* Timestamp Event Register, ETMv3.5 */
    __IO uint32_t AUXCR;        /* Auxiliary Control Register, ETMv3.5 */
    __IO uint32_t TRACEIDR;     /* CoreSight Trace ID Register */
    __IO uint32_t RESERVED1;
    __IO uint32_t IDR2;         /* ETM ID Register 2 */
    __IO uint32_t RESERVED2[13];
    __IO uint32_t VMIDCVR;      /* VMID Comparator Value Register, ETMv3.5 */
    __IO uint32_t RESERVED3[47];
    __IO uint32_t OSLAR;        /* OS Lock Access Register */
    __IO uint32_t OSLSR;        /* OS Lock Status Register */
    __IO uint32_t OSSRR;        /* OS Save and Restore Register */
    __IO uint32_t RESERVED4;
    __IO uint32_t PDCR;         /* Power Down Control Register, ETMv3.5 */
    __IO uint32_t PDSR;         /* Device Power-Down Status Register */
    __IO uint32_t RESERVED5[762];
    __IO uint32_t ITCTRL;       /* Integration Mode Control Register */
    __IO uint32_t RESERVED6[39];
    __IO uint32_t CLAIMSET;     /* Claim Tag Set Register */
    __IO uint32_t CLAIMCLR;     /* Claim Tag Clear Register */
    __IO uint32_t RESERVED7[2];
    __IO uint32_t LAR;          /* Lock Access Register */
    __IO uint32_t LSR;          /* Lock Status Register */
    __IO uint32_t AUTHSTATUS;   /* Authentication Status Register */
    __IO uint32_t RESERVED8[3];
    __IO uint32_t DEVID;        /* CoreSight Device Configuration Register */
    __IO uint32_t DEVTYPE;      /* CoreSight Device Type Register */
    __IO uint32_t PIDR4;        /* Peripheral ID4 */
    __IO uint32_t PIDR5;        /* Peripheral ID5 */
    __IO uint32_t PIDR6;        /* Peripheral ID6 */
    __IO uint32_t PIDR7;        /* Peripheral ID7 */
    __IO uint32_t PIDR0;        /* Peripheral ID0 */
    __IO uint32_t PIDR1;        /* Peripheral ID1 */
    __IO uint32_t PIDR2;        /* Peripheral ID2 */
    __IO uint32_t PIDR3;        /* Peripheral ID3 */
    __IO uint32_t CIDR0;        /* Component ID0 */
    __IO uint32_t CIDR1;        /* Component ID1 */
    __IO uint32_t CIDR2;        /* Component ID2 */
    __IO uint32_t CIDR3;        /* Component ID3 */
  } ETM_Type;

#define ETM_BASE 0xE0041000
#define ETM ((ETM_Type*)ETM_BASE)
//}}}

//{{{  DBGMCU defines
/********************  Bit definition for DBGMCU1_IDCODE register  *************/
#define DBGMCU1_IDCODE_DEV_ID_Pos                     (0U)
#define DBGMCU1_IDCODE_DEV_ID_Msk                     (0xFFFU << DBGMCU1_IDCODE_DEV_ID_Pos) /*!< 0x00000FFF */
#define DBGMCU1_IDCODE_DEV_ID                         DBGMCU1_IDCODE_DEV_ID_Msk
#define DBGMCU1_IDCODE_REV_ID_Pos                     (16U)
#define DBGMCU1_IDCODE_REV_ID_Msk                     (0xFFFFU << DBGMCU1_IDCODE_REV_ID_Pos) /*!< 0xFFFF0000 */
#define DBGMCU1_IDCODE_REV_ID                         DBGMCU1_IDCODE_REV_ID_Msk

/********************  Bit definition for DBGMCU1_CR register  *****************/
#define DBGMCU1_CR_DBG_SLEEP_Pos                      (0U)
#define DBGMCU1_CR_DBG_SLEEP_Msk                      (0x1U << DBGMCU1_CR_DBG_SLEEP_Pos) /*!< 0x00000001 */
#define DBGMCU1_CR_DBG_SLEEP                          DBGMCU1_CR_DBG_SLEEP_Msk
#define DBGMCU1_CR_DBG_STOP_Pos                       (1U)
#define DBGMCU1_CR_DBG_STOP_Msk                       (0x1U << DBGMCU1_CR_DBG_STOP_Pos) /*!< 0x00000002 */
#define DBGMCU1_CR_DBG_STOP                           DBGMCU1_CR_DBG_STOP_Msk
#define DBGMCU1_CR_DBG_STANDBY_Pos                    (2U)
#define DBGMCU1_CR_DBG_STANDBY_Msk                    (0x1U << DBGMCU1_CR_DBG_STANDBY_Pos) /*!< 0x00000004 */
#define DBGMCU1_CR_DBG_STANDBY                        DBGMCU1_CR_DBG_STANDBY_Msk
#define DBGMCU1_CR_TRACE_IOEN_Pos                     (5U)
#define DBGMCU1_CR_TRACE_IOEN_Msk                     (0x1U << DBGMCU1_CR_TRACE_IOEN_Pos) /*!< 0x00000020 */
#define DBGMCU1_CR_TRACE_IOEN                         DBGMCU1_CR_TRACE_IOEN_Msk

#define DBGMCU1_CR_TRACE_MODE_Pos                     (6U)
#define DBGMCU1_CR_TRACE_MODE_Msk                     (0x3U << DBGMCU1_CR_TRACE_MODE_Pos) /*!< 0x000000C0 */
#define DBGMCU1_CR_TRACE_MODE                         DBGMCU1_CR_TRACE_MODE_Msk
#define DBGMCU1_CR_TRACE_MODE_0                       (0x1U << DBGMCU1_CR_TRACE_MODE_Pos) /*!< 0x00000040 */
#define DBGMCU1_CR_TRACE_MODE_1                       (0x2U << DBGMCU1_CR_TRACE_MODE_Pos) /*!< 0x00000080 */

/********************  Bit definition for DBGMCU1_APB1_FZ register  ************/
#define DBGMCU1_APB1_FZ_DBG_TIM2_STOP_Pos             (0U)
#define DBGMCU1_APB1_FZ_DBG_TIM2_STOP_Msk             (0x1U << DBGMCU1_APB1_FZ_DBG_TIM2_STOP_Pos) /*!< 0x00000001 */
#define DBGMCU1_APB1_FZ_DBG_TIM2_STOP                 DBGMCU1_APB1_FZ_DBG_TIM2_STOP_Msk
#define DBGMCU1_APB1_FZ_DBG_TIM3_STOP_Pos             (1U)
#define DBGMCU1_APB1_FZ_DBG_TIM3_STOP_Msk             (0x1U << DBGMCU1_APB1_FZ_DBG_TIM3_STOP_Pos) /*!< 0x00000002 */
#define DBGMCU1_APB1_FZ_DBG_TIM3_STOP                 DBGMCU1_APB1_FZ_DBG_TIM3_STOP_Msk
#define DBGMCU1_APB1_FZ_DBG_TIM4_STOP_Pos             (2U)
#define DBGMCU1_APB1_FZ_DBG_TIM4_STOP_Msk             (0x1U << DBGMCU1_APB1_FZ_DBG_TIM4_STOP_Pos) /*!< 0x00000004 */
#define DBGMCU1_APB1_FZ_DBG_TIM4_STOP                 DBGMCU1_APB1_FZ_DBG_TIM4_STOP_Msk
#define DBGMCU1_APB1_FZ_DBG_TIM5_STOP_Pos             (3U)
#define DBGMCU1_APB1_FZ_DBG_TIM5_STOP_Msk             (0x1U << DBGMCU1_APB1_FZ_DBG_TIM5_STOP_Pos) /*!< 0x00000008 */
#define DBGMCU1_APB1_FZ_DBG_TIM5_STOP                 DBGMCU1_APB1_FZ_DBG_TIM5_STOP_Msk
#define DBGMCU1_APB1_FZ_DBG_TIM6_STOP_Pos             (4U)
#define DBGMCU1_APB1_FZ_DBG_TIM6_STOP_Msk             (0x1U << DBGMCU1_APB1_FZ_DBG_TIM6_STOP_Pos) /*!< 0x00000010 */
#define DBGMCU1_APB1_FZ_DBG_TIM6_STOP                 DBGMCU1_APB1_FZ_DBG_TIM6_STOP_Msk
#define DBGMCU1_APB1_FZ_DBG_TIM7_STOP_Pos             (5U)
#define DBGMCU1_APB1_FZ_DBG_TIM7_STOP_Msk             (0x1U << DBGMCU1_APB1_FZ_DBG_TIM7_STOP_Pos) /*!< 0x00000020 */
#define DBGMCU1_APB1_FZ_DBG_TIM7_STOP                 DBGMCU1_APB1_FZ_DBG_TIM7_STOP_Msk
#define DBGMCU1_APB1_FZ_DBG_TIM12_STOP_Pos            (6U)
#define DBGMCU1_APB1_FZ_DBG_TIM12_STOP_Msk            (0x1U << DBGMCU1_APB1_FZ_DBG_TIM12_STOP_Pos) /*!< 0x00000040 */
#define DBGMCU1_APB1_FZ_DBG_TIM12_STOP                DBGMCU1_APB1_FZ_DBG_TIM12_STOP_Msk
#define DBGMCU1_APB1_FZ_DBG_TIM13_STOP_Pos            (7U)
#define DBGMCU1_APB1_FZ_DBG_TIM13_STOP_Msk            (0x1U << DBGMCU1_APB1_FZ_DBG_TIM13_STOP_Pos) /*!< 0x00000080 */
#define DBGMCU1_APB1_FZ_DBG_TIM13_STOP                DBGMCU1_APB1_FZ_DBG_TIM13_STOP_Msk
#define DBGMCU1_APB1_FZ_DBG_TIM14_STOP_Pos            (8U)
#define DBGMCU1_APB1_FZ_DBG_TIM14_STOP_Msk            (0x1U << DBGMCU1_APB1_FZ_DBG_TIM14_STOP_Pos) /*!< 0x00000100 */
#define DBGMCU1_APB1_FZ_DBG_TIM14_STOP                DBGMCU1_APB1_FZ_DBG_TIM14_STOP_Msk
#define DBGMCU1_APB1_FZ_DBG_RTC_STOP_Pos              (10U)
#define DBGMCU1_APB1_FZ_DBG_RTC_STOP_Msk              (0x1U << DBGMCU1_APB1_FZ_DBG_RTC_STOP_Pos) /*!< 0x00000400 */
#define DBGMCU1_APB1_FZ_DBG_RTC_STOP                  DBGMCU1_APB1_FZ_DBG_RTC_STOP_Msk
#define DBGMCU1_APB1_FZ_DBG_WWDG_STOP_Pos             (11U)
#define DBGMCU1_APB1_FZ_DBG_WWDG_STOP_Msk             (0x1U << DBGMCU1_APB1_FZ_DBG_WWDG_STOP_Pos) /*!< 0x00000800 */
#define DBGMCU1_APB1_FZ_DBG_WWDG_STOP                 DBGMCU1_APB1_FZ_DBG_WWDG_STOP_Msk
#define DBGMCU1_APB1_FZ_DBG_IWDG_STOP_Pos             (12U)
#define DBGMCU1_APB1_FZ_DBG_IWDG_STOP_Msk             (0x1U << DBGMCU1_APB1_FZ_DBG_IWDG_STOP_Pos) /*!< 0x00001000 */
#define DBGMCU1_APB1_FZ_DBG_IWDG_STOP                 DBGMCU1_APB1_FZ_DBG_IWDG_STOP_Msk
#define DBGMCU1_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT_Pos    (21U)
#define DBGMCU1_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT_Msk    (0x1U << DBGMCU1_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT_Pos) /*!< 0x00200000 */
#define DBGMCU1_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT        DBGMCU1_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT_Msk
#define DBGMCU1_APB1_FZ_DBG_I2C2_SMBUS_TIMEOUT_Pos    (22U)
#define DBGMCU1_APB1_FZ_DBG_I2C2_SMBUS_TIMEOUT_Msk    (0x1U << DBGMCU1_APB1_FZ_DBG_I2C2_SMBUS_TIMEOUT_Pos) /*!< 0x00400000 */
#define DBGMCU1_APB1_FZ_DBG_I2C2_SMBUS_TIMEOUT        DBGMCU1_APB1_FZ_DBG_I2C2_SMBUS_TIMEOUT_Msk
#define DBGMCU1_APB1_FZ_DBG_I2C3_SMBUS_TIMEOUT_Pos    (23U)
#define DBGMCU1_APB1_FZ_DBG_I2C3_SMBUS_TIMEOUT_Msk    (0x1U << DBGMCU1_APB1_FZ_DBG_I2C3_SMBUS_TIMEOUT_Pos) /*!< 0x00800000 */
#define DBGMCU1_APB1_FZ_DBG_I2C3_SMBUS_TIMEOUT        DBGMCU1_APB1_FZ_DBG_I2C3_SMBUS_TIMEOUT_Msk
#define DBGMCU1_APB1_FZ_DBG_CAN1_STOP_Pos             (25U)
#define DBGMCU1_APB1_FZ_DBG_CAN1_STOP_Msk             (0x1U << DBGMCU1_APB1_FZ_DBG_CAN1_STOP_Pos) /*!< 0x02000000 */
#define DBGMCU1_APB1_FZ_DBG_CAN1_STOP                 DBGMCU1_APB1_FZ_DBG_CAN1_STOP_Msk
#define DBGMCU1_APB1_FZ_DBG_CAN2_STOP_Pos             (26U)
#define DBGMCU1_APB1_FZ_DBG_CAN2_STOP_Msk             (0x1U << DBGMCU1_APB1_FZ_DBG_CAN2_STOP_Pos) /*!< 0x04000000 */
#define DBGMCU1_APB1_FZ_DBG_CAN2_STOP                 DBGMCU1_APB1_FZ_DBG_CAN2_STOP_Msk
/* Old IWDGSTOP bit definition, maintained for legacy purpose */
#define  DBGMCU1_APB1_FZ_DBG_IWDEG_STOP           DBGMCU1_APB1_FZ_DBG_IWDG_STOP

/********************  Bit definition for DBGMCU1_APB2_FZ register  ************/
#define DBGMCU1_APB2_FZ_DBG_TIM1_STOP_Pos             (0U)
#define DBGMCU1_APB2_FZ_DBG_TIM1_STOP_Msk             (0x1U << DBGMCU1_APB2_FZ_DBG_TIM1_STOP_Pos) /*!< 0x00000001 */
#define DBGMCU1_APB2_FZ_DBG_TIM1_STOP                 DBGMCU1_APB2_FZ_DBG_TIM1_STOP_Msk
#define DBGMCU1_APB2_FZ_DBG_TIM8_STOP_Pos             (1U)
#define DBGMCU1_APB2_FZ_DBG_TIM8_STOP_Msk             (0x1U << DBGMCU1_APB2_FZ_DBG_TIM8_STOP_Pos) /*!< 0x00000002 */
#define DBGMCU1_APB2_FZ_DBG_TIM8_STOP                 DBGMCU1_APB2_FZ_DBG_TIM8_STOP_Msk
#define DBGMCU1_APB2_FZ_DBG_TIM9_STOP_Pos             (16U)
#define DBGMCU1_APB2_FZ_DBG_TIM9_STOP_Msk             (0x1U << DBGMCU1_APB2_FZ_DBG_TIM9_STOP_Pos) /*!< 0x00010000 */
#define DBGMCU1_APB2_FZ_DBG_TIM9_STOP                 DBGMCU1_APB2_FZ_DBG_TIM9_STOP_Msk
#define DBGMCU1_APB2_FZ_DBG_TIM10_STOP_Pos            (17U)
#define DBGMCU1_APB2_FZ_DBG_TIM10_STOP_Msk            (0x1U << DBGMCU1_APB2_FZ_DBG_TIM10_STOP_Pos) /*!< 0x00020000 */
#define DBGMCU1_APB2_FZ_DBG_TIM10_STOP                DBGMCU1_APB2_FZ_DBG_TIM10_STOP_Msk
#define DBGMCU1_APB2_FZ_DBG_TIM11_STOP_Pos            (18U)
#define DBGMCU1_APB2_FZ_DBG_TIM11_STOP_Msk            (0x1U << DBGMCU1_APB2_FZ_DBG_TIM11_STOP_Pos) /*!< 0x00040000 */
#define DBGMCU1_APB2_FZ_DBG_TIM11_STOP                DBGMCU1_APB2_FZ_DBG_TIM11_STOP_Msk
//}}}
//{{{  DBGMCU         0xE0042000
#define DBGMCU1_BASE        0xE0042000U
#define DBGMCU1              ((DBGMCU1_TypeDef *) DBGMCU_BASE)
typedef struct {
  __IO uint32_t IDCODE;  /*!< MCU device ID code,               Address offset: 0x00 */
  __IO uint32_t CR;      /*!< Debug MCU configuration register, Address offset: 0x04 */
  __IO uint32_t APB1FZ;  /*!< Debug MCU APB1 freeze register,   Address offset: 0x08 */
  __IO uint32_t APB2FZ;  /*!< Debug MCU APB2 freeze register,   Address offset: 0x0C */
  }DBGMCU1_TypeDef;
//}}}
//}}}

cLcd* lcd = nullptr;

UART_HandleTypeDef DebugUartHandle;
//{{{
void initDebugUart() {

  __HAL_RCC_USART1_FORCE_RESET();
  __HAL_RCC_USART1_RELEASE_RESET();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_USART1_CLK_ENABLE();

  // PA9 - USART1 tx pin configuration
  GPIO_InitTypeDef  GPIO_InitStruct;
  GPIO_InitStruct.Pin       = GPIO_PIN_9;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init (GPIOA, &GPIO_InitStruct);

  // PA10 - USART1 rx pin configuration
  //GPIO_InitStruct.Pin = GPIO_PIN_10;
  //HAL_GPIO_Init (GPIOA, &GPIO_InitStruct);

  // 8 Bits, One Stop bit, Parity = None, RTS,CTS flow control
  DebugUartHandle.Instance   = USART1;
  DebugUartHandle.Init.BaudRate   = 230400;
  DebugUartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  DebugUartHandle.Init.StopBits   = UART_STOPBITS_1;
  DebugUartHandle.Init.Parity     = UART_PARITY_NONE;
  DebugUartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  DebugUartHandle.Init.Mode       = UART_MODE_TX;
  //DebugUartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init (&DebugUartHandle);
  }
//}}}

//{{{  system
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
uint32_t SystemCoreClock = 16000000;
int globalCounter = 0;

//{{{
void configureSWO() {

  DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN; // Enable IO trace pins
  if (!(DBGMCU->CR & DBGMCU_CR_TRACE_IOEN))
    return;

  // default 64k baud rate
  // SWOSpeed in Hz, note that cpuCoreFreqHz is expected to be match the CPU core clock
  //uint32_t SWOSpeed = 64000;
  uint32_t SWOSpeed = 2000000;
  uint32_t cpuCoreFreqHz = 180000000;
  uint32_t SWOPrescaler = (cpuCoreFreqHz / SWOSpeed) - 1;

  // enable trace in core debug
  CoreDebug->DEMCR = CoreDebug_DEMCR_TRCENA_Msk;

  // TPI SPPR - Selected PIN Protocol Register =  protocol for trace output 2: SWO NRZ, 1: SWO Manchester encoding
  TPI->SPPR = 0x00000002;

  // TPI Async Clock Prescaler Register =  Scale the baud rate of the asynchronous output
  TPI->ACPR = SWOPrescaler;

  // ITM Lock Access Register = ITM_LAR_KEY = C5ACCE55 enables write access to Control Register 0xE00 :: 0xFFC
  ITM->LAR = ITM_LAR_KEY;

  // ITM Trace Control Register
  ITM->TCR = ITM_TCR_TraceBusID_Msk | ITM_TCR_SWOENA_Msk | ITM_TCR_SYNCENA_Msk | ITM_TCR_ITMENA_Msk;

  // ITM Trace Privilege Register =
  ITM->TPR = ITM_TPR_PRIVMASK_Msk;

  // ITM Trace Enable Register =  Enabled tracing on stimulus ports. One bit per stimulus port
  ITM->TER = 0xFFFFFFFF;

  // DWT Control Register =
  //*((volatile unsigned*)(ITM_BASE + 0x01000)) = 0x400003FE;
  DWT->CTRL = 0x400003FE;

  // TPI Formatter and Flush Control Register =
  //*((volatile unsigned*)(ITM_BASE + 0x40304)) = 0x00000100;
  TPI->FFCR = 0x00000100;
  }
//}}}
//{{{
void configureDtrace1() {

  __HAL_RCC_GPIOE_CLK_ENABLE();
  //*((uint32_t*)(0x40023830)) |= 0x00000010;
  *((uint32_t*)(0x40021000)) = 0x00002AA0;    // GPIOE_MODER:   PE2..PE6 = Alternate function mode
  *((uint32_t*)(0x40021008)) = 0x00003FF0;    // GPIOE_OSPEEDR: PE2..PE6 = 100 MHz speed
  *((uint32_t*)(0x4002100C)) = 0x00000000;    // GPIOE_PUPDR:   PE2..PE6 = No Pull-up/Pull-down
  *((uint32_t*)(0x40021020)) = 0x00000000;    // GPIOE_AFRL:    PE2..PE6 = AF0

  //DBGMCU->CR = DBGMCU_CR_TRACE_IOEN | DBGMCU1_CR_TRACE_MODE_0 | DBGMCU1_CR_TRACE_MODE_1; // Enable IO trace pins
  //DBGMCU->CR = 0xE0;
  DBGMCU->CR = 0x60;
  if (!(DBGMCU->CR & DBGMCU_CR_TRACE_IOEN))
    return;

  // default 64k baud rate
  // SWOSpeed in Hz, note that cpuCoreFreqHz is expected to be match the CPU core clock
  //uint32_t SWOSpeed = 64000;
  uint32_t SWOSpeed = 2000000;
  uint32_t cpuCoreFreqHz = 168000000;
  uint32_t SWOPrescaler = (cpuCoreFreqHz / SWOSpeed) - 1;

  // enable trace in core debug
  CoreDebug->DEMCR = CoreDebug_DEMCR_TRCENA_Msk;

  // TPI SPPR - Selected PIN Protocol Register =  protocol for trace output 2: SWO NRZ, 1: SWO Manchester encoding
  //TPI->SPPR = 0x00000002;
  TPI->SPPR = 0x00000000;  // sync
  //TPI->CSPSR = 0x08;       // 4 pins TRACED[0..3], PE2 clk, PE3,4,5,6 data
  TPI->CSPSR = 0x01;       // 4 pins TRACED[0..3], PE2 clk, PE3,4,5,6 data

  // TPI Async Clock Prescaler Register =  Scale the baud rate of the asynchronous output
  TPI->ACPR = SWOPrescaler;

  // ITM Lock Access Register = ITM_LAR_KEY = C5ACCE55 enables write access to Control Register 0xE00 :: 0xFFC
  ITM->LAR = ITM_LAR_KEY;

  // ITM Trace Control Register
  ITM->TCR = ITM_TCR_TraceBusID_Msk | ITM_TCR_SWOENA_Msk | ITM_TCR_SYNCENA_Msk | ITM_TCR_ITMENA_Msk;

  // ITM Trace Privilege Register =
  ITM->TPR = ITM_TPR_PRIVMASK_Msk;

  // ITM Trace Enable Register =  Enabled tracing on stimulus ports. One bit per stimulus port
  ITM->TER = 0xFFFFFFFF;

  // DWT Control Register =
  //*((volatile unsigned*)(ITM_BASE + 0x01000)) = 0x400003FE;
  DWT->CTRL = 0x400003FE;

  // TPI Formatter and Flush Control Register =
  //*((volatile unsigned*)(ITM_BASE + 0x40304)) = 0x00000100;
  TPI->FFCR = 0x00000102;
}
//}}}
//{{{
void configureDtrace4() {

  __HAL_RCC_GPIOE_CLK_ENABLE();
  //*((uint32_t*)(0x40023830)) |= 0x00000010;
  *((uint32_t*)(0x40021000)) = 0x00002AA0;    // GPIOE_MODER:   PE2..PE6 = Alternate function mode
  *((uint32_t*)(0x40021008)) = 0x00003FF0;    // GPIOE_OSPEEDR: PE2..PE6 = 100 MHz speed
  *((uint32_t*)(0x4002100C)) = 0x00000000;    // GPIOE_PUPDR:   PE2..PE6 = No Pull-up/Pull-down
  *((uint32_t*)(0x40021020)) = 0x00000000;    // GPIOE_AFRL:    PE2..PE6 = AF0

  //DBGMCU->CR = DBGMCU_CR_TRACE_IOEN | DBGMCU1_CR_TRACE_MODE_0 | DBGMCU1_CR_TRACE_MODE_1; // Enable IO trace pins
  DBGMCU->CR = 0xE0;
  //DBGMCU->CR = 0x60;
  if (!(DBGMCU->CR & DBGMCU_CR_TRACE_IOEN))
    return;

  // default 64k baud rate
  // SWOSpeed in Hz, note that cpuCoreFreqHz is expected to be match the CPU core clock
  //uint32_t SWOSpeed = 64000;
  uint32_t SWOSpeed = 2000000;
  uint32_t cpuCoreFreqHz = 180000000;
  uint32_t SWOPrescaler = (cpuCoreFreqHz / SWOSpeed) - 1;

  // enable trace in core debug
  CoreDebug->DEMCR = CoreDebug_DEMCR_TRCENA_Msk;

  // TPI SPPR - Selected PIN Protocol Register =  protocol for trace output 2: SWO NRZ, 1: SWO Manchester encoding
  //TPI->SPPR = 0x00000002;
  TPI->SPPR = 0x00000000;  // sync
  TPI->CSPSR = 0x08;       // 4 pins TRACED[0..3], PE2 clk, PE3,4,5,6 data
  //TPI->CSPSR = 0x01;       // 4 pins TRACED[0..3], PE2 clk, PE3,4,5,6 data

  // TPI Async Clock Prescaler Register =  Scale the baud rate of the asynchronous output
  TPI->ACPR = SWOPrescaler;

  // ITM Lock Access Register = ITM_LAR_KEY = C5ACCE55 enables write access to Control Register 0xE00 :: 0xFFC
  ITM->LAR = ITM_LAR_KEY;

  // ITM Trace Control Register
  ITM->TCR = ITM_TCR_TraceBusID_Msk | ITM_TCR_SWOENA_Msk | ITM_TCR_SYNCENA_Msk | ITM_TCR_ITMENA_Msk;

  // ITM Trace Privilege Register =
  ITM->TPR = ITM_TPR_PRIVMASK_Msk;

  // ITM Trace Enable Register =  Enabled tracing on stimulus ports. One bit per stimulus port
  ITM->TER = 0xFFFFFFFF;

  // DWT Control Register =
  //*((volatile unsigned*)(ITM_BASE + 0x01000)) = 0x400003FE;
  DWT->CTRL = 0x400003FE;

  // TPI Formatter and Flush Control Register =
  //*((volatile unsigned*)(ITM_BASE + 0x40304)) = 0x00000100;
  TPI->FFCR = 0x00000102;
}
//}}}
//{{{
void configureTracing() {
//#define ETM_TraceMode() ETM->CR &= ~ETM_CR_PROGRAMMING
//#define ETM_SetupMode() ETM->CR |= ETM_CR_PROGRAMMING

  DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN; // Enable IO trace pins
  if (!(DBGMCU->CR & DBGMCU_CR_TRACE_IOEN))
    return;

  // default 64k baud rate
  // SWOSpeed in Hz, note that cpuCoreFreqHz is expected to be match the CPU core clock
  //uint32_t SWOSpeed = 64000;
  uint32_t SWOSpeed = 8000000;
  uint32_t cpuCoreFreqHz = 168000000;
  uint32_t SWOPrescaler = (cpuCoreFreqHz / SWOSpeed) - 1;

  // Configure Trace Port Interface Unit */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // Enable access to registers
  TPI->ACPR = SWOPrescaler;
  TPI->SPPR = 2;     // Pin protocol = NRZ/USART
  TPI->FFCR = 0x102; // TPIU packet framing enabled when bit 2 set,  0x100 only DWT/ITM and not ETM.

  // Configure PC sampling and exception trace
  DWT->CTRL = (1 << DWT_CTRL_CYCTAP_Pos) |     // Prescaler for PC sampling, 0 = x32, 1 = x512
              (0 << DWT_CTRL_POSTPRESET_Pos) | // Postscaler for PC sampling, Divider = value + 1
              (1 << DWT_CTRL_PCSAMPLENA_Pos) | // Enable PC sampling
              (2 << DWT_CTRL_SYNCTAP_Pos) |    // Sync packet interval 0=Off, 1=2^23 cycles, 2=2^25, 3=2^27
              (1 << DWT_CTRL_EXCTRCENA_Pos) |  // Enable exception trace
              (1 << DWT_CTRL_CYCCNTENA_Pos);   // Enable cycle counter

  // Configure instrumentation trace macroblock
  ITM->LAR = 0xC5ACCE55;
  ITM->TCR = (1 << ITM_TCR_TraceBusID_Pos) | // Trace bus ID for TPIU
             (1 << ITM_TCR_DWTENA_Pos) |     // Enable events from DWT
             (1 << ITM_TCR_SYNCENA_Pos) |    // Enable sync packets
             (1 << ITM_TCR_ITMENA_Pos);      // Main enable for ITM
  ITM->TER = 0xFFFFFFFF; // Enable all stimulus ports

  // Configure embedded trace macroblock
  ETM->LAR = 0xC5ACCE55;
  ETM_SetupMode();
  ETM->CR = ETM_CR_ETMEN |           // Enable ETM output port
            ETM_CR_STALL_PROCESSOR | // Stall processor when fifo is full
            ETM_CR_BRANCH_OUTPUT;    // Report all branches
  ETM->TRACEIDR = 2;                 // Trace bus ID for TPIU
  ETM->TECR1 = ETM_TECR1_EXCLUDE;    // Trace always enabled
  ETM->FFRR = ETM_FFRR_EXCLUDE;      // Stalling always enabled
  ETM->FFLR = 24;                    // Stall <N bytes free FIFO (1..24), larger less latency, but more stalls
                                     // Note: we do not enable ETM trace yet, only for specific parts of code.
  }
//}}}
//{{{
void configureWatchpoint() {
// how to configure DWT to monitor a watchpoint, data value reported when watchpoint hit

  // Monitor all accesses to GPIOC (range length 32 bytes)
  DWT->COMP0 = (uint32_t)GPIOD;
  DWT->MASK0 = 5;
  DWT->FUNCTION0 = (2 << DWT_FUNCTION_FUNCTION_Pos) | // Report data and addr on watchpoint hit
                   (1 << DWT_FUNCTION_EMITRANGE_Pos);

  // Monitor all accesses to globalCounter (range length 4 bytes)
  DWT->COMP1 = (uint32_t)&globalCounter;
  DWT->MASK1 = 2;
  DWT->FUNCTION1 = (3 << DWT_FUNCTION_FUNCTION_Pos); // Report data and PC on watchpoint hit
  }
//}}}
//{{{
uint32_t my_ITM_SendChar (uint32_t port, uint32_t ch) {

  // if ITM enabled && ITM Port #0 enabled
  if (((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) && ((ITM->TER & 1UL) != 0UL)) {
    while (ITM->PORT[port].u32 == 0UL) {
      __NOP();
      }
    ITM->PORT[port].u8 = (uint8_t)ch;
    }
  return (ch);
  }
//}}}

//{{{
void SystemCoreClockUpdate() {

  uint32_t tmp = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;
  switch (tmp) {
    case 0x00:  /* HSI used as system clock source */
      SystemCoreClock = HSI_VALUE;
      break;

    case 0x04:  /* HSE used as system clock source */
      SystemCoreClock = HSE_VALUE;
      break;

    case 0x08:  /* PLL used as system clock source */
      /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N SYSCLK = PLL_VCO / PLL_P */
      pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
      pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;

      if (pllsource != 0) /* HSE used as PLL clock source */
        pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
      else /* HSI used as PLL clock source */
        pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);

      pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2;
      SystemCoreClock = pllvco/pllp;
      break;

    default:
      SystemCoreClock = HSI_VALUE;
      break;
    }

  /* Compute HCLK frequency --------------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];

  /* HCLK frequency */
  SystemCoreClock >>= tmp;
  }
//}}}
//{{{
void SystemInit() {

  // FPU settings
  SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  // set CP10 and CP11 Full Access

  // Reset the RCC clock configuration to the default reset state
  // Set HSION bit
  RCC->CR |= (uint32_t)0x00000001;

  // Reset CFGR register
  RCC->CFGR = 0x00000000;

  // Reset HSEON, CSSON and PLLON bits
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  // Reset PLLCFGR register
  RCC->PLLCFGR = 0x24003010;

  // Reset HSEBYP bit
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  // Disable all interrupts
  RCC->CIR = 0x00000000;

  // Configure the Vector Table location add offset address
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; // Vector Table Relocation in Internal SRAM
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; // Vector Table Relocation in Internal FLASH
#endif
}
//}}}

//{{{
void SystemClockConfig180() {
//  System Clock source            = PLL (HSE)
//    SYSCLK(Hz)                     = 180000000
//    HCLK(Hz)                       = 180000000
//    AHB Prescaler                  = 1
//    APB1 Prescaler                 = 4
//    APB2 Prescaler                 = 2
//    HSE Frequency(Hz)              = 8000000
//    PLL_M                          = 8
//    PLL_N                          = 360
//    PLL_P                          = 2
//    PLL_Q                          = 7
//    VDD(V)                         = 3.3
//    Main regulator output voltage  = Scale1 mode
//    Flash Latency(WS)              = 5

  // Enable Power Control clock
  __HAL_RCC_PWR_CLK_ENABLE();

  // The voltage scaling allows optimizing the power consumption when the device is
  // clocked below the maximum system frequency, to update the voltage scaling value
  // regarding system frequency refer to product datasheet.
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  // Enable HSE Oscillator and activate PLL with HSE as source
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig (&RCC_OscInitStruct);

  // Activate the Over-Drive mode
  HAL_PWREx_EnableOverDrive();

  // PLLSAI_VCO Input  = HSE_VALUE / PLL_M = 1mhz
  // PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN     = 130mhz
  // PLLLCDCLK         = PLLSAI_VCO Output / PLLSAIR    = 130/2 = 65mhz
  // LTDC clock        = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 65/2  = 32.5mhz
  RCC_PeriphCLKInitTypeDef rccPeriphClkInit;
  rccPeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  rccPeriphClkInit.PLLSAI.PLLSAIN = LTDC_CLOCK_4;  // hclk = 192mhz, 138/4 = 34.5mhz
  rccPeriphClkInit.PLLSAI.PLLSAIR = 2;
  rccPeriphClkInit.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  HAL_RCCEx_PeriphCLKConfig (&rccPeriphClkInit);

  //rccPeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SDIO | RCC_PERIPHCLK_CK48;
  //rccPeriphClkInit.SdioClockSelection = RCC_SDIOCLKSOURCE_CK48;
  //rccPeriphClkInit.Clk48ClockSelection = RCC_CK48CLKSOURCE_PLLSAIP;
  //rccPeriphClkInit.PLLSAI.PLLSAIN = 384;
  //rccPeriphClkInit.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
  //HAL_RCCEx_PeriphCLKConfig (&rccPeriphClkInit);

  // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig (&RCC_ClkInitStruct, FLASH_LATENCY_5);
  }
//}}}
//{{{
void memoryTest() {

  uint32_t readAddress = SDRAM_BANK1_ADDR;
  uint32_t phase = 0;
  while (1) {
    uint32_t len = (readAddress == SDRAM_BANK1_ADDR) ? SDRAM_BANK1_LEN : SDRAM_BANK2_LEN;
    uint32_t reportMask = 0xFFFFF;

    //  write
    for (uint32_t i = readAddress; i < readAddress + len; i++)
      *(uint8_t*)readAddress = (i+phase) & 0xFF;

    // read
    uint8_t readErr = 0;
    for (uint32_t i = readAddress; i < readAddress + len; i++) {
      uint8_t read = *(uint8_t*)readAddress;
      if (read != ((i+phase) & 0xFF)) {
        readErr++;
        //printf ("add:%x exp:%x got:%x\n", i, (i+phase) & 0xFF, read);
        }

      if ((i & reportMask) == reportMask) {
        if (readErr)
          printf ("add:%x err:%x rate:%d\n", (unsigned int)i, (unsigned int)readErr, (int)((readErr * 100) / reportMask));
        else
          printf ("add:%lx ok\n", i);

        if (readErr) { // red
          //BSP_LED_Off (LED3);
          //BSP_LED_On (LED4);
          readErr = 0;
          }
        else { // green
          //BSP_LED_Off (LED4);
          //BSP_LED_On (LED3);
          }
        }
      }

    phase++;
    readAddress = (readAddress == SDRAM_BANK1_ADDR) ? SDRAM_BANK2_ADDR : SDRAM_BANK1_ADDR;
    }
  }
//}}}
//}}}
//{{{  sdRam
//{{{
void sdramGpioInit() {
// Timing configuration 90 MHz SD clock frequency (180MHz/2)
//     PG08 <-> FMC_SDCLK
//     PC00 <-> FMC_SDNWE
//     PC02 <-> FMC_SDNE0  BANK1 address 0xC0000000        PB06 <-> FMC_SDNE1  BANK2 address 0xD0000000
//     PB05 <-> FMC_SDCKE1                                 PC03 <-> FMC_SDCKE0
// PD14..15 <-> FMC_D00..01    PF00..05 <-> FMC_A00..05    PE00 <-> FMC_NBL0
// PD00..01 <-> FMC_D02..03    PF12..15 <-> FMC_A06..09    PE01 <-> FMC_NBL1
// PE07..15 <-> FMC_D04..12    PG00..01 <-> FMC_A10..11    PG15 <-> FMC_NCAS
// PD08..10 <-> FMC_D13..15                                PF11 <-> FMC_NRAS

  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_FMC_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_Init_Structure;
  GPIO_Init_Structure.Mode = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_Init_Structure.Pull = GPIO_NOPULL;
  GPIO_Init_Structure.Alternate = GPIO_AF12_FMC;

  GPIO_Init_Structure.Pin = GPIO_PIN_5 | GPIO_PIN_6;
  HAL_GPIO_Init (GPIOB, &GPIO_Init_Structure);

  GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3;
  HAL_GPIO_Init (GPIOC, &GPIO_Init_Structure);

  GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_1  | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                            GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init (GPIOD, &GPIO_Init_Structure);

  GPIO_Init_Structure.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_7 | GPIO_PIN_8  | GPIO_PIN_9  |
                            GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init (GPIOE, &GPIO_Init_Structure);

  GPIO_Init_Structure.Pin = GPIO_PIN_0  | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3  | GPIO_PIN_4 | GPIO_PIN_5 |
                            GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init (GPIOF, &GPIO_Init_Structure);

  GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
  HAL_GPIO_Init (GPIOG, &GPIO_Init_Structure);
  }
//}}}
//{{{
void sdramBank1Init() {
// Micron MT48LC8M16A2 – 2m x 16bits x 4banks
// PC2, PC3 - bank 1 - 0xC0000000 : 0xC10000000 - len 0x1000000

  SDRAM_HandleTypeDef hsdram;
  hsdram.Instance = FMC_SDRAM_DEVICE;
  hsdram.Init.SDBank             = FMC_SDRAM_BANK1;
  hsdram.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_9; // 8,9,10,11
  hsdram.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;   // 11,12,13
  hsdram.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_16;  // 8,16,32
  hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_3;
  hsdram.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_3;
  //hsdram.Init.ReadBurst          = FMC_SDRAM_RBURST_DISABLE;
  hsdram.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
  hsdram.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_1;

  FMC_SDRAM_TimingTypeDef SDRAM_Timing;
  SDRAM_Timing.LoadToActiveDelay    = 2; // TMRD: 2 Clock cycles
  SDRAM_Timing.ExitSelfRefreshDelay = 7; // TXSR: min = 70ns (6 x 11.90ns)
  SDRAM_Timing.SelfRefreshTime      = 4; // TRAS: min = 42ns (4 x 11.90ns) max=120k (ns)
  SDRAM_Timing.RowCycleDelay        = 7; // TRC:  min = 63 (6 x 11.90ns)
  SDRAM_Timing.WriteRecoveryTime    = 2; // TWR:  2 Clock cycles
  SDRAM_Timing.RPDelay              = 2; // TRP:  15ns => 2 x 11.90ns
  SDRAM_Timing.RCDDelay             = 2; // TRCD: 15ns => 2 x 11.90ns

  if (HAL_SDRAM_Init (&hsdram, &SDRAM_Timing) != HAL_OK) {
    printf ("HAL_SDRAM_Init error\n");
    while (1) {}
    }

  // Configure clock configuration enable command
  FMC_SDRAM_CommandTypeDef Command;
  Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = 0;
  HAL_SDRAM_SendCommand (&hsdram, &Command, 0x1000);

  // Insert 100 ms delay
  HAL_Delay (100);

  // Configure PALL (precharge all) command
  Command.CommandMode = FMC_SDRAM_CMD_PALL;
  Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = 0;
  HAL_SDRAM_SendCommand (&hsdram, &Command, 0x1000);

  // Configure Auto-Refresh command
  Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber = 4;
  Command.ModeRegisterDefinition = 0;
  HAL_SDRAM_SendCommand (&hsdram, &Command, 0x1000);

  Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition =
    SDRAM_MODEREG_BURST_LENGTH_2 | SDRAM_MODEREG_CAS_LATENCY_3 | SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
  HAL_SDRAM_SendCommand (&hsdram, &Command, 0x1000);

  // Set refresh rate counter //* (15.62 us x Freq) - 20 - SDRAM refresh counter (90MHz SD clock)
  HAL_SDRAM_ProgramRefreshRate (&hsdram, 0x1386);
  //HAL_SDRAM_ProgramRefreshRate (&hsdram, 0x0569);
  }
//}}}
//{{{
void sdramBank2Init() {
// PB6, PB5 - Bank2 - 0xD0000000 : 0xD0800000 len 0x800000
  SDRAM_HandleTypeDef hsdram;
  hsdram.Instance = FMC_SDRAM_DEVICE;
  hsdram.Init.SDBank             = FMC_SDRAM_BANK2;
  hsdram.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_8;  // 8,9,10,11
  hsdram.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;    // 11,12,13
  hsdram.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_16;   // 8,16,32
  hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_3;
  hsdram.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_3;
  //hsdram.Init.ReadBurst          = FMC_SDRAM_RBURST_DISABLE;
  hsdram.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
  hsdram.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_1;

  FMC_SDRAM_TimingTypeDef SDRAM_Timing;
  SDRAM_Timing.LoadToActiveDelay    = 2; // TMRD: 2 Clock cycles
  SDRAM_Timing.ExitSelfRefreshDelay = 7; // TXSR: min = 70ns (6 x 11.90ns)
  SDRAM_Timing.SelfRefreshTime      = 4; // TRAS: min = 42ns (4 x 11.90ns) max=120k (ns)
  SDRAM_Timing.RowCycleDelay        = 7; // TRC:  min = 63 (6 x 11.90ns)
  SDRAM_Timing.WriteRecoveryTime    = 2; // TWR:  2 Clock cycles
  SDRAM_Timing.RPDelay              = 2; // TRP:  15ns => 2 x 11.90ns
  SDRAM_Timing.RCDDelay             = 2; // TRCD: 15ns => 2 x 11.90ns

  if (HAL_SDRAM_Init (&hsdram, &SDRAM_Timing) != HAL_OK) {
    printf ("HAL_SDRAM_Init error\n");
    while (1) {}
    }

  // Configure clock configuration enable command
  FMC_SDRAM_CommandTypeDef Command;
  Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = 0;
  HAL_SDRAM_SendCommand (&hsdram, &Command, 0x1000);

  // Insert 100 ms delay
  HAL_Delay (100);

  // Configure PALL (precharge all) command
  Command.CommandMode = FMC_SDRAM_CMD_PALL;
  Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = 0;
  HAL_SDRAM_SendCommand (&hsdram, &Command, 0x1000);

  // Configure Auto-Refresh command
  Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber = 4;
  Command.ModeRegisterDefinition = 0;
  HAL_SDRAM_SendCommand (&hsdram, &Command, 0x1000);

  // Program external memory mode register
  Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = // burstSequential, operatingModeNormal
    SDRAM_MODEREG_WRITEBURST_MODE_SINGLE | SDRAM_MODEREG_CAS_LATENCY_3 | SDRAM_MODEREG_BURST_LENGTH_4;
  HAL_SDRAM_SendCommand (&hsdram, &Command, 0x1000);

  // Set refresh rate counter //* (15.62 us x Freq) - 20 - SDRAM refresh counter (90MHz SD clock)
  HAL_SDRAM_ProgramRefreshRate  (&hsdram, 0x0569);
  }
//}}}
//}}}
//{{{  sd
typedef enum { MSD_OK, MSD_ERROR, MSD_ERROR_SD_NOT_PRESENT, MSD_NO_HIGH_SPEED } MSD_RESULT;
//{{{  static vars
SD_HandleTypeDef uSdHandle;
HAL_SD_CardInfoTypedef uSdCardInfo;
DMA_HandleTypeDef dmaRxHandle;
DMA_HandleTypeDef dmaTxHandle;

const uint32_t mReadCacheSize = 0x40;
uint8_t* mReadCache = 0;
uint32_t mReadCacheBlock = 0xFFFFFFB0;
uint32_t mReads = 0;
uint32_t mReadHits = 0;
uint32_t mReadMultipleLen = 0;
uint32_t mReadBlock = 0xFFFFFFFF;

uint32_t mWrites = 0;
int32_t mWriteMultipleLen = 0;
uint32_t mWriteBlock = 0xFFFFFFFF;
//}}}

extern "C" { void SDIO_IRQHandler() { HAL_SD_IRQHandler (&uSdHandle); } }
extern "C" { void DMA2_Stream3_IRQHandler() { HAL_DMA_IRQHandler (uSdHandle.hdmarx); } }
extern "C" { void DMA2_Stream6_IRQHandler() { HAL_DMA_IRQHandler (uSdHandle.hdmatx); } }

//{{{
uint8_t SD_Init() {

  uSdHandle.Instance = SDIO;
  uSdHandle.Init.ClockEdge           = SDIO_CLOCK_EDGE_RISING;
  uSdHandle.Init.ClockBypass         = SDIO_CLOCK_BYPASS_DISABLE;  // SDIO_CLOCK_BYPASS_ENABLE;
  uSdHandle.Init.ClockPowerSave      = SDIO_CLOCK_POWER_SAVE_DISABLE;
  uSdHandle.Init.BusWide             = SDIO_BUS_WIDE_1B;
  uSdHandle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  uSdHandle.Init.ClockDiv            = 0;

  __HAL_RCC_SDIO_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  //{{{  gpio init
  // sdPresent init - PC6
  GPIO_InitTypeDef gpio_init_structure;
  gpio_init_structure.Pin = GPIO_PIN_6;
  gpio_init_structure.Mode = GPIO_MODE_INPUT;
  gpio_init_structure.Pull = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init (GPIOC, &gpio_init_structure);

  // SDIO D0..D3 - PC8..PC11
  gpio_init_structure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
  gpio_init_structure.Mode = GPIO_MODE_AF_PP;
  gpio_init_structure.Alternate = GPIO_AF12_SDIO;
  HAL_GPIO_Init (GPIOC, &gpio_init_structure);

  // SDIO CMD - PD2
  gpio_init_structure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init (GPIOD, &gpio_init_structure);

  // SDIO CLK - PC12
  gpio_init_structure.Pin = GPIO_PIN_12;
  //gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init (GPIOC, &gpio_init_structure);
  //}}}
  //{{{  DMA rx parameters
  dmaRxHandle.Instance                 = DMA2_Stream3;
  dmaRxHandle.Init.Channel             = DMA_CHANNEL_4;
  dmaRxHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  dmaRxHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
  dmaRxHandle.Init.MemInc              = DMA_MINC_ENABLE;
  dmaRxHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  dmaRxHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  dmaRxHandle.Init.Mode                = DMA_PFCTRL;
  dmaRxHandle.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
  dmaRxHandle.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
  dmaRxHandle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  dmaRxHandle.Init.MemBurst            = DMA_MBURST_INC4;
  dmaRxHandle.Init.PeriphBurst         = DMA_PBURST_INC4;
  __HAL_LINKDMA (&uSdHandle, hdmarx, dmaRxHandle);
  HAL_DMA_DeInit (&dmaRxHandle);
  HAL_DMA_Init (&dmaRxHandle);
  //}}}
  //{{{  DMA tx parameters
  dmaTxHandle.Instance                 = DMA2_Stream6;
  dmaTxHandle.Init.Channel             = DMA_CHANNEL_4;
  dmaTxHandle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  dmaTxHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
  dmaTxHandle.Init.MemInc              = DMA_MINC_ENABLE;
  dmaTxHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  dmaTxHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  dmaTxHandle.Init.Mode                = DMA_PFCTRL;
  dmaTxHandle.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
  dmaTxHandle.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
  dmaTxHandle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  dmaTxHandle.Init.MemBurst            = DMA_MBURST_INC4;
  dmaTxHandle.Init.PeriphBurst         = DMA_PBURST_INC4;

  __HAL_LINKDMA (&uSdHandle, hdmatx, dmaTxHandle);
  HAL_DMA_DeInit (&dmaTxHandle);
  HAL_DMA_Init (&dmaTxHandle);
  //}}}

  HAL_NVIC_SetPriority (DMA2_Stream3_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ (DMA2_Stream3_IRQn);
  HAL_NVIC_SetPriority (DMA2_Stream6_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ (DMA2_Stream6_IRQn);

  HAL_NVIC_SetPriority (SDIO_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ (SDIO_IRQn);

  // HAL SD initialization
  if (HAL_SD_Init (&uSdHandle, &uSdCardInfo) != SD_OK)
    return MSD_ERROR;
  if (HAL_SD_WideBusOperation_Config (&uSdHandle, SDIO_BUS_WIDE_4B) != SD_OK)
    return MSD_ERROR;
  if (HAL_SD_HighSpeed (&uSdHandle) == SD_OK)
    lcd->info ("high speed ok");
  return MSD_OK;
  }
//}}}
//{{{
uint8_t SD_ITConfig() {

  //GPIO_InitTypeDef gpio_init_structure;
  //gpio_init_structure.Pin = SD_DETECT_PIN;
  //gpio_init_structure.Pull = GPIO_PULLUP;
  //gpio_init_structure.Speed = GPIO_SPEED_FAST;
  //gpio_init_structure.Mode = GPIO_MODE_IT_RISING_FALLING;
  //HAL_GPIO_Init (SD_DETECT_GPIO_PORT, &gpio_init_structure);

  // Enable and set SD detect EXTI Interrupt to the lowest priority */
  //HAL_NVIC_SetPriority ((IRQn_Type)(SD_DETECT_EXTI_IRQn), 0x0F, 0x00);
  //HAL_NVIC_EnableIRQ ((IRQn_Type)(SD_DETECT_EXTI_IRQn));

  return MSD_OK;
  }
//}}}

//{{{
bool SD_present() {
  return !(GPIOC->IDR & GPIO_PIN_6);
  }
//}}}
//{{{
int8_t SD_IsReady() {
  return (SD_present() && (HAL_SD_GetStatus (&uSdHandle) == SD_TRANSFER_OK)) ? 0 : -1;
  }
//}}}
//{{{
int8_t SD_GetCapacity (uint32_t* block_num, uint16_t* block_size) {

  if (SD_present()) {
    HAL_SD_CardInfoTypedef info;
    HAL_SD_Get_CardInfo (&uSdHandle, &info);
    *block_num = (info.CardCapacity) / 512 - 1;
    *block_size = 512;
    return 0;
    }

  return -1;
  }
//}}}

//{{{
HAL_SD_TransferStateTypedef SD_GetStatus() {
  return HAL_SD_GetStatus (&uSdHandle);
  }
//}}}
//{{{
void SD_GetCardInfo (HAL_SD_CardInfoTypedef* CardInfo) {
  HAL_SD_Get_CardInfo (&uSdHandle, CardInfo);
  }
//}}}
//{{{
std::string SD_info() {
  return "r:" + dec (mReadHits) + ":" + dec (mReads) + ":"  + dec (mReadBlock + mReadMultipleLen) +
         " w:" + dec (mWrites);
  }
//}}}

//{{{
uint8_t SD_Read (uint8_t* buf, uint32_t blk_addr, uint16_t blocks) {

  uint32_t tmp = (uint32_t)buf;

  auto result = HAL_SD_ReadBlocks_DMA (&uSdHandle, (uint32_t*)buf, blk_addr * 512, 512, blocks);
  if (result != SD_OK) {
    lcd->debug ("HAL_SD_ReadBlocks_DMA:" + hex (tmp) + " " + dec (blk_addr) + " num:" + dec (blocks) +  dec (result));
    return MSD_ERROR;
    }

  result = HAL_SD_CheckReadOperation (&uSdHandle, 500);
  if (result != SD_OK) {
    lcd->debug ("HAL_SD_CheckReadOperation:" + hex (tmp) + " " + dec (blk_addr) + " num:" + dec (blocks) +  dec (result));
    return MSD_ERROR;
    }
  //SCB_InvalidateDCache_by_Addr ((uint32_t*)((uint32_t)buf & 0xFFFFFFE0), (blocks * 512) + 32);

  return MSD_OK;
  }
//}}}
//{{{
uint8_t SD_Write (uint8_t* buf, uint32_t blk_addr, uint16_t blocks) {

  //if (HAL_SD_WriteBlocks (&uSdHandle, (uint32_t*)buf, blk_addr * 512, blocks) != SD_OK)
  //  return MSD_ERROR;
  //can't remove ?
  HAL_SD_CheckWriteOperation (&uSdHandle, 0xFFFFFFFF);

  return MSD_OK;
  }
//}}}
//{{{
int8_t SD_ReadCached (uint8_t* buf, uint32_t blk_addr, uint16_t blocks) {

  return SD_Read (buf, blk_addr, blocks);
  }
//}}}
//{{{
int8_t SD_WriteCached (uint8_t* buf, uint32_t blk_addr, uint16_t blocks) {

  if (SD_present()) {
    mWrites++;
    SD_Write (buf, blk_addr, blocks);

    mReadCacheBlock = 0xFFFFFFF0;
    if (blk_addr != mWriteBlock + mWriteMultipleLen) {
      if (mWriteMultipleLen) {
        // flush pending multiple
        //cLcd::debug ("wm:" + dec (mWriteBlock) + "::" + dec (mWriteMultipleLen));
        mWriteMultipleLen = 0;
        }
      mWriteBlock = blk_addr;
      }
    mWriteMultipleLen += blocks;

    return 0;
    }

  return -1;
  }
//}}}

//{{{
uint8_t SD_Erase (uint64_t StartAddr, uint64_t EndAddr) {
  if (HAL_SD_Erase (&uSdHandle, StartAddr, EndAddr) != SD_OK)
    return MSD_ERROR;
  else
    return MSD_OK;
  }
//}}}

// diskio
static volatile DSTATUS Stat = STA_NOINIT;
//{{{
DSTATUS diskInitialize() {

  return SD_GetStatus() == SD_TRANSFER_OK ? 0 : STA_NOINIT;
  }
//}}}

//{{{
DSTATUS diskStatus() {

  return SD_GetStatus() == SD_TRANSFER_OK ? 0 : STA_NOINIT;
  }
//}}}
//{{{
DRESULT diskIoctl (BYTE cmd, void* buff) {

  DRESULT res = RES_ERROR;

  switch (cmd) {
    // Make sure that no pending write process
    case CTRL_SYNC :
      res = RES_OK;
      break;

    // Get number of sectors on the disk (DWORD)
    case GET_SECTOR_COUNT : {
      HAL_SD_CardInfoTypedef CardInfo;
      SD_GetCardInfo (&CardInfo);
      *(DWORD*)buff = CardInfo.CardCapacity / SECTOR_SIZE;
      res = RES_OK;
      break;
      }

    // Get R/W sector size (WORD)
    case GET_SECTOR_SIZE :
      *(WORD*)buff = SECTOR_SIZE;
      res = RES_OK;
      break;

    // Get erase block size in unit of sector (DWORD)
    case GET_BLOCK_SIZE :
      *(DWORD*)buff = SECTOR_SIZE;
      res = RES_OK;
      break;

    default:
      res = RES_PARERR;
    }

  return res;
  }
//}}}

//{{{
DRESULT diskRead (BYTE* buffer, DWORD sector, UINT count) {

  if ((uint32_t)buffer & 0x03) {
    lcd->info ("diskRead align b:" + hex ((int)buffer) + " sec:" + dec (sector) + " num:" + dec (count));

    // not 32bit aligned, dma fails,
    auto tempBuffer = (uint8_t*)pvPortMalloc (count * SECTOR_SIZE);

    // read into 32bit aligned tempBuffer
    auto result = SD_ReadCached (tempBuffer, sector, count) == MSD_OK ? RES_OK : RES_ERROR;
    memcpy (buffer, tempBuffer, count * SECTOR_SIZE);

    vPortFree (tempBuffer);
    return result;
    }

  else {
    //lcd->info ("diskRead - sec:" + dec (sector) + " num:" + dec (count));
    return SD_ReadCached ((uint8_t*)buffer, sector, count) == MSD_OK ? RES_OK : RES_ERROR;
    }
  }
//}}}
//{{{
DRESULT diskWrite (const BYTE* buffer, DWORD sector, UINT count) {
  return SD_WriteCached ((uint8_t*)buffer, (uint64_t)(sector * SECTOR_SIZE), count) == MSD_OK ? RES_OK : RES_ERROR;
  }
//}}}
//}}}

std::vector<std::string> mFileNames;
//{{{
void listDirectory (std::string directoryName, std::string ext, std::string ext1) {

  cDirectory directory (directoryName);
  if (directory.getError()) {
    //{{{  open error
    lcd->info (COL_RED, "directory open error:"  + dec (directory.getError()));
    return;
    }
    //}}}

  cFileInfo fileInfo;
  while ((directory.find (fileInfo) == FR_OK) && !fileInfo.getEmpty()) {
    if (fileInfo.getBack()) {
      }
    else if (fileInfo.isDirectory()) {
      listDirectory (directoryName + "/" + fileInfo.getName(), ext, ext1);
      }
    else if (fileInfo.matchExtension (ext.c_str()) || fileInfo.matchExtension (ext1.c_str())) {
      mFileNames.push_back (directoryName + "/" + fileInfo.getName());
      }
    }
  }
//}}}
//{{{
int main() {

  HAL_Init();
  SystemClockConfig180();
  initDebugUart();
  BSP_PB_Init (BUTTON_KEY, BUTTON_MODE_GPIO);
  //configureDtrace4();
  sdramGpioInit();
  sdramBank1Init();
  sdramBank2Init();

  HeapRegion_t xHeapRegions[] = {
    //{(uint8_t*)0x10000000, 0x10000 },
    {(uint8_t*)SDRAM_BANK2_ADDR + (LCD_WIDTH*LCD_HEIGHT*4), 0x800000 - (LCD_WIDTH*LCD_HEIGHT*4) },
    { nullptr, 0 } };
  heapInit (xHeapRegions);
  //{{{  init frameBuffer
  memset ((void*)SDRAM_BANK2_ADDR, 0, (LCD_WIDTH*LCD_HEIGHT*4));

  lcd = new cLcd (SDRAM_BANK2_ADDR, SDRAM_BANK2_ADDR + (LCD_WIDTH*LCD_HEIGHT*2));
  const std::string kHello = "built " + std::string(__TIME__) + " on " + std::string(__DATE__) +
                              " heap:" + dec (0x800000 - (LCD_WIDTH*LCD_HEIGHT*4));
  lcd->init ("stm32F429disco test - " + kHello);
  lcd->displayOn();
  lcd->render();
  //}}}
  if (BSP_PB_GetState (BUTTON_KEY) == GPIO_PIN_SET) {
    //{{{  ps2
    initPs2gpio();
    initPs2touchpad();
    }
    //}}}
  else {
    //{{{  sd
    if (SD_present()) {
      int ret = SD_Init();
      lcd->debug ("SDinit " + dec(ret));

      cFatFs* fatFs = cFatFs::create();
      if (fatFs->mount() != FR_OK)
        lcd->debug ("fatFs mount problem");
      else
        lcd->debug ("SD label:" + fatFs->getLabel() + " vsn:" + hex (fatFs->getVolumeSerialNumber()) +
                   " freeSectors:" + dec (fatFs->getFreeSectors()));
      listDirectory ("", "gif", "GIF");
      }
    else
      lcd->debug ("no SD card");
    }
    //}}}

  for (auto fileStr : mFileNames) {
    auto t0 = HAL_GetTick();
    //{{{  read file and header
    cFile file (fileStr, FA_OPEN_EXISTING | FA_READ);

    auto buf = (uint8_t*)pvPortMalloc (file.getSize());
    if (!buf)
      lcd->debug ("no buf");

    auto bytesRead = 0;
    file.read (buf, file.getSize(), bytesRead);
    //}}}
    iPic* pic = new cDecodePic ();
    auto tRead = HAL_GetTick();

    //{{{  scale calc
    // calc scale
    //auto scaleShift = 0;
    //auto scale = 1;
    //while ((scaleShift < 3) &&
    //       ((width / scale > lcd->getWidth()) || (height /scale > lcd->getHeight()))) {
    //  scale *= 2;
    //  scaleShift++;
    //  }
    //width >>= scaleShift;
    //height >>= scaleShift;
    //}}}
    pic->setPic (buf, file.getSize());
    vPortFree (buf);
    auto tDecode = HAL_GetTick();
    auto width = pic->getWidth();
    auto height = pic->getHeight();
    lcd->info ("fileSize:" + dec(file.getSize()) + " " + dec(width) + ":" + dec(height) +
               "read:" + dec(tRead-t0) + " dec:" + dec(tDecode-tRead) + " " +
               fileStr);

    if (pic->getPic()) {
      cLcd::cTile picTile (pic->getPic(), pic->getComponents(), width,
                           0,0, width > lcd->getWidth() ? lcd->getWidth() : width,
                                height > lcd->getHeight() ? lcd->getHeight() : height);
      lcd->startRender();
      lcd->clear (COL_BLACK);
      lcd->copy (picTile, 0,0);
      lcd->sizeCpuBiLinear (picTile, 0,0, lcd->getWidth(), lcd->getHeight());
      picTile.free();
      lcd->endRender (true);
      }
    else
      lcd->debug ("- no piccy");

    delete (pic);
    }

  HAL_Delay (60000);

  while (true) {
    lcd->startRender();
    lcd->clear (COL_BLACK);
    //std::string str = "x:" + dec(touchX) + " y:" + dec(touchY) + " z:" + dec(touchZ);
    //lcd->text (COL_YELLOW, getFontHeight(), str, 100, 200, lcd->getWidth(), getBoxHeight());
    //auto x = lcd->getWidth() - ((touchY-1300)/4);
    //auto y = lcd->getHeight() - ((touchX-1300)/4);
    //lcd->ellipse (COL_YELLOW, x, y, touchZ, touchZ);
    lcd->endRender (true);
    //uint16_t value = PS2get();
    //char ch = (char)value;
    //std::string charStr (1, ch);
    //std::string releaseStr = (value & 0x100) ? " release " : " ";
    //lcd->info ("key:" + charStr + releaseStr + hex (value));
    }
  }
//}}}
