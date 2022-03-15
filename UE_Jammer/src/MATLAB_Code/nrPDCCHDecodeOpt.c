/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: nrPDCCHDecodeOpt.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "nrPDCCHDecodeOpt.h"
#include "nrPRBS.h"
#include "nrSymbolDemodulate.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * nrPDCCHDecode Physical downlink control channel decoding
 *    DCICW = nrPDCCHDecode(SYM,NID,NRNTI) returns a vector of soft bits
 *    DCICW resulting from the inverse of physical downlink control channel
 *    processing as defined in TS 38.211 Section 7.3.2.
 *    SYM is the received PDCCH symbols (complex column vector).
 *    NID is the pdcch-DMRS-ScramblingID (0...65535), if configured, for a
 *    UE-specific search space, else, it is the physical layer cell identity,
 *    NCellID (0...1007).
 *    NRNTI is the C-RNTI (1...65519) for a PDCCH in a UE specific search
 *    space or 0 otherwise.
 *    The two supported sets for {NID,NRNTI} are {NCellID,0} or
 *    {pdcch-DMRS-ScramblingID,C-RNTI}.
 *
 *    DCICW = nrPDCCHDecode(...,NVAR) allows specification of the noise
 *    variance estimate NVAR employed for PDCCH demodulation. When not
 *    specified, it defaults to 1e-10.
 *
 *    % Example:
 *    % Generate PDCCH symbols configured with pdcch-DMRS-ScramblingID and
 *    % then recover DCI codeword using PDCCH decoder.
 *
 *    nID = 2^11;                 % pdcch-DMRS-ScramblingID, (0...65535)
 *    nRNTI = 123;                % C-RNTI
 *    nVar = 0;                   % Noise variance estimate
 *    dciCW = randi([0 1],560,1); % DCI codeword
 *    sym = nrPDCCH(dciCW,nID,nRNTI);
 *
 *    rxDCICW = nrPDCCHDecode(sym,nID,nRNTI,nVar);
 *
 *    isequal(dciCW, rxDCICW<0)
 *
 *    See also nrPDCCH, nrPDCCHPRBS.
 *
 * Arguments    : const emxArray_creal32_T *sym
 *                double cinit
 *                float dciCW_data[]
 *                int *dciCW_size
 * Return Type  : void
 */
void nrPDCCHDecodeOpt(const emxArray_creal32_T *sym, double cinit,
                      float dciCW_data[], int *dciCW_size)
{
  emxArray_real32_T *recDCICW;
  int n;
  emxInit_real32_T(&recDCICW, 3);
  /*    Copyright 2018-2020 The MathWorks, Inc. */
  /*    Reference: */
  /*    [1] 3GPP TS 38.211, "3rd Generation Partnership Project; Technical  */
  /*    Specification Group Radio Access Network; NR; Physical Channel and  */
  /*    Modulation (Release 15). Section 7.3.2. */
  /*  Section 7.3.2.4 Demodulation */
  nrSymbolDemodulate(sym, recDCICW);
  /*  Section 7.3.2.3 Descrambling */
  n = recDCICW->size[0];
  if ((recDCICW->size[0] > 0) &&
      ((recDCICW->size[1] == 0) || (recDCICW->size[1] > recDCICW->size[0]))) {
    n = recDCICW->size[1];
  }
  if ((n > 0) && ((recDCICW->size[2] == 0) || (recDCICW->size[2] > n))) {
    n = recDCICW->size[2];
  }
  /* nrPDCCHPRBS PDCCH scrambling sequence */
  /*    [SEQ,CINIT] = nrPDCCHPRBS(NID,NRNTI,N) returns the N-element */
  /*    pseudo-random binary sequence (PRBS) output SEQ from a generator that */
  /*    is initialized by the 31-bit integer, CINIT, deduced from specified NID
   */
  /*    and NRNTI, as per TS 38.211 Section 7.3.2.3. */
  /*  */
  /*    NID is the pdcch-DMRS-ScramblingID (0...65535), if configured, for a */
  /*    UE-specific search space, else, it is the physical layer cell identity,
   */
  /*    NCellID (0...1007). */
  /*    NRNTI is the C-RNTI (1...65519) for a PDCCH in a UE specific search */
  /*    space or 0 otherwise. */
  /*    The two supported sets for {NID,NRNTI} are {NCellID,0} or */
  /*    {pdcch-DMRS-ScramblingID,C-RNTI}. */
  /*  */
  /*    [SEQ,CINIT] = nrPDCCHPRBS(...,NAME,VALUE,...) specifies additional */
  /*    options as NAME,VALUE pairs to allow control over the format of the */
  /*    sequence: */
  /*  */
  /*    'MappingType' - 'binary' to map true to 1, false to 0 (default) */
  /*                    'signed' to map true to -1, false to 1 */
  /*  */
  /*    For 'binary' mapping, the output data type is logical. For 'signed', */
  /*    the output data type is controlled by the additional pair: */
  /*  */
  /*    'OutputDataType' - 'double' for double precision (default) */
  /*                       'single' for single precision */
  /*  */
  /*    % Example 1: */
  /*    % Generate a 100-element sequence initialized by */
  /*    % pdcch-DMRS-ScramblingID as 10, and C-RNTI as 20. */
  /*  */
  /*    out1 = nrPDCCHPRBS(10,20,100); */
  /*  */
  /*    % Example 2: */
  /*    % Generate a 120-element sequence with NCellID as 100, and nRNTI as 0.
   */
  /*  */
  /*    out2 = nrPDCCHPRBS(100,0,120); */
  /*  */
  /*    See also nrPDCCH, nrPDCCHDecode, nrPRBS. */
  /*    Copyright 2018-2020 The MathWorks, Inc. */
  /*    Reference: */
  /*    [1] 3GPP TS 38.211, "3rd Generation Partnership Project; Technical */
  /*    Specification Group Radio Access Network; NR; Physical Channel and */
  /*    Modulation (Release 15). Section 7.3.2.3. */
  /* cinit = mod(double(nRNTI)*2^16 + double(nID),2^31); */
  /*  Get scrambling sequence */
  nrPRBS(cinit, n, dciCW_data, dciCW_size);
  for (n = 0; n < *dciCW_size; n++) {
    dciCW_data[n] *= recDCICW->data[n];
  }
  emxFree_real32_T(&recDCICW);
}

/*
 * File trailer for nrPDCCHDecodeOpt.c
 *
 * [EOF]
 */
