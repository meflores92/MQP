/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: find.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "find.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const boolean_T x_data[]
 *                int x_size
 *                int i_data[]
 *                int *i_size
 * Return Type  : void
 */
void b_eml_find(const boolean_T x_data[], int x_size, int i_data[], int *i_size)
{
  int idx;
  int ii;
  boolean_T exitg1;
  idx = 0;
  *i_size = x_size;
  ii = 0;
  exitg1 = false;
  while ((!exitg1) && (ii <= x_size - 1)) {
    if (x_data[ii]) {
      idx++;
      i_data[idx - 1] = ii + 1;
      if (idx >= x_size) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }
  if (x_size == 1) {
    if (idx == 0) {
      *i_size = 0;
    }
  } else if (1 > idx) {
    *i_size = 0;
  } else {
    *i_size = idx;
  }
}

/*
 * Arguments    : const boolean_T x_data[]
 *                const int x_size[2]
 *                int i_data[]
 *                int i_size[2]
 * Return Type  : void
 */
void c_eml_find(const boolean_T x_data[], const int x_size[2], int i_data[],
                int i_size[2])
{
  int idx;
  int ii;
  int nx;
  boolean_T exitg1;
  nx = x_size[1];
  idx = 0;
  i_size[0] = 1;
  i_size[1] = x_size[1];
  ii = 0;
  exitg1 = false;
  while ((!exitg1) && (ii <= nx - 1)) {
    if (x_data[ii]) {
      idx++;
      i_data[idx - 1] = ii + 1;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }
  if (x_size[1] == 1) {
    if (idx == 0) {
      i_size[0] = 1;
      i_size[1] = 0;
    }
  } else if (1 > idx) {
    i_size[1] = 0;
  } else {
    i_size[1] = idx;
  }
}

/*
 * Arguments    : const boolean_T x[72]
 *                int i_data[]
 *                int *i_size
 * Return Type  : void
 */
void eml_find(const boolean_T x[72], int i_data[], int *i_size)
{
  int idx;
  int ii;
  boolean_T exitg1;
  idx = 0;
  ii = 0;
  exitg1 = false;
  while ((!exitg1) && (ii < 72)) {
    if (x[ii]) {
      idx++;
      i_data[idx - 1] = ii + 1;
      if (idx >= 72) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }
  if (1 > idx) {
    *i_size = 0;
  } else {
    *i_size = idx;
  }
}

/*
 * File trailer for find.c
 *
 * [EOF]
 */
