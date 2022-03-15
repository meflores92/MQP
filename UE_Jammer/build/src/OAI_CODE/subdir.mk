################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/OAI_CODE/backtrace.c \
../src/OAI_CODE/oai_dfts.c \
../src/OAI_CODE/time_meas.c 

C_DEPS += \
./src/OAI_CODE/backtrace.d \
./src/OAI_CODE/oai_dfts.d \
./src/OAI_CODE/time_meas.d 

OBJS += \
./src/OAI_CODE/backtrace.o \
./src/OAI_CODE/oai_dfts.o \
./src/OAI_CODE/time_meas.o 


# Each subdirectory must supply rules for building sources it contributes
src/OAI_CODE/%.o: ../src/OAI_CODE/%.c src/OAI_CODE/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -msse2 -msse3 -msse4 -I/usr/local/include -I/usr/include/gtk-3.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/gio-unix-2.0/ -I/usr/include/mirclient -I/usr/include/mircore -I/usr/include/mircookie -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/atk-1.0 -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-OAI_CODE

clean-src-2f-OAI_CODE:
	-$(RM) ./src/OAI_CODE/backtrace.d ./src/OAI_CODE/backtrace.o ./src/OAI_CODE/oai_dfts.d ./src/OAI_CODE/oai_dfts.o ./src/OAI_CODE/time_meas.d ./src/OAI_CODE/time_meas.o

.PHONY: clean-src-2f-OAI_CODE

