#ifndef C_TIMING_DELAY_H_INCLUDED
#define C_TIMING_DELAY_H_INCLUDED

#include <string.h>
#include "c_timing.h"

/**************************************************************************
Variable wait time calculator to produce a constant delay between waits
with variable computation time between calls.
**************************************************************************/

typedef struct TIM_Delay{
    TIM_Timestamp last;
    unsigned long long int targetNanoseconds;
    unsigned long long int lastDiff;
    unsigned long long int totalDiff;
}TIM_Delay;

/*************************************
Initialization
*************************************/

TIM_Delay* TIM_newDelayMillis(unsigned long long int targetMilliseconds);
TIM_Delay* TIM_newDelayMicro(unsigned long long int targetMicroseconds);
TIM_Delay* TIM_newDelayNano(unsigned long long int targetNanoseconds);
TIM_Delay* TIM_newDelayFPS(unsigned long long int targetFramesPerSecond);

TIM_ERROR_ENUM TIM_initDelayNano(TIM_Delay *delay, unsigned long long int targetNanoseconds);
TIM_ERROR_ENUM TIM_initDelayMillis(TIM_Delay *delay, unsigned long long int targetMilliseconds);
TIM_ERROR_ENUM TIM_initDelayMicro(TIM_Delay *delay, unsigned long long int targetMilliseconds);
TIM_ERROR_ENUM TIM_initDelayFPS(TIM_Delay *delay, unsigned long long int targetFramesPerSecond);

/*************************************
Deallocation
*************************************/

void TIM_freeDelay(TIM_Delay *delay);
void TIM_freeDelayData(TIM_Delay *delay);

/*************************************
Operations
*************************************/

TIM_ERROR_ENUM TIM_DelaySleep(TIM_Delay *delay);

unsigned long long int TIM_DelayDroppedFrames(TIM_Delay *delay);

#endif /* C_TIMING_DELAY_H_INCLUDED */
