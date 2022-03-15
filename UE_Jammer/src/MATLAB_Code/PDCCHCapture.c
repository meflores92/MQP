/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: PDCCHCapture.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "PDCCHCapture.h"
#include "find.h"
#include "nrDCIDecodeOpt.h"
#include "nrExtractResources.h"
#include "nrOFDMDemodulateOpt.h"
#include "nrPDCCHDecodeOpt.h"
#include "operation_data.h"
#include "operation_emxutil.h"
#include "operation_rtwutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const creal32_T rxWaveN[1668]
 *                unsigned short dmrsRNTI
 *                unsigned short scramblingRNTI
 *                unsigned short UERNTI
 *                unsigned short nSlot
 *                signed char dciBits[39]
 *                double *isValid
 *                float IPhase[216]
 *                float QPhase[216]
 * Return Type  : void
 */
void PDCCHCapture(const creal32_T rxWaveN[1668], unsigned short dmrsRNTI,
                  unsigned short scramblingRNTI, unsigned short UERNTI,
                  unsigned short nSlot, signed char dciBits[39],
                  double *isValid, float IPhase[216], float QPhase[216])
{
  emxArray_creal32_T *rxSym;
  creal32_T rxCarrGrid[624];
  double allAngles[624];
  double allInd[216];
  double im;
  double r;
  double re;
  float b_tmp_data[432];
  float b_recvDMRSAngle[72];
  float recvDMRSAngle_data[72];
  float b_im;
  float f;
  float f1;
  int tmp_data[72];
  int b_nSlot;
  int b_re_tmp;
  int cIdx;
  int k;
  int re_tmp;
  unsigned int x;
  boolean_T recvDMRSAngle[72];
  boolean_T exitg1;
  *isValid = 0.0;
  /* info.CyclicPrefixLengths =
   * [132,108,108,108,108,108,108,108,108,108,108,108,108,108,132,108,108,108,108,108,108,108,108,108,108,108,108,108];
   */
  /* info.SymbolPhases =
   * [-65140.9236721844,-876441.518498480,-1687742.11332478,-2499042.70815107,-3310343.30297737,-4121643.89780367,-4932944.49262996,-5744245.08745626,-6555545.68228255,-7366846.27710885,-8178146.87193515,-8989447.46676144,-9800748.06158774,-10612048.6564140,-11435193.0555444,-12246493.6503707,-13057794.2451970,-13869094.8400233,-14680395.4348495,-15491696.0296758,-16302996.6245021,-17114297.2193284,-17925597.8141547,-18736898.4089810,-19548199.0038073,-20359499.5986336,-21170800.1934599,-21982100.7882862];
   */
  /* info.SymbolPhases = -65140.9236721844 -(11370000*slot); %DELTA 811300
   * %DELTAB 823140  13*A + 1*B 11370040 10558740 */
  /*  Carrier configuration */
  /*  carrier = nrCarrierConfig; */
  /*  carrier.NCellID = 0;                    % Cell identity */
  /*  carrier.SubcarrierSpacing = 30;         % Carrier/BWP Subcarrier spacing
   */
  /*  carrier.CyclicPrefix = 'normal';        % Cyclic prefix */
  /*  carrier.NSlot      = nSlot;             % Slot counter */
  /*  carrier.NFrame     = 0;                % Frame counter */
  /*  carrier.NStartGrid = 0;                 % Carrier offset */
  /*  carrier.NSizeGrid  = 48;                % Size of carrier in RB */
  /*  CORESET configuration */
  /*  coreset = nrCORESETConfig; */
  /*  coreset.CORESETID = 0;                  % CORESET ID (0...11) */
  /*  coreset.FrequencyResources = ones(1,8); % 6 RB sized */
  /*  coreset.Duration = 1;                   % CORESET symbol duration (1,2,3)
   */
  /*  coreset.CCEREGMapping = 'interleaved';  % CORESET Mapping */
  /*  coreset.REGBundleSize = 6;              % L (2,6) or (3,6) */
  /*  coreset.InterleaverSize = 2;            % R (2,3,6) */
  /*  coreset.ShiftIndex = carrier.NCellID;   % default to NCellID */
  /*  Search space configuration */
  /*  ss = nrSearchSpaceConfig; */
  /*  ss.SearchSpaceID = 2; */
  /*  ss.CORESETID = 0;                       % Associated CORESET ID (0...11)
   */
  /*  ss.SearchSpaceType = 'ue';              % 'ue', 'common' */
  /*  ss.StartSymbolWithinSlot = 0;           % Starting symbol in slot */
  /*  ss.SlotPeriodAndOffset = [1 0];         % Search space period and offset
   */
  /*  ss.Duration = 1;                        % Search space duration in slots
   */
  /*  ss.NumCandidates = [0 0 2 0 0];         % For (1,2,4,8,16) levels
   * respectively */
  /*  % PDCCH configuration */
  /*  pdcch = nrPDCCHConfig; */
  /*  pdcch.NStartBWP  = 0;                  % BWP offset wrt CRB 0 */
  /*  pdcch.NSizeBWP   = 48;                  % Size of BWP in resource blocks
   */
  /*  pdcch.CORESET = coreset;                % Associated CORESET */
  /*  pdcch.SearchSpace = ss;                 % Associated SearchSpace */
  /*  pdcch.RNTI = 0;                         % C-RNTI */
  /*  pdcch.DMRSScramblingID = [];            % Use carrier.NCellID instead */
  /*  pdcch.AggregationLevel = 4;             % Number of CCEs in PDCCH
   * (1,2,4,8,16) */
  /*  pdcch.AllocatedCandidate = 1;           % 1-based scalar */
  /*  OFDM demodulate the carrier */
  nrOFDMDemodulateOpt(rxWaveN, rxCarrGrid);
  memset(&allAngles[0], 0, 624U * sizeof(double));
  cIdx = 0;
  emxInit_creal32_T(&rxSym, 3);
  exitg1 = false;
  while ((!exitg1) && (cIdx < 2)) {
    b_nSlot =
        (unsigned short)(((unsigned int)(unsigned short)(nSlot << 1) + cIdx) +
                         1U);
    for (k = 0; k < 72; k++) {
      re_tmp = iv[k + 72 * cIdx] - 1;
      b_re_tmp = k + 72 * (b_nSlot - 1);
      f = rt_atan2f_snf(rxCarrGrid[re_tmp].im, rxCarrGrid[re_tmp].re) -
          (float)rt_atan2d_snf(dcv[b_re_tmp].im, dcv[b_re_tmp].re);
      b_recvDMRSAngle[k] = f;
      recvDMRSAngle[k] = (f < 0.0F);
    }
    eml_find(recvDMRSAngle, tmp_data, &b_nSlot);
    for (k = 0; k < b_nSlot; k++) {
      recvDMRSAngle_data[k] = b_recvDMRSAngle[tmp_data[k] - 1] + 6.28318548F;
    }
    for (k = 0; k < b_nSlot; k++) {
      b_recvDMRSAngle[tmp_data[k] - 1] = recvDMRSAngle_data[k];
    }
    /* off = mean(differance); */
    k = iv[72 * cIdx];
    for (re_tmp = 0; re_tmp < k; re_tmp++) {
      allAngles[re_tmp] = b_recvDMRSAngle[0];
    }
    for (re_tmp = 0; re_tmp < 71; re_tmp++) {
      k = re_tmp + 72 * cIdx;
      b_nSlot = iv[k];
      k = iv[k + 1] - b_nSlot;
      for (b_re_tmp = 0; b_re_tmp < k; b_re_tmp++) {
        allAngles[b_nSlot + b_re_tmp] =
            (b_recvDMRSAngle[re_tmp + 1] + b_recvDMRSAngle[re_tmp]) / 2.0F;
      }
    }
    k = iv[72 * cIdx + 71];
    b_nSlot = 624 - k;
    for (re_tmp = 0; re_tmp <= b_nSlot; re_tmp++) {
      allAngles[(k + re_tmp) - 1] = b_recvDMRSAngle[71];
    }
    for (k = 0; k < 624; k++) {
      im = allAngles[k];
      re = im * 0.0;
      if (-im == 0.0) {
        re = exp(re);
        im = 0.0;
      } else {
        r = exp(re / 2.0);
        re = r * (r * cos(-im));
        im = r * (r * sin(-im));
      }
      f = rxCarrGrid[k].re;
      f1 = rxCarrGrid[k].im;
      b_im = f * (float)im + f1 * (float)re;
      f = f * (float)re - f1 * (float)im;
      rxCarrGrid[k].re = f;
      rxCarrGrid[k].im = b_im;
    }
    /* [hest,nVar] =
     * nrChannelEstimateInf(rxCarrGrid,allDMRSInd(:,cIdx),allDMRS(:,nSlot*2+cIdx));
     */
    /*  Channel estimate */
    for (k = 0; k < 216; k++) {
      allInd[k] = iv1[k + 216 * cIdx];
    }
    nrExtractResources(allInd, rxCarrGrid, rxSym);
    /*  Equalization */
    /* [pdcchEq,csi] = nrEqualizeMMSE(rxSym,pdcchHest,nVar); */
    /*  Demodulate */
    x = ((unsigned int)scramblingRNTI << 16) + dmrsRNTI;
    /*  Apply CSI */
    /*  csiRep = repmat(csi(1:216).',2,1); */
    /*  scalRxCW = rxCW.*csiRep(:); */
    /*  Decode */
    if (x == 0U) {
      x = 0U;
    } else {
      x = (unsigned int)fmod(x, 2.147483648E+9);
    }
    nrPDCCHDecodeOpt(rxSym, x, b_tmp_data, &b_nSlot);
    nrDCIDecodeOpt(b_tmp_data, b_nSlot, dciBits, &x);
    if (x == UERNTI) {
      *isValid = 1.0;
      exitg1 = true;
    } else {
      cIdx++;
    }
  }
  for (k = 0; k < 216; k++) {
    IPhase[k] = rxSym->data[k].re;
    QPhase[k] = rxSym->data[k].im;
  }
  emxFree_creal32_T(&rxSym);
  /*  figure; */
  /*  plot(rxSym,'o'); */
  /*  xlabel('In-Phase'); ylabel('Quadrature') */
  /*  title('Equalized PDCCH Constellation Custom'); */
  /*  m = max(abs([real(rxSym(:)); imag(rxSym(:))])) * 1.1; */
  /*  axis([-m m -m m]); */
}

/*
 * File trailer for PDCCHCapture.c
 *
 * [EOF]
 */
