################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/011i2c_master_rx_testing.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/011i2c_master_rx_testing.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/011i2c_master_rx_testing.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"C:/Users/rafae/Documents/Embedded-C/MCU1-Course/MCU1/tutor_drivers/drivers/inc" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/011i2c_master_rx_testing.d ./Src/011i2c_master_rx_testing.o ./Src/011i2c_master_rx_testing.su ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

