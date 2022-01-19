################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/timing/CoderTimeAPI.c \
../src/timing/coder_posix_time.c \
../src/timing/get_tic.c \
../src/timing/get_tic_data.c \
../src/timing/get_tic_initialize.c \
../src/timing/get_tic_terminate.c \
../src/timing/tic.c \
../src/timing/timeKeeper.c \
../src/timing/toc.c 

OBJS += \
./src/timing/CoderTimeAPI.o \
./src/timing/coder_posix_time.o \
./src/timing/get_tic.o \
./src/timing/get_tic_data.o \
./src/timing/get_tic_initialize.o \
./src/timing/get_tic_terminate.o \
./src/timing/tic.o \
./src/timing/timeKeeper.o \
./src/timing/toc.o 

C_DEPS += \
./src/timing/CoderTimeAPI.d \
./src/timing/coder_posix_time.d \
./src/timing/get_tic.d \
./src/timing/get_tic_data.d \
./src/timing/get_tic_initialize.d \
./src/timing/get_tic_terminate.d \
./src/timing/tic.d \
./src/timing/timeKeeper.d \
./src/timing/toc.d 


# Each subdirectory must supply rules for building sources it contributes
src/timing/%.o: ../src/timing/%.c src/timing/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


