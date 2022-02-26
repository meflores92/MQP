/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: timeKeeper.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 18-Jan-2022 10:16:38
 */

#ifndef TIMEKEEPER_H
#define TIMEKEEPER_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_timeKeeper(double *outTime_tv_sec, double *outTime_tv_nsec);

void savedTime_not_empty_init(void);

void timeKeeper(double newTime_tv_sec, double newTime_tv_nsec);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for timeKeeper.h
 *
 * [EOF]
 */
