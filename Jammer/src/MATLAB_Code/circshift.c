/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: circshift.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "circshift.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : double a_data[]
 *                const int *a_size
 *                double p
 * Return Type  : void
 */
void circshift(double a_data[], const int *a_size, double p)
{
  double buffer_data[2214];
  int dim;
  int k;
  int loop_ub;
  int ns;
  int nv;
  int stride;
  boolean_T shiftright;
  dim = 2;
  if (*a_size != 1) {
    dim = 1;
  }
  if ((*a_size != 0) && (*a_size != 1)) {
    if (p < 0.0) {
      ns = (int)-p;
      shiftright = false;
    } else {
      ns = (int)p;
      shiftright = true;
    }
    if (dim <= 1) {
      loop_ub = *a_size;
    } else {
      loop_ub = 1;
    }
    if (ns > loop_ub) {
      ns -= ns / loop_ub * loop_ub;
    }
    if (ns > (loop_ub >> 1)) {
      ns = loop_ub - ns;
      shiftright = !shiftright;
    }
    loop_ub = (short)floor((double)*a_size / 2.0);
    if (0 <= loop_ub - 1) {
      memset(&buffer_data[0], 0, loop_ub * sizeof(double));
    }
    if (dim <= 1) {
      nv = *a_size;
    } else {
      nv = 1;
    }
    stride = 1;
    for (k = 0; k <= dim - 2; k++) {
      stride *= *a_size;
    }
    if ((nv > 1) && (ns > 0)) {
      for (dim = 0; dim < stride; dim++) {
        if (shiftright) {
          for (k = 0; k < ns; k++) {
            buffer_data[k] = a_data[dim + ((k + nv) - ns) * stride];
          }
          loop_ub = ns + 1;
          for (k = nv; k >= loop_ub; k--) {
            a_data[dim + (k - 1) * stride] =
                a_data[dim + ((k - ns) - 1) * stride];
          }
          for (k = 0; k < ns; k++) {
            a_data[dim + k * stride] = buffer_data[k];
          }
        } else {
          for (k = 0; k < ns; k++) {
            buffer_data[k] = a_data[dim + k * stride];
          }
          loop_ub = nv - ns;
          for (k = 0; k < loop_ub; k++) {
            a_data[dim + k * stride] = a_data[dim + (k + ns) * stride];
          }
          for (k = 0; k < ns; k++) {
            a_data[dim + ((k + nv) - ns) * stride] = buffer_data[k];
          }
        }
      }
    }
  }
}

/*
 * File trailer for circshift.c
 *
 * [EOF]
 */
