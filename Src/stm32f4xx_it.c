#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "cSdPrivate.h"

void NMI_Handler();
void HardFault_Handler();
void MemManage_Handler();
void BusFault_Handler();
void UsageFault_Handler();
void SVC_Handler();
void DebugMon_Handler();
void PendSV_Handler();
void SysTick_Handler();
void EXTI0_IRQHandler();

void NMI_Handler() { }

void HardFault_Handler() { while (1) { } }
void MemManage_Handler() { while (1) { } }
void BusFault_Handler() { while (1) { } }
void UsageFault_Handler() { while (1) { } }

void SVC_Handler() { }
void DebugMon_Handler() { }
void PendSV_Handler() { }

void SysTick_Handler() { HAL_IncTick(); }

void SDIO_IRQHandler() { HAL_SD_IRQHandler (&uSdHandle); }
void DMA2_Stream3_IRQHandler() { HAL_DMA_IRQHandler (uSdHandle.hdmarx); }
void DMA2_Stream6_IRQHandler() { HAL_DMA_IRQHandler (uSdHandle.hdmatx); }
