/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: FFTImplementationCallback.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 19-Jan-2022 16:08:45
 */

/* Include Files */
#include "FFTImplementationCallback.h"
#include "operation_emxutil.h"
#include "operation_types.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const creal_T x_data[]
 *                int n2blue
 *                const double costab_data[]
 *                const int costab_size[2]
 *                const double sintab_data[]
 *                const int sintab_size[2]
 *                const double sintabinv_data[]
 *                creal_T y_data[]
 *                int y_size[3]
 * Return Type  : void
 */
void c_FFTImplementationCallback_dob(const creal_T x_data[], int n2blue,
                                     const double costab_data[],
                                     const int costab_size[2],
                                     const double sintab_data[],
                                     const int sintab_size[2],
                                     const double sintabinv_data[],
                                     creal_T y_data[], int y_size[3])
{
  emxArray_creal_T *b_y;
  emxArray_creal_T *fv;
  emxArray_creal_T *fy;
  emxArray_creal_T *r;
  emxArray_real_T b_costab_data;
  emxArray_real_T b_sintab_data;
  creal_T wwc_data[3071];
  creal_T tmp_data[1536];
  double im;
  double nt_im;
  double nt_re;
  double temp_im;
  double temp_re;
  double twid_im;
  double twid_re;
  int b_k;
  int c_k;
  int chan;
  int i;
  int ihi;
  int istart;
  int iy;
  int j;
  int ju;
  int k;
  int nRowsD2_tmp;
  int nRowsM2_tmp;
  int rt;
  int temp_re_tmp;
  int xoff;
  int y;
  boolean_T tst;
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
    fv, fy, r, b_y, xoff, b_k, iy, twid_im, im, twid_re, temp_re, tmp_data,    \
    nRowsM2_tmp, nRowsD2_tmp, c_k, ju, i, tst, temp_im, b_costab_data, istart, \
    temp_re_tmp, j, ihi, b_sintab_data)
  {
    emxInit_creal_T(&fv, 1);
    emxInit_creal_T(&fy, 1);
    emxInit_creal_T(&r, 1);
    emxInit_creal_T(&b_y, 1);
#pragma omp for nowait
    for (chan = 0; chan < 2; chan++) {
      xoff = chan * 1536;
      for (b_k = 0; b_k < 1536; b_k++) {
        iy = xoff + b_k;
        twid_im = x_data[iy].re;
        im = x_data[iy].im;
        twid_re = wwc_data[b_k + 1535].re;
        temp_re = wwc_data[b_k + 1535].im;
        tmp_data[b_k].re = twid_re * twid_im + temp_re * im;
        tmp_data[b_k].im = twid_re * im - temp_re * twid_im;
      }
      iy = b_y->size[0];
      b_y->size[0] = n2blue;
      emxEnsureCapacity_creal_T(b_y, iy);
      if (n2blue > 1536) {
        iy = b_y->size[0];
        b_y->size[0] = n2blue;
        emxEnsureCapacity_creal_T(b_y, iy);
        for (iy = 0; iy < n2blue; iy++) {
          b_y->data[iy].re = 0.0;
          b_y->data[iy].im = 0.0;
        }
      }
      if (1536 < n2blue) {
        xoff = 1534;
      } else {
        xoff = n2blue - 2;
      }
      nRowsM2_tmp = n2blue - 2;
      nRowsD2_tmp = n2blue / 2;
      c_k = nRowsD2_tmp / 2;
      iy = 0;
      ju = 0;
      for (i = 0; i <= xoff; i++) {
        b_y->data[iy] = tmp_data[i];
        iy = n2blue;
        tst = true;
        while (tst) {
          iy >>= 1;
          ju ^= iy;
          tst = ((ju & iy) == 0);
        }
        iy = ju;
      }
      b_y->data[iy] = tmp_data[xoff + 1];
      if (n2blue > 1) {
        for (i = 0; i <= nRowsM2_tmp; i += 2) {
          temp_re = b_y->data[i + 1].re;
          temp_im = b_y->data[i + 1].im;
          twid_im = b_y->data[i].re;
          im = b_y->data[i].im;
          b_y->data[i + 1].re = b_y->data[i].re - b_y->data[i + 1].re;
          b_y->data[i + 1].im = b_y->data[i].im - b_y->data[i + 1].im;
          twid_im += temp_re;
          im += temp_im;
          b_y->data[i].re = twid_im;
          b_y->data[i].im = im;
        }
      }
      xoff = 2;
      iy = 4;
      b_k = c_k;
      ju = ((c_k - 1) << 2) + 1;
      while (b_k > 0) {
        for (i = 0; i < ju; i += iy) {
          temp_re_tmp = i + xoff;
          temp_re = b_y->data[temp_re_tmp].re;
          temp_im = b_y->data[temp_re_tmp].im;
          b_y->data[temp_re_tmp].re = b_y->data[i].re - temp_re;
          b_y->data[temp_re_tmp].im = b_y->data[i].im - temp_im;
          b_y->data[i].re += temp_re;
          b_y->data[i].im += temp_im;
        }
        istart = 1;
        for (j = b_k; j < nRowsD2_tmp; j += b_k) {
          twid_re = costab_data[j];
          twid_im = sintab_data[j];
          i = istart;
          ihi = istart + ju;
          while (i < ihi) {
            temp_re_tmp = i + xoff;
            temp_re = twid_re * b_y->data[temp_re_tmp].re -
                      twid_im * b_y->data[temp_re_tmp].im;
            temp_im = twid_re * b_y->data[temp_re_tmp].im +
                      twid_im * b_y->data[temp_re_tmp].re;
            b_y->data[temp_re_tmp].re = b_y->data[i].re - temp_re;
            b_y->data[temp_re_tmp].im = b_y->data[i].im - temp_im;
            b_y->data[i].re += temp_re;
            b_y->data[i].im += temp_im;
            i += iy;
          }
          istart++;
        }
        b_k /= 2;
        xoff = iy;
        iy += iy;
        ju -= xoff;
      }
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
      c_FFTImplementationCallback_r2b(wwc_data, 3071, n2blue, &b_costab_data,
                                      &b_sintab_data, r);
      iy = fy->size[0];
      fy->size[0] = b_y->size[0];
      emxEnsureCapacity_creal_T(fy, iy);
      xoff = b_y->size[0];
      for (iy = 0; iy < xoff; iy++) {
        fy->data[iy].re = b_y->data[iy].re * r->data[iy].re -
                          b_y->data[iy].im * r->data[iy].im;
        fy->data[iy].im = b_y->data[iy].re * r->data[iy].im +
                          b_y->data[iy].im * r->data[iy].re;
      }
      iy = b_y->size[0];
      b_y->size[0] = n2blue;
      emxEnsureCapacity_creal_T(b_y, iy);
      if (n2blue > fy->size[0]) {
        iy = b_y->size[0];
        b_y->size[0] = n2blue;
        emxEnsureCapacity_creal_T(b_y, iy);
        for (iy = 0; iy < n2blue; iy++) {
          b_y->data[iy].re = 0.0;
          b_y->data[iy].im = 0.0;
        }
      }
      iy = fv->size[0];
      fv->size[0] = b_y->size[0];
      emxEnsureCapacity_creal_T(fv, iy);
      xoff = b_y->size[0];
      for (iy = 0; iy < xoff; iy++) {
        fv->data[iy] = b_y->data[iy];
      }
      xoff = fy->size[0];
      istart = n2blue;
      if (xoff < n2blue) {
        istart = xoff;
      }
      iy = 0;
      ju = 0;
      for (i = 0; i <= istart - 2; i++) {
        fv->data[iy] = fy->data[i];
        iy = n2blue;
        tst = true;
        while (tst) {
          iy >>= 1;
          ju ^= iy;
          tst = ((ju & iy) == 0);
        }
        iy = ju;
      }
      fv->data[iy] = fy->data[istart - 1];
      iy = b_y->size[0];
      b_y->size[0] = fv->size[0];
      emxEnsureCapacity_creal_T(b_y, iy);
      xoff = fv->size[0];
      for (iy = 0; iy < xoff; iy++) {
        b_y->data[iy] = fv->data[iy];
      }
      if (n2blue > 1) {
        for (i = 0; i <= nRowsM2_tmp; i += 2) {
          temp_re = b_y->data[i + 1].re;
          temp_im = b_y->data[i + 1].im;
          twid_im = b_y->data[i].re;
          im = b_y->data[i].im;
          b_y->data[i + 1].re = b_y->data[i].re - b_y->data[i + 1].re;
          b_y->data[i + 1].im = b_y->data[i].im - b_y->data[i + 1].im;
          twid_im += temp_re;
          im += temp_im;
          b_y->data[i].re = twid_im;
          b_y->data[i].im = im;
        }
      }
      xoff = 2;
      iy = 4;
      ju = ((c_k - 1) << 2) + 1;
      while (c_k > 0) {
        for (i = 0; i < ju; i += iy) {
          temp_re_tmp = i + xoff;
          temp_re = b_y->data[temp_re_tmp].re;
          temp_im = b_y->data[temp_re_tmp].im;
          b_y->data[temp_re_tmp].re = b_y->data[i].re - temp_re;
          b_y->data[temp_re_tmp].im = b_y->data[i].im - temp_im;
          b_y->data[i].re += temp_re;
          b_y->data[i].im += temp_im;
        }
        istart = 1;
        for (j = c_k; j < nRowsD2_tmp; j += c_k) {
          twid_re = costab_data[j];
          twid_im = sintabinv_data[j];
          i = istart;
          ihi = istart + ju;
          while (i < ihi) {
            temp_re_tmp = i + xoff;
            temp_re = twid_re * b_y->data[temp_re_tmp].re -
                      twid_im * b_y->data[temp_re_tmp].im;
            temp_im = twid_re * b_y->data[temp_re_tmp].im +
                      twid_im * b_y->data[temp_re_tmp].re;
            b_y->data[temp_re_tmp].re = b_y->data[i].re - temp_re;
            b_y->data[temp_re_tmp].im = b_y->data[i].im - temp_im;
            b_y->data[i].re += temp_re;
            b_y->data[i].im += temp_im;
            i += iy;
          }
          istart++;
        }
        c_k /= 2;
        xoff = iy;
        iy += iy;
        ju -= xoff;
      }
      iy = fv->size[0];
      fv->size[0] = b_y->size[0];
      emxEnsureCapacity_creal_T(fv, iy);
      xoff = b_y->size[0];
      for (iy = 0; iy < xoff; iy++) {
        fv->data[iy] = b_y->data[iy];
      }
      if (b_y->size[0] > 1) {
        twid_re = 1.0 / (double)b_y->size[0];
        iy = fv->size[0];
        fv->size[0] = b_y->size[0];
        emxEnsureCapacity_creal_T(fv, iy);
        xoff = b_y->size[0];
        for (iy = 0; iy < xoff; iy++) {
          fv->data[iy].re = twid_re * b_y->data[iy].re;
          fv->data[iy].im = twid_re * b_y->data[iy].im;
        }
      }
      for (b_k = 0; b_k < 1536; b_k++) {
        twid_re = wwc_data[b_k + 1535].re;
        temp_re = wwc_data[b_k + 1535].im;
        twid_im = twid_re * fv->data[b_k + 1535].re +
                  temp_re * fv->data[b_k + 1535].im;
        twid_re = twid_re * fv->data[b_k + 1535].im -
                  temp_re * fv->data[b_k + 1535].re;
        if (twid_re == 0.0) {
          iy = b_k + 1536 * chan;
          y_data[iy].re = twid_im / 1536.0;
          y_data[iy].im = 0.0;
        } else if (twid_im == 0.0) {
          iy = b_k + 1536 * chan;
          y_data[iy].re = 0.0;
          y_data[iy].im = twid_re / 1536.0;
        } else {
          iy = b_k + 1536 * chan;
          y_data[iy].re = twid_im / 1536.0;
          y_data[iy].im = twid_re / 1536.0;
        }
      }
    }
    emxFree_creal_T(&b_y);
    emxFree_creal_T(&r);
    emxFree_creal_T(&fy);
    emxFree_creal_T(&fv);
  }
}

