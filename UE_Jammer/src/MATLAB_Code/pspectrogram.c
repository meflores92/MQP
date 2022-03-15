/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: pspectrogram.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "pspectrogram.h"
#include "computepsd.h"
#include "operation_data.h"
#include "operation_emxutil.h"
#include "operation_internal_types.h"
#include "operation_types.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : emxArray_creal32_T *y
 *                double f_data[]
 *                const int *f_size
 *                emxArray_real32_T *Pxx
 * Return Type  : void
 */
void centerOutputs(emxArray_creal32_T *y, double f_data[], const int *f_size,
                   emxArray_real32_T *Pxx)
{
  creal32_T buffer_data[768];
  double b_y;
  float b_buffer_data[768];
  float xtmp_im;
  float xtmp_re;
  int b_i;
  int b_i1;
  int i;
  int i1;
  int i2;
  int ib;
  int j;
  int k;
  int ns;
  int stride;
  int vlend2;
  int vspread;
  boolean_T shiftright;
  if (y->size[0] == 0) {
    b_y = 0.0;
  } else {
    b_y = fmod(y->size[0], 2.0);
  }
  if (b_y == 0.0) {
    b_y = (double)y->size[0] / 2.0 - 1.0;
    ns = (int)b_y - 1;
    shiftright = true;
    if ((int)b_y > (y->size[0] >> 1)) {
      ns = (y->size[0] - (int)b_y) - 1;
      shiftright = false;
    }
    stride = y->size[0];
    if (stride <= 600) {
      stride = 600;
    }
    stride = (short)floor((double)stride / 2.0);
    if (0 <= stride - 1) {
      memset(&buffer_data[0], 0, stride * sizeof(creal32_T));
    }
    b_i = y->size[0];
    ib = y->size[0];
    for (i = 0; i < 600; i++) {
      vlend2 = i * b_i;
      if (shiftright) {
        for (k = 0; k <= ns; k++) {
          buffer_data[k] = y->data[(((vlend2 + k) + b_i) - ns) - 1];
        }
        b_i1 = ns + 2;
        for (k = ib; k >= b_i1; k--) {
          stride = vlend2 + k;
          y->data[stride - 1] = y->data[(stride - ns) - 2];
        }
        for (k = 0; k <= ns; k++) {
          y->data[vlend2 + k] = buffer_data[k];
        }
      } else {
        for (k = 0; k <= ns; k++) {
          buffer_data[k] = y->data[vlend2 + k];
        }
        b_i1 = b_i - ns;
        for (k = 0; k <= b_i1 - 2; k++) {
          stride = vlend2 + k;
          y->data[stride] = y->data[(stride + ns) + 1];
        }
        for (k = 0; k <= ns; k++) {
          y->data[(((vlend2 + k) + b_i) - ns) - 1] = buffer_data[k];
        }
      }
    }
  } else {
    vlend2 = y->size[0] / 2 - 1;
    vspread = y->size[0];
    if ((vlend2 + 1) << 1 == y->size[0]) {
      i2 = 1;
      for (i = 0; i < 600; i++) {
        i1 = i2;
        i2 += vspread;
        ib = i1 + vlend2;
        for (k = 0; k <= vlend2; k++) {
          stride = (i1 + k) - 1;
          xtmp_re = y->data[stride].re;
          xtmp_im = y->data[stride].im;
          b_i = ib + k;
          y->data[stride] = y->data[b_i];
          y->data[b_i].re = xtmp_re;
          y->data[b_i].im = xtmp_im;
        }
      }
    } else {
      i2 = 1;
      for (i = 0; i < 600; i++) {
        i1 = i2;
        i2 += vspread;
        ib = i1 + vlend2;
        xtmp_re = y->data[ib].re;
        xtmp_im = y->data[ib].im;
        for (k = 0; k <= vlend2; k++) {
          b_i = ib + k;
          b_i1 = (i1 + k) - 1;
          y->data[b_i] = y->data[b_i1];
          y->data[b_i1] = y->data[b_i + 1];
        }
        ib = (ib + vlend2) + 1;
        y->data[ib].re = xtmp_re;
        y->data[ib].im = xtmp_im;
      }
    }
  }
  if (*f_size == 0) {
    b_y = 0.0;
  } else {
    b_y = fmod(*f_size, 2.0);
  }
  if (b_y == 0.0) {
    b_y = f_data[(int)((double)*f_size / 2.0) - 1];
    for (b_i = 0; b_i < *f_size; b_i++) {
      f_data[b_i] -= b_y;
    }
  } else {
    b_y = f_data[(int)(((double)*f_size + 1.0) / 2.0) - 1];
    for (b_i = 0; b_i < *f_size; b_i++) {
      f_data[b_i] -= b_y;
    }
  }
  if (Pxx->size[0] == 0) {
    b_y = 0.0;
  } else {
    b_y = fmod(Pxx->size[0], 2.0);
  }
  if (b_y == 0.0) {
    b_y = (double)Pxx->size[0] / 2.0 - 1.0;
    vlend2 = (Pxx->size[0] == 1);
    if (b_y < 0.0) {
      ns = (int)-b_y;
      shiftright = false;
    } else {
      ns = (int)b_y;
      shiftright = true;
    }
    if (ns > Pxx->size[vlend2]) {
      ns -= ns / Pxx->size[vlend2] * Pxx->size[vlend2];
    }
    if (ns > (Pxx->size[vlend2] >> 1)) {
      ns = Pxx->size[vlend2] - ns;
      shiftright = !shiftright;
    }
    stride = Pxx->size[0];
    if (stride <= 600) {
      stride = 600;
    }
    stride = (short)floor((double)stride / 2.0);
    if (0 <= stride - 1) {
      memset(&b_buffer_data[0], 0, stride * sizeof(float));
    }
    b_i = Pxx->size[vlend2];
    ib = Pxx->size[vlend2];
    stride = 1;
    for (k = 0; k < vlend2; k++) {
      stride *= Pxx->size[0];
    }
    vspread = 1;
    b_i1 = vlend2 + 2;
    for (k = b_i1; k < 3; k++) {
      vspread *= 600;
    }
    i2 = stride * Pxx->size[vlend2];
    if ((Pxx->size[vlend2] > 1) && (ns > 0)) {
      for (i = 0; i < vspread; i++) {
        vlend2 = i * i2;
        for (j = 0; j < stride; j++) {
          i1 = vlend2 + j;
          if (shiftright) {
            for (k = 0; k < ns; k++) {
              b_buffer_data[k] = Pxx->data[i1 + ((k + b_i) - ns) * stride];
            }
            b_i1 = ns + 1;
            for (k = ib; k >= b_i1; k--) {
              Pxx->data[i1 + (k - 1) * stride] =
                  Pxx->data[i1 + ((k - ns) - 1) * stride];
            }
            for (k = 0; k < ns; k++) {
              Pxx->data[i1 + k * stride] = b_buffer_data[k];
            }
          } else {
            for (k = 0; k < ns; k++) {
              b_buffer_data[k] = Pxx->data[i1 + k * stride];
            }
            b_i1 = (b_i - ns) - 1;
            for (k = 0; k <= b_i1; k++) {
              Pxx->data[i1 + k * stride] = Pxx->data[i1 + (k + ns) * stride];
            }
            for (k = 0; k < ns; k++) {
              Pxx->data[i1 + ((k + b_i) - ns) * stride] = b_buffer_data[k];
            }
          }
        }
      }
    }
  } else if (Pxx->size[0] > 1) {
    vlend2 = Pxx->size[0] / 2 - 1;
    vspread = Pxx->size[0];
    if ((vlend2 + 1) << 1 == Pxx->size[0]) {
      i2 = 1;
      for (i = 0; i < 600; i++) {
        i1 = i2;
        i2 += vspread;
        ib = i1 + vlend2;
        for (k = 0; k <= vlend2; k++) {
          stride = (i1 + k) - 1;
          xtmp_re = Pxx->data[stride];
          b_i = ib + k;
          Pxx->data[stride] = Pxx->data[b_i];
          Pxx->data[b_i] = xtmp_re;
        }
      }
    } else {
      i2 = 1;
      for (i = 0; i < 600; i++) {
        i1 = i2;
        i2 += vspread;
        ib = i1 + vlend2;
        xtmp_re = Pxx->data[ib];
        for (k = 0; k <= vlend2; k++) {
          b_i = ib + k;
          b_i1 = (i1 + k) - 1;
          Pxx->data[b_i] = Pxx->data[b_i1];
          Pxx->data[b_i1] = Pxx->data[b_i + 1];
        }
        ib = (ib + vlend2) + 1;
        Pxx->data[ib] = xtmp_re;
      }
    }
  }
}

