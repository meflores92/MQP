/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: FFTImplementationCallback.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "FFTImplementationCallback.h"
#include "operation_data.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static void f_FFTImplementationCallback_r2b(const emxArray_creal_T *x,
                                            int n1_unsigned,
                                            const emxArray_real_T *costab,
                                            const emxArray_real_T *sintab,
                                            emxArray_creal_T *y);

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal_T *x
 *                int n1_unsigned
 *                const emxArray_real_T *costab
 *                const emxArray_real_T *sintab
 *                emxArray_creal_T *y
 * Return Type  : void
 */
static void f_FFTImplementationCallback_r2b(const emxArray_creal_T *x,
                                            int n1_unsigned,
                                            const emxArray_real_T *costab,
                                            const emxArray_real_T *sintab,
                                            emxArray_creal_T *y)
{
  double temp_im;
  double temp_re;
  double twid_im;
  double twid_re;
  int i;
  int iDelta2;
  int iheight;
  int ihi;
  int istart;
  int iy;
  int ju;
  int k;
  int nRowsD2;
  int temp_re_tmp;
  boolean_T tst;
  iDelta2 = y->size[0];
  y->size[0] = n1_unsigned;
  emxEnsureCapacity_creal_T(y, iDelta2);
  if (n1_unsigned > x->size[0]) {
    iDelta2 = y->size[0];
    y->size[0] = n1_unsigned;
    emxEnsureCapacity_creal_T(y, iDelta2);
    for (iDelta2 = 0; iDelta2 < n1_unsigned; iDelta2++) {
      y->data[iDelta2].re = 0.0;
      y->data[iDelta2].im = 0.0;
    }
  }
  iheight = x->size[0];
  if (iheight >= n1_unsigned) {
    iheight = n1_unsigned;
  }
  istart = n1_unsigned - 2;
  nRowsD2 = n1_unsigned / 2;
  k = nRowsD2 / 2;
  iy = 0;
  ju = 0;
  for (i = 0; i <= iheight - 2; i++) {
    y->data[iy] = x->data[i];
    iDelta2 = n1_unsigned;
    tst = true;
    while (tst) {
      iDelta2 >>= 1;
      ju ^= iDelta2;
      tst = ((ju & iDelta2) == 0);
    }
    iy = ju;
  }
  y->data[iy] = x->data[iheight - 1];
  if (n1_unsigned > 1) {
    for (i = 0; i <= istart; i += 2) {
      temp_re = y->data[i + 1].re;
      temp_im = y->data[i + 1].im;
      twid_re = y->data[i].re;
      twid_im = y->data[i].im;
      y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
      y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
      twid_re += temp_re;
      twid_im += temp_im;
      y->data[i].re = twid_re;
      y->data[i].im = twid_im;
    }
  }
  iy = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      temp_re_tmp = i + iy;
      temp_re = y->data[temp_re_tmp].re;
      temp_im = y->data[temp_re_tmp].im;
      y->data[temp_re_tmp].re = y->data[i].re - temp_re;
      y->data[temp_re_tmp].im = y->data[i].im - temp_im;
      y->data[i].re += temp_re;
      y->data[i].im += temp_im;
    }
    istart = 1;
    for (ju = k; ju < nRowsD2; ju += k) {
      twid_re = costab->data[ju];
      twid_im = sintab->data[ju];
      i = istart;
      ihi = istart + iheight;
      while (i < ihi) {
        temp_re_tmp = i + iy;
        temp_re = twid_re * y->data[temp_re_tmp].re -
                  twid_im * y->data[temp_re_tmp].im;
        temp_im = twid_re * y->data[temp_re_tmp].im +
                  twid_im * y->data[temp_re_tmp].re;
        y->data[temp_re_tmp].re = y->data[i].re - temp_re;
        y->data[temp_re_tmp].im = y->data[i].im - temp_im;
        y->data[i].re += temp_re;
        y->data[i].im += temp_im;
        i += iDelta2;
      }
      istart++;
    }
    k /= 2;
    iy = iDelta2;
    iDelta2 += iDelta2;
    iheight -= iy;
  }
  if (y->size[0] > 1) {
    twid_re = 1.0 / (double)y->size[0];
    iy = y->size[0];
    for (iDelta2 = 0; iDelta2 < iy; iDelta2++) {
      y->data[iDelta2].re *= twid_re;
      y->data[iDelta2].im *= twid_re;
    }
  }
}

/*
 * Arguments    : const emxArray_creal32_T *x
 *                int n2blue
 *                int nfft
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                const emxArray_real32_T *sintabinv
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void c_FFTImplementationCallback_dob(const emxArray_creal32_T *x, int n2blue,
                                     int nfft, const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     const emxArray_real32_T *sintabinv,
                                     emxArray_creal32_T *y)
{
  emxArray_creal32_T *b_fv;
  emxArray_creal32_T *fv;
  emxArray_creal32_T *wwc;
  float nt_im;
  float nt_re;
  int i;
  int k;
  int minNrowsNx;
  int nInt2;
  int nInt2m1;
  int rt;
  emxInit_creal32_T(&wwc, 1);
  nInt2m1 = (nfft + nfft) - 1;
  i = wwc->size[0];
  wwc->size[0] = nInt2m1;
  emxEnsureCapacity_creal32_T(wwc, i);
  rt = 0;
  wwc->data[nfft - 1].re = 1.0F;
  wwc->data[nfft - 1].im = 0.0F;
  nInt2 = nfft << 1;
  for (k = 0; k <= nfft - 2; k++) {
    minNrowsNx = ((k + 1) << 1) - 1;
    if (nInt2 - rt <= minNrowsNx) {
      rt += minNrowsNx - nInt2;
    } else {
      rt += minNrowsNx;
    }
    nt_im = 3.14159274F * (float)rt / (float)nfft;
    if (nt_im == 0.0F) {
      nt_re = 1.0F;
      nt_im = 0.0F;
    } else {
      nt_re = cosf(nt_im);
      nt_im = sinf(nt_im);
    }
    i = (nfft - k) - 2;
    wwc->data[i].re = nt_re;
    wwc->data[i].im = -nt_im;
  }
  i = nInt2m1 - 1;
  for (k = i; k >= nfft; k--) {
    wwc->data[k] = wwc->data[(nInt2m1 - k) - 1];
  }
  emxInit_creal32_T(&fv, 1);
  i = fv->size[0];
  fv->size[0] = nfft;
  emxEnsureCapacity_creal32_T(fv, i);
  if (nfft > x->size[0]) {
    i = fv->size[0];
    fv->size[0] = nfft;
    emxEnsureCapacity_creal32_T(fv, i);
    for (i = 0; i < nfft; i++) {
      fv->data[i].re = 0.0F;
      fv->data[i].im = 0.0F;
    }
  }
  i = y->size[0];
  y->size[0] = fv->size[0];
  emxEnsureCapacity_creal32_T(y, i);
  minNrowsNx = fv->size[0];
  for (i = 0; i < minNrowsNx; i++) {
    y->data[i] = fv->data[i];
  }
  minNrowsNx = x->size[0];
  if (nfft < minNrowsNx) {
    minNrowsNx = nfft;
  }
  for (k = 0; k < minNrowsNx; k++) {
    nInt2m1 = (nfft + k) - 1;
    nt_re = wwc->data[nInt2m1].re;
    nt_im = wwc->data[nInt2m1].im;
    y->data[k].re = nt_re * x->data[k].re + nt_im * x->data[k].im;
    y->data[k].im = nt_re * x->data[k].im - nt_im * x->data[k].re;
  }
  i = minNrowsNx + 1;
  for (k = i; k <= nfft; k++) {
    y->data[k - 1].re = 0.0F;
    y->data[k - 1].im = 0.0F;
  }
  emxInit_creal32_T(&b_fv, 1);
  d_FFTImplementationCallback_r2b(y, n2blue, costab, sintab, fv);
  d_FFTImplementationCallback_r2b(wwc, n2blue, costab, sintab, b_fv);
  i = b_fv->size[0];
  b_fv->size[0] = fv->size[0];
  emxEnsureCapacity_creal32_T(b_fv, i);
  minNrowsNx = fv->size[0];
  for (i = 0; i < minNrowsNx; i++) {
    nt_re =
        fv->data[i].re * b_fv->data[i].im + fv->data[i].im * b_fv->data[i].re;
    b_fv->data[i].re =
        fv->data[i].re * b_fv->data[i].re - fv->data[i].im * b_fv->data[i].im;
    b_fv->data[i].im = nt_re;
  }
  c_FFTImplementationCallback_r2b(b_fv, n2blue, costab, sintabinv, fv);
  i = (int)(float)nfft;
  minNrowsNx = wwc->size[0];
  emxFree_creal32_T(&b_fv);
  for (k = i; k <= minNrowsNx; k++) {
    nt_re = wwc->data[k - 1].re * fv->data[k - 1].re +
            wwc->data[k - 1].im * fv->data[k - 1].im;
    nt_im = wwc->data[k - 1].re * fv->data[k - 1].im -
            wwc->data[k - 1].im * fv->data[k - 1].re;
    if (nt_im == 0.0F) {
      nInt2m1 = k - (int)(float)nfft;
      y->data[nInt2m1].re = nt_re / (float)nfft;
      y->data[nInt2m1].im = 0.0F;
    } else if (nt_re == 0.0F) {
      nInt2m1 = k - (int)(float)nfft;
      y->data[nInt2m1].re = 0.0F;
      y->data[nInt2m1].im = nt_im / (float)nfft;
    } else {
      nInt2m1 = k - (int)(float)nfft;
      y->data[nInt2m1].re = nt_re / (float)nfft;
      y->data[nInt2m1].im = nt_im / (float)nfft;
    }
  }
  emxFree_creal32_T(&fv);
  emxFree_creal32_T(&wwc);
}

/*
 * Arguments    : int nRows
 *                boolean_T useRadix2
 *                emxArray_real32_T *costab
 *                emxArray_real32_T *sintab
 *                emxArray_real32_T *sintabinv
 * Return Type  : void
 */
