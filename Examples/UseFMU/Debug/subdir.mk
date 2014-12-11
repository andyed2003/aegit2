################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../myC.c 

OBJS += \
./myC.o 

C_DEPS += \
./myC.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/andy/git/aegit/Examples/TestFMUPumpC_V1/external" -I"/home/andy/git/aegit/Examples/TestFMUPumpC_V1/Release_X86_64_LINUX64" -I"/home/andy/git/aegit/Examples/TestFMUPumpC_V1/src" -I"/home/andy/git/aegit/Examples/TestFMUPumpC_V1" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


