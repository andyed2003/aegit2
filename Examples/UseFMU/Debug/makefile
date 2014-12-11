################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: UseFMU

# Tool invocations
UseFMU: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/andy/git/aegit/Examples/TestFMUPumpC_V1/Release_X86_64_LINUX64" -L"/home/andy/git/aegit/Examples/TestFMUPumpC_V1/Release_i686_WIN32" -o "UseFMU" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) UseFMU
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