void c_FFTImplementationCallback_gen(int nRows, boolean_T useRadix2,
                                     emxArray_real32_T *costab,
                                     emxArray_real32_T *sintab,
                                     emxArray_real32_T *sintabinv)
{
  emxArray_real32_T *costab1q;
  float e;
  int i;
  int k;
  int n;
  int nd2;
  emxInit_real32_T(&costab1q, 2);
  e = 6.28318548F / (float)nRows;
  n = nRows / 2 / 2;
  i = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = n + 1;
  emxEnsureCapacity_real32_T(costab1q, i);
  costab1q->data[0] = 1.0F;
  nd2 = n / 2 - 1;
  for (k = 0; k <= nd2; k++) {
    costab1q->data[k + 1] = cosf(e * (float)(k + 1));
  }
  i = nd2 + 2;
  nd2 = n - 1;
  for (k = i; k <= nd2; k++) {
    costab1q->data[k] = sinf(e * (float)(n - k));
  }
  costab1q->data[n] = 0.0F;
  if (!useRadix2) {
    n = costab1q->size[1] - 1;
    nd2 = (costab1q->size[1] - 1) << 1;
    i = costab->size[0] * costab->size[1];
    costab->size[0] = 1;
    costab->size[1] = nd2 + 1;
    emxEnsureCapacity_real32_T(costab, i);
    i = sintab->size[0] * sintab->size[1];
    sintab->size[0] = 1;
    sintab->size[1] = nd2 + 1;
    emxEnsureCapacity_real32_T(sintab, i);
    costab->data[0] = 1.0F;
    sintab->data[0] = 0.0F;
    i = sintabinv->size[0] * sintabinv->size[1];
    sintabinv->size[0] = 1;
    sintabinv->size[1] = nd2 + 1;
    emxEnsureCapacity_real32_T(sintabinv, i);
    for (k = 0; k < n; k++) {
      sintabinv->data[k + 1] = costab1q->data[(n - k) - 1];
    }
    i = costab1q->size[1];
    for (k = i; k <= nd2; k++) {
      sintabinv->data[k] = costab1q->data[k - n];
    }
    for (k = 0; k < n; k++) {
      costab->data[k + 1] = costab1q->data[k + 1];
      sintab->data[k + 1] = -costab1q->data[(n - k) - 1];
    }
    i = costab1q->size[1];
    for (k = i; k <= nd2; k++) {
      costab->data[k] = -costab1q->data[nd2 - k];
      sintab->data[k] = -costab1q->data[k - n];
    }
  } else {
    n = costab1q->size[1] - 1;
    nd2 = (costab1q->size[1] - 1) << 1;
    i = costab->size[0] * costab->size[1];
    costab->size[0] = 1;
    costab->size[1] = nd2 + 1;
    emxEnsureCapacity_real32_T(costab, i);
    i = sintab->size[0] * sintab->size[1];
    sintab->size[0] = 1;
    sintab->size[1] = nd2 + 1;
    emxEnsureCapacity_real32_T(sintab, i);
    costab->data[0] = 1.0F;
    sintab->data[0] = 0.0F;
    for (k = 0; k < n; k++) {
      costab->data[k + 1] = costab1q->data[k + 1];
      sintab->data[k + 1] = costab1q->data[(n - k) - 1];
    }
    i = costab1q->size[1];
    for (k = i; k <= nd2; k++) {
      costab->data[k] = -costab1q->data[nd2 - k];
      sintab->data[k] = costab1q->data[k - n];
    }
    sintabinv->size[0] = 1;
    sintabinv->size[1] = 0;
  }
  emxFree_real32_T(&costab1q);
}

/*
 * Arguments    : int nfft
 *                boolean_T useRadix2
 *                int *n2blue
 *                int *nRows
 * Return Type  : void
 */
void c_FFTImplementationCallback_get(int nfft, boolean_T useRadix2, int *n2blue,
                                     int *nRows)
{
  int k;
  int n;
  int pmax;
  int pmin;
  int pow2p;
  boolean_T exitg1;
  *n2blue = 1;
  if (useRadix2) {
    *nRows = nfft;
  } else {
    if (nfft > 0) {
      n = (nfft + nfft) - 1;
      pmax = 31;
      if (n <= 1) {
        pmax = 0;
      } else {
        pmin = 0;
        exitg1 = false;
        while ((!exitg1) && (pmax - pmin > 1)) {
          k = (pmin + pmax) >> 1;
          pow2p = 1 << k;
          if (pow2p == n) {
            pmax = k;
            exitg1 = true;
          } else if (pow2p > n) {
            pmax = k;
          } else {
            pmin = k;
          }
        }
      }
      *n2blue = 1 << pmax;
    }
    *nRows = *n2blue;
  }
}

