/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: psdfreqvec.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "psdfreqvec.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : double w_data[]
 *                int *w_size
 * Return Type  : void
 */
void psdfreqvec(double w_data[], int *w_size)
{
  int w1_data[1536];
  int i;
  for (i = 0; i < 1536; i++) {
    w1_data[i] = 30000 * i;
  }
  w1_data[768] = 23040000;
  w1_data[1535] = 46050000;
  *w_size = 1536;
  for (i = 0; i < 1536; i++) {
    w_data[i] = w1_data[i];
  }
}

/*
 * File trailer for psdfreqvec.c
 *
 * [EOF]
 */
