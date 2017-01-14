// main.cpp
#define bigMalloc(size,tag)    pvPortMalloc(size)
#define bigFree                vPortFree
#define smallMalloc(size,tag)  malloc(size)
#define smallFree              free
//{{{  includes
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>

#include "heap.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"

#include "fatFs.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include "FreeSansBold.h"

#include "cJpegPic.h"

#include "diskio.h"
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

//{{{
template <typename T> std::string hex (T value, uint16_t width = 0) {

	std::ostringstream os;
	os << std::hex << std::setfill ('0') << std::setw (width) << value;
	return os.str();
	}
//}}}
//{{{
template <typename T> std::string dec (T value, uint16_t width = 0, char fill = ' ') {

	std::ostringstream os;
	os << std::setfill (fill) << std::setw (width) << value;
	return os.str();
	}
//}}}

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

//{{{  sys
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
void SDRAMgpioInit() {
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
void SDRAMbank1Init() {
// Micron MT48LC8M16A2 � 2m x 16bits x 4banks
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
void SDRAMbank2Init() {
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
	rccPeriphClkInit.PLLSAI.PLLSAIN = 130;  // hclk = 192mhz, 138/4 = 34.5mhz
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
//{{{  Lcd
//{{{  colour defines
#define COL_WHITE         0xFFFF
//#define COL_LIGHTGREY     0xFFD3D3D3
//#define COL_GREY          0xFF808080
//#define COL_DARKGREY      0xFF404040
//#define COL_DARKERGREY    0xFF202020
#define COL_BLACK         0x0000

#define COL_BLUE          0x001F
//#define COL_LIGHTBLUE     0xFF8080FF
//#define COL_DARKBLUE      0xFF000080

#define COL_GREEN         0x07E0
//#define COL_LIGHTGREEN    0xFF80FF80
//#define COL_DARKGREEN     0xFF008000
//#define COL_DARKERGREEN   0xFF004000

#define COL_RED           0xF800
//#define COL_LIGHTRED      0xFFFF8080
//#define COL_DARKRED       0xFF800000

#define COL_CYAN          0x07FF
//#define COL_LIGHTCYAN     0xFF80FFFF
//#define COL_DARKCYAN      0xFF008080

#define COL_MAGENTA       0xF81F
//#define COL_LIGHTMAGENTA  0xFFFF80FF
//#define COL_DARKMAGENTA   0xFF800080

#define COL_YELLOW        0xFFE0
//#define COL_LIGHTYELLOW   0xFFFFFF80
//#define COL_DARKYELLOW    0xFF808000
//#define COL_DARKERYELLOW  0xFF202000

//#define COL_BROWN         0xFFA52A2A
//#define COL_ORANGE        0xFFFFA500
//#define COL_DARKORANGE    0xFFC07800
//#define COL_DARKERORANGE  0xFF805000
//}}}
//{{{  lcd vars
const int kEnd = 0;
const int kStamp = 1;

#define DMA2D_BUFFER         0x10000000
#define DMA2D_BUFFER_SIZE        0x9000  // SIZE = rest of dtcm

class cLcd;
cLcd* lcd = nullptr;

//{{{  struct tLTDC
typedef struct {
	//SemaphoreHandle_t sem;
	uint32_t timeouts;
	uint32_t lineIrq;
	uint32_t fifoUnderunIrq;
	uint32_t transferErrorIrq;
	uint32_t lastTicks;
	uint32_t lineTicks;
	uint32_t frameWait;
	} tLTDC;
//}}}
tLTDC ltdc;

uint8_t showAlpha[2];
uint32_t showFrameBufferAddress[2];

uint32_t* mDma2dBuf = nullptr;
uint32_t* mDma2dIsrBuf = nullptr;
//}}}

uint16_t getFontHeight() { return 26; }
uint16_t getBigFontHeight() { return 40; }
uint16_t getSmallFontHeight() { return 12; }
uint16_t getBoxHeight() { return 30; }
//{{{
void LCD_DMA2D_IRQHandler() {

	// clear interrupts
	DMA2D->IFCR = DMA2D_ISR_TCIF | DMA2D_ISR_TEIF | DMA2D_ISR_CEIF;

	while (true) {
		uint32_t opcode = *mDma2dIsrBuf++;
		switch (opcode) {
			case kEnd: {
				DMA2D->CR = 0;
				mDma2dIsrBuf = mDma2dBuf;
				return;
				}

			case kStamp:
				DMA2D->FGPFCCR = DMA2D_INPUT_A8;  // fgnd PFC
				DMA2D->BGPFCCR = DMA2D_INPUT_RGB565;
				DMA2D->OPFCCR  = DMA2D_INPUT_RGB565;
				DMA2D->OMAR    = *mDma2dIsrBuf;   // output start address
				DMA2D->BGMAR   = *mDma2dIsrBuf++; // - repeated to bgnd start addres
				DMA2D->OOR     = *mDma2dIsrBuf;   // output stride
				DMA2D->BGOR    = *mDma2dIsrBuf++; // - repeated to bgnd stride
				DMA2D->NLR     = *mDma2dIsrBuf++; // width:height
				DMA2D->FGMAR   = *mDma2dIsrBuf++; // fgnd start address
				DMA2D->FGOR    = 0;               // fgnd stride
				DMA2D->CR = DMA2D_M2M_BLEND | DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE | DMA2D_CR_START;
				while (!(DMA2D->ISR & DMA2D_FLAG_TC)) {}
				DMA2D->IFCR |= DMA2D_IFSR_CTEIF | DMA2D_IFSR_CTCIF | DMA2D_IFSR_CTWIF|
											 DMA2D_IFSR_CCAEIF | DMA2D_IFSR_CCTCIF | DMA2D_IFSR_CCEIF;
				break;

			default: // opcode==address : value
				*(uint32_t*)opcode = *mDma2dIsrBuf++;
				if (opcode == (uint32_t)DMA2D) { // address of CR
					while (!(DMA2D->ISR & DMA2D_FLAG_TC)) {}
					DMA2D->IFCR |= DMA2D_IFSR_CTEIF | DMA2D_IFSR_CTCIF | DMA2D_IFSR_CTWIF|
												 DMA2D_IFSR_CCAEIF | DMA2D_IFSR_CCTCIF | DMA2D_IFSR_CCEIF;
				 break;
				 }
			}
		}
	}
//}}}

class cLcd {
public:
	//{{{
	cLcd (uint32_t buffer0, uint32_t buffer1)  {

		mBuffer[false] = buffer0;
		mBuffer[true] = buffer1;
		updateNumDrawLines();
		}
	//}}}
	~cLcd() {}
	//{{{
	void init (std::string title) {

		mDrawBuffer = !mDrawBuffer;
		ltdcInit (mBuffer[mDrawBuffer]);

		// zero out first opcode, point past it
		mDma2dBuf = (uint32_t*)DMA2D_BUFFER;
		mDma2dIsrBuf = mDma2dBuf;
		mDma2dCurBuf = mDma2dBuf;
		*mDma2dCurBuf = kEnd;

		// dma2d IRQ init
		//HAL_NVIC_SetPriority (DMA2D_IRQn, 0x0F, 0);
		//HAL_NVIC_EnableIRQ (DMA2D_IRQn);

		// font init
		FT_Init_FreeType (&FTlibrary);
		FT_New_Memory_Face (FTlibrary, (FT_Byte*)freeSansBold, sizeof (freeSansBold), 0, &FTface);
		FTglyphSlot = FTface->glyph;

		// preload fontChars
		for (char ch = 0x20; ch <= 0x7F; ch++)
			loadChar (getFontHeight(), ch);
		//for (char ch = 0x21; ch <= 0x3F; ch++)
		//  loadChar (getBigFontHeight(), ch);
		//for (char ch = 0x21; ch <= 0x3F; ch++)
		//  loadChar (getSmallFontHeight(), ch);

		FT_Done_Face (FTface);
		//FT_Done_FreeType (FTlibrary);

		mTitle = title;
		updateNumDrawLines();
		}
	//}}}

	uint16_t getLcdWidthPix() { return 800; }
	uint16_t getLcdHeightPix() { return 1280; }

	//{{{
	void setShowDebug (bool title, bool info, bool lcdStats, bool footer) {

		mShowTitle = title;
		mShowInfo = info;
		mShowLcdStats = lcdStats;
		mShowFooter = footer;

		updateNumDrawLines();
		}
	//}}}
	//{{{
	void debug (std::string str) {
		info (COL_WHITE, str);
		render();
		}
	//}}}
	//{{{
	void info (std::string str) {
		info (COL_WHITE, str);
		}
	//}}}
	//{{{
	void info (uint16_t colour, std::string str) {

		bool tailing = mLastLine == (int)mFirstLine + mNumDrawLines - 1;

		auto line = (mLastLine < mMaxLine-1) ? mLastLine+1 : mLastLine;
		mLines[line].mTime = HAL_GetTick(); //xTaskGetTickCount();
		mLines[line].mColour = colour;
		mLines[line].mString = str;
		mLastLine = line;

		if (tailing)
			mFirstLine = mLastLine - mNumDrawLines + 1;
		}
	//}}}

	//{{{
	void press (int pressCount, int16_t x, int16_t y, uint16_t z, int16_t xinc, int16_t yinc) {

		if ((pressCount > 30) && (x <= mStringPos) && (y <= getBoxHeight()))
			reset();
		else if (pressCount == 0) {
			if (x <= mStringPos) {
				// set displayFirstLine
				if (y < 2 * getBoxHeight())
					displayTop();
				else if (y > getLcdHeightPix() - 2 * getBoxHeight())
					displayTail();
				}
			}
		else {
			// inc firstLine
			float value = mFirstLine - ((2.0f * yinc) / getBoxHeight());

			if (value < 0)
				mFirstLine = 0;
			else if (mLastLine <= (int)mNumDrawLines-1)
				mFirstLine = 0;
			else if (value > mLastLine - mNumDrawLines + 1)
				mFirstLine = mLastLine - mNumDrawLines + 1;
			else
				mFirstLine = value;
			}
		}
	//}}}
	//{{{
	void startRender() {

		mDrawBuffer = !mDrawBuffer;
		setLayer (0, mBuffer[mDrawBuffer]);

		// frameSync;
		//ltdc.frameWait = 1;
		//if (xSemaphoreTake (ltdc.sem, 100) == pdFALSE)
		//  ltdc.timeouts++;

		mDrawStartTime = HAL_GetTick();
		}
	//}}}
	//{{{
	void renderCursor (uint16_t colour, int16_t x, int16_t y, int16_t z) {
		ellipse (colour, x, y, z, z);
		}
	//}}}
	//{{{
	void endRender (bool forceInfo) {

		auto y = 0;
		if ((mShowTitle || forceInfo) && !mTitle.empty()) {
			//{{{  draw title
			text (COL_YELLOW, getFontHeight(), mTitle, 0, y, getLcdWidthPix(), getBoxHeight());
			y += getBoxHeight();
			}
			//}}}
		if (mShowInfo || forceInfo) {
			//{{{  draw info lines
			if (mLastLine >= 0) {
				// draw scroll bar
				auto yorg = getBoxHeight() + ((int)mFirstLine * mNumDrawLines * getBoxHeight() / (mLastLine + 1));
				auto height = mNumDrawLines * mNumDrawLines * getBoxHeight() / (mLastLine + 1);
				rectClipped (COL_YELLOW, 0, yorg, 8, height);
				}

			auto lastLine = (int)mFirstLine + mNumDrawLines - 1;
			if (lastLine > mLastLine)
				lastLine = mLastLine;

			for (auto lineIndex = (int)mFirstLine; lineIndex <= lastLine; lineIndex++) {
				auto x = 0;
				auto xinc = text (COL_GREEN, getFontHeight(),
													dec ((mLines[lineIndex].mTime-mStartTime) / 1000) + "." +
													dec ((mLines[lineIndex].mTime-mStartTime) % 1000, 3, '0'),
													x, y, getLcdWidthPix(), getBoxHeight());
				x += xinc + 3;

				text (mLines[lineIndex].mColour, getFontHeight(), mLines[lineIndex].mString,
							x, y, getLcdWidthPix()-x, getLcdHeightPix());

				y += getBoxHeight();
				}
			}
			//}}}
		if (mShowLcdStats) {
			//{{{  draw lcdStats
			std::string str = dec (ltdc.lineIrq) + ":f " +
												dec (ltdc.lineTicks) + "ms " +
												dec (mDma2dTimeouts) + " " +
												dec (ltdc.transferErrorIrq) + " " +
												dec (ltdc.fifoUnderunIrq);
			text (COL_WHITE, getFontHeight(), str, 0, getLcdHeightPix() - 2 * getBoxHeight(), getLcdWidthPix(), 24);
			}
			//}}}
		if (mShowFooter || forceInfo)
			//{{{  draw footer
			text (COL_WHITE, getFontHeight(),
						//dec (xPortGetFreeHeapSize()) + " " +
						//dec (xPortGetMinimumEverFreeHeapSize()) + " " +
						//dec (osGetCPUUsage()) + "% " +
					dec (mDrawTime) + "ms " + dec (mDma2dCurBuf - mDma2dBuf),
						0, -getFontHeight() + getLcdHeightPix(), getLcdWidthPix(), getFontHeight());
			//}}}

		// terminate opCode buffer
		*mDma2dCurBuf = kEnd;

		// send opCode buffer
		LCD_DMA2D_IRQHandler();
		showLayer (0, mBuffer[mDrawBuffer], 255);

		// reset opcode buffer
		mDma2dCurBuf = mDma2dBuf;
		*mDma2dCurBuf = kEnd;

		mDrawTime = HAL_GetTick() - mDrawStartTime;
		}
	//}}}

	//{{{
	void render() {
		startRender();
		clear (COL_BLACK);
		endRender (true);
		}
	//}}}
	//{{{
	void displayOn() {
		GPIOD->BSRR = GPIO_PIN_13;   // ADJ hi
		}
	//}}}
	//{{{
	void displayOff() {
		GPIOD->BSRR = GPIO_PIN_13 << 16;   // ADJ lo
		}
	//}}}

	//{{{
	void pixel (uint16_t colour, int16_t x, int16_t y) {

		rect (colour, x, y, 1, 1);
		}
	//}}}
	//{{{
	void rect (uint16_t colour, int16_t x, int16_t y, uint16_t width, uint16_t height) {

		// often same colour
		if (colour != mCurDstColour) {
			*mDma2dCurBuf++ = (uint32_t)&(DMA2D->OCOLR); // OCOLR - output colour
			*mDma2dCurBuf++ = colour;
			mCurDstColour = colour;
			}

		// quite often same stride
		if (uint32_t(getLcdWidthPix() - width) != mDstStride) {
			*mDma2dCurBuf++ = (uint32_t)&(DMA2D->OOR); // OOR - output stride
			mDstStride = getLcdWidthPix() - width;
			*mDma2dCurBuf++ = mDstStride;
			}

		*mDma2dCurBuf++ = (uint32_t)&(DMA2D->OMAR); // OMAR - output start address 3c
		*mDma2dCurBuf++ = mCurFrameBufferAddress + ((y * getLcdWidthPix()) + x) * dstComponents;

		*mDma2dCurBuf++ = (uint32_t)&(DMA2D->NLR);
		*mDma2dCurBuf++ = (width << 16) | height;

		*mDma2dCurBuf++ = (uint32_t)DMA2D; // CR
		*mDma2dCurBuf++ = DMA2D_R2M | DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE | DMA2D_CR_START;
		}
	//}}}
	//{{{
	void stamp (uint16_t colour, uint8_t* src, int16_t x, int16_t y, uint16_t width, uint16_t height) {

		// often same colour
		if (colour != mCurSrcColour) {
			uint32_t fullColour = ((colour & 0xF800) << 8) | ((colour & 0x07E0) << 5) | ((colour & 0x001F) << 3);
			*mDma2dCurBuf++ = (uint32_t)&(DMA2D->FGCOLR); // FGCOLR - fgnd colour
			*mDma2dCurBuf++ = fullColour;
			mCurSrcColour = colour;
			}

		*mDma2dCurBuf++ = kStamp;
		*mDma2dCurBuf++ = mCurFrameBufferAddress + ((y * getLcdWidthPix()) + x) * dstComponents; // output start address
		mDstStride = getLcdWidthPix() - width;
		*mDma2dCurBuf++ = mDstStride;                                          // stride
		*mDma2dCurBuf++ = (width << 16) | height;                              // width:height
		*mDma2dCurBuf++ = (uint32_t)src;                                       // fgnd start address
		}
	//}}}
	//{{{
	int text (uint16_t colour, uint16_t fontHeight, std::string str, int16_t x, int16_t y, uint16_t width, uint16_t height) {

		auto xend = x + width;
		for (uint16_t i = 0; i < str.size(); i++) {
			if ((str[i] >= 0x20) && (str[i] <= 0x7F)) {
				auto fontCharIt = mFontCharMap.find (fontHeight<<8 | str[i]);
				if (fontCharIt != mFontCharMap.end()) {
					auto fontChar = fontCharIt->second;
					if (x + fontChar->left + fontChar->pitch >= xend)
						break;
					else if (fontChar->bitmap)
						stampClipped (colour, fontChar->bitmap, x + fontChar->left, y + fontHeight - fontChar->top, fontChar->pitch, fontChar->rows);

					x += fontChar->advance;
					}
				}
			}

		return x;
		}
	//}}}
	//{{{
	void copy (uint8_t* src, int16_t x, int16_t y, uint16_t width, uint16_t height) {
	// copy

		// output
		*mDma2dCurBuf++ = (uint32_t)DMA2D + 0x3C; // OMAR - output start address
		*mDma2dCurBuf++ = mCurFrameBufferAddress + ((y * getLcdWidthPix()) + x) * dstComponents;

		mDstStride = getLcdWidthPix() - width;
		*mDma2dCurBuf++ = (uint32_t)DMA2D + 0x40; // OOR - output stride
		*mDma2dCurBuf++ = mDstStride;

		*mDma2dCurBuf++ = (uint32_t)DMA2D + 0x44; // NLR - width:height
		*mDma2dCurBuf++ = (width << 16) | height;

		// src - fgnd
		*mDma2dCurBuf++ = (uint32_t)DMA2D + 0x1C; // FGPFCCR - fgnd PFC
		//*mDma2dCurBuf++ = DMA2D_INPUT_RGB565;
		*mDma2dCurBuf++ = DMA2D_INPUT_RGB888;

		*mDma2dCurBuf++ = (uint32_t)DMA2D + 0x0C; // FGMAR - fgnd address
		*mDma2dCurBuf++ = (uint32_t)src;

		*mDma2dCurBuf++ = (uint32_t)DMA2D + 0x10; // FGOR - fgnd stride
		*mDma2dCurBuf++ = 0;

		*mDma2dCurBuf++ = (uint32_t)DMA2D;        // CR
		*mDma2dCurBuf++ = DMA2D_M2M_PFC | DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE | DMA2D_CR_START;
		}
	//}}}
	//{{{
	void copy (uint8_t* src, int16_t srcx, int16_t srcy, uint16_t srcWidth, uint16_t srcHeight,
									 int16_t dstx, int16_t dsty, uint16_t dstWidth, uint16_t dstHeight) {
	// copy src to dst
	// - some corner cases missing, not enough src for dst needs padding

		// output
		*mDma2dCurBuf++ = (uint32_t)DMA2D + 0x3C; // OMAR - output start address
		*mDma2dCurBuf++ = mCurFrameBufferAddress + ((dsty * getLcdWidthPix()) + dstx) * dstComponents;

		*mDma2dCurBuf++ = (uint32_t)DMA2D + 0x40; // OOR - output stride
		mDstStride = getLcdWidthPix() - dstWidth;
		*mDma2dCurBuf++ = mDstStride;

		*mDma2dCurBuf++ = (uint32_t)DMA2D + 0x44; // NLR - width:height
		*mDma2dCurBuf++ = (dstWidth << 16) | dstHeight;

		// src - fgnd
		*mDma2dCurBuf++ = (uint32_t)DMA2D + 0x1C; // FGPFCCR - fgnd PFC
		*mDma2dCurBuf++ = DMA2D_INPUT_RGB888;

		*mDma2dCurBuf++ = (uint32_t)DMA2D + 0x0C; // FGMAR - fgnd address
		*mDma2dCurBuf++ = (uint32_t)(src + ((srcy * srcWidth) + srcx) * dstComponents);

		*mDma2dCurBuf++ = (uint32_t)DMA2D + 0x10; // FGOR - fgnd stride
		*mDma2dCurBuf++ = srcWidth - dstWidth;

		// CR
		*mDma2dCurBuf++ = (uint32_t)DMA2D;        // CR
		*mDma2dCurBuf++ = DMA2D_M2M_PFC | DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE | DMA2D_CR_START;
		}
	//}}}

	//{{{
	void pixelClipped (uint16_t colour, int16_t x, int16_t y) {

			rectClipped (colour, x, y, 1, 1);
		}
	//}}}
	//{{{
	void stampClipped (uint16_t colour, uint8_t* src, int16_t x, int16_t y, uint16_t width, uint16_t height) {

		if (!width || !height || x < 0)
			return;

		if (y < 0) {
			// top clip
			if (y + height <= 0)
				return;
			height += y;
			src += -y * width;
			y = 0;
			}

		if (y + height > getLcdHeightPix()) {
			// bottom yclip
			if (y >= getLcdHeightPix())
				return;
			height = getLcdHeightPix() - y;
			}

		stamp (colour, src, x, y, width, height);
		}
	//}}}
	//{{{
	void rectClipped (uint16_t colour, int16_t x, int16_t y, uint16_t width, uint16_t height) {

		if (x >= getLcdWidthPix())
			return;
		if (y >= getLcdHeightPix())
			return;

		int xend = x + width;
		if (xend <= 0)
			return;

		int yend = y + height;
		if (yend <= 0)
			return;

		if (x < 0)
			x = 0;
		if (xend > getLcdWidthPix())
			xend = getLcdWidthPix();

		if (y < 0)
			y = 0;
		if (yend > getLcdHeightPix())
			yend = getLcdHeightPix();

		if (!width)
			return;
		if (!height)
			return;

		rect (colour, x, y, xend - x, yend - y);
		}
	//}}}
	//{{{
	void rectOutline (uint16_t colour, int16_t x, int16_t y, uint16_t width, uint16_t height, uint8_t thickness) {

		rectClipped (colour, x, y, width, thickness);
		rectClipped (colour, x + width-thickness, y, thickness, height);
		rectClipped (colour, x, y + height-thickness, width, thickness);
		rectClipped (colour, x, y, thickness, height);
		}
	//}}}
	//{{{
	void clear (uint16_t colour) {

		rect (colour, 0, 0, getLcdWidthPix(), getLcdHeightPix());
		}
	//}}}
	//{{{
	void ellipse (uint16_t colour, int16_t x, int16_t y, uint16_t xradius, uint16_t yradius) {

		if (!xradius)
			return;
		if (!yradius)
			return;

		int x1 = 0;
		int y1 = -yradius;
		int err = 2 - 2*xradius;
		float k = (float)yradius / xradius;

		do {
			rectClipped (colour, (x-(uint16_t)(x1 / k)), y + y1, (2*(uint16_t)(x1 / k) + 1), 1);
			rectClipped (colour, (x-(uint16_t)(x1 / k)), y - y1, (2*(uint16_t)(x1 / k) + 1), 1);

			int e2 = err;
			if (e2 <= x1) {
				err += ++x1 * 2 + 1;
				if (-y1 == x && e2 <= y1)
					e2 = 0;
				}
			if (e2 > y1)
				err += ++y1*2 + 1;
			} while (y1 <= 0);
		}
	//}}}
	//{{{
	void ellipseOutline (uint16_t colour, int16_t x, int16_t y, uint16_t xradius, uint16_t yradius) {

		if (xradius && yradius) {
			int x1 = 0;
			int y1 = -yradius;
			int err = 2 - 2*xradius;
			float k = (float)yradius / xradius;

			do {
				rectClipped (colour, x - (uint16_t)(x1 / k), y + y1, 1, 1);
				rectClipped (colour, x + (uint16_t)(x1 / k), y + y1, 1, 1);
				rectClipped (colour, x + (uint16_t)(x1 / k), y - y1, 1, 1);
				rectClipped (colour, x - (uint16_t)(x1 / k), y - y1, 1, 1);

				int e2 = err;
				if (e2 <= x1) {
					err += ++x1*2 + 1;
					if (-y1 == x1 && e2 <= y1)
						e2 = 0;
					}
				if (e2 > y1)
					err += ++y1*2 + 1;
				} while (y1 <= 0);
			}
		}
	//}}}
	//{{{
	void line (uint16_t colour, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {

		int16_t deltax = (x2 - x1) > 0 ? (x2 - x1) : -(x2 - x1);        /* The difference between the x's */
		int16_t deltay = (y2 - y1) > 0 ? (y2 - y1) : -(y2 - y1);        /* The difference between the y's */
		int16_t x = x1;                       /* Start x off at the first pixel */
		int16_t y = y1;                       /* Start y off at the first pixel */

		int16_t xinc1;
		int16_t xinc2;
		if (x2 >= x1) {               /* The x-values are increasing */
			xinc1 = 1;
			xinc2 = 1;
			}
		else {                         /* The x-values are decreasing */
			xinc1 = -1;
			xinc2 = -1;
			}

		int yinc1;
		int yinc2;
		if (y2 >= y1) {                 /* The y-values are increasing */
			yinc1 = 1;
			yinc2 = 1;
			}
		else {                         /* The y-values are decreasing */
			yinc1 = -1;
			yinc2 = -1;
			}

		int den = 0;
		int num = 0;
		int num_add = 0;
		int num_pixels = 0;
		if (deltax >= deltay) {        /* There is at least one x-value for every y-value */
			xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
			yinc2 = 0;                  /* Don't change the y for every iteration */
			den = deltax;
			num = deltax / 2;
			num_add = deltay;
			num_pixels = deltax;         /* There are more x-values than y-values */
			}
		else {                         /* There is at least one y-value for every x-value */
			xinc2 = 0;                  /* Don't change the x for every iteration */
			yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
			den = deltay;
			num = deltay / 2;
			num_add = deltax;
			num_pixels = deltay;         /* There are more y-values than x-values */
		}

		for (int curpixel = 0; curpixel <= num_pixels; curpixel++) {
			rectClipped (colour, x, y, 1, 1);   /* Draw the current pixel */
			num += num_add;                            /* Increase the numerator by the top of the fraction */
			if (num >= den) {                          /* Check if numerator >= denominator */
				num -= den;                             /* Calculate the new numerator value */
				x += xinc1;                             /* Change the x as appropriate */
				y += yinc1;                             /* Change the y as appropriate */
				}
			x += xinc2;                               /* Change the x as appropriate */
			y += yinc2;                               /* Change the y as appropriate */
			}
		}
	//}}}

private:
	const int dstComponents = 2;
	//{{{
	class cFontChar {
	public:
		uint8_t* bitmap;
		int16_t left;
		int16_t top;
		int16_t pitch;
		int16_t rows;
		int16_t advance;
		};
	//}}}
	typedef std::map<uint16_t, cFontChar*> cFontCharMap;

	//{{{
	void ltdcInit (uint32_t frameBufferAddress) {

		//{{{  clocks
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();
		__HAL_RCC_GPIOF_CLK_ENABLE();
		__HAL_RCC_GPIOG_CLK_ENABLE();

		__HAL_RCC_DMA2D_CLK_ENABLE();
		__HAL_RCC_LTDC_CLK_ENABLE();
		//}}}
		//{{{  gpio
		//  CK <-> PG.07   DE <-> PF.10  ADJ <-> PD.13 - optional HS <-> PC.06, VS <-> PA.04
		//  R2 <-> PC.10   G2 <-> PA.06   B2 <-> PD.06
		//  R3 <-> PB.00   G3 <-> PG.10   B3 <-> PG.11
		//  R4 <-> PA.11   G4 <-> PB.10   B4 <-> PG.12
		//  R5 <-> PA.12   G5 <-> PB.11   B5 <-> PA.03
		//  R6 <-> PB.01   G6 <-> PC.07   B6 <-> PB.08
		//  R7 <-> PG.06   G7 <-> PD.03   B7 <-> PB.09

		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		//GPIO_InitStructure.Pull = GPIO_NOPULL;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Pin = GPIO_PIN_13;
		HAL_GPIO_Init (GPIOD, &GPIO_InitStructure);

		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Alternate = GPIO_AF14_LTDC;

		GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_11 | GPIO_PIN_12; // GPIO_PIN_4
		HAL_GPIO_Init (GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
		HAL_GPIO_Init (GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.Pin = GPIO_PIN_7;// ; | GPIO_PIN_10; // GPIO_PIN_6
		HAL_GPIO_Init (GPIOC, &GPIO_InitStructure);

		GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_6;
		HAL_GPIO_Init (GPIOD, &GPIO_InitStructure);

		GPIO_InitStructure.Pin = GPIO_PIN_10;
		HAL_GPIO_Init (GPIOF, &GPIO_InitStructure);

		GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11;
		HAL_GPIO_Init (GPIOG, &GPIO_InitStructure);

		GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
		GPIO_InitStructure.Alternate= GPIO_AF9_LTDC;
		HAL_GPIO_Init (GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_12;
		HAL_GPIO_Init (GPIOG, &GPIO_InitStructure);
		//}}}

		#define LCD_WIDTH   800  // min 39Mhz typ 45Mhz max 51.42Mhz
		#define LCD_HEIGHT 1280
		#define HORIZ_SYNC   64  // min  136  typ 176   max 216
		#define VERT_SYNC     1  // min   12  typ  25   max  38

		LtdcHandler.Instance = LTDC;
		LtdcHandler.Init.HorizontalSync     = HORIZ_SYNC - 1;
		LtdcHandler.Init.AccumulatedHBP     = HORIZ_SYNC - 1;
		LtdcHandler.Init.AccumulatedActiveW = HORIZ_SYNC + LCD_WIDTH - 1;
		LtdcHandler.Init.TotalWidth         = HORIZ_SYNC + LCD_WIDTH - 1;
		LtdcHandler.Init.VerticalSync       = VERT_SYNC - 1;
		LtdcHandler.Init.AccumulatedVBP     = VERT_SYNC - 1;
		LtdcHandler.Init.AccumulatedActiveH = VERT_SYNC + LCD_HEIGHT - 1;
		LtdcHandler.Init.TotalHeigh         = VERT_SYNC + LCD_HEIGHT - 1;
		LtdcHandler.Init.HSPolarity = LTDC_HSPOLARITY_AL;
		LtdcHandler.Init.VSPolarity = LTDC_VSPOLARITY_AL;
		LtdcHandler.Init.DEPolarity = LTDC_DEPOLARITY_AL;
		LtdcHandler.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
		LtdcHandler.Init.Backcolor.Red = 0;
		LtdcHandler.Init.Backcolor.Blue = 0;
		LtdcHandler.Init.Backcolor.Green = 0;
		HAL_LTDC_Init (&LtdcHandler);

		//DMA2D->AMTCR = 0x3F01;

		layerInit (0, frameBufferAddress);
		mSetFrameBufferAddress[1] = frameBufferAddress;
		showFrameBufferAddress[1] = frameBufferAddress;
		showAlpha[1] = 0;

		ltdc.timeouts = 0;
		ltdc.lineIrq = 0;
		ltdc.fifoUnderunIrq = 0;
		ltdc.transferErrorIrq = 0;
		ltdc.lastTicks = 0;
		ltdc.lineTicks = 0;
		ltdc.frameWait = 0;

		//vSemaphoreCreateBinary (ltdc.sem);

	 // HAL_NVIC_SetPriority (LTDC_IRQn, 0xE, 0);
	 // HAL_NVIC_EnableIRQ (LTDC_IRQn);

		// set line interupt line number
		//LTDC->LIPCR = 0;

		// enable line interrupt
		//LTDC->IER |= LTDC_IT_LI;
		}
	//}}}
	//{{{
	void layerInit (uint8_t layer, uint32_t frameBufferAddress) {

		LTDC_LayerCfgTypeDef* curLayerCfg = &LtdcHandler.LayerCfg[layer];

		curLayerCfg->WindowX0 = 0;
		curLayerCfg->WindowX1 = getLcdWidthPix();
		curLayerCfg->WindowY0 = 0;
		curLayerCfg->WindowY1 = getLcdHeightPix();

		curLayerCfg->PixelFormat = LTDC_PIXEL_FORMAT_RGB565;

		curLayerCfg->FBStartAdress = (uint32_t)frameBufferAddress;

		curLayerCfg->Alpha = 255;
		curLayerCfg->Alpha0 = 0;

		curLayerCfg->Backcolor.Blue = 0;
		curLayerCfg->Backcolor.Green = 0;
		curLayerCfg->Backcolor.Red = 0;

		curLayerCfg->BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
		curLayerCfg->BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;

		curLayerCfg->ImageWidth = getLcdWidthPix();
		curLayerCfg->ImageHeight = getLcdHeightPix();

		HAL_LTDC_ConfigLayer (&LtdcHandler, curLayerCfg, layer);

		// local state
		mCurFrameBufferAddress = frameBufferAddress;
		mSetFrameBufferAddress[layer] = frameBufferAddress;
		showFrameBufferAddress[layer] = frameBufferAddress;
		showAlpha[layer] = 255;
		}
	//}}}
	//{{{
	void setLayer (uint8_t layer, uint32_t frameBufferAddress) {

		mCurFrameBufferAddress = frameBufferAddress;
		mSetFrameBufferAddress[layer] = frameBufferAddress;
		}
	//}}}
	//{{{
	void showLayer (uint8_t layer, uint32_t frameBufferAddress, uint8_t alpha) {

		showFrameBufferAddress[layer] = frameBufferAddress;
		showAlpha[layer] = alpha;

		LTDC_Layer_TypeDef* ltdcLayer = (LTDC_Layer_TypeDef*)((uint32_t)LTDC + 0x84); // + (0x80*layer));
		ltdcLayer->CFBAR = showFrameBufferAddress[0];
		if (showAlpha[0]) {
			ltdcLayer->CR |= LTDC_LxCR_LEN;
			ltdcLayer->CACR &= ~LTDC_LxCACR_CONSTA;
			ltdcLayer->CACR = showAlpha[0];
			}
		else
			ltdcLayer->CR &= ~LTDC_LxCR_LEN;
		LTDC->SRCR |= LTDC_SRCR_IMR;
		}
	//}}}

	//{{{
	cFontChar* loadChar (uint16_t fontHeight, char ch) {

		FT_Set_Pixel_Sizes (FTface, 0, fontHeight);
		FT_Load_Char (FTface, ch, FT_LOAD_RENDER);

		auto fontChar = new cFontChar();
		fontChar->left = FTglyphSlot->bitmap_left;
		fontChar->top = FTglyphSlot->bitmap_top;
		fontChar->pitch = FTglyphSlot->bitmap.pitch;
		fontChar->rows = FTglyphSlot->bitmap.rows;
		fontChar->advance = FTglyphSlot->advance.x / 64;
		fontChar->bitmap = nullptr;

		if (FTglyphSlot->bitmap.buffer) {
			fontChar->bitmap = (uint8_t*)malloc (FTglyphSlot->bitmap.pitch * FTglyphSlot->bitmap.rows);
			memcpy (fontChar->bitmap, FTglyphSlot->bitmap.buffer, FTglyphSlot->bitmap.pitch * FTglyphSlot->bitmap.rows);
			}

		auto insertPair = mFontCharMap.insert (cFontCharMap::value_type (fontHeight<<8 | ch, fontChar));
		auto fontCharIt = insertPair.first;

		return fontCharIt->second;
		}
	//}}}
	//{{{
	void reset() {

		for (auto i = 0; i < mMaxLine; i++)
			mLines[i].clear();

		mStartTime = HAL_GetTick();
		mLastLine = -1;
		mFirstLine = 0;
		}
	//}}}
	//{{{
	void displayTop() {
		mFirstLine = 0;
		}
	//}}}
	//{{{
	void displayTail() {

		if (mLastLine > (int)mNumDrawLines-1)
			mFirstLine = mLastLine - mNumDrawLines + 1;
		else
			mFirstLine = 0;
		}
	//}}}
	//{{{
	void updateNumDrawLines() {

		mStringPos = getBoxHeight()*3;

		auto numDrawLines = getLcdHeightPix() / getBoxHeight();
		if (mShowTitle && !mTitle.empty())
			numDrawLines--;
		if (mShowLcdStats)
			numDrawLines--;
		if (mShowFooter)
			numDrawLines--;

		mNumDrawLines = numDrawLines;
		}
	//}}}

	//{{{  vars
	LTDC_HandleTypeDef LtdcHandler;

	int mStartTime = 0;

	float mFirstLine = 0;
	int mNumDrawLines = 0;
	int mStringPos = 0;

	bool mShowTitle = true;
	bool mShowInfo = true;
	bool mShowLcdStats = false;
	bool mShowFooter = true;

	std::string mTitle;

	int mLastLine = -1;
	int mMaxLine = 256;

	//{{{
	class cLine {
	public:
		cLine() {}
		~cLine() {}

		//{{{
		void clear() {
			mTime = 0;
			mColour = COL_WHITE;
			mString = "";
			}
		//}}}

		int mTime = 0;
		int mColour = COL_WHITE;
		std::string mString;
		};
	//}}}
	cLine mLines[256];

	int mDrawStartTime = 0;
	int mDrawTime = 0;
	bool mDrawBuffer = false;
	uint32_t mBuffer[2] = {0,0};

	uint32_t* mDma2dCurBuf = nullptr;
	uint32_t mDma2dTimeouts = 0;

	uint32_t mCurFrameBufferAddress = 0;
	uint32_t mSetFrameBufferAddress[2];
	uint32_t mCurDstColour = 0;
	uint32_t mCurSrcColour = 0;
	uint32_t mDstStride = 0;

	cFontCharMap mFontCharMap;

	FT_Library FTlibrary;
	FT_Face FTface;
	FT_GlyphSlot FTglyphSlot;
	//}}}
	};
//}}}
//{{{  Ps2
//{{{  ps2 keyboard defines
#define PS2_TAB       9
#define PS2_ENTER     13
#define PS2_BACKSPACE     127
#define PS2_ESC       27
#define PS2_INSERT      0
#define PS2_DELETE      127
#define PS2_HOME      0
#define PS2_END       0
#define PS2_PAGEUP      25
#define PS2_PAGEDOWN      26
#define PS2_UPARROW     11
#define PS2_LEFTARROW     8
#define PS2_DOWNARROW     10
#define PS2_RIGHTARROW      21

#define PS2_F1        0
#define PS2_F2        0
#define PS2_F3        0
#define PS2_F4        0
#define PS2_F5        0
#define PS2_F6        0
#define PS2_F7        0
#define PS2_F8        0
#define PS2_F9        0
#define PS2_F10       0
#define PS2_F11       0
#define PS2_F12       0
#define PS2_SCROLL      0

#define PS2_INVERTED_EXCLAMATION  161 //
#define PS2_CENT_SIGN     162 //
#define PS2_POUND_SIGN      163 //
#define PS2_CURRENCY_SIGN   164 //
#define PS2_YEN_SIGN      165 //
#define PS2_BROKEN_BAR                  166 //
#define PS2_SECTION_SIGN    167 //
#define PS2_DIAERESIS     168 //
#define PS2_COPYRIGHT_SIGN    169 //
#define PS2_FEMININE_ORDINAL    170 //
#define PS2_LEFT_DOUBLE_ANGLE_QUOTE 171 //
#define PS2_NOT_SIGN      172 //
#define PS2_HYPHEN      173
#define PS2_REGISTERED_SIGN   174 //
#define PS2_MACRON      175 //
#define PS2_DEGREE_SIGN     176 //
#define PS2_PLUS_MINUS_SIGN   177 //
#define PS2_SUPERSCRIPT_TWO   178 //
#define PS2_SUPERSCRIPT_THREE   179 //
#define PS2_ACUTE_ACCENT    180 //
#define PS2_MICRO_SIGN      181 //
#define PS2_PILCROW_SIGN    182 //
#define PS2_MIDDLE_DOT      183 //
#define PS2_CEDILLA     184 //
#define PS2_SUPERSCRIPT_ONE   185 //
#define PS2_MASCULINE_ORDINAL   186 //
#define PS2_RIGHT_DOUBLE_ANGLE_QUOTE  187 //
#define PS2_FRACTION_ONE_QUARTER  188 //
#define PS2_FRACTION_ONE_HALF   189 //
#define PS2_FRACTION_THREE_QUARTERS 190 //
#define PS2_INVERTED_QUESTION MARK  191 //
#define PS2_A_GRAVE     192 //
#define PS2_A_ACUTE     193 //
#define PS2_A_CIRCUMFLEX    194 //
#define PS2_A_TILDE     195 //
#define PS2_A_DIAERESIS     196 //
#define PS2_A_RING_ABOVE    197 //
#define PS2_AE        198 //
#define PS2_C_CEDILLA     199 //
#define PS2_E_GRAVE     200 //
#define PS2_E_ACUTE     201 //
#define PS2_E_CIRCUMFLEX    202 //
#define PS2_E_DIAERESIS     203 //
#define PS2_I_GRAVE     204 //
#define PS2_I_ACUTE     205 //
#define PS2_I_CIRCUMFLEX    206 //
#define PS2_I_DIAERESIS     207 //
#define PS2_ETH       208 //
#define PS2_N_TILDE     209 //
#define PS2_O_GRAVE     210 //
#define PS2_O_ACUTE     211 //
#define PS2_O_CIRCUMFLEX    212 //
#define PS2_O_TILDE     213 //
#define PS2_O_DIAERESIS     214 //
#define PS2_MULTIPLICATION    215 //
#define PS2_O_STROKE      216 //
#define PS2_U_GRAVE     217 //
#define PS2_U_ACUTE     218 //
#define PS2_U_CIRCUMFLEX    219 //
#define PS2_U_DIAERESIS     220 //
#define PS2_Y_ACUTE     221 //
#define PS2_THORN     222 //
#define PS2_SHARP_S     223 //
#define PS2_a_GRAVE     224 //
#define PS2_a_ACUTE     225 //
#define PS2_a_CIRCUMFLEX    226 //
#define PS2_a_TILDE     227 //
#define PS2_a_DIAERESIS     228 //
#define PS2_a_RING_ABOVE    229 //
#define PS2_ae        230 //
#define PS2_c_CEDILLA     231 //
#define PS2_e_GRAVE     232 //
#define PS2_e_ACUTE     233 //
#define PS2_e_CIRCUMFLEX    234 //
#define PS2_e_DIAERESIS     235 //
#define PS2_i_GRAVE     236 //
#define PS2_i_ACUTE     237 //
#define PS2_i_CIRCUMFLEX    238 //
#define PS2_i_DIAERESIS     239 //
#define PS2_eth       240 //
#define PS2_n_TILDE     241 //
#define PS2_o_GRAVE     242 //
#define PS2_o_ACUTE     243 //
#define PS2_o_CIRCUMFLEX    244 //
#define PS2_o_TILDE     245 //
#define PS2_o_DIAERESIS     246 //
#define PS2_DIVISION      247 //
#define PS2_o_STROKE      248 //
#define PS2_u_GRAVE     249 //
#define PS2_u_ACUTE     250 //
#define PS2_u_CIRCUMFLEX    251 //
#define PS2_u_DIAERESIS     252 //
#define PS2_y_ACUTE     253 //
#define PS2_thorn     254 //
#define PS2_y_DIAERESIS     255 //


#define PS2_KEYMAP_SIZE 136
//}}}
//{{{  struct ps2Keymap_t
typedef struct {
	uint8_t noshift[PS2_KEYMAP_SIZE];
	uint8_t shift[PS2_KEYMAP_SIZE];
	uint8_t uses_altgr;
	uint8_t altgr[PS2_KEYMAP_SIZE];
	} ps2Keymap_t;
//}}}
//{{{
const ps2Keymap_t kPs2Keymap = {
	// without shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	 0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '`', 0,
	 0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'q', '1', 0,
	 0, 0, 'z', 's', 'a', 'w', '2', 0,
	 0, 'c', 'x', 'd', 'e', '4', '3', 0,
	 0, ' ', 'v', 'f', 't', 'r', '5', 0,
	 0, 'n', 'b', 'h', 'g', 'y', '6', 0,
	 0, 0, 'm', 'j', 'u', '7', '8', 0,
	 0, ',', 'k', 'i', 'o', '0', '9', 0,
	 0, '.', '/', 'l', ';', 'p', '-', 0,
	 0, 0, '\'', 0, '[', '=', 0, 0,
	 0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, ']', 0, '\\', 0, 0,
	 0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
	 0, '1', 0, '4', '7', 0, 0, 0,
	 '0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	 PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	 0, 0, 0, PS2_F7 },

	// with shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	 0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '~', 0,
	 0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'Q', '!', 0,
	 0, 0, 'Z', 'S', 'A', 'W', '@', 0,
	 0, 'C', 'X', 'D', 'E', '$', '#', 0,
	 0, ' ', 'V', 'F', 'T', 'R', '%', 0,
	 0, 'N', 'B', 'H', 'G', 'Y', '^', 0,
	 0, 0, 'M', 'J', 'U', '&', '*', 0,
	 0, '<', 'K', 'I', 'O', ')', '(', 0,
	 0, '>', '?', 'L', ':', 'P', '_', 0,
	 0, 0, '"', 0, '{', '+', 0, 0,
	 0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '}', 0, '|', 0, 0,
	 0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
	 0, '1', 0, '4', '7', 0, 0, 0,
	 '0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	 PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	 0, 0, 0, PS2_F7 },

	0
	};
//}}}

//{{{  ps2 vars
int bitPos = -1;
uint16_t data = 0;
bool raw = true;
bool ps2rx = true;

volatile int inPtr = 0;
volatile int outPtr = 0;
int rxData[32];

bool rxExpandCode = false;
bool rxReleaseCode = false;
bool shifted = false;
bool ctrled = false;

bool stream = false;
int streamByte = -1;
uint8_t streamBytes[6];

int touchX = 0;
int touchY = 0;
int touchZ = 0;
//}}}
extern "C" { void EXTI2_IRQHandler(); }
//{{{
void EXTI2_IRQHandler() {

	if (__HAL_GPIO_EXTI_GET_IT (GPIO_PIN_2) != RESET) {
		__HAL_GPIO_EXTI_CLEAR_IT (GPIO_PIN_2);

		if (ps2rx) {
			bool bit = (GPIOG->IDR & GPIO_PIN_3) != 0;

			if (bitPos == -1) {
				//{{{  wait for lo start bit
				if (!bit) {
					// lo start bit
					bitPos = 0;
					data = 0;
					}
				}
				//}}}
			else if (bitPos < 8) {
				// get data bits 0..7
				data = data | (bit << bitPos);
				bitPos++;
				}
			else if (bitPos == 8) {
			 //{{{  parity bit - got data
			 if (stream) {
				 if (streamByte == -1) {
					 if ((data & 0xC0) == 0x80) {
						 streamByte = 0;
						 streamBytes[streamByte] =  data;
						 }
					 }
				 else {
					 streamByte++;
					 if ((streamByte == 3) && ((data & 0xc0) != 0xc0))
						 streamByte = -1;
					 else {
						 streamBytes[streamByte] = data;
							if (streamByte == 5) {
								touchX = ((streamBytes[3] & 0x10) << 8) | ((streamBytes[1] & 0x0F) << 8) | streamBytes[4];
								touchY = ((streamBytes[3] & 0x20) << 7) | ((streamBytes[1] & 0xF0) << 4) | streamBytes[5];
								touchZ = streamBytes[2];
								streamByte = -1;
								}
							}
					 }
				 }
			 else if (raw) {
				 rxData[inPtr] = data | (0x100 * rxReleaseCode);
				 inPtr = (inPtr + 1) % 32;
				 }
			 else if (data == 0xE0)
				 rxExpandCode = true;
			 else if (data == 0xF0)
				 rxReleaseCode = true;
			 else if (data == 0x12) // SHIFT_L;
				 shifted = !rxReleaseCode;
			 else if (data == 0x59) // SHIFT_R;
				 shifted = !rxReleaseCode;
			 else if (data == 0x14) // CTRL_L
				 ctrled = !rxReleaseCode;
			 else {
				 if (rxExpandCode) {
					 if (data == 0x70)
						 data = PS2_INSERT;
					 else if (data == 0x6C)
						 data = PS2_HOME;
					 else if (data == 0x7D)
						 data = PS2_PAGEUP;
					 else if (data == 0x71)
						 data = PS2_DELETE;
					 else if (data == 0x6C)
						 data = PS2_HOME;
					 else if (data == 0x69)
						 data = PS2_END;
					 else if (data == 0x6C)
						 data = PS2_PAGEDOWN;
					 else if (data == 0x75)
						 data = PS2_UPARROW;
					 else if (data == 0x6B)
						 data = PS2_LEFTARROW;
					 else if (data == 0x72)
						 data = PS2_DOWNARROW;
					 else if (data == 0x74)
						 data = PS2_RIGHTARROW;
					 else if (data == 0x4A)
						 data = '/';
					 else if (data == 0x5A)
						 data = PS2_ENTER;
					 else
						 data |= 0x200;
					 }
				 else if (shifted)
					 data = kPs2Keymap.shift[data];
				 else
					 data = kPs2Keymap.noshift[data];

				 rxData[inPtr] = data | (0x100 * rxReleaseCode);
				 inPtr = (inPtr + 1) % 32;
				 rxExpandCode = false;
				 rxReleaseCode = false;
				 }

			 bitPos++;
			 }
			 //}}}
			else if (bitPos == 9) {
				//{{{  expect hi stop bit
				if (bit)
					bitPos = -1;
				}
				//}}}
			}
		}
	}
//}}}

//{{{
uint16_t ps2get() {
	while (inPtr == outPtr) {}
	uint16_t ch = rxData[outPtr];
	outPtr = (outPtr + 1) % 32;
	return ch;
	}
//}}}
//{{{
void ps2send (uint8_t value)  {

	HAL_GPIO_WritePin (GPIOG, GPIO_PIN_2, GPIO_PIN_RESET); // set clock lo, release inhibit, if necessary
	HAL_Delay (2); // Wait out any final clock pulse, 100us

	ps2rx = false;
	HAL_GPIO_WritePin (GPIOG, GPIO_PIN_3, GPIO_PIN_RESET); // set data lo, start bit
	HAL_GPIO_WritePin (GPIOG, GPIO_PIN_2, GPIO_PIN_SET);   // set clock hi, float

	uint8_t parity = 1;
	for (int bit = 0; bit < 8; bit++) {
		while (HAL_GPIO_ReadPin (GPIOG, GPIO_PIN_2)) {} // wait for rising edge
		HAL_GPIO_WritePin (GPIOG, GPIO_PIN_3, (value & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET); // set data to i�th data bit
		parity = parity + value;  // Accumulate parity
		value = value >> 1;       // Shift right to get next bit
		while (!HAL_GPIO_ReadPin (GPIOG, GPIO_PIN_2)) {} // wait for falling edge
		}

	while (HAL_GPIO_ReadPin (GPIOG, GPIO_PIN_2)) {} // wait for rising edge
	HAL_GPIO_WritePin (GPIOG, GPIO_PIN_3, (parity & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET); // set data to parity bit
	while (!HAL_GPIO_ReadPin (GPIOG, GPIO_PIN_2)) {} // wait for falling edge

	while (HAL_GPIO_ReadPin (GPIOG, GPIO_PIN_2)) {} // wait for rising edge
	HAL_GPIO_WritePin (GPIOG, GPIO_PIN_3, GPIO_PIN_SET); // set data hi, stop bit
	while (!HAL_GPIO_ReadPin (GPIOG, GPIO_PIN_2)) {} // wait for falling edge

	while (HAL_GPIO_ReadPin (GPIOG, GPIO_PIN_2)) {} // wait for rising edge
	if (HAL_GPIO_ReadPin (GPIOG, GPIO_PIN_2) == true)
		lcd->info ("ps2send - missing line control bit");
	while (!HAL_GPIO_ReadPin (GPIOG, GPIO_PIN_2)) {} // wait for falling edge
	ps2rx = true;

	if (ps2get() != 0xFA)
		lcd->info ("ps2send - no 0xFA ack");
	}
//}}}
//{{{
void ps2sendTouchpadSpecialCommand (uint8_t arg) {
// send touchpad special command sequence

	for (int i = 0; i < 4; i++) {
		ps2send (0xE8);   // touchpad setResolution
		ps2send ((arg >> (6-2*i)) & 3);
		}
	}
//}}}
//{{{
void initPs2gpio() {

	bitPos = -1;
	inPtr = 0;
	outPtr = 0;

	__HAL_RCC_GPIOG_CLK_ENABLE();

	// PS2 clock
	GPIO_InitTypeDef GPIO_Init_Structure;
	GPIO_Init_Structure.Mode = GPIO_MODE_IT_RISING;
	GPIO_Init_Structure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_Init_Structure.Pull = GPIO_PULLUP;
	GPIO_Init_Structure.Pin = GPIO_PIN_2;
	HAL_GPIO_Init (GPIOG, &GPIO_Init_Structure);

	// PS2 data
	GPIO_Init_Structure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_Init_Structure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_Init_Structure.Pull = GPIO_PULLUP;
	GPIO_Init_Structure.Pin = GPIO_PIN_3;
	HAL_GPIO_Init (GPIOG, &GPIO_Init_Structure);

	// Enable and set EXTI line 2 Interrupt to the lowest priority
	HAL_NVIC_SetPriority (EXTI2_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ (EXTI2_IRQn);

	HAL_GPIO_WritePin (GPIOG, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin (GPIOG, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_Delay (100);
	}
//}}}
//{{{
void initPs2keyboard() {

	ps2send (0xFF);
	if (ps2get() != 0xAA)
		lcd->info ("initPs2keyboard - missing 0xAA reset");

	for (int i = 0; i < 8; i++) {
		ps2send (0xED); // send leds
		ps2send (i);
		HAL_Delay (100);
		}

	ps2send (0x0F2); // sendId
	lcd->info ("keyboard id " + hex (ps2get()) + hex(ps2get()));
	}
//}}}
//{{{
void initPs2touchpad() {

	ps2send (0xFF);  // Touchpad reset
	if (ps2get() != 0xAA)
		lcd->info ("initPs2touchpad - missing 0xAA reset");
	if (ps2get() != 0x00)
		lcd->info ("initPs2touchpad - missing 0x00 reset");

	//ps2sendTouchpadSpecialCommand (0x00);
	//ps2send (0xE9); // touchpad statusRequest prefixed by specialCommand
	//auto minor = ps2get();
	//ps2get();  // 0x47
	//auto major = ps2get();
	//lcd->info ("Identify " + hex (major & 0x0F) + "." + hex (minor) + " modelCode:" + hex (major >> 4));

	//ps2sendTouchpadSpecialCommand (0x02);
	//ps2send (0xE9); // touchpad statusRequest prefixed by specialCommand
	//auto capMsb = ps2get();
	//ps2get();  // 0x47
	//auto capLsb = PS2get();
	//lcd->info ("Capabilities " + hex ((capMsb << 8) | capLsb));

	//ps2sendTouchpadSpecialCommand (0x03);
	//ps2send (0xE9); // touchpad statusRequest prefixed by specialCommand
	//ps2send modelId1 = ps2get();
	//auto modelId2 = ps2get();
	//auto modelId3 = ps2get();
	//lcd->info ("ModelId " + hex ((modelId1 << 16) | (modelId2 << 8) | modelId3));

	ps2sendTouchpadSpecialCommand (0x80);
	ps2send (0xF3); // touchpad setSampleRate prefixed by specialCommand
	ps2send (0x14); // - setSampleRate = 20

	ps2send (0xF4); // touchpad enable streaming
	}
//}}}
//}}}
//{{{  Sd
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

	//if (HAL_SD_HighSpeed (&uSdHandle) == SD_OK)
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

#include "resize.h"
std::vector<std::string> mFileNames;
//{{{
void listDirectory (std::string directoryName, std::string ext) {

	//lcd->info ("dir " + directoryName);
	//lcd->render();

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
			//debug (fileInfo.getName());
			}

		else if (fileInfo.isDirectory()) {
			listDirectory (directoryName + "/" + fileInfo.getName(), ext);
			}
		else if (fileInfo.matchExtension (ext.c_str())) {
			mFileNames.push_back (directoryName + "/" + fileInfo.getName());
			//lcd->info (fileInfo.getName());
			//lcd->render();
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
	SDRAMgpioInit();
	SDRAMbank1Init();
	SDRAMbank2Init();

	HeapRegion_t xHeapRegions[] = { {(uint8_t*)SDRAM_BANK2_ADDR + 0x400000, 0x400000 }, { nullptr, 0 } };
	heapInit (xHeapRegions);

	// clear frameBuffer
	memset ((void*)SDRAM_BANK2_ADDR, 0, 0x400000);

	lcd = new cLcd (SDRAM_BANK2_ADDR, SDRAM_BANK2_ADDR + 0x200000);
	const std::string kHello = "built " + std::string(__TIME__) + " on " + std::string(__DATE__);
	lcd->init ("stm32F429disco test - " + kHello);
	lcd->displayOn();
	lcd->render();

	if (BSP_PB_GetState (BUTTON_KEY) == GPIO_PIN_SET) {
		//{{{  ps2
		initPs2gpio();
		initPs2touchpad();
		stream = true;
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
			listDirectory ("", "JPG");
			}
		else
			lcd->debug ("no SD card");
		}
		//}}}

	for (auto fileStr : mFileNames) {
		lcd->info (fileStr);
		cFile file (fileStr, FA_OPEN_EXISTING | FA_READ);
		auto buf = (uint8_t*)pvPortMalloc (file.getSize());
		auto bytesRead = 0;
		file.read (buf, file.getSize(), bytesRead);
		lcd->info ("- read");

		cJpegPic* jpeg = new cJpegPic(3, buf);
		jpeg->readHeader();
		auto width = jpeg->getWidth();
		auto height = jpeg->getHeight();

		auto scaleShift = 0;
		auto scale = 1;
		while ((scaleShift < 3) &&
					 ((width / scale > lcd->getLcdWidthPix()) || (height /scale > lcd->getLcdHeightPix()))) {
			scale *= 2;
			scaleShift++;
			}

		auto out = jpeg->decodeBody (scaleShift);
		delete (jpeg);
		vPortFree (buf);

		lcd->info ("- decode " + dec(file.getSize()) + " " + dec(width) + ":" + dec(height) + " " + dec(scaleShift));


		lcd->startRender();
		lcd->clear (COL_BLACK);
		//lcd->copy (out, 0, 0, width >> scaleShift , height >> scaleShift);
		lcd->endRender (true);

		uint32_t* workBuf = (uint32_t*)pvPortMalloc ((width >> scaleShift) * 600 * 4);
		lcd->debug ("- buf " + hex ((uint32_t)workBuf) + " " +
								hex ((uint32_t)out) + " " +
								hex ((width >> scaleShift) * 500 * 4));

		for (int i = 200; i < 800; i += 3) {
			RESIZE_InitTypedef Resize = {
				(void*)out, width >> scaleShift, DMA2D_INPUT_RGB888, 0, 0, width >> scaleShift, height >> scaleShift,
				(void*)SDRAM_BANK2_ADDR, 800, DMA2D_RGB565, 0, 0, i, i,
				workBuf };
			resize (&Resize);
			//vPortFree (workBuf);
			//lcd->debug ("done");
			}

		vPortFree (out);
		}

	while (true) {
		lcd->startRender();
		lcd->clear (COL_BLACK);
		//std::string str = "x:" + dec(touchX) + " y:" + dec(touchY) + " z:" + dec(touchZ);
		//lcd->text (COL_YELLOW, getFontHeight(), str, 100, 200, lcd->getLcdWidthPix(), getBoxHeight());
		auto x = lcd->getLcdWidthPix() - ((touchY-1300)/4);
		auto y = lcd->getLcdHeightPix() - ((touchX-1300)/4);
		lcd->ellipse (COL_YELLOW, x, y, touchZ, touchZ);
		lcd->endRender (true);
		//uint16_t value = PS2get();
		//char ch = (char)value;
		//std::string charStr (1, ch);
		//std::string releaseStr = (value & 0x100) ? " release " : " ";
		//lcd->info ("key:" + charStr + releaseStr + hex (value));
		}
	}
//}}}
