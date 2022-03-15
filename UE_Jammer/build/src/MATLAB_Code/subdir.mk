################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MATLAB_Code/FFTImplementationCallback.c \
../src/MATLAB_Code/OFDMInfo.c \
../src/MATLAB_Code/OFDMModulate.c \
../src/MATLAB_Code/PDCCHCapture.c \
../src/MATLAB_Code/PDCCHFind.c \
../src/MATLAB_Code/SyncFrame.c \
../src/MATLAB_Code/SyncFrameInit.c \
../src/MATLAB_Code/SyncFrameRecover.c \
../src/MATLAB_Code/bsearch.c \
../src/MATLAB_Code/bsxfun.c \
../src/MATLAB_Code/cart2pol.c \
../src/MATLAB_Code/circshift.c \
../src/MATLAB_Code/colon.c \
../src/MATLAB_Code/computeDFT.c \
../src/MATLAB_Code/computepsd.c \
../src/MATLAB_Code/convmtx.c \
../src/MATLAB_Code/demodulate.c \
../src/MATLAB_Code/diff.c \
../src/MATLAB_Code/eml_setop.c \
../src/MATLAB_Code/fft.c \
../src/MATLAB_Code/find.c \
../src/MATLAB_Code/floor.c \
../src/MATLAB_Code/getPUSCH.c \
../src/MATLAB_Code/getSNR.c \
../src/MATLAB_Code/ifft.c \
../src/MATLAB_Code/interp1.c \
../src/MATLAB_Code/interp2.c \
../src/MATLAB_Code/kfc.c \
../src/MATLAB_Code/kiss_fft.c \
../src/MATLAB_Code/kiss_fftnd.c \
../src/MATLAB_Code/kiss_fftndr.c \
../src/MATLAB_Code/kiss_fftr.c \
../src/MATLAB_Code/log2.c \
../src/MATLAB_Code/mean.c \
../src/MATLAB_Code/meshgrid.c \
../src/MATLAB_Code/minOrMax.c \
../src/MATLAB_Code/mod.c \
../src/MATLAB_Code/modulateOpt.c \
../src/MATLAB_Code/nrChannelEstimateOpt.c \
../src/MATLAB_Code/nrDCIDecodeOpt.c \
../src/MATLAB_Code/nrDCIEncode.c \
../src/MATLAB_Code/nrExtractResources.c \
../src/MATLAB_Code/nrOFDMDemodulateOpt.c \
../src/MATLAB_Code/nrPDCCHDecodeOpt.c \
../src/MATLAB_Code/nrPRBS.c \
../src/MATLAB_Code/nrRateRecoverPolar.c \
../src/MATLAB_Code/nrSymbolDemodulate.c \
../src/MATLAB_Code/nrTimingEstimate.c \
../src/MATLAB_Code/operation.c \
../src/MATLAB_Code/operation_data.c \
../src/MATLAB_Code/operation_emxAPI.c \
../src/MATLAB_Code/operation_emxutil.c \
../src/MATLAB_Code/operation_initialize.c \
../src/MATLAB_Code/operation_rtwutil.c \
../src/MATLAB_Code/operation_terminate.c \
../src/MATLAB_Code/polar_decode.c \
../src/MATLAB_Code/polyfit.c \
../src/MATLAB_Code/psdfreqvec.c \
../src/MATLAB_Code/pspectrogram.c \
../src/MATLAB_Code/relop.c \
../src/MATLAB_Code/repmat.c \
../src/MATLAB_Code/rtGetInf.c \
../src/MATLAB_Code/rtGetNaN.c \
../src/MATLAB_Code/rt_nonfinite.c \
../src/MATLAB_Code/sortIdx.c \
../src/MATLAB_Code/sortLE.c \
../src/MATLAB_Code/sortrows.c \
../src/MATLAB_Code/spectrogram.c \
../src/MATLAB_Code/strcmp.c \
../src/MATLAB_Code/sum.c \
../src/MATLAB_Code/unique.c \
../src/MATLAB_Code/unwrap.c \
../src/MATLAB_Code/var.c \
../src/MATLAB_Code/welchparse.c \
../src/MATLAB_Code/xcorr.c \
../src/MATLAB_Code/xnrm2.c 

