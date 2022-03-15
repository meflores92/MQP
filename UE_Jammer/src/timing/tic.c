/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: tic.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 18-Jan-2022 10:16:38
 */

/* Include Files */
#include "tic.h"
#include "get_tic_data.h"
#include "timeKeeper.h"
#include "coder_posix_time.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void tic(void)
{
  coderTimespec b_timespec;
  if (!freq_not_empty) {
    freq_not_empty = true;
    coderInitTimeFunctions(&freq);
  }
  coderTimeClockGettimeMonotonic(&b_timespec, freq);
  timeKeeper(b_timespec.tv_sec, b_timespec.tv_nsec);
}

/*
 * File trailer for tic.c
 *
 * [EOF]
 */
