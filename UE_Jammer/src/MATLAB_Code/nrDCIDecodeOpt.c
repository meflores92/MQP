/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: nrDCIDecodeOpt.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "nrDCIDecodeOpt.h"
#include "log2.h"
#include "mod.h"
#include "operation_data.h"
#include "operation_emxutil.h"
#include "operation_rtwutil.h"
#include "operation_types.h"
#include "polar_decode.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * nrDCIDecode Downlink control information decoding
 *    DCIBITS = nrDCIDecode(DCICW,K,L) decodes the input soft bits, DCICW, as
 *    per TS 38.212 Sections 7.3.4, 7.3.3 and 7.3.2 to output the decoded DCI
 *    bits, DCIBITS of length K. The processing includes rate recovery, polar
 *    decoding and CRC decoding.
 *    L is the specified list length used for polar decoding.
 *    The input DCICW must be a column vector of soft bits (LLRs) and the
 *    output DCIBITS is the output DCI message of length K.
 *
 *    [DCIBITS,MASK] = nrDCIDecode(...) also outputs the masked value. MASK
 *    equals the RNTI value when there is no CRC error for the decoded block.
 *
 *    [DCIBITS,MASK] = nrDCIDecode(...,RNTI) also accepts an RNTI value that
 *    may have been used at the transmit end for masking. When an RNTI is
 *    specified, MASK equals 0 when there is no CRC error for the decoded
 *    block.
 *
 *    % Example:
 *    % Decode an encoded DCI block and check the recovered information
 *    % elements.
 *
 *    K = 32;
 *    RNTI = 100;
 *    E = 240;
 *    L = 8;
 *    dciBits = randi([0 1],K,1);
 *    dciCW   = nrDCIEncode(dciBits,RNTI,E);
 *    [recBits,mask] = nrDCIDecode(1-2*dciCW,K,L,RNTI);
 *    isequal(recBits,dciBits)
 *    mask
 *
 *    See also nrDCIEncode, nrPDCCHDecode, nrPDCCH.
 *
 * Arguments    : const float dciCW_data[]
 *                int dciCW_size
 *                signed char dciBits[39]
 *                unsigned int *mask
 * Return Type  : void
 */