C_DEPS += \
./src/MATLAB_Code/FFTImplementationCallback.d \
./src/MATLAB_Code/OFDMInfo.d \
./src/MATLAB_Code/OFDMModulate.d \
./src/MATLAB_Code/PDCCHCapture.d \
./src/MATLAB_Code/PDCCHFind.d \
./src/MATLAB_Code/SyncFrame.d \
./src/MATLAB_Code/SyncFrameInit.d \
./src/MATLAB_Code/SyncFrameRecover.d \
./src/MATLAB_Code/bsearch.d \
./src/MATLAB_Code/bsxfun.d \
./src/MATLAB_Code/cart2pol.d \
./src/MATLAB_Code/circshift.d \
./src/MATLAB_Code/colon.d \
./src/MATLAB_Code/computeDFT.d \
./src/MATLAB_Code/computepsd.d \
./src/MATLAB_Code/convmtx.d \
./src/MATLAB_Code/demodulate.d \
./src/MATLAB_Code/diff.d \
./src/MATLAB_Code/eml_setop.d \
./src/MATLAB_Code/fft.d \
./src/MATLAB_Code/find.d \
./src/MATLAB_Code/floor.d \
./src/MATLAB_Code/getPUSCH.d \
./src/MATLAB_Code/getSNR.d \
./src/MATLAB_Code/ifft.d \
./src/MATLAB_Code/interp1.d \
./src/MATLAB_Code/interp2.d \
./src/MATLAB_Code/kfc.d \
./src/MATLAB_Code/kiss_fft.d \
./src/MATLAB_Code/kiss_fftnd.d \
./src/MATLAB_Code/kiss_fftndr.d \
./src/MATLAB_Code/kiss_fftr.d \
./src/MATLAB_Code/log2.d \
./src/MATLAB_Code/mean.d \
./src/MATLAB_Code/meshgrid.d \
./src/MATLAB_Code/minOrMax.d \
./src/MATLAB_Code/mod.d \
./src/MATLAB_Code/modulateOpt.d \
./src/MATLAB_Code/nrChannelEstimateOpt.d \
./src/MATLAB_Code/nrDCIDecodeOpt.d \
./src/MATLAB_Code/nrDCIEncode.d \
./src/MATLAB_Code/nrExtractResources.d \
./src/MATLAB_Code/nrOFDMDemodulateOpt.d \
./src/MATLAB_Code/nrPDCCHDecodeOpt.d \
./src/MATLAB_Code/nrPRBS.d \
./src/MATLAB_Code/nrRateRecoverPolar.d \
./src/MATLAB_Code/nrSymbolDemodulate.d \
./src/MATLAB_Code/nrTimingEstimate.d \
./src/MATLAB_Code/operation.d \
./src/MATLAB_Code/operation_data.d \
./src/MATLAB_Code/operation_emxAPI.d \
./src/MATLAB_Code/operation_emxutil.d \
./src/MATLAB_Code/operation_initialize.d \
./src/MATLAB_Code/operation_rtwutil.d \
./src/MATLAB_Code/operation_terminate.d \
./src/MATLAB_Code/polar_decode.d \
./src/MATLAB_Code/polyfit.d \
./src/MATLAB_Code/psdfreqvec.d \
./src/MATLAB_Code/pspectrogram.d \
./src/MATLAB_Code/relop.d \
./src/MATLAB_Code/repmat.d \
./src/MATLAB_Code/rtGetInf.d \
./src/MATLAB_Code/rtGetNaN.d \
./src/MATLAB_Code/rt_nonfinite.d \
./src/MATLAB_Code/sortIdx.d \
./src/MATLAB_Code/sortLE.d \
./src/MATLAB_Code/sortrows.d \
./src/MATLAB_Code/spectrogram.d \
./src/MATLAB_Code/strcmp.d \
./src/MATLAB_Code/sum.d \
./src/MATLAB_Code/unique.d \
./src/MATLAB_Code/unwrap.d \
./src/MATLAB_Code/var.d \
./src/MATLAB_Code/welchparse.d \
./src/MATLAB_Code/xcorr.d \
./src/MATLAB_Code/xnrm2.d 

