/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: getSNR.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "getSNR.h"
#include "demodulate.h"
#include "eml_setop.h"
#include "mean.h"
#include "nrChannelEstimateOpt.h"
#include "operation_emxutil.h"
#include "operation_rtwutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const creal32_T rxWaveN[1668]
 * Return Type  : float
 */
float getSNR(const creal32_T rxWaveN[1668])
{
  emxArray_creal32_T *hest;
  emxArray_creal32_T *y;
  emxArray_real32_T *c_y;
  creal32_T fcv[1536];
  creal32_T rxCarrGrid[624];
  double c_data[1536];
  float SNR;
  float b_y;
  int ia_data[1536];
  int c_size;
  int ib_size;
  int nx;
  /* info.CyclicPrefixLengths =
   * [132,108,108,108,108,108,108,108,108,108,108,108,108,108,132,108,108,108,108,108,108,108,108,108,108,108,108,108];
   */
  /* info.SymbolPhases =
   * [-65140.9236721844,-876441.518498480,-1687742.11332478,-2499042.70815107,-3310343.30297737,-4121643.89780367,-4932944.49262996,-5744245.08745626,-6555545.68228255,-7366846.27710885,-8178146.87193515,-8989447.46676144,-9800748.06158774,-10612048.6564140,-11435193.0555444,-12246493.6503707,-13057794.2451970,-13869094.8400233,-14680395.4348495,-15491696.0296758,-16302996.6245021,-17114297.2193284,-17925597.8141547,-18736898.4089810,-19548199.0038073,-20359499.5986336,-21170800.1934599,-21982100.7882862];
   */
  /* info.SymbolPhases = -65140.9236721844 -(11370000*slot); %DELTA 811300
   * %DELTAB 823140  13*A + 1*B 11370040 10558740 */
  /*  Cell identity */
  /*  Carrier/BWP Subcarrier spacing */
  /*  Cyclic prefix */
  /*  Slot counter */
  /*  Frame counter */
  /*  Carrier offset */
  /*  Size of carrier in RB */
  /* nrOFDMDemodulate OFDM demodulation */
  /*    GRID = nrOFDMDemodulate(CARRIER,WAVEFORM) performs OFDM demodulation of
   */
  /*    the time domain waveform, WAVEFORM, given carrier configuration object
   */
  /*    CARRIER. */
  /*  */
  /*    CARRIER is a carrier configuration object, <a  */
  /*    href="matlab:help('nrCarrierConfig')" */
  /*    >nrCarrierConfig</a>. Only these */
  /*    object properties are relevant for this function: */
  /*  */
  /*    SubcarrierSpacing - Subcarrier spacing in kHz (15, 30, 60, 120, 240) */
  /*    CyclicPrefix      - Cyclic prefix ('normal', 'extended') */
  /*    NSizeGrid         - Number of resource blocks in carrier resource grid
   */
  /*                        (1...275) */
  /*    NSlot             - Slot number */
  /*  */
  /*    WAVEFORM is a T-by-R matrix where T is the number of time-domain  */
  /*    samples in the waveform and R is the number of receive antennas. */
  /*  */
  /*    GRID is an array of size K-by-L-by-R where K is the number of */
  /*    subcarriers and L is the number of OFDM symbols. */
  /*  */
  /*    GRID = nrOFDMDemodulate(WAVEFORM,NRB,SCS,INITIALNSLOT) performs OFDM */
  /*    demodulation of waveform WAVEFORM for NRB resource blocks with  */
  /*    subcarrier spacing SCS and initial slot number INITIALNSLOT. */
  /*  */
  /*    NRB is the number of resource blocks (1...275). */
  /*  */
  /*    SCS is the subcarrier spacing in kHz (15, 30, 60, 120, 240). */
  /*  */
  /*    INITIALNSLOT is the 0-based initial slot number, a non-negative scalar
   */
  /*    integer. The function selects the appropriate cyclic prefix length for
   */
  /*    the OFDM demodulation based on the initial slot number modulo the */
  /*    number of slots per subframe. */
  /*  */
  /*    GRID = nrOFDMDemodulate(...,NAME,VALUE) specifies additional options as
   */
  /*    NAME,VALUE pairs to allow control over the OFDM demodulation: */
  /*  */
  /*    CyclicPrefix         - Cyclic prefix ('normal' (default), 'extended').
   */
  /*                           This option is only applicable for function */
  /*                           syntaxes not using nrCarrierConfig */
  /*    Nfft                 - Desired number of FFT points to use in the OFDM
   */
  /*                           demodulator. If absent or set to [], a default */
  /*                           value is selected based on other parameters, see
   */
  /*                           <a href="matlab: doc('nrOFDMDemodulate')" */
  /*                           >nrOFDMDemodulate</a> for details */
  /*    SampleRate           - Sample rate of input waveform. If absent or set
   */
  /*                           to [], the default value is SampleRate = Nfft *
   */
  /*                           SCS. If required, the input waveform is */
  /*                           resampled from the specified sample rate to the
   */
  /*                           sample rate used during OFDM demodulation, Nfft
   */
  /*                           * SCS */
  /*    CarrierFrequency     - Carrier frequency (in Hz) to calculate the phase
   */
  /*                           decompensation applied for each OFDM symbol  */
  /*                           (denoted f_0 in TS 38.211 Section 5.4). Default
   */
  /*                           is 0 */
  /*    CyclicPrefixFraction - Starting position of OFDM symbol demodulation  */
  /*                           (FFT window position) within the cyclic prefix.
   */
  /*                           Specified as a fraction of the cyclic prefix, in
   */
  /*                           the range [0,1], with 0 representing the start */
  /*                           of the cyclic prefix and 1 representing the end
   */
  /*                           of the cyclic prefix. Default is 0.5 */
  /*  */
  /*    Note that for the numerologies specified in TS 38.211 Section 4.2,  */
  /*    extended cyclic prefix length is only applicable for 60 kHz subcarrier
   */
  /*    spacing. */
  /*  */
  /*    % Example: */
  /*  */
  /*    % Configure carrier for 20 MHz bandwidth */
  /*    carrier = nrCarrierConfig; */
  /*    carrier.NSizeGrid = 106; */
  /*  */
  /*    % Configure PDSCH and create PDSCH DM-RS symbols and indices */
  /*    pdsch = nrPDSCHConfig; */
  /*    pdsch.NumLayers = 2; */
  /*    sym = nrPDSCHDMRS(carrier,pdsch); */
  /*    ind = nrPDSCHDMRSIndices(carrier,pdsch); */
  /*  */
  /*    % Create a carrier resource grid and map PDSCH DM-RS symbols */
  /*    txGrid = nrResourceGrid(carrier,pdsch.NumLayers); */
  /*    txGrid(ind) = sym; */
  /*  */
  /*    % Perform OFDM modulation */
  /*    [txWaveform,info] = nrOFDMModulate(carrier,txGrid); */
  /*  */
  /*    % Apply a simple 2-by-1 channel */
  /*    H = [0.6; 0.4]; */
  /*    rxWaveform = txWaveform * H; */
  /*  */
  /*    % Perform OFDM demodulation */
  /*    rxGrid = nrOFDMDemodulate(carrier,rxWaveform); */
  /*  */
  /*    See also nrCarrierConfig, nrOFDMInfo, nrOFDMModulate, nrResourceGrid. */
  /*    Copyright 2020 The MathWorks, Inc. */
  /* narginchk(2,14); */
  /*  Validate inputs and get OFDM information */
  /* [waveform,info,nSlot,N,hasSampleRate] = validateInputs(varargin{:}); */
  /*  Perform OFDM demodulation */
  /*      sum = 0; */
  /*      for x = 1:100 */
  /*      tic */
  /*      grid =
   * nr5g.internal.OFDMDemodulate(waveform,info,nSlot,N,hasSampleRate); */
  /*      a =toc; */
  /*      sum = sum+a; */
  /*      end */
  /*      sum/100 */
  /*       */
  /*      sum = 0; */
  /*      for x = 1:100 */
  /*      tic */
  /*      grid = OFDMDemodulateOpt(waveform,info,nSlot,N,hasSampleRate); */
  /*      a =toc; */
  /*      sum = sum+a; */
  /*      end */
  /*      sum/100 */
  /* OFDMDemodulate OFDM demodulation */
  /*  */
  /*    Note: This is an internal undocumented function and its API and/or */
  /*    functionality may change in subsequent releases. */
  /*  */
  /*    GRID =
   * OFDMDemodulate(WAVEFORM,INFO,NSLOT,N,HASSAMPLERATE,ISCARRIERSYNTAX) */
  /*    performs OFDM demodulation of the time-domain waveform WAVEFORM given */
  /*    OFDM related information INFO and number of OFDM symbols N to be */
  /*    demodulated. NSLOT is used to select the correct cyclic prefix lengths
   */
  /*    and symbol phases in INFO to use during OFDM demodulation. */
  /*    HASSAMPLERATE (true,false) specifies whether or not a user-specified */
  /*    sample is present. */
  /*    Copyright 2020 The MathWorks, Inc. */
  /*  Resampling (if required) */
  /*      r = info.Resampling; */
  /*      if (hasSampleRate || isempty(coder.target)) */
  /*          if (any([r.L r.M]~=1)) */
  /*              h = designMultirateFIR(r.M,r.L,r.TW,r.AStop); */
  /*              fftin =
   * cast(resample(double(waveform),r.M,r.L,h),class(waveform)); */
  /*          else */
  /*              fftin = waveform; */
  /*          end */
  /*      else */
  /*          % Inputs to 'designMultirateFIR' must be compile-time constants
   * for */
  /*          % code generation, but 'r.M', 'r.L' and 'r.TW' cannot be computed
   */
  /*          % as constants if the function syntax is using nrCarrierConfig. */
  /*          % This 'else' should not be reached when performing code */
  /*          % generation, the function syntax is using nrCarrierConfig and */
  /*          % resampling is required, because an outer function should have */
  /*          % issued an error for this case */
  /*          fftin = waveform; */
  /*      end */
  /*  Update cyclic prefix lengths and symbol phases to span all subframes */
  /*  at least partially spanned by the waveform, and to take into */
  /*  consideration the initial slot number */
  /*  OFDM demodulation, TS 38.211 Section 5.3.1 */
  /* OFDMDEMOD OFDM demodulate the time-domain input received signal */
  /*  */
  /*    Y = OFDMDEMOD(X,NFFT,CPLEN) performs OFDM demodulation on the input X */
  /*    and outputs the result in Y.  Specify X as an Ns-by-Nr matrix of real */
  /*    or complex values. Ns is the number of samples and Nr is the number of
   */
  /*    receive antennas.  NFFT is the FFT size, specified as a scalar. CPLEN */
  /*    is the cyclic prefix length, specified as a scalar or row vector of */
  /*    integers in the range 0 and NFFT, both inclusive. When CPLEN is */
  /*    specified as a vector, its length equals the number of OFDM symbols, */
  /*    Nsym, in the input X. */
  /*        Y is the complex OFDM demodulated output signal of size */
  /*    NFFT-by-Nsym-by-Nr, where Nsym = Ns/(NFFT+CPLEN) if CPLEN is a scalar,
   */
  /*    or Nsym = (Ns-sum(CPLEN))/(NFFT) = length(CPLEN) if CPLEN is a vector.
   */
  /*  */
  /*    Y = OFDMDEMOD(X,NFFT,CPLEN,SYMOFFSET) accepts a scalar or row vector */
  /*    indicating the sampling offset, SYMOFFSET, for every OFDM symbol, with
   */
  /*    a value between 0 and CPLEN, both inclusive. The OFDM demodulation is */
  /*    performed based on NFFT samples following the offset position. When */
  /*    specified as a scalar, the same offset is used for all symbols. When */
  /*    not specified, SYMOFFSET defaults to an offset equal to the cyclic */
  /*    prefix length. */
  /*  */
  /*    Y = OFDMDEMOD(X,NFFT,CPLEN,SYMOFFSET,NULLIDX) also accepts a column */
  /*    vector of FFT indices, NULLIDX, indicating the null carrier locations */
  /*    from 1 to NFFT. For this syntax, the number of rows in the output Y is
   */
  /*    NFFT-length(NULLIDX). Nulls are removed at the locations indicated by */
  /*    NULLIDX. NULLIDX can be used to account for both guard bands and DC */
  /*    subcarriers. The DC subcarrier is the center of the frequency band and
   */
  /*    has an index value of (NFFT/2+1) if NFFT is even or (NFFT+1)/2 if NFFT
   */
  /*    is odd. */
  /*  */
  /*    [Y,PILOTS] = OFDMDEMOD(X,NFFT,CPLEN,SYMOFFSET,NULLIDX,PILOTIDX) also */
  /*    accepts a column vector of FFT indices, PILOTIDX, indicating the pilot
   */
  /*    carrier locations from 1 to NFFT. For this syntax, the number of rows */
  /*    in the output Y is NFFT-length(NULLIDX)-length(PILOTIDX), and accounts
   */
  /*    for the removal of both pilots and nulls. */
  /*        PILOTS is the pilot subcarrier output signal of size */
  /*    Np-by-Nsym-by-Nr, where Np is equal to length(PILOTIDX). The pilot */
  /*    subcarrier locations are assumed to be the same across each OFDM symbol
   */
  /*    and transmit antenna. */
  /*  */
  /*    % Example: Demodulation with different CP lengths for different symbols
   */
  /*  */
  /*      nfft = 64; */
  /*      cplen = [16 32]; */
  /*      nSym = 2; */
  /*      dataIn = complex(randn(nfft,nSym),randn(nfft,nSym)); */
  /*      y1 = ofdmmod(dataIn,nfft,cplen); */
  /*  */
  /*      x1 = ofdmdemod(y1,nfft,cplen); */
  /*      max(x1-dataIn) */
  /*  */
  /*    % Example: Demodulation with nulls and pilots unpacking */
  /*  */
  /*      nfft     = 64; */
  /*      cplen    = 16; */
  /*      nSym     = 10; */
  /*      nullIdx  = [1:6 33 64-4:64]'; */
  /*      pilotIdx = [12 26 40 54]'; */
  /*      numDataCarrs = nfft-length(nullIdx)-length(pilotIdx); */
  /*      dataIn = complex(randn(numDataCarrs,nSym),randn(numDataCarrs,nSym));
   */
  /*      pilots = repmat(pskmod((0:3).',4),1,nSym); */
  /*      y2 = ofdmmod(dataIn,nfft,cplen,nullIdx,pilotIdx,pilots); */
  /*  */
  /*      symOffset = cplen; */
  /*      [x2,rxPilots] = ofdmdemod(y2,nfft,cplen,symOffset,nullIdx,pilotIdx);
   */
  /*      max(x2-dataIn) */
  /*      max(rxPilots-pilots) */
  /*  */
  /*    See also ofdmmod, comm.OFDMDemodulator. */
  /*    Copyright 2017-2018 The MathWorks, Inc. */
  /*  parse, validate inputs, set up processing */
  /* [prmStr,dataIdx] = setup(x,nfft,cplen,varargin{:}); */
  do_vectors(c_data, &c_size, ia_data, &nx, &ib_size);
  /*  Call internal fcn to compute grid */
  /*  Output signals */
  demodulate(rxWaveN, fcv);
  /*  Phase decompensation, TS 38.211 Section 5.4 */
  for (ib_size = 0; ib_size < 624; ib_size++) {
    c_size = (int)c_data[ib_size] - 1;
    rxCarrGrid[ib_size].re =
        -fcv[c_size].re - fcv[c_size].im * -3.62385642E-11F;
    rxCarrGrid[ib_size].im =
        fcv[c_size].re * -3.62385642E-11F + -fcv[c_size].im;
  }
  emxInit_creal32_T(&hest, 4);
  emxInit_creal32_T(&y, 1);
  nrChannelEstimateOpt(rxCarrGrid, hest, &b_y);
  c_size = y->size[0];
  y->size[0] = hest->size[0] * hest->size[1];
  emxEnsureCapacity_creal32_T(y, c_size);
  nx = y->size[0];
  for (ib_size = 0; ib_size < nx; ib_size++) {
    y->data[ib_size].re = hest->data[ib_size].re * hest->data[ib_size].re -
                          hest->data[ib_size].im * hest->data[ib_size].im;
    y->data[ib_size].im = hest->data[ib_size].re * hest->data[ib_size].im +
                          hest->data[ib_size].im * hest->data[ib_size].re;
  }
  emxFree_creal32_T(&hest);
  emxInit_real32_T(&c_y, 1);
  nx = y->size[0];
  c_size = c_y->size[0];
  c_y->size[0] = y->size[0];
  emxEnsureCapacity_real32_T(c_y, c_size);
  for (ib_size = 0; ib_size < nx; ib_size++) {
    c_y->data[ib_size] =
        rt_hypotf_snf(y->data[ib_size].re, y->data[ib_size].im);
  }
  emxFree_creal32_T(&y);
  SNR = 10.0F * log10f(mean(c_y) / b_y);
  emxFree_real32_T(&c_y);
  return SNR;
}

/*
 * File trailer for getSNR.c
 *
 * [EOF]
 */