/*
 * Arguments    : const emxArray_creal32_T *x
 *                int n1_unsigned
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void c_FFTImplementationCallback_r2b(const emxArray_creal32_T *x,
                                     int n1_unsigned,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     emxArray_creal32_T *y)
{
  float temp_im;
  float temp_re;
  float twid_im;
  float twid_re;
  int i;
  int iDelta2;
  int iheight;
  int ihi;
  int istart;
  int iy;
  int ju;
  int k;
  int nRowsD2;
  int temp_re_tmp;
  boolean_T tst;
  iDelta2 = y->size[0];
  y->size[0] = n1_unsigned;
  emxEnsureCapacity_creal32_T(y, iDelta2);
  if (n1_unsigned > x->size[0]) {
    iDelta2 = y->size[0];
    y->size[0] = n1_unsigned;
    emxEnsureCapacity_creal32_T(y, iDelta2);
    for (iDelta2 = 0; iDelta2 < n1_unsigned; iDelta2++) {
      y->data[iDelta2].re = 0.0F;
      y->data[iDelta2].im = 0.0F;
    }
  }
  iheight = x->size[0];
  if (iheight >= n1_unsigned) {
    iheight = n1_unsigned;
  }
  istart = n1_unsigned - 2;
  nRowsD2 = n1_unsigned / 2;
  k = nRowsD2 / 2;
  iy = 0;
  ju = 0;
  for (i = 0; i <= iheight - 2; i++) {
    y->data[iy] = x->data[i];
    iDelta2 = n1_unsigned;
    tst = true;
    while (tst) {
      iDelta2 >>= 1;
      ju ^= iDelta2;
      tst = ((ju & iDelta2) == 0);
    }
    iy = ju;
  }
  y->data[iy] = x->data[iheight - 1];
  if (n1_unsigned > 1) {
    for (i = 0; i <= istart; i += 2) {
      temp_re = y->data[i + 1].re;
      temp_im = y->data[i + 1].im;
      twid_re = y->data[i].re;
      twid_im = y->data[i].im;
      y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
      y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
      twid_re += temp_re;
      twid_im += temp_im;
      y->data[i].re = twid_re;
      y->data[i].im = twid_im;
    }
  }
  iy = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      temp_re_tmp = i + iy;
      temp_re = y->data[temp_re_tmp].re;
      temp_im = y->data[temp_re_tmp].im;
      y->data[temp_re_tmp].re = y->data[i].re - temp_re;
      y->data[temp_re_tmp].im = y->data[i].im - temp_im;
      y->data[i].re += temp_re;
      y->data[i].im += temp_im;
    }
    istart = 1;
    for (ju = k; ju < nRowsD2; ju += k) {
      twid_re = costab->data[ju];
      twid_im = sintab->data[ju];
      i = istart;
      ihi = istart + iheight;
      while (i < ihi) {
        temp_re_tmp = i + iy;
        temp_re = twid_re * y->data[temp_re_tmp].re -
                  twid_im * y->data[temp_re_tmp].im;
        temp_im = twid_re * y->data[temp_re_tmp].im +
                  twid_im * y->data[temp_re_tmp].re;
        y->data[temp_re_tmp].re = y->data[i].re - temp_re;
        y->data[temp_re_tmp].im = y->data[i].im - temp_im;
        y->data[i].re += temp_re;
        y->data[i].im += temp_im;
        i += iDelta2;
      }
      istart++;
    }
    k /= 2;
    iy = iDelta2;
    iDelta2 += iDelta2;
    iheight -= iy;
  }
  if (y->size[0] > 1) {
    twid_re = 1.0F / (float)y->size[0];
    iy = y->size[0];
    for (iDelta2 = 0; iDelta2 < iy; iDelta2++) {
      y->data[iDelta2].re *= twid_re;
      y->data[iDelta2].im *= twid_re;
    }
  }
}

/*
 * Arguments    : const emxArray_creal32_T *x
 *                int n2blue
 *                int nfft
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                const emxArray_real32_T *sintabinv
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void d_FFTImplementationCallback_dob(const emxArray_creal32_T *x, int n2blue,
                                     int nfft, const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     const emxArray_real32_T *sintabinv,
                                     emxArray_creal32_T *y)
{
  emxArray_creal32_T *b_fv;
  emxArray_creal32_T *fv;
  emxArray_creal32_T *wwc;
  float nt_im;
  float nt_re;
  int i;
  int k;
  int minNrowsNx;
  int nInt2;
  int nInt2m1;
  int rt;
  emxInit_creal32_T(&wwc, 1);
  nInt2m1 = (nfft + nfft) - 1;
  i = wwc->size[0];
  wwc->size[0] = nInt2m1;
  emxEnsureCapacity_creal32_T(wwc, i);
  rt = 0;
  wwc->data[nfft - 1].re = 1.0F;
  wwc->data[nfft - 1].im = 0.0F;
  nInt2 = nfft << 1;
  for (k = 0; k <= nfft - 2; k++) {
    minNrowsNx = ((k + 1) << 1) - 1;
    if (nInt2 - rt <= minNrowsNx) {
      rt += minNrowsNx - nInt2;
    } else {
      rt += minNrowsNx;
    }
    nt_im = -3.14159274F * (float)rt / (float)nfft;
    if (nt_im == 0.0F) {
      nt_re = 1.0F;
      nt_im = 0.0F;
    } else {
      nt_re = cosf(nt_im);
      nt_im = sinf(nt_im);
    }
    i = (nfft - k) - 2;
    wwc->data[i].re = nt_re;
    wwc->data[i].im = -nt_im;
  }
  i = nInt2m1 - 1;
  for (k = i; k >= nfft; k--) {
    wwc->data[k] = wwc->data[(nInt2m1 - k) - 1];
  }
  emxInit_creal32_T(&fv, 1);
  i = fv->size[0];
  fv->size[0] = nfft;
  emxEnsureCapacity_creal32_T(fv, i);
  if (nfft > x->size[0]) {
    i = fv->size[0];
    fv->size[0] = nfft;
    emxEnsureCapacity_creal32_T(fv, i);
    for (i = 0; i < nfft; i++) {
      fv->data[i].re = 0.0F;
      fv->data[i].im = 0.0F;
    }
  }
  i = y->size[0];
  y->size[0] = fv->size[0];
  emxEnsureCapacity_creal32_T(y, i);
  minNrowsNx = fv->size[0];
  for (i = 0; i < minNrowsNx; i++) {
    y->data[i] = fv->data[i];
  }
  minNrowsNx = x->size[0];
  if (nfft < minNrowsNx) {
    minNrowsNx = nfft;
  }
  for (k = 0; k < minNrowsNx; k++) {
    nInt2m1 = (nfft + k) - 1;
    nt_re = wwc->data[nInt2m1].re;
    nt_im = wwc->data[nInt2m1].im;
    y->data[k].re = nt_re * x->data[k].re + nt_im * x->data[k].im;
    y->data[k].im = nt_re * x->data[k].im - nt_im * x->data[k].re;
  }
  i = minNrowsNx + 1;
  for (k = i; k <= nfft; k++) {
    y->data[k - 1].re = 0.0F;
    y->data[k - 1].im = 0.0F;
  }
  emxInit_creal32_T(&b_fv, 1);
  d_FFTImplementationCallback_r2b(y, n2blue, costab, sintab, fv);
  d_FFTImplementationCallback_r2b(wwc, n2blue, costab, sintab, b_fv);
  i = b_fv->size[0];
  b_fv->size[0] = fv->size[0];
  emxEnsureCapacity_creal32_T(b_fv, i);
  minNrowsNx = fv->size[0];
  for (i = 0; i < minNrowsNx; i++) {
    nt_re =
        fv->data[i].re * b_fv->data[i].im + fv->data[i].im * b_fv->data[i].re;
    b_fv->data[i].re =
        fv->data[i].re * b_fv->data[i].re - fv->data[i].im * b_fv->data[i].im;
    b_fv->data[i].im = nt_re;
  }
  c_FFTImplementationCallback_r2b(b_fv, n2blue, costab, sintabinv, fv);
  i = (int)(float)nfft;
  minNrowsNx = wwc->size[0];
  emxFree_creal32_T(&b_fv);
  for (k = i; k <= minNrowsNx; k++) {
    nInt2m1 = k - (int)(float)nfft;
    y->data[nInt2m1].re = wwc->data[k - 1].re * fv->data[k - 1].re +
                          wwc->data[k - 1].im * fv->data[k - 1].im;
    y->data[nInt2m1].im = wwc->data[k - 1].re * fv->data[k - 1].im -
                          wwc->data[k - 1].im * fv->data[k - 1].re;
  }
  emxFree_creal32_T(&fv);
  emxFree_creal32_T(&wwc);
}

/*
 * Arguments    : int nRows
 *                boolean_T useRadix2
 *                emxArray_real32_T *costab
 *                emxArray_real32_T *sintab
 *                emxArray_real32_T *sintabinv
 * Return Type  : void
 */
