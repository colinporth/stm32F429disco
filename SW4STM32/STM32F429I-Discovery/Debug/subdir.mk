################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:/Users/colin/Desktop/stm32F429disco/Src/cSd.cpp \
C:/Users/colin/Desktop/stm32F429disco/Src/main.cpp 

C_SRCS += \
C:/Users/colin/Desktop/stm32F429disco/Src/heap_5.c \
C:/Users/colin/Desktop/stm32F429disco/Src/stm32f429i_discovery.c \
C:/Users/colin/Desktop/stm32F429disco/Src/stm32f429i_discovery_sdram.c \
C:/Users/colin/Desktop/stm32F429disco/Src/stm32f4xx_it.c \
C:/Users/colin/Desktop/stm32F429disco/Src/syscalls.c 

OBJS += \
./cSd.o \
./heap_5.o \
./main.o \
./stm32f429i_discovery.o \
./stm32f429i_discovery_sdram.o \
./stm32f4xx_it.o \
./syscalls.o 

C_DEPS += \
./heap_5.d \
./stm32f429i_discovery.d \
./stm32f429i_discovery_sdram.d \
./stm32f4xx_it.d \
./syscalls.d 

CPP_DEPS += \
./cSd.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
cSd.o: C:/Users/colin/Desktop/stm32F429disco/Src/cSd.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=c++0x -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include" -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

heap_5.o: C:/Users/colin/Desktop/stm32F429disco/Src/heap_5.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.o: C:/Users/colin/Desktop/stm32F429disco/Src/main.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=c++0x -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include" -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f429i_discovery.o: C:/Users/colin/Desktop/stm32F429disco/Src/stm32f429i_discovery.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f429i_discovery_sdram.o: C:/Users/colin/Desktop/stm32F429disco/Src/stm32f429i_discovery_sdram.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4xx_it.o: C:/Users/colin/Desktop/stm32F429disco/Src/stm32f4xx_it.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

syscalls.o: C:/Users/colin/Desktop/stm32F429disco/Src/syscalls.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Inc" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