OBJS += \
./src/MATLAB_Code/FFTImplementationCallback.o \
./src/MATLAB_Code/OFDMInfo.o \
./src/MATLAB_Code/OFDMModulate.o \
./src/MATLAB_Code/PDCCHCapture.o \
./src/MATLAB_Code/PDCCHFind.o \
./src/MATLAB_Code/SyncFrame.o \
./src/MATLAB_Code/SyncFrameInit.o \
./src/MATLAB_Code/SyncFrameRecover.o \
./src/MATLAB_Code/bsearch.o \
./src/MATLAB_Code/bsxfun.o \
./src/MATLAB_Code/cart2pol.o \
./src/MATLAB_Code/circshift.o \
./src/MATLAB_Code/colon.o \
./src/MATLAB_Code/computeDFT.o \
./src/MATLAB_Code/computepsd.o \
./src/MATLAB_Code/convmtx.o \
./src/MATLAB_Code/demodulate.o \
./src/MATLAB_Code/diff.o \
./src/MATLAB_Code/eml_setop.o \
./src/MATLAB_Code/fft.o \
./src/MATLAB_Code/find.o \
./src/MATLAB_Code/floor.o \
./src/MATLAB_Code/getPUSCH.o \
./src/MATLAB_Code/getSNR.o \
./src/MATLAB_Code/ifft.o \
./src/MATLAB_Code/interp1.o \
./src/MATLAB_Code/interp2.o \
./src/MATLAB_Code/kfc.o \
./src/MATLAB_Code/kiss_fft.o \
./src/MATLAB_Code/kiss_fftnd.o \
./src/MATLAB_Code/kiss_fftndr.o \
./src/MATLAB_Code/kiss_fftr.o \
./src/MATLAB_Code/log2.o \
./src/MATLAB_Code/mean.o \
./src/MATLAB_Code/meshgrid.o \
./src/MATLAB_Code/minOrMax.o \
./src/MATLAB_Code/mod.o \
./src/MATLAB_Code/modulateOpt.o \
./src/MATLAB_Code/nrChannelEstimateOpt.o \
./src/MATLAB_Code/nrDCIDecodeOpt.o \
./src/MATLAB_Code/nrDCIEncode.o \
./src/MATLAB_Code/nrExtractResources.o \
./src/MATLAB_Code/nrOFDMDemodulateOpt.o \
./src/MATLAB_Code/nrPDCCHDecodeOpt.o \
./src/MATLAB_Code/nrPRBS.o \
./src/MATLAB_Code/nrRateRecoverPolar.o \
./src/MATLAB_Code/nrSymbolDemodulate.o \
./src/MATLAB_Code/nrTimingEstimate.o \
./src/MATLAB_Code/operation.o \
./src/MATLAB_Code/operation_data.o \
./src/MATLAB_Code/operation_emxAPI.o \
./src/MATLAB_Code/operation_emxutil.o \
./src/MATLAB_Code/operation_initialize.o \
./src/MATLAB_Code/operation_rtwutil.o \
./src/MATLAB_Code/operation_terminate.o \
./src/MATLAB_Code/polar_decode.o \
./src/MATLAB_Code/polyfit.o \
./src/MATLAB_Code/psdfreqvec.o \
./src/MATLAB_Code/pspectrogram.o \
./src/MATLAB_Code/relop.o \
./src/MATLAB_Code/repmat.o \
./src/MATLAB_Code/rtGetInf.o \
./src/MATLAB_Code/rtGetNaN.o \
./src/MATLAB_Code/rt_nonfinite.o \
./src/MATLAB_Code/sortIdx.o \
./src/MATLAB_Code/sortLE.o \
./src/MATLAB_Code/sortrows.o \
./src/MATLAB_Code/spectrogram.o \
./src/MATLAB_Code/strcmp.o \
./src/MATLAB_Code/sum.o \
./src/MATLAB_Code/unique.o \
./src/MATLAB_Code/unwrap.o \
./src/MATLAB_Code/var.o \
./src/MATLAB_Code/welchparse.o \
./src/MATLAB_Code/xcorr.o \
./src/MATLAB_Code/xnrm2.o 