/*
 * Arguments    : int nRows
 *                emxArray_real32_T *costab
 *                emxArray_real32_T *sintab
 *                int sintabinv_size[2]
 * Return Type  : void
 */
void c_FFTImplementationCallback_gen(int nRows, emxArray_real32_T *costab,
                                     emxArray_real32_T *sintab,
                                     int sintabinv_size[2])
{
  emxArray_real32_T *costab1q;
  float e;
  int i;
  int i1;
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
  if (nd2 + 1 < 400) {
    for (k = 0; k <= nd2; k++) {
      costab1q->data[k + 1] = cosf(e * (float)(k + 1));
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = 0; k <= nd2; k++) {
      costab1q->data[k + 1] = cosf(e * (float)(k + 1));
    }
  }
  i = nd2 + 2;
  i1 = n - 1;
  if ((n - nd2) - 2 < 400) {
    for (k = i; k <= i1; k++) {
      costab1q->data[k] = sinf(e * (float)(n - k));
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = i; k <= i1; k++) {
      costab1q->data[k] = sinf(e * (float)(n - k));
    }
  }
  costab1q->data[n] = 0.0F;
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
  if (costab1q->size[1] - 1 < 400) {
    for (k = 0; k < n; k++) {
      costab->data[k + 1] = costab1q->data[k + 1];
      sintab->data[k + 1] = -costab1q->data[(n - k) - 1];
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = 0; k < n; k++) {
      costab->data[k + 1] = costab1q->data[k + 1];
      sintab->data[k + 1] = -costab1q->data[(n - k) - 1];
    }
  }
  i = costab1q->size[1];
  if ((nd2 - costab1q->size[1]) + 1 < 400) {
    for (k = i; k <= nd2; k++) {
      costab->data[k] = -costab1q->data[nd2 - k];
      sintab->data[k] = -costab1q->data[k - n];
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = i; k <= nd2; k++) {
      costab->data[k] = -costab1q->data[nd2 - k];
      sintab->data[k] = -costab1q->data[k - n];
    }
  }
  emxFree_real32_T(&costab1q);
  sintabinv_size[0] = 1;
  sintabinv_size[1] = 0;
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
  int pmax;
  int pmin;
  int pow2p;
  boolean_T exitg1;
  *n2blue = 1;
  if (useRadix2) {
    *nRows = nfft;
  } else {
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
    *n2blue = 1 << pmax;
    *nRows = *n2blue;
  }
}

