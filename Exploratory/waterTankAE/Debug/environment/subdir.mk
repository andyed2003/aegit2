################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../environment/environmentImpl.c 

OBJS += \
./environment/environmentImpl.o 

C_DEPS += \
./environment/environmentImpl.d 


# Each subdirectory must supply rules for building sources it contributes
environment/%.o: ../environment/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/andy/git/aegit/Exploratory/waterTankAE/FakeFMI" -I"/home/andy/git/aegit/Exploratory/waterTankAE/controller" -I"/home/andy/git/aegit/Exploratory/waterTankAE/environment" -I"/home/andy/git/aegit/Exploratory/waterTankAE/common" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


