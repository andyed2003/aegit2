################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/waterTankAE/Common.c \
../src/waterTankAE/MainEntry.c \
../src/waterTankAE/controllerImpl.c \
../src/waterTankAE/environmentImpl.c 

OBJS += \
./src/waterTankAE/Common.o \
./src/waterTankAE/MainEntry.o \
./src/waterTankAE/controllerImpl.o \
./src/waterTankAE/environmentImpl.o 

C_DEPS += \
./src/waterTankAE/Common.d \
./src/waterTankAE/MainEntry.d \
./src/waterTankAE/controllerImpl.d \
./src/waterTankAE/environmentImpl.d 


# Each subdirectory must supply rules for building sources it contributes
src/waterTankAE/%.o: ../src/waterTankAE/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


