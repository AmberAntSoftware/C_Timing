#ifndef C_TIMING_DELAY_H_INCLUDED
#define C_TIMING_DELAY_H_INCLUDED

#include <string.h>
#include "c_timing.h"

typedef struct TIM_Delay{
    TIM_Timestamp last;
    unsigned long long int targetNanoseconds;
}TIM_Delay;

TIM_Delay* TIM_newDelayMillis(unsigned long long int targetMilliseconds);
TIM_Delay* TIM_newDelayMicro(unsigned long long int targetMicroseconds);
TIM_Delay* TIM_newDelayNano(unsigned long long int targetNanoseconds);
TIM_Delay* TIM_newDelayFPS(unsigned long long int targetFramesPerSecond);

TIM_ERROR_ENUM TIM_initDelayNano(TIM_Delay *delay, unsigned long long int targetNanoseconds);
TIM_ERROR_ENUM TIM_initDelayMillis(TIM_Delay *delay, unsigned long long int targetMilliseconds);
TIM_ERROR_ENUM TIM_initDelayMicro(TIM_Delay *delay, unsigned long long int targetMilliseconds);
TIM_ERROR_ENUM TIM_initDelayFPS(TIM_Delay *delay, unsigned long long int targetFramesPerSecond);

void TIM_freeDelay(TIM_Delay *delay);
void TIM_freeDelayData(TIM_Delay *delay);

TIM_ERROR_ENUM TIM_DelaySleep(TIM_Delay *delay);

#endif /* C_TIMING_DELAY_H_INCLUDED */