void d_FFTImplementationCallback_gen(int nRows, boolean_T useRadix2,
                                     emxArray_real32_T *costab,
                                     emxArray_real32_T *sintab,
                                     emxArray_real32_T *sintabinv)
{
  emxArray_real32_T *costab1q;
  float e;
  int i;
  int k;
  int n;
  int nd2;
  emxInit_real32_T(&costab1q, 2);
  e = 6.28318548F / (float)nRows;
  n = nRows / 2 / 2;
  i = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = n + 1;
  emxEnsureCapacity_real32_T(costab1q, i);
  costab1q->data[0] = 1.0F;
  nd2 = n / 2 - 1;
  for (k = 0; k <= nd2; k++) {
    costab1q->data[k + 1] = cosf(e * (float)(k + 1));
  }
  i = nd2 + 2;
  nd2 = n - 1;
  for (k = i; k <= nd2; k++) {
    costab1q->data[k] = sinf(e * (float)(n - k));
  }
  costab1q->data[n] = 0.0F;
  if (!useRadix2) {
    n = costab1q->size[1] - 1;
    nd2 = (costab1q->size[1] - 1) << 1;
    i = costab->size[0] * costab->size[1];
    costab->size[0] = 1;
    costab->size[1] = nd2 + 1;
    emxEnsureCapacity_real32_T(costab, i);
    i = sintab->size[0] * sintab->size[1];
    sintab->size[0] = 1;
    sintab->size[1] = nd2 + 1;
    emxEnsureCapacity_real32_T(sintab, i);
    costab->data[0] = 1.0F;
    sintab->data[0] = 0.0F;
    i = sintabinv->size[0] * sintabinv->size[1];
    sintabinv->size[0] = 1;
    sintabinv->size[1] = nd2 + 1;
    emxEnsureCapacity_real32_T(sintabinv, i);
    for (k = 0; k < n; k++) {
      sintabinv->data[k + 1] = costab1q->data[(n - k) - 1];
    }
    i = costab1q->size[1];
    for (k = i; k <= nd2; k++) {
      sintabinv->data[k] = costab1q->data[k - n];
    }
    for (k = 0; k < n; k++) {
      costab->data[k + 1] = costab1q->data[k + 1];
      sintab->data[k + 1] = -costab1q->data[(n - k) - 1];
    }
    i = costab1q->size[1];
    for (k = i; k <= nd2; k++) {
      costab->data[k] = -costab1q->data[nd2 - k];
      sintab->data[k] = -costab1q->data[k - n];
    }
  } else {
    n = costab1q->size[1] - 1;
    nd2 = (costab1q->size[1] - 1) << 1;
    i = costab->size[0] * costab->size[1];
    costab->size[0] = 1;
    costab->size[1] = nd2 + 1;
    emxEnsureCapacity_real32_T(costab, i);
    i = sintab->size[0] * sintab->size[1];
    sintab->size[0] = 1;
    sintab->size[1] = nd2 + 1;
    emxEnsureCapacity_real32_T(sintab, i);
    costab->data[0] = 1.0F;
    sintab->data[0] = 0.0F;
    for (k = 0; k < n; k++) {
      costab->data[k + 1] = costab1q->data[k + 1];
      sintab->data[k + 1] = -costab1q->data[(n - k) - 1];
    }
    i = costab1q->size[1];
    for (k = i; k <= nd2; k++) {
      costab->data[k] = -costab1q->data[nd2 - k];
      sintab->data[k] = -costab1q->data[k - n];
    }
    sintabinv->size[0] = 1;
    sintabinv->size[1] = 0;
  }
  emxFree_real32_T(&costab1q);
}

/*
 * Arguments    : const emxArray_creal32_T *x
 *                int unsigned_nRows
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void d_FFTImplementationCallback_r2b(const emxArray_creal32_T *x,
                                     int unsigned_nRows,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     emxArray_creal32_T *y)
{
  float temp_im;
  float temp_re;
  float twid_im;
  float twid_re;
  int i;
  int iDelta2;
  int iheight;
  int ihi;
  int iy;
  int j;
  int ju;
  int k;
  int nRowsD2;
  int temp_re_tmp;
  boolean_T tst;
  iy = y->size[0];
  y->size[0] = unsigned_nRows;
  emxEnsureCapacity_creal32_T(y, iy);
  if (unsigned_nRows > x->size[0]) {
    iy = y->size[0];
    y->size[0] = unsigned_nRows;
    emxEnsureCapacity_creal32_T(y, iy);
    for (iy = 0; iy < unsigned_nRows; iy++) {
      y->data[iy].re = 0.0F;
      y->data[iy].im = 0.0F;
    }
  }
  iDelta2 = x->size[0];
  if (iDelta2 >= unsigned_nRows) {
    iDelta2 = unsigned_nRows;
  }
  iheight = unsigned_nRows - 2;
  nRowsD2 = unsigned_nRows / 2;
  k = nRowsD2 / 2;
  iy = 0;
  ju = 0;
  for (i = 0; i <= iDelta2 - 2; i++) {
    y->data[iy] = x->data[i];
    iy = unsigned_nRows;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }
    iy = ju;
  }
  y->data[iy] = x->data[iDelta2 - 1];
  if (unsigned_nRows > 1) {
    for (i = 0; i <= iheight; i += 2) {
      temp_re = y->data[i + 1].re;
      temp_im = y->data[i + 1].im;
      twid_re = y->data[i].re;
      twid_im = y->data[i].im;
      y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
      y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
      twid_re += temp_re;
      twid_im += temp_im;
      y->data[i].re = twid_re;
      y->data[i].im = twid_im;
    }
  }
  iy = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      temp_re_tmp = i + iy;
      temp_re = y->data[temp_re_tmp].re;
      temp_im = y->data[temp_re_tmp].im;
      y->data[temp_re_tmp].re = y->data[i].re - temp_re;
      y->data[temp_re_tmp].im = y->data[i].im - temp_im;
      y->data[i].re += temp_re;
      y->data[i].im += temp_im;
    }
    ju = 1;
    for (j = k; j < nRowsD2; j += k) {
      twid_re = costab->data[j];
      twid_im = sintab->data[j];
      i = ju;
      ihi = ju + iheight;
      while (i < ihi) {
        temp_re_tmp = i + iy;
        temp_re = twid_re * y->data[temp_re_tmp].re -
                  twid_im * y->data[temp_re_tmp].im;
        temp_im = twid_re * y->data[temp_re_tmp].im +
                  twid_im * y->data[temp_re_tmp].re;
        y->data[temp_re_tmp].re = y->data[i].re - temp_re;
        y->data[temp_re_tmp].im = y->data[i].im - temp_im;
        y->data[i].re += temp_re;
        y->data[i].im += temp_im;
        i += iDelta2;
      }
      ju++;
    }
    k /= 2;
    iy = iDelta2;
    iDelta2 += iDelta2;
    iheight -= iy;
  }
}

/*
 * Arguments    : const creal_T x_data[]
 *                int n2blue
 *                const double costab_data[]
 *                const int costab_size[2]
 *                const double sintab_data[]
 *                const int sintab_size[2]
 *                const double sintabinv_data[]
 *                const int sintabinv_size[2]
 *                creal_T y_data[]
 *                int y_size[3]
 * Return Type  : void
 */
