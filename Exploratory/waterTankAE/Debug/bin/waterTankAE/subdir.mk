################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bin/waterTankAE/Common.c \
../bin/waterTankAE/MainEntry.c \
../bin/waterTankAE/controllerImpl.c \
../bin/waterTankAE/environmentImpl.c 

OBJS += \
./bin/waterTankAE/Common.o \
./bin/waterTankAE/MainEntry.o \
./bin/waterTankAE/controllerImpl.o \
./bin/waterTankAE/environmentImpl.o 

C_DEPS += \
./bin/waterTankAE/Common.d \
./bin/waterTankAE/MainEntry.d \
./bin/waterTankAE/controllerImpl.d \
./bin/waterTankAE/environmentImpl.d 


# Each subdirectory must supply rules for building sources it contributes
bin/waterTankAE/%.o: ../bin/waterTankAE/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


