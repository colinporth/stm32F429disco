################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_cortex.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_dma.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_flash.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_flash_ex.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_gpio.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_i2c.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_i2c_ex.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_ltdc.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_pwr.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_pwr_ex.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_rcc.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_rcc_ex.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_sd.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_sdram.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_spi.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_uart.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_ll_fmc.c \
C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_ll_sdmmc.c 

OBJS += \
./HAL_Driver/stm32f4xx_hal.o \
./HAL_Driver/stm32f4xx_hal_cortex.o \
./HAL_Driver/stm32f4xx_hal_dma.o \
./HAL_Driver/stm32f4xx_hal_flash.o \
./HAL_Driver/stm32f4xx_hal_flash_ex.o \
./HAL_Driver/stm32f4xx_hal_gpio.o \
./HAL_Driver/stm32f4xx_hal_i2c.o \
./HAL_Driver/stm32f4xx_hal_i2c_ex.o \
./HAL_Driver/stm32f4xx_hal_ltdc.o \
./HAL_Driver/stm32f4xx_hal_pwr.o \
./HAL_Driver/stm32f4xx_hal_pwr_ex.o \
./HAL_Driver/stm32f4xx_hal_rcc.o \
./HAL_Driver/stm32f4xx_hal_rcc_ex.o \
./HAL_Driver/stm32f4xx_hal_sd.o \
./HAL_Driver/stm32f4xx_hal_sdram.o \
./HAL_Driver/stm32f4xx_hal_spi.o \
./HAL_Driver/stm32f4xx_hal_uart.o \
./HAL_Driver/stm32f4xx_ll_fmc.o \
./HAL_Driver/stm32f4xx_ll_sdmmc.o 

C_DEPS += \
./HAL_Driver/stm32f4xx_hal.d \
./HAL_Driver/stm32f4xx_hal_cortex.d \
./HAL_Driver/stm32f4xx_hal_dma.d \
./HAL_Driver/stm32f4xx_hal_flash.d \
./HAL_Driver/stm32f4xx_hal_flash_ex.d \
./HAL_Driver/stm32f4xx_hal_gpio.d \
./HAL_Driver/stm32f4xx_hal_i2c.d \
./HAL_Driver/stm32f4xx_hal_i2c_ex.d \
./HAL_Driver/stm32f4xx_hal_ltdc.d \
./HAL_Driver/stm32f4xx_hal_pwr.d \
./HAL_Driver/stm32f4xx_hal_pwr_ex.d \
./HAL_Driver/stm32f4xx_hal_rcc.d \
./HAL_Driver/stm32f4xx_hal_rcc_ex.d \
./HAL_Driver/stm32f4xx_hal_sd.d \
./HAL_Driver/stm32f4xx_hal_sdram.d \
./HAL_Driver/stm32f4xx_hal_spi.d \
./HAL_Driver/stm32f4xx_hal_uart.d \
./HAL_Driver/stm32f4xx_ll_fmc.d \
./HAL_Driver/stm32f4xx_ll_sdmmc.d 


# Each subdirectory must supply rules for building sources it contributes
HAL_Driver/stm32f4xx_hal.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_hal_cortex.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_cortex.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_hal_dma.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_dma.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_hal_flash.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_flash.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_hal_flash_ex.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_flash_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_hal_gpio.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_hal_i2c.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_i2c.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_hal_i2c_ex.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_i2c_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_hal_ltdc.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_ltdc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_hal_pwr.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_pwr.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_hal_pwr_ex.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_pwr_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_hal_rcc.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_rcc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_hal_rcc_ex.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_rcc_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_hal_sd.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_sd.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_hal_sdram.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_sdram.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_hal_spi.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_spi.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_hal_uart.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_hal_uart.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_ll_fmc.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_ll_fmc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL_Driver/stm32f4xx_ll_sdmmc.o: C:/Users/colin/Desktop/stm32F429disco/HAL_Driver/stm32f4xx_ll_sdmmc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


