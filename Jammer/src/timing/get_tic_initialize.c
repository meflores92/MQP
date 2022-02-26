/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: get_tic_initialize.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 18-Jan-2022 10:16:38
 */

/* Include Files */
#include "get_tic_initialize.h"
#include "CoderTimeAPI.h"
#include "get_tic_data.h"
#include "timeKeeper.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void get_tic_initialize(void)
{
  savedTime_not_empty_init();
  freq_not_empty_init();
  isInitialized_get_tic = true;
}

/*
 * File trailer for get_tic_initialize.c
 *
 * [EOF]
 */