void e_FFTImplementationCallback_dob(
    const creal_T x_data[], int n2blue, const double costab_data[],
    const int costab_size[2], const double sintab_data[],
    const int sintab_size[2], const double sintabinv_data[],
    const int sintabinv_size[2], creal_T y_data[], int y_size[3])
{
  static creal_T wwc_data[3071];
  emxArray_creal_T b_wwc_data;
  emxArray_creal_T tmp_data;
  emxArray_creal_T *b_y;
  emxArray_creal_T *fv;
  emxArray_creal_T *r;
  emxArray_real_T b_costab_data;
  emxArray_real_T b_sintab_data;
  emxArray_real_T b_sintabinv_data;
  emxArray_real_T c_costab_data;
  emxArray_real_T c_sintab_data;
  emxArray_real_T d_costab_data;
  creal_T b_tmp_data[1536];
  double im;
  double nt_im;
  double nt_re;
  double re;
  int b_k;
  int chan;
  int k;
  int re_tmp;
  int rt;
  int tmp_size;
  int wwc_size;
  int xoff;
  int y;
  wwc_size = 3071;
  rt = 0;
  wwc_data[1535].re = 1.0;
  wwc_data[1535].im = 0.0;
  for (k = 0; k < 1535; k++) {
    y = ((k + 1) << 1) - 1;
    if (3072 - rt <= y) {
      rt = (y + rt) - 3072;
    } else {
      rt += y;
    }
    nt_im = 3.1415926535897931 * (double)rt / 1536.0;
    if (nt_im == 0.0) {
      nt_re = 1.0;
      nt_im = 0.0;
    } else {
      nt_re = cos(nt_im);
      nt_im = sin(nt_im);
    }
    wwc_data[1534 - k].re = nt_re;
    wwc_data[1534 - k].im = -nt_im;
  }
  for (k = 1534; k >= 0; k--) {
    wwc_data[k + 1536] = wwc_data[1534 - k];
  }
  y_size[0] = 1536;
  y_size[1] = 2;
  y_size[2] = 1;
#pragma omp parallel num_threads(omp_get_max_threads()) private(               \
    fv, r, b_y, xoff, b_k, tmp_data, re_tmp, re, im, b_costab_data,            \
    b_sintab_data, b_wwc_data, c_costab_data, c_sintab_data, d_costab_data,    \
    b_sintabinv_data) firstprivate(tmp_size, b_tmp_data)
  {
    emxInit_creal_T(&fv, 1);
    emxInit_creal_T(&r, 1);
    emxInit_creal_T(&b_y, 1);
#pragma omp for nowait
    for (chan = 0; chan < 2; chan++) {
      xoff = chan * 1536;
      tmp_size = 1536;
      for (b_k = 0; b_k < 1536; b_k++) {
        re_tmp = xoff + b_k;
        re = x_data[re_tmp].re;
        im = x_data[re_tmp].im;
        b_tmp_data[b_k].re =
            wwc_data[b_k + 1535].re * re + wwc_data[b_k + 1535].im * im;
        b_tmp_data[b_k].im =
            wwc_data[b_k + 1535].re * im - wwc_data[b_k + 1535].im * re;
      }
      tmp_data.data = &b_tmp_data[0];
      tmp_data.size = &tmp_size;
      tmp_data.allocatedSize = 1536;
      tmp_data.numDimensions = 1;
      tmp_data.canFreeData = false;
      b_costab_data.data = (double *)&costab_data[0];
      b_costab_data.size = (int *)&costab_size[0];
      b_costab_data.allocatedSize = -1;
      b_costab_data.numDimensions = 2;
      b_costab_data.canFreeData = false;
      b_sintab_data.data = (double *)&sintab_data[0];
      b_sintab_data.size = (int *)&sintab_size[0];
      b_sintab_data.allocatedSize = -1;
      b_sintab_data.numDimensions = 2;
      b_sintab_data.canFreeData = false;
      e_FFTImplementationCallback_r2b(&tmp_data, n2blue, &b_costab_data,
                                      &b_sintab_data, b_y);
      b_wwc_data.data = &wwc_data[0];
      b_wwc_data.size = &wwc_size;
      b_wwc_data.allocatedSize = 3071;
      b_wwc_data.numDimensions = 1;
      b_wwc_data.canFreeData = false;
      c_costab_data.data = (double *)&costab_data[0];
      c_costab_data.size = (int *)&costab_size[0];
      c_costab_data.allocatedSize = -1;
      c_costab_data.numDimensions = 2;
      c_costab_data.canFreeData = false;
      c_sintab_data.data = (double *)&sintab_data[0];
      c_sintab_data.size = (int *)&sintab_size[0];
      c_sintab_data.allocatedSize = -1;
      c_sintab_data.numDimensions = 2;
      c_sintab_data.canFreeData = false;
      e_FFTImplementationCallback_r2b(&b_wwc_data, n2blue, &c_costab_data,
                                      &c_sintab_data, r);
      re_tmp = fv->size[0];
      fv->size[0] = b_y->size[0];
      emxEnsureCapacity_creal_T(fv, re_tmp);
      xoff = b_y->size[0];
      for (re_tmp = 0; re_tmp < xoff; re_tmp++) {
        fv->data[re_tmp].re = b_y->data[re_tmp].re * r->data[re_tmp].re -
                              b_y->data[re_tmp].im * r->data[re_tmp].im;
        fv->data[re_tmp].im = b_y->data[re_tmp].re * r->data[re_tmp].im +
                              b_y->data[re_tmp].im * r->data[re_tmp].re;
      }
      d_costab_data.data = (double *)&costab_data[0];
      d_costab_data.size = (int *)&costab_size[0];
      d_costab_data.allocatedSize = -1;
      d_costab_data.numDimensions = 2;
      d_costab_data.canFreeData = false;
      b_sintabinv_data.data = (double *)&sintabinv_data[0];
      b_sintabinv_data.size = (int *)&sintabinv_size[0];
      b_sintabinv_data.allocatedSize = -1;
      b_sintabinv_data.numDimensions = 2;
      b_sintabinv_data.canFreeData = false;
      f_FFTImplementationCallback_r2b(fv, n2blue, &d_costab_data,
                                      &b_sintabinv_data, b_y);
      re_tmp = fv->size[0];
      fv->size[0] = b_y->size[0];
      emxEnsureCapacity_creal_T(fv, re_tmp);
      xoff = b_y->size[0];
      for (re_tmp = 0; re_tmp < xoff; re_tmp++) {
        fv->data[re_tmp] = b_y->data[re_tmp];
      }
      for (b_k = 0; b_k < 1536; b_k++) {
        re = wwc_data[b_k + 1535].re * fv->data[b_k + 1535].re +
             wwc_data[b_k + 1535].im * fv->data[b_k + 1535].im;
        im = wwc_data[b_k + 1535].re * fv->data[b_k + 1535].im -
             wwc_data[b_k + 1535].im * fv->data[b_k + 1535].re;
        if (im == 0.0) {
          re_tmp = b_k + 1536 * chan;
          y_data[re_tmp].re = re / 1536.0;
          y_data[re_tmp].im = 0.0;
        } else if (re == 0.0) {
          re_tmp = b_k + 1536 * chan;
          y_data[re_tmp].re = 0.0;
          y_data[re_tmp].im = im / 1536.0;
        } else {
          re_tmp = b_k + 1536 * chan;
          y_data[re_tmp].re = re / 1536.0;
          y_data[re_tmp].im = im / 1536.0;
        }
      }
    }
    emxFree_creal_T(&b_y);
    emxFree_creal_T(&r);
    emxFree_creal_T(&fv);
  }
}

/*
 * Arguments    : int nRows
 *                double costab_data[]
 *                int costab_size[2]
 *                double sintab_data[]
 *                int sintab_size[2]
 *                double sintabinv_data[]
 *                int sintabinv_size[2]
 * Return Type  : void
 */