/*
 * Arguments    : const emxArray_creal32_T *y
 *                double f_data[]
 *                int *f_size
 *                const b_struct_T options
 *                emxArray_real32_T *Pxx
 * Return Type  : void
 */
void compute_PSD(const emxArray_creal32_T *y, double f_data[], int *f_size,
                 const b_struct_T options, emxArray_real32_T *Pxx)
{
  emxArray_int32_T *r;
  emxArray_real32_T *b_y;
  double b_f_data[1536];
  int end;
  int i;
  int loop_ub;
  emxInit_real32_T(&b_y, 2);
  i = b_y->size[0] * b_y->size[1];
  b_y->size[0] = y->size[0];
  b_y->size[1] = 600;
  emxEnsureCapacity_real32_T(b_y, i);
  loop_ub = y->size[0] * 600;
  for (i = 0; i < loop_ub; i++) {
    b_y->data[i] =
        (y->data[i].re * y->data[i].re - y->data[i].im * -y->data[i].im) /
        1536.0F;
  }
  loop_ub = *f_size - 1;
  if (0 <= loop_ub) {
    memcpy(&b_f_data[0], &f_data[0], (loop_ub + 1) * sizeof(double));
  }
  computepsd(b_y, b_f_data, *f_size, options.range, Pxx, f_data, f_size);
  end = Pxx->size[0] * 600 - 1;
  loop_ub = 0;
  emxFree_real32_T(&b_y);
  for (i = 0; i <= end; i++) {
    if (Pxx->data[i] < 1.0E-13) {
      loop_ub++;
    }
  }
  emxInit_int32_T(&r, 1);
  i = r->size[0];
  r->size[0] = loop_ub;
  emxEnsureCapacity_int32_T(r, i);
  loop_ub = 0;
  for (i = 0; i <= end; i++) {
    if (Pxx->data[i] < 1.0E-13) {
      r->data[loop_ub] = i + 1;
      loop_ub++;
    }
  }
  loop_ub = r->size[0] - 1;
  for (i = 0; i <= loop_ub; i++) {
    Pxx->data[r->data[i] - 1] = 0.0F;
  }
  emxFree_int32_T(&r);
}

