################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -c -I../Inc -I"D:/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/shared/Drivers/CMSIS/Include" -I"D:/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/shared/Drivers/STM32F401RE_StdPeriph_Driver/inc" -I"D:/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/shared/Middle/button" -I"D:/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/shared/Middle/buzzer" -I"D:/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/shared/Middle/Dino" -I"D:/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/shared/Middle/flash" -I"D:/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/shared/Middle/Game" -I"D:/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/shared/Middle/led" -I"D:/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/shared/Middle/Obstracle" -I"D:/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/shared/Middle/rtos" -I"D:/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/shared/Middle/sensor" -I"D:/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/shared/Middle/serial" -I"D:/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/shared/Middle/ucglib" -I"D:/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master/shared/Utilities" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

