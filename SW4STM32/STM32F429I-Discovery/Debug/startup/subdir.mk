################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
C:/Users/colin/Desktop/stm32F429disco/Src/startup_stm32f429xx.s 

OBJS += \
./startup/startup_stm32f429xx.o 


# Each subdirectory must supply rules for building sources it contributes
startup/startup_stm32f429xx.o: C:/Users/colin/Desktop/stm32F429disco/Src/startup_stm32f429xx.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo %cd%
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