/*
 * Arguments    : const emxArray_creal32_T *y
 *                double f_data[]
 *                int *f_size
 *                const b_struct_T options
 *                emxArray_creal32_T *yout
 * Return Type  : void
 */
void formatSpectrogram(const emxArray_creal32_T *y, double f_data[],
                       int *f_size, const b_struct_T options,
                       emxArray_creal32_T *yout)
{
  static const char b_cv[8] = {'o', 'n', 'e', 's', 'i', 'd', 'e', 'd'};
  int w1_data[1536];
  int exitg1;
  int i;
  int kstr;
  boolean_T b_bool;
  b_bool = false;
  kstr = 0;
  do {
    exitg1 = 0;
    if (kstr < 8) {
      if (cv[(unsigned char)options.range[kstr]] != cv[(int)b_cv[kstr]]) {
        exitg1 = 1;
      } else {
        kstr++;
      }
    } else {
      b_bool = true;
      exitg1 = 1;
    }
  } while (exitg1 == 0);
  if (b_bool) {
    for (kstr = 0; kstr < 1536; kstr++) {
      w1_data[kstr] = 30000 * kstr;
    }
    w1_data[768] = 23040000;
    w1_data[1535] = 46050000;
    *f_size = 769;
    for (kstr = 0; kstr < 769; kstr++) {
      f_data[kstr] = w1_data[kstr];
    }
    kstr = yout->size[0] * yout->size[1];
    yout->size[0] = 769;
    yout->size[1] = 600;
    emxEnsureCapacity_creal32_T(yout, kstr);
    for (kstr = 0; kstr < 600; kstr++) {
      for (i = 0; i < 769; i++) {
        yout->data[i + yout->size[0] * kstr] = y->data[i + 1536 * kstr];
      }
    }
  } else {
    kstr = yout->size[0] * yout->size[1];
    yout->size[0] = 1536;
    yout->size[1] = 600;
    emxEnsureCapacity_creal32_T(yout, kstr);
    for (kstr = 0; kstr < 921600; kstr++) {
      yout->data[kstr] = y->data[kstr];
    }
  }
}

/*
 * File trailer for pspectrogram.c
 *
 * [EOF]
 */
