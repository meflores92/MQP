/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: strcmp.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "strcmp.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const char a[8]
 * Return Type  : boolean_T
 */
boolean_T b_strcmp(const char a[8])
{
  static const char b[8] = {'o', 'n', 'e', 's', 'i', 'd', 'e', 'd'};
  int ret;
  ret = memcmp(&a[0], &b[0], 8);
  return ret == 0;
}

/*
 * File trailer for strcmp.c
 *
 * [EOF]
 */