void e_FFTImplementationCallback_gen(int nRows, double costab_data[],
                                     int costab_size[2], double sintab_data[],
                                     int sintab_size[2],
                                     double sintabinv_data[],
                                     int sintabinv_size[2])
{
  emxArray_real_T *costab;
  emxArray_real_T *costab1q;
  emxArray_real_T *sintab;
  emxArray_real_T *sintabinv;
  double e;
  int i;
  int k;
  int n;
  int nd2;
  emxInit_real_T(&costab1q, 2);
  e = 6.2831853071795862 / (double)nRows;
  n = nRows / 2 / 2;
  i = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = n + 1;
  emxEnsureCapacity_real_T(costab1q, i);
  costab1q->data[0] = 1.0;
  nd2 = n / 2 - 1;
  for (k = 0; k <= nd2; k++) {
    costab1q->data[k + 1] = cos(e * ((double)k + 1.0));
  }
  i = nd2 + 2;
  nd2 = n - 1;
  for (k = i; k <= nd2; k++) {
    costab1q->data[k] = sin(e * (double)(n - k));
  }
  emxInit_real_T(&costab, 2);
  emxInit_real_T(&sintab, 2);
  emxInit_real_T(&sintabinv, 2);
  costab1q->data[n] = 0.0;
  n = costab1q->size[1] - 1;
  nd2 = (costab1q->size[1] - 1) << 1;
  i = costab->size[0] * costab->size[1];
  costab->size[0] = 1;
  costab->size[1] = nd2 + 1;
  emxEnsureCapacity_real_T(costab, i);
  i = sintab->size[0] * sintab->size[1];
  sintab->size[0] = 1;
  sintab->size[1] = nd2 + 1;
  emxEnsureCapacity_real_T(sintab, i);
  costab->data[0] = 1.0;
  sintab->data[0] = 0.0;
  i = sintabinv->size[0] * sintabinv->size[1];
  sintabinv->size[0] = 1;
  sintabinv->size[1] = nd2 + 1;
  emxEnsureCapacity_real_T(sintabinv, i);
  for (k = 0; k < n; k++) {
    sintabinv->data[k + 1] = costab1q->data[(n - k) - 1];
  }
  i = costab1q->size[1];
  for (k = i; k <= nd2; k++) {
    sintabinv->data[k] = costab1q->data[k - n];
  }
  for (k = 0; k < n; k++) {
    costab->data[k + 1] = costab1q->data[k + 1];
    sintab->data[k + 1] = -costab1q->data[(n - k) - 1];
  }
  i = costab1q->size[1];
  for (k = i; k <= nd2; k++) {
    costab->data[k] = -costab1q->data[nd2 - k];
    sintab->data[k] = -costab1q->data[k - n];
  }
  emxFree_real_T(&costab1q);
  costab_size[0] = 1;
  costab_size[1] = costab->size[1];
  nd2 = costab->size[1];
  for (i = 0; i < nd2; i++) {
    costab_data[i] = costab->data[i];
  }
  emxFree_real_T(&costab);
  sintab_size[0] = 1;
  sintab_size[1] = sintab->size[1];
  nd2 = sintab->size[1];
  for (i = 0; i < nd2; i++) {
    sintab_data[i] = sintab->data[i];
  }
  emxFree_real_T(&sintab);
  sintabinv_size[0] = 1;
  sintabinv_size[1] = sintabinv->size[1];
  nd2 = sintabinv->size[1];
  for (i = 0; i < nd2; i++) {
    sintabinv_data[i] = sintabinv->data[i];
  }
  emxFree_real_T(&sintabinv);
}

/*
 * Arguments    : const emxArray_creal_T *x
 *                int unsigned_nRows
 *                const emxArray_real_T *costab
 *                const emxArray_real_T *sintab
 *                emxArray_creal_T *y
 * Return Type  : void
 */
void e_FFTImplementationCallback_r2b(const emxArray_creal_T *x,
                                     int unsigned_nRows,
                                     const emxArray_real_T *costab,
                                     const emxArray_real_T *sintab,
                                     emxArray_creal_T *y)
{
  double temp_im;
  double temp_re;
  double twid_im;
  double twid_re;
  int i;
  int iDelta2;
  int iheight;
  int ihi;
  int iy;
  int j;
  int ju;
  int k;
  int nRowsD2;
  int temp_re_tmp;
  boolean_T tst;
  iy = y->size[0];
  y->size[0] = unsigned_nRows;
  emxEnsureCapacity_creal_T(y, iy);
  if (unsigned_nRows > x->size[0]) {
    iy = y->size[0];
    y->size[0] = unsigned_nRows;
    emxEnsureCapacity_creal_T(y, iy);
    for (iy = 0; iy < unsigned_nRows; iy++) {
      y->data[iy].re = 0.0;
      y->data[iy].im = 0.0;
    }
  }
  iDelta2 = x->size[0];
  if (iDelta2 >= unsigned_nRows) {
    iDelta2 = unsigned_nRows;
  }
  iheight = unsigned_nRows - 2;
  nRowsD2 = unsigned_nRows / 2;
  k = nRowsD2 / 2;
  iy = 0;
  ju = 0;
  for (i = 0; i <= iDelta2 - 2; i++) {
    y->data[iy] = x->data[i];
    iy = unsigned_nRows;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }
    iy = ju;
  }
  y->data[iy] = x->data[iDelta2 - 1];
  if (unsigned_nRows > 1) {
    for (i = 0; i <= iheight; i += 2) {
      temp_re = y->data[i + 1].re;
      temp_im = y->data[i + 1].im;
      twid_re = y->data[i].re;
      twid_im = y->data[i].im;
      y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
      y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
      twid_re += temp_re;
      twid_im += temp_im;
      y->data[i].re = twid_re;
      y->data[i].im = twid_im;
    }
  }
  iy = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      temp_re_tmp = i + iy;
      temp_re = y->data[temp_re_tmp].re;
      temp_im = y->data[temp_re_tmp].im;
      y->data[temp_re_tmp].re = y->data[i].re - temp_re;
      y->data[temp_re_tmp].im = y->data[i].im - temp_im;
      y->data[i].re += temp_re;
      y->data[i].im += temp_im;
    }
    ju = 1;
    for (j = k; j < nRowsD2; j += k) {
      twid_re = costab->data[j];
      twid_im = sintab->data[j];
      i = ju;
      ihi = ju + iheight;
      while (i < ihi) {
        temp_re_tmp = i + iy;
        temp_re = twid_re * y->data[temp_re_tmp].re -
                  twid_im * y->data[temp_re_tmp].im;
        temp_im = twid_re * y->data[temp_re_tmp].im +
                  twid_im * y->data[temp_re_tmp].re;
        y->data[temp_re_tmp].re = y->data[i].re - temp_re;
        y->data[temp_re_tmp].im = y->data[i].im - temp_im;
        y->data[i].re += temp_re;
        y->data[i].im += temp_im;
        i += iDelta2;
      }
      ju++;
    }
    k /= 2;
    iy = iDelta2;
    iDelta2 += iDelta2;
    iheight -= iy;
  }
}

/*
 * Arguments    : const creal_T x_data[]
 *                int n2blue
 *                int nfft
 *                const emxArray_real_T *costab
 *                const emxArray_real_T *sintab
 *                const emxArray_real_T *sintabinv
 *                emxArray_creal_T *y
 * Return Type  : void
 */