void nrDCIDecodeOpt(const float dciCW_data[], int dciCW_size,
                    signed char dciBits[39], unsigned int *mask)
{
  static const double dv[128] = {
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0,
      1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0};
  static const double frozen_bits_indicator_1[128] = {
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0,
      1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0};
  static const double frozen_bits_indicator_2[128] = {
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  static const int a[24] = {8388608, 4194304, 2097152, 1048576, 524288, 262144,
                            131072,  65536,   32768,   16384,   8192,   4096,
                            2048,    1024,    512,     256,     128,    64,
                            32,      16,      8,       4,       2,      1};
  static const short b_iv[512] = {
      0,   256, 128, 384, 64,  320, 192, 448, 32,  288, 160, 416, 96,  352, 224,
      480, 16,  272, 144, 400, 80,  336, 208, 464, 48,  304, 176, 432, 112, 368,
      240, 496, 8,   264, 136, 392, 72,  328, 200, 456, 40,  296, 168, 424, 104,
      360, 232, 488, 24,  280, 152, 408, 88,  344, 216, 472, 56,  312, 184, 440,
      120, 376, 248, 504, 4,   260, 132, 388, 68,  324, 196, 452, 36,  292, 164,
      420, 100, 356, 228, 484, 20,  276, 148, 404, 84,  340, 212, 468, 52,  308,
      180, 436, 116, 372, 244, 500, 12,  268, 140, 396, 76,  332, 204, 460, 44,
      300, 172, 428, 108, 364, 236, 492, 28,  284, 156, 412, 92,  348, 220, 476,
      60,  316, 188, 444, 124, 380, 252, 508, 2,   258, 130, 386, 66,  322, 194,
      450, 34,  290, 162, 418, 98,  354, 226, 482, 18,  274, 146, 402, 82,  338,
      210, 466, 50,  306, 178, 434, 114, 370, 242, 498, 10,  266, 138, 394, 74,
      330, 202, 458, 42,  298, 170, 426, 106, 362, 234, 490, 26,  282, 154, 410,
      90,  346, 218, 474, 58,  314, 186, 442, 122, 378, 250, 506, 6,   262, 134,
      390, 70,  326, 198, 454, 38,  294, 166, 422, 102, 358, 230, 486, 22,  278,
      150, 406, 86,  342, 214, 470, 54,  310, 182, 438, 118, 374, 246, 502, 14,
      270, 142, 398, 78,  334, 206, 462, 46,  302, 174, 430, 110, 366, 238, 494,
      30,  286, 158, 414, 94,  350, 222, 478, 62,  318, 190, 446, 126, 382, 254,
      510, 1,   257, 129, 385, 65,  321, 193, 449, 33,  289, 161, 417, 97,  353,
      225, 481, 17,  273, 145, 401, 81,  337, 209, 465, 49,  305, 177, 433, 113,
      369, 241, 497, 9,   265, 137, 393, 73,  329, 201, 457, 41,  297, 169, 425,
      105, 361, 233, 489, 25,  281, 153, 409, 89,  345, 217, 473, 57,  313, 185,
      441, 121, 377, 249, 505, 5,   261, 133, 389, 69,  325, 197, 453, 37,  293,
      165, 421, 101, 357, 229, 485, 21,  277, 149, 405, 85,  341, 213, 469, 53,
      309, 181, 437, 117, 373, 245, 501, 13,  269, 141, 397, 77,  333, 205, 461,
      45,  301, 173, 429, 109, 365, 237, 493, 29,  285, 157, 413, 93,  349, 221,
      477, 61,  317, 189, 445, 125, 381, 253, 509, 3,   259, 131, 387, 67,  323,
      195, 451, 35,  291, 163, 419, 99,  355, 227, 483, 19,  275, 147, 403, 83,
      339, 211, 467, 51,  307, 179, 435, 115, 371, 243, 499, 11,  267, 139, 395,
      75,  331, 203, 459, 43,  299, 171, 427, 107, 363, 235, 491, 27,  283, 155,
      411, 91,  347, 219, 475, 59,  315, 187, 443, 123, 379, 251, 507, 7,   263,
      135, 391, 71,  327, 199, 455, 39,  295, 167, 423, 103, 359, 231, 487, 23,
      279, 151, 407, 87,  343, 215, 471, 55,  311, 183, 439, 119, 375, 247, 503,
      15,  271, 143, 399, 79,  335, 207, 463, 47,  303, 175, 431, 111, 367, 239,
      495, 31,  287, 159, 415, 95,  351, 223, 479, 63,  319, 191, 447, 127, 383,
      255, 511};
  static const short b_iv1[63] = {
      247, 251, 253, 254, 255, 351, 367, 375, 379, 381, 382, 383, 415,
      431, 438, 439, 441, 442, 443, 444, 445, 446, 447, 462, 463, 467,
      469, 470, 471, 473, 474, 475, 476, 477, 478, 479, 483, 485, 486,
      487, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500,
      501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511};
  emxArray_int16_T *r;
  emxArray_real_T *b_jn;
  double recBlk[512];
  double L_w_even[256];
  double b_L_w_odd[256];
  double L_w_odd[128];
  double b_u_1_data[128];
  double b_u_2_data[128];
  double b_v_1[128];
  double b_v_2[128];
  double blkcrc[87];
  double inPad[87];
  double u_1_data[64];
  double u_2_data[64];
  double v_1[64];
  double v_2[64];
  double b_canMsg[63];
  double canMsg[63];
  double dividendBlk[25];
  double remBits[25];
  double x_data[14];
  double varargin_1[3];
  double b_result;
  double c_result;
  double cl2e;
  double n2;
  double result;
  double x;
  double y;
  int b[24];
  int u_1_size[2];
  int i;
  int idx;
  int k;
  signed char c_canMsg[87];
  boolean_T exitg1;
  /*    Copyright 2018-2019 The MathWorks, Inc. */
  /*    Reference: */
  /*    [1] 3GPP TS 38.212, "3rd Generation Partnership Project; Technical */
  /*    Specification Group Radio Access Network; NR; Multiplexing and channel
   */
  /*    coding (Release 15). Section 7.3. */
  /*   */
  /*      hard = []; */
  /*      for x = 1:length(dciCW) */
  /*          if dciCW(x) >= 0 */
  /*              hard(x) = 1; */
  /*          else */
  /*              hard(x) = -1; */
  /*          end */
  /*      end */
  /*  K includes CRC bits */
  /*  for downlink */
  memset(&recBlk[0], 0, 512U * sizeof(double));
  cl2e = b_log2(dciCW_size);
  cl2e = ceil(cl2e);
  if ((dciCW_size <= 1.125 * rt_powd_snf(2.0, cl2e - 1.0)) &&
      (63.0 / (double)dciCW_size < 0.5625)) {
    cl2e--;
  }
  n2 = b_log2(504.0);
  n2 = ceil(n2);
  varargin_1[0] = cl2e;
  varargin_1[1] = n2;
  varargin_1[2] = 9.0;
  x_data[0] = cl2e;
  x_data[1] = n2;
  x_data[2] = 9.0;
  if (!rtIsNaN(cl2e)) {
    idx = 1;
  } else {
    idx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k <= 3)) {
      if (!rtIsNaN(x_data[k - 1])) {
        idx = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }
  if (idx != 0) {
    cl2e = varargin_1[idx - 1];
    i = idx + 1;
    for (k = i; k < 4; k++) {
      n2 = varargin_1[k - 1];
      if (cl2e > n2) {
        cl2e = n2;
      }
    }
  }
  emxInit_real_T(&b_jn, 1);
  n2 = rt_powd_snf(2.0, fmax(cl2e, 5.0));
  /*  Rate recovery, Section 7.3.4, [1] */
  /* recBlk = nrRateRecoverPolarOpt(dciCW,K,N); */
  idx = (int)n2;
  i = b_jn->size[0];
  b_jn->size[0] = (int)n2;
  emxEnsureCapacity_real_T(b_jn, i);
  for (i = 0; i < idx; i++) {
    b_jn->data[i] = 0.0;
  }
  i = (int)((n2 - 1.0) + 1.0);
  for (idx = 0; idx < i; idx++) {
    b_jn->data[idx] =
        (double)iv3[(int)floor(32.0 * (double)idx / n2)] * (n2 / 32.0) +
        b_mod(idx, n2 / 32.0);
  }
  for (i = 0; i < 432; i++) {
    recBlk[i + 80] = dciCW_data[i];
  }
  emxInit_int16_T(&r, 1);
  i = r->size[0];
  r->size[0] = b_jn->size[0];
  emxEnsureCapacity_int16_T(r, i);
  idx = b_jn->size[0];
  for (i = 0; i < idx; i++) {
    r->data[i] = (short)(b_jn->data[i] + 1.0);
  }
  idx = r->size[0];
  i = b_jn->size[0];
  b_jn->size[0] = r->size[0];
  emxEnsureCapacity_real_T(b_jn, i);
  for (i = 0; i < idx; i++) {
    b_jn->data[i] = recBlk[i];
  }
  idx = b_jn->size[0];
  for (i = 0; i < idx; i++) {
    recBlk[r->data[i] - 1] = b_jn->data[i];
  }
  emxFree_int16_T(&r);
  emxFree_real_T(&b_jn);
  /* recBlk = nrRateRecoverPolarOpt(dciCW,K,N); */
  /*  Polar decoding, Section 7.3.3, [1] */
  /* padCRC = true;              % signifies input prepadding with ones */
  /* nrPolarDecode Polar decode */
  /*    DECBITS = nrPolarDecode(REC,K,E,L) decodes the rate-recovered input, */
  /*    REC, for a (N,K) Polar code, using a CRC-aided successive-cancellation
   */
  /*    list decoder, with the list length specified by L. The input REC is a */
  /*    column vector of length N (a power of 2), representing the */
  /*    log-likelihood ratios as soft inputs to the decoder. K is the number of
   */
  /*    message bits, and E is the rate-matched output length. The output */
  /*    DECBITS is a column vector of length K. */
  /*  */
  /*    DECBITS = nrPolarDecode(...,PADCRC) specifies whether the input was */
  /*    prepadded by ones prior to the CRC encoding with all-zeros register */
  /*    state on the transmit end. PADCRC must be a boolean scalar where for a
   */
  /*    true value, the input is assumed to be prepadded with ones, while a */
  /*    false value indicates no prepadding was used. The default is false. */
  /*  */
  /*    DECBITS = nrPolarDecode(...,PADCRC,RNTI) also specifies the RNTI value
   */
  /*    that may have been used at the transmit end for masking. The default is
   */
  /*    0. */
  /*  */
  /*    DECBITS = nrPolarDecode(REC,K,E,L,NMAX,IIL,CRCLEN) specifies the three
   */
  /*    parameter set of: NMAX (an integer value of either 9 or 10), IIL (a */
  /*    boolean scalar) and CRCLEN (an integer value one of 24, 11 or 6). The */
  /*    allowed value sets of {9,true,24} and {10,false,11},{10,false,6} for */
  /*    {NMAX,IIL,CRCLEN} apply for downlink and uplink configurations. When */
  /*    the three parameters are not specified, the value set for the downlink
   */
  /*    configuration is used. PADCRC is assumed false and RNTI to be 0 for */
  /*    this syntax. */
  /*  */
  /*    % Example: */
  /*    % Transmit polar-encoded block of data and decode using */
  /*    % successive-cancellation list decoder. */
  /*  */
  /*    K = 132;            % Message length */
  /*    E = 256;            % Rate matched output length */
  /*    nVar = 1.0;         % Noise variance */
  /*    L = 8;              % Decoding list length */
  /*  */
  /*    % Object construction */
  /*    chan   = comm.AWGNChannel('NoiseMethod','Variance','Variance',nVar); */
  /*  */
  /*    % Simulate a frame */
  /*    msg    = randi([0 1],K,1,'int8');               % Generate random
   * message */
  /*    enc    = nrPolarEncode(msg,E);                  % Polar encode */
  /*    mod    = nrSymbolModulate(enc,'QPSK');          % Modulate */
  /*    rSig   = chan(mod);                             % Add WGN */
  /*    rxLLR  = nrSymbolDemodulate(rSig,'QPSK',nVar);  % Soft demodulate */
  /*    rxBits = nrPolarDecode(rxLLR,K,E,L);            % Polar decode */
  /*  */
  /*    % Get bit errors */
  /*    numBitErrs = biterr(rxBits, msg); */
  /*    disp(['Number of bit errors: ' num2str(numBitErrs)]); */
  /*  */
  /*    See also nrPolarEncode, nrRateRecoverPolar, nrCRCDecode, nrDCIDecode, */
  /*    nrBCHDecode, nrUCIDecode. */
  /*    Copyright 2018-2019 The MathWorks, Inc. */
  /*  Input is a single code block and assumes CRC bits are included in K */
  /*  output */
  /*  F accounts for nPC bits, if present */
  /* [F,qPC] = nr5g.internal.polar.construct(K,E,nMax); */
  /*  CA-SCL decode */
  /*  References: */
  /*  [1] Tal, I, and Vardy, A., "List decoding of Polar Codes", IEEE */
  /*  Transactions on Information Theory, vol. 61, No. 5, pp. 2213-2226, */
  /*  May 2015. */
  /*  [2] Stimming, A. B., Parizi, M. B., and Burg, A., "LLR-Based */
  /*  Successive Cancellation List Decoding of Polar Codes", IEEE */
  /*  Transaction on Signal Processing, vol. 63, No. 19, pp.5165-5179, */
  /*  2015. */
  /*  Setup */
  /*  includes nPC bits as well */
  /*  CRCs as per TS 38.212, Section 5.1 */
  /*  if crcLen == 24         % '24C', downlink */
  /*      polyStr = '24C'; */
  /*  elseif crcLen == 11     % '11', uplink */
  /*      polyStr = '11'; */
  /*  else % crcLen == 6      % '6', uplink */
  /*      polyStr = '6'; */
  /*  end */
  /*  for idxBit = 0:N-1 */
  /*      % 0-based indexing */
  /*      br(idxBit+1) = polarBitReverse(idxBit,m); */
  /*  end */
  /* piInterl = nr5g.internal.polar.interleaveMap(K); */
  for (k = 0; k < 256; k++) {
    idx = (k + 1) << 1;
    x = recBlk[b_iv[idx - 2]];
    y = recBlk[b_iv[idx - 1]];
    /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
    /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
    /*  function result = f(p, q) */
    /*  result = (p * q + 1)/(p + q); */
    /*  end */
    n2 = x;
    if (x < 0.0) {
      n2 = -1.0;
    } else if (x > 0.0) {
      n2 = 1.0;
    } else if (x == 0.0) {
      n2 = 0.0;
    }
    cl2e = y;
    if (y < 0.0) {
      cl2e = -1.0;
    } else if (y > 0.0) {
      cl2e = 1.0;
    } else if (y == 0.0) {
      cl2e = 0.0;
    }
    b_L_w_odd[k] = n2 * cl2e * fmin(fabs(x), fabs(y));
  }
  polar_decode(u_1_data, u_1_size, v_1);
  polar_decode(u_2_data, u_1_size, v_2);
  for (k = 0; k < 64; k++) {
    idx = (k + 1) << 1;
    n2 = v_2[k];
    b_v_1[idx - 2] = (double)((unsigned long)v_1[k] ^ (unsigned long)n2);
    b_v_1[idx - 1] = n2;
  }
  for (k = 0; k < 128; k++) {
    idx = (k + 1) << 1;
    x = b_L_w_odd[idx - 2];
    y = b_L_w_odd[idx - 1];
    /*  function result = g(p, q, u) */
    /*  result = (p^(1-2*u)) * q; */
    /*  end */
    n2 = b_v_1[k];
    if (n2 == 0.0) {
      result = x + y;
    } else if (n2 == 1.0) {
      result = y - x;
    }
    L_w_odd[k] = result;
  }
  f_polar_decode(L_w_odd, dv, b_u_2_data, u_1_size, b_v_2);
  for (k = 0; k < 128; k++) {
    idx = (k + 1) << 1;
    n2 = b_v_2[k];
    b_L_w_odd[idx - 2] = (double)((unsigned long)b_v_1[k] ^ (unsigned long)n2);
    b_L_w_odd[idx - 1] = n2;
  }
  for (k = 0; k < 256; k++) {
    idx = (k + 1) << 1;
    x = recBlk[b_iv[idx - 2]];
    y = recBlk[b_iv[idx - 1]];
    /*  function result = g(p, q, u) */
    /*  result = (p^(1-2*u)) * q; */
    /*  end */
    n2 = b_L_w_odd[k];
    if (n2 == 0.0) {
      b_result = x + y;
    } else if (n2 == 1.0) {
      b_result = y - x;
    }
    L_w_even[k] = b_result;
  }
  for (k = 0; k < 128; k++) {
    idx = (k + 1) << 1;
    x = L_w_even[idx - 2];
    y = L_w_even[idx - 1];
    /*  result = log((exp(x+y) + 1)/(exp(x) + exp(y))); */
    /*  result = 2*atanh(tanh(x/2) * tanh(y/2)); */
    /*  function result = f(p, q) */
    /*  result = (p * q + 1)/(p + q); */
    /*  end */
    n2 = x;
    if (x < 0.0) {
      n2 = -1.0;
    } else if (x > 0.0) {
      n2 = 1.0;
    } else if (x == 0.0) {
      n2 = 0.0;
    }
    cl2e = y;
    if (y < 0.0) {
      cl2e = -1.0;
    } else if (y > 0.0) {
      cl2e = 1.0;
    } else if (y == 0.0) {
      cl2e = 0.0;
    }
    L_w_odd[k] = n2 * cl2e * fmin(fabs(x), fabs(y));
  }
  f_polar_decode(L_w_odd, frozen_bits_indicator_1, b_u_1_data, u_1_size, b_v_1);
  for (k = 0; k < 128; k++) {
    idx = (k + 1) << 1;
    x = L_w_even[idx - 2];
    y = L_w_even[idx - 1];
    /*  function result = g(p, q, u) */
    /*  result = (p^(1-2*u)) * q; */
    /*  end */
    n2 = b_v_1[k];
    if (n2 == 0.0) {
      c_result = x + y;
    } else if (n2 == 1.0) {
      c_result = y - x;
    }
    L_w_odd[k] = c_result;
  }
  f_polar_decode(L_w_odd, frozen_bits_indicator_2, b_v_1, u_1_size, b_v_2);
  for (i = 0; i < 64; i++) {
    recBlk[i] = u_1_data[i];
    recBlk[i + 64] = u_2_data[i];
  }
  for (i = 0; i < 128; i++) {
    recBlk[i + 128] = b_u_2_data[i];
    recBlk[i + 256] = b_u_1_data[i];
    recBlk[i + 384] = b_v_1[i];
  }
  for (i = 0; i < 63; i++) {
    n2 = recBlk[b_iv1[i]];
    canMsg[i] = n2;
    b_canMsg[i] = n2;
  }
  for (i = 0; i < 63; i++) {
    canMsg[iv4[i]] = b_canMsg[i];
  }
  /*  Initialize core */
  /*  [sttStr, arrayPtrLLR, arrayPtrC] = initializeDataStructures(N,L,m); */
  /*  [iniPathIdx, sttStr] = assignInitialPath(sttStr); */
  /*  [sp, sttStr, arrayPtrLLR, arrayPtrC] = getArrayPtrP(sttStr, ... */
  /*      arrayPtrLLR, arrayPtrC, 1, iniPathIdx); */
  /*  arrayPtrLLR{1,sp}(:,1) = in(br+1);  % LLRs */
  /*  mplus1 = m+1; */
  /*  Main loop */
  /*  for phase = 1:N */
  /*      [sttStr, arrayPtrLLR, arrayPtrC] = recursivelyCalcP(sttStr, ... */
  /*          arrayPtrLLR, arrayPtrC, mplus1, phase); */
  /*       */
  /*      pm2 = mod(phase-1,2); */
  /*      if F(phase)==1 */
  /*          % Path for frozen (and punctured) bits */
  /*          for pathIdx = 1:L */
  /*              if ~sttStr.activePath(pathIdx) */
  /*                  continue; */
  /*              end */
  /*              [sc, sttStr, arrayPtrLLR, arrayPtrC] = getArrayPtrC(sttStr,
   * ... */
  /*                  arrayPtrLLR, arrayPtrC, mplus1, pathIdx); */
  /*              arrayPtrC{mplus1,sc}(1,pm2+1) = 0; % set to 0 */
  /*               */
  /*              % Exact metric update */
  /*              %  sttStr.llrPathMetric(pathIdx) = ... */
  /*              %      sttStr.llrPathMetric(pathIdx) + ... */
  /*              %      log(1 + exp(-arrayPtrLLR{mplus1,sc}(1,1))); */
  /*              % Revised approximation metric update */
  /*              tmp = arrayPtrLLR{mplus1,sc}(1,1); */
  /*              if tmp < 0 */
  /*                  sttStr.llrPathMetric(pathIdx) = ... */
  /*                      sttStr.llrPathMetric(pathIdx) + abs(tmp); */
  /*                  % Else branch doesnt need an update */
  /*              end */
  /*          end */
  /*      else % Path for info bits */
  /*          [sttStr, arrayPtrLLR, arrayPtrC] = contPathsUnfrozenBit(sttStr,
   * ... */
  /*              arrayPtrLLR, arrayPtrC, phase); */
  /*      end */
  /*       */
  /*      if pm2==1 */
  /*          [sttStr, arrayPtrLLR, arrayPtrC] = recursivelyUpdateC(sttStr, ...
   */
  /*              arrayPtrLLR, arrayPtrC, mplus1, phase); */
  /*      end */
  /*  end */
  /*  Return the best codeword in the list. Use CRC checks, if enabled */
  /*  pathIdx1 = 1; */
  /*  p1 = realmax; */
  /*  crcCW = false; */
  /*  for pathIdx = 1:L */
  /*      if ~sttStr.activePath(pathIdx) */
  /*          continue; */
  /*      end */
  /*       */
  /*      [sc, sttStr, arrayPtrLLR, arrayPtrC] = getArrayPtrC(sttStr, ... */
  /*          arrayPtrLLR, arrayPtrC, mplus1, pathIdx); */
  /*      if crcLen>0 */
  /*          canCW = sttStr.savedCWs(:,sc);  % N, with frozen bits */
  /*          canMsg = canCW(F==0,1);         % K bits only (with nPC) */
  /*          canMsg(piInterl+1) = canMsg;    % deinterleave (for k+nPC) */
  /*           */
  /*          if ~isempty(qPC) */
  /*              % Extract the info only bits, minus the PC ones */
  /*              qI = find(F==0)-1; */
  /*              k = 1; */
  /*              out = zeros(length(canMsg)-length(qPC),1); */
  /*              for idx = 1:length(qI) */
  /*                  if ~any(qI(idx)==qPC) */
  /*                      out(k) = canMsg(idx); */
  /*                      k = k+1; */
  /*                  end */
  /*              end */
  /*          else */
  /*              out = canMsg; */
  /*          end */
  /*           */
  /*          % Check CRC: errFlag is 1 for error, 0 for no errors */
  /*          if padCRC  % prepad with ones */
  /*              padCRCMsg = [ones(crcLen,1); out]; */
  /*          else */
  /*              padCRCMsg = out; */
  /*          end */
  /*          [~, errFlag] = nrCRCDecode(padCRCMsg,polyStr,rnti); */
  /*          if errFlag      % ~0 => fail */
  /*              continue;   % move to next path */
  /*          end */
  /*      end */
  /*      crcCW = true; */
  /*      if p1 > sttStr.llrPathMetric(pathIdx) */
  /*          p1 = sttStr.llrPathMetric(pathIdx); */
  /*          pathIdx1 = pathIdx; */
  /*      end */
  /*  end */
  /*   */
  /*  if ~crcCW   % no codeword found which passes crcCheck */
  /*      pathIdx1 = 1; */
  /*      p1 = realmax; */
  /*      for pathIdx = 1:L */
  /*          if ~sttStr.activePath(pathIdx) */
  /*              continue; */
  /*          end */
  /*           */
  /*          if p1 > sttStr.llrPathMetric(pathIdx) */
  /*              p1 = sttStr.llrPathMetric(pathIdx); */
  /*              pathIdx1 = pathIdx; */
  /*          end */
  /*      end */
  /*  end */
  /*   */
  /*  % Get decoded bits */
  /*  [sc, sttStr] = getArrayPtrC(sttStr,arrayPtrLLR,arrayPtrC,mplus1, ... */
  /*      pathIdx1); */
  /*  decCW = sttStr.savedCWs(:,sc);      % N, with frozen bits */
  /*  dec = decCW(F==0,1);                % K, info + nPC bits only */
  /*  dec(piInterl+1) = dec;              % Deinterleave output, K+nPC */
  /*  Remove nPC bits from output, if present */
  /*  if ~isempty(qPC) */
  /*      % Extract the information only bits */
  /*      qI = find(F==0)-1; */
  /*      k = 1; */
  /*      out = zeros(length(outkpc)-3,1); */
  /*      for idx = 1:length(qI) */
  /*          if ~any(qI(idx)==qPC) */
  /*              out(k) = outkpc(idx); */
  /*              k = k+1; */
  /*          end */
  /*      end */
  /*  else */
  /* end */
  /* decBlk = nrPolarDecode(recBlk,K,E,L,true,rnti); */
  /*  CRC decoding, Section 7.3.2, [1] */
  for (i = 0; i < 24; i++) {
    blkcrc[i] = 1.0;
  }
  for (i = 0; i < 63; i++) {
    blkcrc[i + 24] = canMsg[i];
    idx = (blkcrc[i] != 0.0);
    canMsg[i] = idx;
    inPad[i] = idx;
  }
  remBits[0] = 0.0;
  for (idx = 0; idx < 24; idx++) {
    inPad[idx + 63] = 0.0;
    remBits[idx + 1] = inPad[idx];
  }
  for (idx = 0; idx < 63; idx++) {
    memcpy(&dividendBlk[0], &remBits[1], 24U * sizeof(double));
    dividendBlk[24] = inPad[idx + 24];
    if (dividendBlk[0] == 1.0) {
      for (k = 0; k < 25; k++) {
        n2 = (double)iv5[k] + dividendBlk[k];
        dividendBlk[k] = n2;
        remBits[k] = rt_remd_snf(n2, 2.0);
      }
    } else {
      memcpy(&remBits[0], &dividendBlk[0], 25U * sizeof(double));
    }
    c_canMsg[idx] = (signed char)(canMsg[idx] != 0.0);
  }
  for (i = 0; i < 24; i++) {
    c_canMsg[i + 63] = (signed char)(remBits[i + 1] != 0.0);
  }
  memcpy(&inPad[0], &blkcrc[0], 63U * sizeof(double));
  for (k = 0; k < 24; k++) {
    idx = c_canMsg[k + 63];
    inPad[k + 63] = idx;
    b[k] = ((idx > 0) != (blkcrc[k + 63] != 0.0)) * a[k];
  }
  y = b[0];
  for (k = 0; k < 23; k++) {
    y += (double)b[k + 1];
  }
  *mask = (unsigned int)y;
  for (idx = 0; idx < 39; idx++) {
    dciBits[idx] = (signed char)rt_roundd_snf(inPad[idx + 24]);
  }
  /*  remove the prepadding */
}

/*
 * File trailer for nrDCIDecodeOpt.c
 *
 * [EOF]
 */