# Each subdirectory must supply rules for building sources it contributes
src/MATLAB_Code/%.o: ../src/MATLAB_Code/%.c src/MATLAB_Code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -msse2 -msse3 -msse4 -I/usr/local/include -I/usr/include/gtk-3.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/gio-unix-2.0/ -I/usr/include/mirclient -I/usr/include/mircore -I/usr/include/mircookie -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/atk-1.0 -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-MATLAB_Code

clean-src-2f-MATLAB_Code:
	-$(RM) ./src/MATLAB_Code/FFTImplementationCallback.d ./src/MATLAB_Code/FFTImplementationCallback.o ./src/MATLAB_Code/OFDMInfo.d ./src/MATLAB_Code/OFDMInfo.o ./src/MATLAB_Code/OFDMModulate.d ./src/MATLAB_Code/OFDMModulate.o ./src/MATLAB_Code/PDCCHCapture.d ./src/MATLAB_Code/PDCCHCapture.o ./src/MATLAB_Code/PDCCHFind.d ./src/MATLAB_Code/PDCCHFind.o ./src/MATLAB_Code/SyncFrame.d ./src/MATLAB_Code/SyncFrame.o ./src/MATLAB_Code/SyncFrameInit.d ./src/MATLAB_Code/SyncFrameInit.o ./src/MATLAB_Code/SyncFrameRecover.d ./src/MATLAB_Code/SyncFrameRecover.o ./src/MATLAB_Code/bsearch.d ./src/MATLAB_Code/bsearch.o ./src/MATLAB_Code/bsxfun.d ./src/MATLAB_Code/bsxfun.o ./src/MATLAB_Code/cart2pol.d ./src/MATLAB_Code/cart2pol.o ./src/MATLAB_Code/circshift.d ./src/MATLAB_Code/circshift.o ./src/MATLAB_Code/colon.d ./src/MATLAB_Code/colon.o ./src/MATLAB_Code/computeDFT.d ./src/MATLAB_Code/computeDFT.o ./src/MATLAB_Code/computepsd.d ./src/MATLAB_Code/computepsd.o ./src/MATLAB_Code/convmtx.d ./src/MATLAB_Code/convmtx.o ./src/MATLAB_Code/demodulate.d ./src/MATLAB_Code/demodulate.o ./src/MATLAB_Code/diff.d ./src/MATLAB_Code/diff.o ./src/MATLAB_Code/eml_setop.d ./src/MATLAB_Code/eml_setop.o ./src/MATLAB_Code/fft.d ./src/MATLAB_Code/fft.o ./src/MATLAB_Code/find.d ./src/MATLAB_Code/find.o ./src/MATLAB_Code/floor.d ./src/MATLAB_Code/floor.o ./src/MATLAB_Code/getPUSCH.d ./src/MATLAB_Code/getPUSCH.o ./src/MATLAB_Code/getSNR.d ./src/MATLAB_Code/getSNR.o ./src/MATLAB_Code/ifft.d ./src/MATLAB_Code/ifft.o ./src/MATLAB_Code/interp1.d ./src/MATLAB_Code/interp1.o ./src/MATLAB_Code/interp2.d ./src/MATLAB_Code/interp2.o ./src/MATLAB_Code/kfc.d ./src/MATLAB_Code/kfc.o ./src/MATLAB_Code/kiss_fft.d ./src/MATLAB_Code/kiss_fft.o ./src/MATLAB_Code/kiss_fftnd.d ./src/MATLAB_Code/kiss_fftnd.o ./src/MATLAB_Code/kiss_fftndr.d ./src/MATLAB_Code/kiss_fftndr.o ./src/MATLAB_Code/kiss_fftr.d ./src/MATLAB_Code/kiss_fftr.o ./src/MATLAB_Code/log2.d ./src/MATLAB_Code/log2.o ./src/MATLAB_Code/mean.d ./src/MATLAB_Code/mean.o ./src/MATLAB_Code/meshgrid.d ./src/MATLAB_Code/meshgrid.o ./src/MATLAB_Code/minOrMax.d ./src/MATLAB_Code/minOrMax.o ./src/MATLAB_Code/mod.d ./src/MATLAB_Code/mod.o ./src/MATLAB_Code/modulateOpt.d ./src/MATLAB_Code/modulateOpt.o ./src/MATLAB_Code/nrChannelEstimateOpt.d ./src/MATLAB_Code/nrChannelEstimateOpt.o ./src/MATLAB_Code/nrDCIDecodeOpt.d ./src/MATLAB_Code/nrDCIDecodeOpt.o ./src/MATLAB_Code/nrDCIEncode.d ./src/MATLAB_Code/nrDCIEncode.o ./src/MATLAB_Code/nrExtractResources.d ./src/MATLAB_Code/nrExtractResources.o ./src/MATLAB_Code/nrOFDMDemodulateOpt.d ./src/MATLAB_Code/nrOFDMDemodulateOpt.o ./src/MATLAB_Code/nrPDCCHDecodeOpt.d ./src/MATLAB_Code/nrPDCCHDecodeOpt.o ./src/MATLAB_Code/nrPRBS.d ./src/MATLAB_Code/nrPRBS.o ./src/MATLAB_Code/nrRateRecoverPolar.d ./src/MATLAB_Code/nrRateRecoverPolar.o ./src/MATLAB_Code/nrSymbolDemodulate.d ./src/MATLAB_Code/nrSymbolDemodulate.o ./src/MATLAB_Code/nrTimingEstimate.d ./src/MATLAB_Code/nrTimingEstimate.o ./src/MATLAB_Code/operation.d ./src/MATLAB_Code/operation.o ./src/MATLAB_Code/operation_data.d ./src/MATLAB_Code/operation_data.o ./src/MATLAB_Code/operation_emxAPI.d ./src/MATLAB_Code/operation_emxAPI.o ./src/MATLAB_Code/operation_emxutil.d ./src/MATLAB_Code/operation_emxutil.o ./src/MATLAB_Code/operation_initialize.d ./src/MATLAB_Code/operation_initialize.o ./src/MATLAB_Code/operation_rtwutil.d ./src/MATLAB_Code/operation_rtwutil.o ./src/MATLAB_Code/operation_terminate.d ./src/MATLAB_Code/operation_terminate.o ./src/MATLAB_Code/polar_decode.d ./src/MATLAB_Code/polar_decode.o ./src/MATLAB_Code/polyfit.d ./src/MATLAB_Code/polyfit.o ./src/MATLAB_Code/psdfreqvec.d ./src/MATLAB_Code/psdfreqvec.o ./src/MATLAB_Code/pspectrogram.d ./src/MATLAB_Code/pspectrogram.o ./src/MATLAB_Code/relop.d ./src/MATLAB_Code/relop.o ./src/MATLAB_Code/repmat.d ./src/MATLAB_Code/repmat.o ./src/MATLAB_Code/rtGetInf.d ./src/MATLAB_Code/rtGetInf.o ./src/MATLAB_Code/rtGetNaN.d ./src/MATLAB_Code/rtGetNaN.o ./src/MATLAB_Code/rt_nonfinite.d ./src/MATLAB_Code/rt_nonfinite.o ./src/MATLAB_Code/sortIdx.d ./src/MATLAB_Code/sortIdx.o ./src/MATLAB_Code/sortLE.d ./src/MATLAB_Code/sortLE.o ./src/MATLAB_Code/sortrows.d ./src/MATLAB_Code/sortrows.o ./src/MATLAB_Code/spectrogram.d ./src/MATLAB_Code/spectrogram.o ./src/MATLAB_Code/strcmp.d ./src/MATLAB_Code/strcmp.o ./src/MATLAB_Code/sum.d ./src/MATLAB_Code/sum.o ./src/MATLAB_Code/unique.d ./src/MATLAB_Code/unique.o ./src/MATLAB_Code/unwrap.d ./src/MATLAB_Code/unwrap.o ./src/MATLAB_Code/var.d ./src/MATLAB_Code/var.o ./src/MATLAB_Code/welchparse.d ./src/MATLAB_Code/welchparse.o ./src/MATLAB_Code/xcorr.d ./src/MATLAB_Code/xcorr.o ./src/MATLAB_Code/xnrm2.d ./src/MATLAB_Code/xnrm2.o

.PHONY: clean-src-2f-MATLAB_Code

