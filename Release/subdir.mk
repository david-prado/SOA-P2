################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../client.c \
../forked.c \
../pre-forked.c \
../pre-threaded.c \
../secuential.c \
../threaded.c 

OBJS += \
./client.o \
./forked.o \
./pre-forked.o \
./pre-threaded.o \
./secuential.o \
./threaded.o 

C_DEPS += \
./client.d \
./forked.d \
./pre-forked.d \
./pre-threaded.d \
./secuential.d \
./threaded.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