/*
 * Arguments    : const creal_T x_data[]
 *                int x_size
 *                int n1_unsigned
 *                const emxArray_real_T *costab
 *                const emxArray_real_T *sintab
 *                emxArray_creal_T *y
 * Return Type  : void
 */
void c_FFTImplementationCallback_r2b(const creal_T x_data[], int x_size,
                                     int n1_unsigned,
                                     const emxArray_real_T *costab,
                                     const emxArray_real_T *sintab,
                                     emxArray_creal_T *y)
{
  double temp_im;
  double temp_re;
  double twid_im;
  double twid_re;
  int b_i;
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
  y->size[0] = n1_unsigned;
  emxEnsureCapacity_creal_T(y, iy);
  if (n1_unsigned > x_size) {
    iy = y->size[0];
    y->size[0] = n1_unsigned;
    emxEnsureCapacity_creal_T(y, iy);
    if (n1_unsigned < 400) {
      for (i = 0; i < n1_unsigned; i++) {
        y->data[i].re = 0.0;
        y->data[i].im = 0.0;
      }
    } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(i)

      for (i = 0; i < n1_unsigned; i++) {
        y->data[i].re = 0.0;
        y->data[i].im = 0.0;
      }
    }
  }
  if (x_size < n1_unsigned) {
    iDelta2 = x_size;
  } else {
    iDelta2 = n1_unsigned;
  }
  iheight = n1_unsigned - 2;
  nRowsD2 = n1_unsigned / 2;
  k = nRowsD2 / 2;
  iy = 0;
  ju = 0;
  for (b_i = 0; b_i <= iDelta2 - 2; b_i++) {
    y->data[iy] = x_data[b_i];
    iy = n1_unsigned;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }
    iy = ju;
  }
  y->data[iy] = x_data[iDelta2 - 1];
  if (n1_unsigned > 1) {
    for (b_i = 0; b_i <= iheight; b_i += 2) {
      temp_re = y->data[b_i + 1].re;
      temp_im = y->data[b_i + 1].im;
      twid_re = y->data[b_i].re;
      twid_im = y->data[b_i].im;
      y->data[b_i + 1].re = y->data[b_i].re - y->data[b_i + 1].re;
      y->data[b_i + 1].im = y->data[b_i].im - y->data[b_i + 1].im;
      twid_re += temp_re;
      twid_im += temp_im;
      y->data[b_i].re = twid_re;
      y->data[b_i].im = twid_im;
    }
  }
  iy = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (b_i = 0; b_i < iheight; b_i += iDelta2) {
      temp_re_tmp = b_i + iy;
      temp_re = y->data[temp_re_tmp].re;
      temp_im = y->data[temp_re_tmp].im;
      y->data[temp_re_tmp].re = y->data[b_i].re - temp_re;
      y->data[temp_re_tmp].im = y->data[b_i].im - temp_im;
      y->data[b_i].re += temp_re;
      y->data[b_i].im += temp_im;
    }
    ju = 1;
    for (j = k; j < nRowsD2; j += k) {
      twid_re = costab->data[j];
      twid_im = sintab->data[j];
      b_i = ju;
      ihi = ju + iheight;
      while (b_i < ihi) {
        temp_re_tmp = b_i + iy;
        temp_re = twid_re * y->data[temp_re_tmp].re -
                  twid_im * y->data[temp_re_tmp].im;
        temp_im = twid_re * y->data[temp_re_tmp].im +
                  twid_im * y->data[temp_re_tmp].re;
        y->data[temp_re_tmp].re = y->data[b_i].re - temp_re;
        y->data[temp_re_tmp].im = y->data[b_i].im - temp_im;
        y->data[b_i].re += temp_re;
        y->data[b_i].im += temp_im;
        b_i += iDelta2;
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
 * Arguments    : int nRows
 *                emxArray_real_T *costab
 *                emxArray_real_T *sintab
 *                int sintabinv_size[2]
 * Return Type  : void
 */
void d_FFTImplementationCallback_gen(int nRows, emxArray_real_T *costab,
                                     emxArray_real_T *sintab,
                                     int sintabinv_size[2])
{
  emxArray_real_T *costab1q;
  double e;
  int i;
  int i1;
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
  if (nd2 + 1 < 400) {
    for (k = 0; k <= nd2; k++) {
      costab1q->data[k + 1] = cos(e * ((double)k + 1.0));
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = 0; k <= nd2; k++) {
      costab1q->data[k + 1] = cos(e * ((double)k + 1.0));
    }
  }
  i = nd2 + 2;
  i1 = n - 1;
  if ((n - nd2) - 2 < 400) {
    for (k = i; k <= i1; k++) {
      costab1q->data[k] = sin(e * (double)(n - k));
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = i; k <= i1; k++) {
      costab1q->data[k] = sin(e * (double)(n - k));
    }
  }
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
  if (costab1q->size[1] - 1 < 400) {
    for (k = 0; k < n; k++) {
      costab->data[k + 1] = costab1q->data[k + 1];
      sintab->data[k + 1] = -costab1q->data[(n - k) - 1];
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = 0; k < n; k++) {
      costab->data[k + 1] = costab1q->data[k + 1];
      sintab->data[k + 1] = -costab1q->data[(n - k) - 1];
    }
  }
  i = costab1q->size[1];
  if ((nd2 - costab1q->size[1]) + 1 < 400) {
    for (k = i; k <= nd2; k++) {
      costab->data[k] = -costab1q->data[nd2 - k];
      sintab->data[k] = -costab1q->data[k - n];
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = i; k <= nd2; k++) {
      costab->data[k] = -costab1q->data[nd2 - k];
      sintab->data[k] = -costab1q->data[k - n];
    }
  }
  emxFree_real_T(&costab1q);
  sintabinv_size[0] = 1;
  sintabinv_size[1] = 0;
}

/*
 * Arguments    : const emxArray_creal32_T *x
 *                int n1_unsigned
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void d_FFTImplementationCallback_r2b(const emxArray_creal32_T *x,
                                     int n1_unsigned,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     emxArray_creal32_T *y)
{
  float temp_im;
  float temp_re;
  float twid_im;
  float twid_re;
  int b_i;
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
  y->size[0] = n1_unsigned;
  emxEnsureCapacity_creal32_T(y, iy);
  if (n1_unsigned > x->size[0]) {
    iy = y->size[0];
    y->size[0] = n1_unsigned;
    emxEnsureCapacity_creal32_T(y, iy);
    if (n1_unsigned < 400) {
      for (i = 0; i < n1_unsigned; i++) {
        y->data[i].re = 0.0F;
        y->data[i].im = 0.0F;
      }
    } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(i)

      for (i = 0; i < n1_unsigned; i++) {
        y->data[i].re = 0.0F;
        y->data[i].im = 0.0F;
      }
    }
  }
  iDelta2 = x->size[0];
  if (iDelta2 >= n1_unsigned) {
    iDelta2 = n1_unsigned;
  }
  iheight = n1_unsigned - 2;
  nRowsD2 = n1_unsigned / 2;
  k = nRowsD2 / 2;
  iy = 0;
  ju = 0;
  for (b_i = 0; b_i <= iDelta2 - 2; b_i++) {
    y->data[iy] = x->data[b_i];
    iy = n1_unsigned;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }
    iy = ju;
  }
  y->data[iy] = x->data[iDelta2 - 1];
  for (b_i = 0; b_i <= iheight; b_i += 2) {
    temp_re = y->data[b_i + 1].re;
    temp_im = y->data[b_i + 1].im;
    twid_re = y->data[b_i].re;
    twid_im = y->data[b_i].im;
    y->data[b_i + 1].re = y->data[b_i].re - y->data[b_i + 1].re;
    y->data[b_i + 1].im = y->data[b_i].im - y->data[b_i + 1].im;
    twid_re += temp_re;
    twid_im += temp_im;
    y->data[b_i].re = twid_re;
    y->data[b_i].im = twid_im;
  }
  iy = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (b_i = 0; b_i < iheight; b_i += iDelta2) {
      temp_re_tmp = b_i + iy;
      temp_re = y->data[temp_re_tmp].re;
      temp_im = y->data[temp_re_tmp].im;
      y->data[temp_re_tmp].re = y->data[b_i].re - temp_re;
      y->data[temp_re_tmp].im = y->data[b_i].im - temp_im;
      y->data[b_i].re += temp_re;
      y->data[b_i].im += temp_im;
    }
    ju = 1;
    for (j = k; j < nRowsD2; j += k) {
      twid_re = costab->data[j];
      twid_im = sintab->data[j];
      b_i = ju;
      ihi = ju + iheight;
      while (b_i < ihi) {
        temp_re_tmp = b_i + iy;
        temp_re = twid_re * y->data[temp_re_tmp].re -
                  twid_im * y->data[temp_re_tmp].im;
        temp_im = twid_re * y->data[temp_re_tmp].im +
                  twid_im * y->data[temp_re_tmp].re;
        y->data[temp_re_tmp].re = y->data[b_i].re - temp_re;
        y->data[temp_re_tmp].im = y->data[b_i].im - temp_im;
        y->data[b_i].re += temp_re;
        y->data[b_i].im += temp_im;
        b_i += iDelta2;
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
 * Arguments    : int nRows
 *                emxArray_real32_T *costab
 *                emxArray_real32_T *sintab
 *                int sintabinv_size[2]
 * Return Type  : void
 */
void e_FFTImplementationCallback_gen(int nRows, emxArray_real32_T *costab,
                                     emxArray_real32_T *sintab,
                                     int sintabinv_size[2])
{
  emxArray_real32_T *costab1q;
  float e;
  int i;
  int i1;
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
  if (nd2 + 1 < 400) {
    for (k = 0; k <= nd2; k++) {
      costab1q->data[k + 1] = cosf(e * (float)(k + 1));
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = 0; k <= nd2; k++) {
      costab1q->data[k + 1] = cosf(e * (float)(k + 1));
    }
  }
  i = nd2 + 2;
  i1 = n - 1;
  if ((n - nd2) - 2 < 400) {
    for (k = i; k <= i1; k++) {
      costab1q->data[k] = sinf(e * (float)(n - k));
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = i; k <= i1; k++) {
      costab1q->data[k] = sinf(e * (float)(n - k));
    }
  }
  costab1q->data[n] = 0.0F;
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
  if (costab1q->size[1] - 1 < 400) {
    for (k = 0; k < n; k++) {
      costab->data[k + 1] = costab1q->data[k + 1];
      sintab->data[k + 1] = costab1q->data[(n - k) - 1];
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = 0; k < n; k++) {
      costab->data[k + 1] = costab1q->data[k + 1];
      sintab->data[k + 1] = costab1q->data[(n - k) - 1];
    }
  }
  i = costab1q->size[1];
  if ((nd2 - costab1q->size[1]) + 1 < 400) {
    for (k = i; k <= nd2; k++) {
      costab->data[k] = -costab1q->data[nd2 - k];
      sintab->data[k] = costab1q->data[k - n];
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(k)

    for (k = i; k <= nd2; k++) {
      costab->data[k] = -costab1q->data[nd2 - k];
      sintab->data[k] = costab1q->data[k - n];
    }
  }
  emxFree_real32_T(&costab1q);
  sintabinv_size[0] = 1;
  sintabinv_size[1] = 0;
}

/*
 * Arguments    : const emxArray_creal32_T *x
 *                int n1_unsigned
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void e_FFTImplementationCallback_r2b(const emxArray_creal32_T *x,
                                     int n1_unsigned,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     emxArray_creal32_T *y)
{
  float temp_im;
  float temp_re;
  float twid_im;
  float twid_re;
  int b_i;
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
  y->size[0] = n1_unsigned;
  emxEnsureCapacity_creal32_T(y, iy);
  if (n1_unsigned > x->size[0]) {
    iy = y->size[0];
    y->size[0] = n1_unsigned;
    emxEnsureCapacity_creal32_T(y, iy);
    if (n1_unsigned < 400) {
      for (i = 0; i < n1_unsigned; i++) {
        y->data[i].re = 0.0F;
        y->data[i].im = 0.0F;
      }
    } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(i)

      for (i = 0; i < n1_unsigned; i++) {
        y->data[i].re = 0.0F;
        y->data[i].im = 0.0F;
      }
    }
  }
  iDelta2 = x->size[0];
  if (iDelta2 >= n1_unsigned) {
    iDelta2 = n1_unsigned;
  }
  iheight = n1_unsigned - 2;
  nRowsD2 = n1_unsigned / 2;
  k = nRowsD2 / 2;
  iy = 0;
  ju = 0;
  for (b_i = 0; b_i <= iDelta2 - 2; b_i++) {
    y->data[iy] = x->data[b_i];
    iy = n1_unsigned;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }
    iy = ju;
  }
  y->data[iy] = x->data[iDelta2 - 1];
  for (b_i = 0; b_i <= iheight; b_i += 2) {
    temp_re = y->data[b_i + 1].re;
    temp_im = y->data[b_i + 1].im;
    twid_re = y->data[b_i].re;
    twid_im = y->data[b_i].im;
    y->data[b_i + 1].re = y->data[b_i].re - y->data[b_i + 1].re;
    y->data[b_i + 1].im = y->data[b_i].im - y->data[b_i + 1].im;
    twid_re += temp_re;
    twid_im += temp_im;
    y->data[b_i].re = twid_re;
    y->data[b_i].im = twid_im;
  }
  iy = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (b_i = 0; b_i < iheight; b_i += iDelta2) {
      temp_re_tmp = b_i + iy;
      temp_re = y->data[temp_re_tmp].re;
      temp_im = y->data[temp_re_tmp].im;
      y->data[temp_re_tmp].re = y->data[b_i].re - temp_re;
      y->data[temp_re_tmp].im = y->data[b_i].im - temp_im;
      y->data[b_i].re += temp_re;
      y->data[b_i].im += temp_im;
    }
    ju = 1;
    for (j = k; j < nRowsD2; j += k) {
      twid_re = costab->data[j];
      twid_im = sintab->data[j];
      b_i = ju;
      ihi = ju + iheight;
      while (b_i < ihi) {
        temp_re_tmp = b_i + iy;
        temp_re = twid_re * y->data[temp_re_tmp].re -
                  twid_im * y->data[temp_re_tmp].im;
        temp_im = twid_re * y->data[temp_re_tmp].im +
                  twid_im * y->data[temp_re_tmp].re;
        y->data[temp_re_tmp].re = y->data[b_i].re - temp_re;
        y->data[temp_re_tmp].im = y->data[b_i].im - temp_im;
        y->data[b_i].re += temp_re;
        y->data[b_i].im += temp_im;
        b_i += iDelta2;
      }
      ju++;
    }
    k /= 2;
    iy = iDelta2;
    iDelta2 += iDelta2;
    iheight -= iy;
  }
  twid_re = 1.0F / (float)y->size[0];
  iy = y->size[0];
  if (y->size[0] < 400) {
    for (i = 0; i < iy; i++) {
      y->data[i].re *= twid_re;
      y->data[i].im *= twid_re;
    }
  } else {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(i)

    for (i = 0; i < iy; i++) {
      y->data[i].re *= twid_re;
      y->data[i].im *= twid_re;
    }
  }
}

/*
 * File trailer for FFTImplementationCallback.c
 *
 * [EOF]
 */
