// cSd.cpp
//{{{  includes
#include <stdint.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "heap.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include "cSd.h"
//}}}
//{{{
template <typename T> std::string dec (T value, uint16_t width = 0, char fill = ' ') {

  std::ostringstream os;
  os << std::setfill (fill) << std::setw (width) << value;
  return os.str();
  }
//}}}
//{{{
template <typename T> std::string hex (T value, uint16_t width = 0) {

  std::ostringstream os;
  os << std::hex << std::setfill ('0') << std::setw (width) << value;
  return os.str();
  }
//}}}

//{{{  static vars
static HAL_SD_CardInfoTypedef uSdCardInfo;
static DMA_HandleTypeDef dmaRxHandle;
static DMA_HandleTypeDef dmaTxHandle;

static const uint32_t mReadCacheSize = 0x40;
static uint8_t* mReadCache = 0;
static uint32_t mReadCacheBlock = 0xFFFFFFB0;
static uint32_t mReads = 0;
static uint32_t mReadHits = 0;
static uint32_t mReadMultipleLen = 0;
static uint32_t mReadBlock = 0xFFFFFFFF;

static uint32_t mWrites = 0;
static uint32_t mWriteMultipleLen = 0;
static uint32_t mWriteBlock = 0xFFFFFFFF;
//}}}

SD_HandleTypeDef uSdHandle;
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
  uSdHandle.Init.ClockDiv            = SDIO_TRANSFER_CLK_DIV;

  __HAL_RCC_SDIO_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  //{{{  gpio init
  GPIO_InitTypeDef  gpio_init_structure;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_SDIO;

  gpio_init_structure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
  HAL_GPIO_Init (GPIOC, &gpio_init_structure);

  gpio_init_structure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init (GPIOD, &gpio_init_structure);
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
  return true;
  //!(SD_DETECT_GPIO_PORT->IDR & SD_DETECT_PIN);
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

  if (HAL_SD_ReadBlocks_DMA (&uSdHandle, (uint32_t*)buf, blk_addr * 512, 512, blocks) != SD_OK)
    return MSD_ERROR;
  if (HAL_SD_CheckReadOperation (&uSdHandle, 0xFFFFFFF) != SD_OK)
    return MSD_ERROR;

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
uint8_t SD_Erase (uint64_t StartAddr, uint64_t EndAddr) {
  if (HAL_SD_Erase (&uSdHandle, StartAddr, EndAddr) != SD_OK)
    return MSD_ERROR;
  else
    return MSD_OK;
  }
//}}}

// usb MSC interface
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