void f_FFTImplementationCallback_dob(const creal_T x_data[], int n2blue,
                                     int nfft, const emxArray_real_T *costab,
                                     const emxArray_real_T *sintab,
                                     const emxArray_real_T *sintabinv,
                                     emxArray_creal_T *y)
{
  emxArray_creal_T *b_fv;
  emxArray_creal_T *fv;
  emxArray_creal_T *wwc;
  double nt_im;
  double nt_re;
  int i;
  int k;
  int minNrowsNx;
  int nInt2;
  int nInt2m1;
  int rt;
  emxInit_creal_T(&wwc, 1);
  nInt2m1 = (nfft + nfft) - 1;
  i = wwc->size[0];
  wwc->size[0] = nInt2m1;
  emxEnsureCapacity_creal_T(wwc, i);
  rt = 0;
  wwc->data[nfft - 1].re = 1.0;
  wwc->data[nfft - 1].im = 0.0;
  nInt2 = nfft << 1;
  for (k = 0; k <= nfft - 2; k++) {
    minNrowsNx = ((k + 1) << 1) - 1;
    if (nInt2 - rt <= minNrowsNx) {
      rt += minNrowsNx - nInt2;
    } else {
      rt += minNrowsNx;
    }
    nt_im = -3.1415926535897931 * (double)rt / (double)nfft;
    if (nt_im == 0.0) {
      nt_re = 1.0;
      nt_im = 0.0;
    } else {
      nt_re = cos(nt_im);
      nt_im = sin(nt_im);
    }
    i = (nfft - k) - 2;
    wwc->data[i].re = nt_re;
    wwc->data[i].im = -nt_im;
  }
  i = nInt2m1 - 1;
  for (k = i; k >= nfft; k--) {
    wwc->data[k] = wwc->data[(nInt2m1 - k) - 1];
  }
  emxInit_creal_T(&fv, 1);
  i = fv->size[0];
  fv->size[0] = nfft;
  emxEnsureCapacity_creal_T(fv, i);
  if (nfft > 3312) {
    i = fv->size[0];
    fv->size[0] = nfft;
    emxEnsureCapacity_creal_T(fv, i);
    for (i = 0; i < nfft; i++) {
      fv->data[i].re = 0.0;
      fv->data[i].im = 0.0;
    }
  }
  i = y->size[0];
  y->size[0] = fv->size[0];
  emxEnsureCapacity_creal_T(y, i);
  minNrowsNx = fv->size[0];
  for (i = 0; i < minNrowsNx; i++) {
    y->data[i] = fv->data[i];
  }
  if (nfft < 3312) {
    minNrowsNx = nfft - 1;
  } else {
    minNrowsNx = 3311;
  }
  for (k = 0; k <= minNrowsNx; k++) {
    nInt2m1 = (nfft + k) - 1;
    nt_re = wwc->data[nInt2m1].re;
    nt_im = wwc->data[nInt2m1].im;
    y->data[k].re = nt_re * x_data[k].re + nt_im * x_data[k].im;
    y->data[k].im = nt_re * x_data[k].im - nt_im * x_data[k].re;
  }
  i = minNrowsNx + 2;
  for (k = i; k <= nfft; k++) {
    y->data[k - 1].re = 0.0;
    y->data[k - 1].im = 0.0;
  }
  emxInit_creal_T(&b_fv, 1);
  e_FFTImplementationCallback_r2b(y, n2blue, costab, sintab, fv);
  e_FFTImplementationCallback_r2b(wwc, n2blue, costab, sintab, b_fv);
  i = b_fv->size[0];
  b_fv->size[0] = fv->size[0];
  emxEnsureCapacity_creal_T(b_fv, i);
  minNrowsNx = fv->size[0];
  for (i = 0; i < minNrowsNx; i++) {
    nt_re =
        fv->data[i].re * b_fv->data[i].im + fv->data[i].im * b_fv->data[i].re;
    b_fv->data[i].re =
        fv->data[i].re * b_fv->data[i].re - fv->data[i].im * b_fv->data[i].im;
    b_fv->data[i].im = nt_re;
  }
  f_FFTImplementationCallback_r2b(b_fv, n2blue, costab, sintabinv, fv);
  i = wwc->size[0];
  emxFree_creal_T(&b_fv);
  for (k = nfft; k <= i; k++) {
    minNrowsNx = k - nfft;
    y->data[minNrowsNx].re = wwc->data[k - 1].re * fv->data[k - 1].re +
                             wwc->data[k - 1].im * fv->data[k - 1].im;
    y->data[minNrowsNx].im = wwc->data[k - 1].re * fv->data[k - 1].im -
                             wwc->data[k - 1].im * fv->data[k - 1].re;
  }
  emxFree_creal_T(&fv);
  emxFree_creal_T(&wwc);
}

/*
 * Arguments    : int nRows
 *                boolean_T useRadix2
 *                emxArray_real_T *costab
 *                emxArray_real_T *sintab
 *                emxArray_real_T *sintabinv
 * Return Type  : void
 */
void f_FFTImplementationCallback_gen(int nRows, boolean_T useRadix2,
                                     emxArray_real_T *costab,
                                     emxArray_real_T *sintab,
                                     emxArray_real_T *sintabinv)
{
  emxArray_real_T *costab1q;
  double e;
  int i;
  int k;
  int n;
  int nd2;
  emxInit_real_T(&costab1q, 2);
  e = 6.2831853071795862 / (double)nRows;
  n = nRows / 2 / 2;
  i = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = n + 1;
  emxEnsureCapacity_real_T(costab1q, i);
  costab1q->data[0] = 1.0;
  nd2 = n / 2 - 1;
  for (k = 0; k <= nd2; k++) {
    costab1q->data[k + 1] = cos(e * ((double)k + 1.0));
  }
  i = nd2 + 2;
  nd2 = n - 1;
  for (k = i; k <= nd2; k++) {
    costab1q->data[k] = sin(e * (double)(n - k));
  }
  costab1q->data[n] = 0.0;
  if (!useRadix2) {
    n = costab1q->size[1] - 1;
    nd2 = (costab1q->size[1] - 1) << 1;
    i = costab->size[0] * costab->size[1];
    costab->size[0] = 1;
    costab->size[1] = nd2 + 1;
    emxEnsureCapacity_real_T(costab, i);
    i = sintab->size[0] * sintab->size[1];
    sintab->size[0] = 1;
    sintab->size[1] = nd2 + 1;
    emxEnsureCapacity_real_T(sintab, i);
    costab->data[0] = 1.0;
    sintab->data[0] = 0.0;
    i = sintabinv->size[0] * sintabinv->size[1];
    sintabinv->size[0] = 1;
    sintabinv->size[1] = nd2 + 1;
    emxEnsureCapacity_real_T(sintabinv, i);
    for (k = 0; k < n; k++) {
      sintabinv->data[k + 1] = costab1q->data[(n - k) - 1];
    }
    i = costab1q->size[1];
    for (k = i; k <= nd2; k++) {
      sintabinv->data[k] = costab1q->data[k - n];
    }
    for (k = 0; k < n; k++) {
      costab->data[k + 1] = costab1q->data[k + 1];
      sintab->data[k + 1] = -costab1q->data[(n - k) - 1];
    }
    i = costab1q->size[1];
    for (k = i; k <= nd2; k++) {
      costab->data[k] = -costab1q->data[nd2 - k];
      sintab->data[k] = -costab1q->data[k - n];
    }
  } else {
    n = costab1q->size[1] - 1;
    nd2 = (costab1q->size[1] - 1) << 1;
    i = costab->size[0] * costab->size[1];
    costab->size[0] = 1;
    costab->size[1] = nd2 + 1;
    emxEnsureCapacity_real_T(costab, i);
    i = sintab->size[0] * sintab->size[1];
    sintab->size[0] = 1;
    sintab->size[1] = nd2 + 1;
    emxEnsureCapacity_real_T(sintab, i);
    costab->data[0] = 1.0;
    sintab->data[0] = 0.0;
    for (k = 0; k < n; k++) {
      costab->data[k + 1] = costab1q->data[k + 1];
      sintab->data[k + 1] = -costab1q->data[(n - k) - 1];
    }
    i = costab1q->size[1];
    for (k = i; k <= nd2; k++) {
      costab->data[k] = -costab1q->data[nd2 - k];
      sintab->data[k] = -costab1q->data[k - n];
    }
    sintabinv->size[0] = 1;
    sintabinv->size[1] = 0;
  }
  emxFree_real_T(&costab1q);
}

