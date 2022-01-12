/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * nrOFDMDemodulateOpt.c
 *
 * Code generation for function 'nrOFDMDemodulateOpt'
 *
 */

/* Include files */
#include "nrOFDMDemodulateOpt.h"
#include "FFTImplementationCallback.h"
#include "PDCCHSPEEDTESTOPTMin_emxutil.h"
#include "PDCCHSPEEDTESTOPTMin_types.h"
#include "eml_setop.h"
#include "mod.h"
#include "rt_nonfinite.h"
#include "tic.h"
#include "toc.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>
#include "kiss_fft.h"
#include <stdio.h>


/* Function Definitions */
void nrOFDMDemodulateOpt(const emxArray_creal_T *waveform, double nSlot,
                         emxArray_creal_T *grid)
{
  static creal_T tmp_data[1734];
  static const double dv[28] = {
      -65140.9236721844,    -876441.51849848,     -1.68774211332478E+6,
      -2.49904270815107E+6, -3.31034330297737E+6, -4.12164389780367E+6,
      -4.93294449262996E+6, -5.74424508745626E+6, -6.55554568228255E+6,
      -7.36684627710885E+6, -8.17814687193515E+6, -8.98944746676144E+6,
      -9.80074806158774E+6, -1.0612048656414E+7,  -1.14351930555444E+7,
      -1.22464936503707E+7, -1.3057794245197E+7,  -1.38690948400233E+7,
      -1.46803954348495E+7, -1.54916960296758E+7, -1.63029966245021E+7,
      -1.71142972193284E+7, -1.79255978141547E+7, -1.8736898408981E+7,
      -1.95481990038073E+7, -2.03594995986336E+7, -2.11708001934599E+7,
      -2.19821007882862E+7};
  static const double symbolPhases[28] = {
      -65140.9236721844,    -876441.51849848,     -1.68774211332478E+6,
      -2.49904270815107E+6, -3.31034330297737E+6, -4.12164389780367E+6,
      -4.93294449262996E+6, -5.74424508745626E+6, -6.55554568228255E+6,
      -7.36684627710885E+6, -8.17814687193515E+6, -8.98944746676144E+6,
      -9.80074806158774E+6, -1.0612048656414E+7,  -1.14351930555444E+7,
      -1.22464936503707E+7, -1.3057794245197E+7,  -1.38690948400233E+7,
      -1.46803954348495E+7, -1.54916960296758E+7, -1.63029966245021E+7,
      -1.71142972193284E+7, -1.79255978141547E+7, -1.8736898408981E+7,
      -1.95481990038073E+7, -2.03594995986336E+7, -2.11708001934599E+7,
      -2.19821007882862E+7};
  static const unsigned char b_uv[28] = {
      132U, 108U, 108U, 108U, 108U, 108U, 108U, 108U, 108U, 108U,
      108U, 108U, 108U, 108U, 132U, 108U, 108U, 108U, 108U, 108U,
      108U, 108U, 108U, 108U, 108U, 108U, 108U, 108U};
  static const unsigned char cpLengths[28] = {
      132U, 108U, 108U, 108U, 108U, 108U, 108U, 108U, 108U, 108U,
      108U, 108U, 108U, 108U, 132U, 108U, 108U, 108U, 108U, 108U,
      108U, 108U, 108U, 108U, 108U, 108U, 108U, 108U};
  emxArray_creal_T *b_grid;
  emxArray_creal_T *postCPRemoval;
  emxArray_creal_T *rxFullGrid;
  creal_T y[14];
  double costab_data[3073];
  double sintab_data[3073];
  double sintabinv_data[3073];
  double b_y_data[1602];
  double costab1q_data[1537];
  double c_data[1536];
  double b_a[28];
  double symOffset[14];
  double currentIdx;
  double tmp_im;
  int currentIdx_data[1734];
  int ia_data[1536];
  int y_size[2];
  int i;
  int i1;
  int k;
  int pmax;
  int pmin;
  int pow2p;
  int symIdx;
  int y_size_idx_1;
  short y_data[1602];
  unsigned char a[28];
  boolean_T exitg1;
  boolean_T shiftright;
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
  currentIdx = b_mod(nSlot, 20.0);
  if (rtIsNaN(currentIdx) || rtIsInf(currentIdx)) {
    tmp_im = rtNaN;
  } else if (currentIdx == 0.0) {
    tmp_im = 0.0;
  } else {
    tmp_im = fmod(currentIdx, 2.0);
    if (tmp_im == 0.0) {
      tmp_im = 0.0;
    } else if (currentIdx < 0.0) {
      tmp_im += 2.0;
    }
  }
  currentIdx = -tmp_im * 14.0;
  if (currentIdx < 0.0) {
    pmax = (int)-currentIdx;
    shiftright = false;
  } else {
    pmax = (int)currentIdx;
    shiftright = true;
  }
  if (pmax > 28) {
    pmax -= pmax / 28 * 28;
  }
  if (pmax > 14) {
    pmax = 28 - pmax;
    shiftright = !shiftright;
  }
  memset(&symOffset[0], 0, 14U * sizeof(double));
  for (i = 0; i < 28; i++) {
    a[i] = cpLengths[i];
  }
  if (pmax > 0) {
    if (shiftright) {
      for (k = 0; k < pmax; k++) {
        symOffset[k] = b_uv[(k - pmax) + 28];
      }
      i = pmax + 1;
      for (k = 28; k >= i; k--) {
        a[k - 1] = a[(k - pmax) - 1];
      }
      for (k = 0; k < pmax; k++) {
        a[k] = (unsigned char)symOffset[k];
      }
    } else {
      for (k = 0; k < pmax; k++) {
        symOffset[k] = b_uv[k];
      }
      i = 27 - pmax;
      for (k = 0; k <= i; k++) {
        a[k] = a[k + pmax];
      }
      for (k = 0; k < pmax; k++) {
        a[(k - pmax) + 28] = (unsigned char)symOffset[k];
      }
    }
  }
  if (currentIdx < 0.0) {
    pmax = (int)-currentIdx;
    shiftright = false;
  } else {
    pmax = (int)currentIdx;
    shiftright = true;
  }
  if (pmax > 28) {
    pmax -= pmax / 28 * 28;
  }
  if (pmax > 14) {
    pmax = 28 - pmax;
    shiftright = !shiftright;
  }
  memset(&symOffset[0], 0, 14U * sizeof(double));
  memcpy(&b_a[0], &symbolPhases[0], 28U * sizeof(double));
  if (pmax > 0) {
    if (shiftright) {
      for (k = 0; k < pmax; k++) {
        symOffset[k] = dv[(k - pmax) + 28];
      }
      i = pmax + 1;
      for (k = 28; k >= i; k--) {
        b_a[k - 1] = b_a[(k - pmax) - 1];
      }
      if (0 <= pmax - 1) {
        memcpy(&b_a[0], &symOffset[0], pmax * sizeof(double));
      }
    } else {
      if (0 <= pmax - 1) {
        memcpy(&symOffset[0], &dv[0], pmax * sizeof(double));
      }
      i = 27 - pmax;
      for (k = 0; k <= i; k++) {
        b_a[k] = b_a[k + pmax];
      }
      for (k = 0; k < pmax; k++) {
        b_a[(k - pmax) + 28] = symOffset[k];
      }
    }
  }
  /*  OFDM demodulation, TS 38.211 Section 5.3.1 */
  for (k = 0; k < 14; k++) {
    symOffset[k] = trunc((double)a[k] * 0.5);
  }
  emxInit_creal_T(&postCPRemoval, 3);
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
  do_vectors(c_data, &pmax, ia_data, &pmin, &pow2p);
  tic();
  /*  Call internal fcn to compute grid */
  i = postCPRemoval->size[0] * postCPRemoval->size[1] * postCPRemoval->size[2];
  postCPRemoval->size[0] = 1536;
  postCPRemoval->size[1] = 14;
  postCPRemoval->size[2] = 1;
  emxEnsureCapacity_creal_T(postCPRemoval, i);
  currentIdx = 0.0;
  for (symIdx = 0; symIdx < 14; symIdx++) {
    i = a[symIdx];
    pmax = (int)symOffset[symIdx];
    pow2p = (pmax - i) + 1535;
    y_size_idx_1 = pow2p + 1;
    for (i1 = 0; i1 <= pow2p; i1++) {
      y_data[i1] = (short)((a[symIdx] + i1) + 1);
    }
    if (i < pmax + 1) {
      y_size[1] = 0;
    } else {
      pmax = (i - pmax) - 1;
      y_size[1] = pmax + 1;
      for (i1 = 0; i1 <= pmax; i1++) {
        b_y_data[i1] = (symOffset[symIdx] + 1.0) + (double)i1;
      }
    }
    pmin = (pow2p + y_size[1]) + 1;
    for (i1 = 0; i1 < y_size_idx_1; i1++) {
      currentIdx_data[i1] = (int)(currentIdx + (double)y_data[i1]) - 1;
    }
    pmax = y_size[1];
    for (i1 = 0; i1 < pmax; i1++) {
      currentIdx_data[(i1 + pow2p) + 1] = (int)(currentIdx + b_y_data[i1]) - 1;
    }
    for (i1 = 0; i1 < pmin; i1++) {
      tmp_data[i1] = waveform->data[currentIdx_data[i1]];
    }
    for (i1 = 0; i1 < 1536; i1++) {
      postCPRemoval->data[i1 + 1536 * symIdx] = tmp_data[i1];
    }
    currentIdx = (currentIdx + (double)i) + 1536.0;
  }
  pmax = 31;
  pmin = 0;
  exitg1 = false;
  while ((!exitg1) && (pmax - pmin > 1)) {
    k = (pmin + pmax) >> 1;
    pow2p = 1 << k;
    if (pow2p == 3071) {
      pmax = k;
      exitg1 = true;
    } else if (pow2p > 3071) {
      pmax = k;
    } else {
      pmin = k;
    }
  }
  pow2p = 1 << pmax;
  currentIdx = 6.2831853071795862 / (double)pow2p;
  y_size_idx_1 = pow2p / 2 / 2;
  symIdx = y_size_idx_1 + 1;
  costab1q_data[0] = 1.0;
  pmax = y_size_idx_1 / 2 - 1;
  for (k = 0; k <= pmax; k++) {
    costab1q_data[k + 1] = cos(currentIdx * ((double)k + 1.0));
  }
  i = pmax + 2;
  i1 = y_size_idx_1 - 1;
  for (k = i; k <= i1; k++) {
    costab1q_data[k] = sin(currentIdx * (double)(y_size_idx_1 - k));
  }
  costab1q_data[y_size_idx_1] = 0.0;
  pmin = y_size_idx_1 << 1;
  costab_data[0] = 1.0;
  sintab_data[0] = 0.0;
  for (k = 0; k < y_size_idx_1; k++) {
    sintabinv_data[k + 1] = costab1q_data[(y_size_idx_1 - k) - 1];
  }
  for (k = symIdx; k <= pmin; k++) {
    sintabinv_data[k] = costab1q_data[k - y_size_idx_1];
  }
  for (k = 0; k < y_size_idx_1; k++) {
    costab_data[k + 1] = costab1q_data[k + 1];
    sintab_data[k + 1] = -costab1q_data[(y_size_idx_1 - k) - 1];
  }
  for (k = symIdx; k <= pmin; k++) {
	  costab_data[k] = -costab1q_data[pmin - k];
	  sintab_data[k] = -costab1q_data[k - y_size_idx_1];
  }


  emxInit_creal_T(&rxFullGrid, 3);

  int length = postCPRemoval->size[0];


  kiss_fft_cpx *cx_in	= NULL;
  cx_in	= KISS_FFT_MALLOC(length * sizeof(kiss_fft_cpx));

  kiss_fft_cpx *kout	= NULL;
  kout	= KISS_FFT_MALLOC(1536 * sizeof(kiss_fft_cpx));
  int index;


  rxFullGrid->size[0] = 1536;
  rxFullGrid->size[1] = 14;
  rxFullGrid->size[2] = 1;
  rxFullGrid->numDimensions = 3;

  int rt = 1536*14;
  emxEnsureCapacity_creal_T(rxFullGrid, rt);

  for(index = 0; index<length; index++){
	  cx_in[index].i = postCPRemoval->data[index].im;
	  cx_in[index].r = postCPRemoval->data[index].re;
  }



  kiss_fft_cfg cfg = kiss_fft_alloc( 1536 ,0 ,NULL,NULL );
  kiss_fft(cfg , cx_in , kout);
  kiss_fft_free(cfg);

  length = rxFullGrid->size[0];

  for(index = 0; index<length;index++){
	  rxFullGrid->data[index].re = kout[index].r;
	  rxFullGrid->data[index].im = kout[index].i;
  }


 // c_FFTImplementationCallback_dob(postCPRemoval, pow2p, costab_data,
//		  sintab_data, sintabinv_data, rxFullGrid);


  pmax = 0;
  emxFree_creal_T(&postCPRemoval);
  for (y_size_idx_1 = 0; y_size_idx_1 < 14; y_size_idx_1++) {
    pmin = pmax - 1;
    pmax += 1535;
    pmin++;
    pmax++;
    for (k = 0; k < 768; k++) {
      pow2p = pmin + k;
      currentIdx = rxFullGrid->data[pow2p].re;
      tmp_im = rxFullGrid->data[pow2p].im;
      rxFullGrid->data[pow2p] = rxFullGrid->data[pow2p + 768];
      rxFullGrid->data[pow2p + 768].re = currentIdx;
      rxFullGrid->data[pow2p + 768].im = tmp_im;
    }
  }
  emxInit_creal_T(&b_grid, 3);
  toc();


  /*  Output signals */
  i = b_grid->size[0] * b_grid->size[1] * b_grid->size[2];
  b_grid->size[0] = 624;
  b_grid->size[1] = 14;
  b_grid->size[2] = 1;
  emxEnsureCapacity_creal_T(b_grid, i);
  /*  Phase decompensation, TS 38.211 Section 5.4 */
  for (k = 0; k < 14; k++) {
    for (i = 0; i < 624; i++) {
      b_grid->data[i + 624 * k] =
          rxFullGrid->data[((int)c_data[i] + 1536 * k) - 1];
    }
    currentIdx = b_a[k];
    y[k].re = currentIdx * 0.0;
    y[k].im = -currentIdx;
    if (-currentIdx == 0.0) {
      y[k].re = 1.0;
      y[k].im = 0.0;
    } else {
      y[k].re = cos(-currentIdx);
      y[k].im = sin(-currentIdx);
    }
  }
  emxFree_creal_T(&rxFullGrid);
  i = grid->size[0] * grid->size[1] * grid->size[2];
  grid->size[0] = 624;
  grid->size[1] = 14;
  grid->size[2] = 1;
  emxEnsureCapacity_creal_T(grid, i);
  for (k = 0; k < 14; k++) {
    currentIdx = y[k].re;
    tmp_im = y[k].im;
    for (pmax = 0; pmax < 624; pmax++) {
      i = pmax + 624 * k;
      grid->data[i].re =
          b_grid->data[i].re * currentIdx - b_grid->data[i].im * tmp_im;
      grid->data[i].im =
          b_grid->data[i].re * tmp_im + b_grid->data[i].im * currentIdx;
    }
  }
  emxFree_creal_T(&b_grid);
}

/* End of code generation (nrOFDMDemodulateOpt.c) */
