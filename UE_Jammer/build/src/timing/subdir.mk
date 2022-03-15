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


# Each subdirectory must supply rules for building sources it contributes
src/timing/%.o: ../src/timing/%.c src/timing/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -msse2 -msse3 -msse4 -I/usr/local/include -I/usr/include/gtk-3.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/gio-unix-2.0/ -I/usr/include/mirclient -I/usr/include/mircore -I/usr/include/mircookie -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/atk-1.0 -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-timing

clean-src-2f-timing:
	-$(RM) ./src/timing/CoderTimeAPI.d ./src/timing/CoderTimeAPI.o ./src/timing/coder_posix_time.d ./src/timing/coder_posix_time.o ./src/timing/get_tic.d ./src/timing/get_tic.o ./src/timing/get_tic_data.d ./src/timing/get_tic_data.o ./src/timing/get_tic_initialize.d ./src/timing/get_tic_initialize.o ./src/timing/get_tic_terminate.d ./src/timing/get_tic_terminate.o ./src/timing/tic.d ./src/timing/tic.o ./src/timing/timeKeeper.d ./src/timing/timeKeeper.o ./src/timing/toc.d ./src/timing/toc.o

.PHONY: clean-src-2f-timing

