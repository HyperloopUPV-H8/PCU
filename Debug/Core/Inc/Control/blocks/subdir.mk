################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Inc/Control/blocks/alpha_beta.cpp \
../Core/Inc/Control/blocks/dutys.cpp \
../Core/Inc/Control/blocks/theta.cpp \
../Core/Inc/Control/blocks/times.cpp \
../Core/Inc/Control/blocks/voltages.cpp 

OBJS += \
./Core/Inc/Control/blocks/alpha_beta.o \
./Core/Inc/Control/blocks/dutys.o \
./Core/Inc/Control/blocks/theta.o \
./Core/Inc/Control/blocks/times.o \
./Core/Inc/Control/blocks/voltages.o 

CPP_DEPS += \
./Core/Inc/Control/blocks/alpha_beta.d \
./Core/Inc/Control/blocks/dutys.d \
./Core/Inc/Control/blocks/theta.d \
./Core/Inc/Control/blocks/times.d \
./Core/Inc/Control/blocks/voltages.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/Control/blocks/%.o Core/Inc/Control/blocks/%.su Core/Inc/Control/blocks/%.cyclo: ../Core/Inc/Control/blocks/%.cpp Core/Inc/Control/blocks/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++20 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H723xx -DDATA_IN_D2_SRAM -DHSE_VALUE=BOARD -DBOARD=25000000UL -c -I"/home/dgoncas/Desktop/Firmware_workspace/ST-LIB" -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"/home/dgoncas/Desktop/Firmware_workspace/ST-LIB/Inc" -I"/home/dgoncas/Desktop/Firmware_workspace/ST-LIB/Inc/HALAL/Models" -I"/home/dgoncas/Desktop/Firmware_workspace/ST-LIB/Inc/HALAL/Services" -I"/home/dgoncas/Desktop/Firmware_workspace/ST-LIB/Inc/ST-LIB_LOW" -I"/home/dgoncas/Desktop/Firmware_workspace/ST-LIB/Inc/ST-LIB_HIGH" -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I"/home/dgoncas/Desktop/Firmware_workspace/ST-LIB/Inc/HALAL/Services/Communication" -I"/home/dgoncas/Desktop/Firmware_workspace/ST-LIB/Inc/HALAL/Services/Communication/Ethernet" -I"/home/dgoncas/Desktop/Firmware_workspace/ST-LIB/Inc/HALAL/Services/Communication/Ethernet/UDP" -I"/home/dgoncas/Desktop/Firmware_workspace/ST-LIB/Inc/HALAL/Services/Communication/Ethernet/TCP" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-Control-2f-blocks

clean-Core-2f-Inc-2f-Control-2f-blocks:
	-$(RM) ./Core/Inc/Control/blocks/alpha_beta.cyclo ./Core/Inc/Control/blocks/alpha_beta.d ./Core/Inc/Control/blocks/alpha_beta.o ./Core/Inc/Control/blocks/alpha_beta.su ./Core/Inc/Control/blocks/dutys.cyclo ./Core/Inc/Control/blocks/dutys.d ./Core/Inc/Control/blocks/dutys.o ./Core/Inc/Control/blocks/dutys.su ./Core/Inc/Control/blocks/theta.cyclo ./Core/Inc/Control/blocks/theta.d ./Core/Inc/Control/blocks/theta.o ./Core/Inc/Control/blocks/theta.su ./Core/Inc/Control/blocks/times.cyclo ./Core/Inc/Control/blocks/times.d ./Core/Inc/Control/blocks/times.o ./Core/Inc/Control/blocks/times.su ./Core/Inc/Control/blocks/voltages.cyclo ./Core/Inc/Control/blocks/voltages.d ./Core/Inc/Control/blocks/voltages.o ./Core/Inc/Control/blocks/voltages.su

.PHONY: clean-Core-2f-Inc-2f-Control-2f-blocks

