################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/autofit/autofit.c \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftapi.c \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftbase.c \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftbbox.c \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftbitmap.c \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftdebug.c \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftglyph.c \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/smooth/ftgrays.c \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftinit.c \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftlcdfil.c \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftpatent.c \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/smooth/ftsmooth.c \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftsystem.c \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/md5.c \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/raster/raster.c \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/sfnt/sfnt.c \
C:/Users/colin/Desktop/stm32F429disco/freetype/src/truetype/truetype.c 

OBJS += \
./freetype/autofit.o \
./freetype/ftapi.o \
./freetype/ftbase.o \
./freetype/ftbbox.o \
./freetype/ftbitmap.o \
./freetype/ftdebug.o \
./freetype/ftglyph.o \
./freetype/ftgrays.o \
./freetype/ftinit.o \
./freetype/ftlcdfil.o \
./freetype/ftpatent.o \
./freetype/ftsmooth.o \
./freetype/ftsystem.o \
./freetype/md5.o \
./freetype/raster.o \
./freetype/sfnt.o \
./freetype/truetype.o 

C_DEPS += \
./freetype/autofit.d \
./freetype/ftapi.d \
./freetype/ftbase.d \
./freetype/ftbbox.d \
./freetype/ftbitmap.d \
./freetype/ftdebug.d \
./freetype/ftglyph.d \
./freetype/ftgrays.d \
./freetype/ftinit.d \
./freetype/ftlcdfil.d \
./freetype/ftpatent.d \
./freetype/ftsmooth.d \
./freetype/ftsystem.d \
./freetype/md5.d \
./freetype/raster.d \
./freetype/sfnt.d \
./freetype/truetype.d 


# Each subdirectory must supply rules for building sources it contributes
freetype/autofit.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/autofit/autofit.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

freetype/ftapi.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftapi.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

freetype/ftbase.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftbase.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

freetype/ftbbox.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftbbox.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

freetype/ftbitmap.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftbitmap.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

freetype/ftdebug.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftdebug.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

freetype/ftglyph.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftglyph.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

freetype/ftgrays.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/smooth/ftgrays.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

freetype/ftinit.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftinit.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

freetype/ftlcdfil.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftlcdfil.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

freetype/ftpatent.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftpatent.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

freetype/ftsmooth.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/smooth/ftsmooth.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

freetype/ftsystem.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/ftsystem.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

freetype/md5.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/base/md5.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

freetype/raster.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/raster/raster.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

freetype/sfnt.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/sfnt/sfnt.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

freetype/truetype.o: C:/Users/colin/Desktop/stm32F429disco/freetype/src/truetype/truetype.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DFT2_BUILD_LIBRARY -DSTM32F429xx -DUSE_STM32F429I_DISCO -I"C:/Users/colin/Desktop/stm32F429disco/HAL_Driver" -I"C:/Users/colin/Desktop/stm32F429disco/Src" -I"C:/Users/colin/Desktop/stm32F429disco/freetype/include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -Wno-unused-variable -Wno-pointer-sign -Wno-main -Wno-format -Wno-address -Wno-unused-but-set-variable -Wno-strict-aliasing -Wno-parentheses -Wno-missing-braces -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