/*
 * Arguments    : const emxArray_creal32_T *x
 *                int n2blue
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                const emxArray_real32_T *sintabinv
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void g_FFTImplementationCallback_dob(const emxArray_creal32_T *x, int n2blue,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     const emxArray_real32_T *sintabinv,
                                     emxArray_creal32_T *y)
{
  emxArray_creal32_T *b_wwc;
  emxArray_creal32_T *c_y;
  emxArray_creal32_T *fv;
  emxArray_creal32_T *r;
  emxArray_creal32_T *r1;
  emxArray_creal32_T *wwc;
  float im;
  float nt_im;
  float nt_re;
  float re;
  int b_k;
  int b_y;
  int chan;
  int k;
  int re_tmp;
  int rt;
  int xoff;
  emxInit_creal32_T(&wwc, 1);
  rt = wwc->size[0];
  wwc->size[0] = 3071;
  emxEnsureCapacity_creal32_T(wwc, rt);
  rt = 0;
  wwc->data[1535].re = 1.0F;
  wwc->data[1535].im = 0.0F;
  for (k = 0; k < 1535; k++) {
    b_y = ((k + 1) << 1) - 1;
    if (3072 - rt <= b_y) {
      rt = (b_y + rt) - 3072;
    } else {
      rt += b_y;
    }
    nt_im = -3.14159274F * (float)rt / 1536.0F;
    if (nt_im == 0.0F) {
      nt_re = 1.0F;
      nt_im = 0.0F;
    } else {
      nt_re = cosf(nt_im);
      nt_im = sinf(nt_im);
    }
    wwc->data[1534 - k].re = nt_re;
    wwc->data[1534 - k].im = -nt_im;
  }
  for (k = 1534; k >= 0; k--) {
    wwc->data[k + 1536] = wwc->data[1534 - k];
  }
  emxInit_creal32_T(&b_wwc, 1);
  rt = b_wwc->size[0];
  b_wwc->size[0] = 3071;
  emxEnsureCapacity_creal32_T(b_wwc, rt);
  for (rt = 0; rt < 3071; rt++) {
    b_wwc->data[rt] = wwc->data[rt];
  }
  emxFree_creal32_T(&wwc);
  rt = y->size[0] * y->size[1];
  y->size[0] = 1536;
  y->size[1] = 600;
  emxEnsureCapacity_creal32_T(y, rt);
#pragma omp parallel num_threads(omp_get_max_threads()) private(               \
    fv, r, r1, c_y, xoff, b_k, re_tmp, re, im)
  {
    emxInit_creal32_T(&fv, 1);
    emxInit_creal32_T(&r, 1);
    emxInit_creal32_T(&r1, 1);
    emxInit_creal32_T(&c_y, 1);
#pragma omp for nowait
    for (chan = 0; chan < 600; chan++) {
      xoff = chan * 1536;
      b_k = c_y->size[0];
      c_y->size[0] = 1536;
      emxEnsureCapacity_creal32_T(c_y, b_k);
      b_k = r1->size[0];
      r1->size[0] = 1536;
      emxEnsureCapacity_creal32_T(r1, b_k);
      for (b_k = 0; b_k < 1536; b_k++) {
        r1->data[b_k] = c_y->data[b_k];
        re_tmp = xoff + b_k;
        re = x->data[re_tmp].re;
        im = x->data[re_tmp].im;
        r1->data[b_k].re =
            b_wwc->data[b_k + 1535].re * re + b_wwc->data[b_k + 1535].im * im;
        r1->data[b_k].im =
            b_wwc->data[b_k + 1535].re * im - b_wwc->data[b_k + 1535].im * re;
      }
      d_FFTImplementationCallback_r2b(r1, n2blue, costab, sintab, c_y);
      d_FFTImplementationCallback_r2b(b_wwc, n2blue, costab, sintab, r);
      b_k = fv->size[0];
      fv->size[0] = c_y->size[0];
      emxEnsureCapacity_creal32_T(fv, b_k);
      re_tmp = c_y->size[0];
      for (b_k = 0; b_k < re_tmp; b_k++) {
        fv->data[b_k].re = c_y->data[b_k].re * r->data[b_k].re -
                           c_y->data[b_k].im * r->data[b_k].im;
        fv->data[b_k].im = c_y->data[b_k].re * r->data[b_k].im +
                           c_y->data[b_k].im * r->data[b_k].re;
      }
      c_FFTImplementationCallback_r2b(fv, n2blue, costab, sintabinv, c_y);
      b_k = fv->size[0];
      fv->size[0] = c_y->size[0];
      emxEnsureCapacity_creal32_T(fv, b_k);
      re_tmp = c_y->size[0];
      for (b_k = 0; b_k < re_tmp; b_k++) {
        fv->data[b_k] = c_y->data[b_k];
      }
      for (b_k = 0; b_k < 1536; b_k++) {
        r1->data[b_k].re =
            b_wwc->data[b_k + 1535].re * fv->data[b_k + 1535].re +
            b_wwc->data[b_k + 1535].im * fv->data[b_k + 1535].im;
        r1->data[b_k].im =
            b_wwc->data[b_k + 1535].re * fv->data[b_k + 1535].im -
            b_wwc->data[b_k + 1535].im * fv->data[b_k + 1535].re;
        y->data[b_k + 1536 * chan] = r1->data[b_k];
      }
    }
    emxFree_creal32_T(&c_y);
    emxFree_creal32_T(&r1);
    emxFree_creal32_T(&r);
    emxFree_creal32_T(&fv);
  }
  emxFree_creal32_T(&b_wwc);
}

/*
 * Arguments    : int nRows
 *                emxArray_real32_T *costab
 *                emxArray_real32_T *sintab
 *                emxArray_real32_T *sintabinv
 * Return Type  : void
 */
void g_FFTImplementationCallback_gen(int nRows, emxArray_real32_T *costab,
                                     emxArray_real32_T *sintab,
                                     emxArray_real32_T *sintabinv)
{
  emxArray_real32_T *b_costab;
  emxArray_real32_T *b_sintab;
  emxArray_real32_T *b_sintabinv;
  emxArray_real32_T *costab1q;
  float e;
  int i;
  int k;
  int n;
  int nd2;
  emxInit_real32_T(&costab1q, 2);
  e = 6.28318548F / (float)nRows;
  n = nRows / 2 / 2;
  i = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = n + 1;
  emxEnsureCapacity_real32_T(costab1q, i);
  costab1q->data[0] = 1.0F;
  nd2 = n / 2 - 1;
  for (k = 0; k <= nd2; k++) {
    costab1q->data[k + 1] = cosf(e * (float)(k + 1));
  }
  i = nd2 + 2;
  nd2 = n - 1;
  for (k = i; k <= nd2; k++) {
    costab1q->data[k] = sinf(e * (float)(n - k));
  }
  emxInit_real32_T(&b_costab, 2);
  emxInit_real32_T(&b_sintab, 2);
  emxInit_real32_T(&b_sintabinv, 2);
  costab1q->data[n] = 0.0F;
  n = costab1q->size[1] - 1;
  nd2 = (costab1q->size[1] - 1) << 1;
  i = b_costab->size[0] * b_costab->size[1];
  b_costab->size[0] = 1;
  b_costab->size[1] = nd2 + 1;
  emxEnsureCapacity_real32_T(b_costab, i);
  i = b_sintab->size[0] * b_sintab->size[1];
  b_sintab->size[0] = 1;
  b_sintab->size[1] = nd2 + 1;
  emxEnsureCapacity_real32_T(b_sintab, i);
  b_costab->data[0] = 1.0F;
  b_sintab->data[0] = 0.0F;
  i = b_sintabinv->size[0] * b_sintabinv->size[1];
  b_sintabinv->size[0] = 1;
  b_sintabinv->size[1] = nd2 + 1;
  emxEnsureCapacity_real32_T(b_sintabinv, i);
  for (k = 0; k < n; k++) {
    b_sintabinv->data[k + 1] = costab1q->data[(n - k) - 1];
  }
  i = costab1q->size[1];
  for (k = i; k <= nd2; k++) {
    b_sintabinv->data[k] = costab1q->data[k - n];
  }
  for (k = 0; k < n; k++) {
    b_costab->data[k + 1] = costab1q->data[k + 1];
    b_sintab->data[k + 1] = -costab1q->data[(n - k) - 1];
  }
  i = costab1q->size[1];
  for (k = i; k <= nd2; k++) {
    b_costab->data[k] = -costab1q->data[nd2 - k];
    b_sintab->data[k] = -costab1q->data[k - n];
  }
  emxFree_real32_T(&costab1q);
  i = costab->size[0] * costab->size[1];
  costab->size[0] = 1;
  costab->size[1] = b_costab->size[1];
  emxEnsureCapacity_real32_T(costab, i);
  nd2 = b_costab->size[1];
  for (i = 0; i < nd2; i++) {
    costab->data[i] = b_costab->data[i];
  }
  emxFree_real32_T(&b_costab);
  i = sintab->size[0] * sintab->size[1];
  sintab->size[0] = 1;
  sintab->size[1] = b_sintab->size[1];
  emxEnsureCapacity_real32_T(sintab, i);
  nd2 = b_sintab->size[1];
  for (i = 0; i < nd2; i++) {
    sintab->data[i] = b_sintab->data[i];
  }
  emxFree_real32_T(&b_sintab);
  i = sintabinv->size[0] * sintabinv->size[1];
  sintabinv->size[0] = 1;
  sintabinv->size[1] = b_sintabinv->size[1];
  emxEnsureCapacity_real32_T(sintabinv, i);
  nd2 = b_sintabinv->size[1];
  for (i = 0; i < nd2; i++) {
    sintabinv->data[i] = b_sintabinv->data[i];
  }
  emxFree_real32_T(&b_sintabinv);
}

/*
 * File trailer for FFTImplementationCallback.c
 *
 * [EOF]
 */
