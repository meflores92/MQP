/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: get_tic.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 18-Jan-2022 10:16:38
 */

/* Include Files */
#include "get_tic.h"
#include "get_tic_data.h"
#include "get_tic_initialize.h"
#include "tic.h"
#include "toc.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void get_tic(void)
{
  if (!isInitialized_get_tic) {
    get_tic_initialize();
  }
  tic();
  toc();
}

/*
 * File trailer for get_tic.c
 *
 * [EOF]
 */
