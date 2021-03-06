#include "c_timing_delay.h"


/*************************************
Internal Computations
*************************************/

#ifdef _WIN32
static unsigned long long int X_TIM_CALC_DELAY = 1;
#else
static unsigned long long int X_TIM_CALC_DELAY = 0;
#endif // _WIN32

static void X_TIM_X_initCalcDelayNano(){

    const unsigned int GRANULARITY = 100000;
    const unsigned int ROUNDS = 10;
    TIM_Timestamp start, end;
    unsigned long long int accu = 0;

    int i;
    for (i = 0; i < ROUNDS; i++){
        if(TIM_initTimestamp(&start) == TIM_ERROR_FAILURE){
            X_TIM_CALC_DELAY = 1;
            return;
        }
        if(TIM_sleepNano(GRANULARITY) == TIM_ERROR_FAILURE){
            X_TIM_CALC_DELAY = 1;
            return;
        }
        if(TIM_initTimestamp(&end) == TIM_ERROR_FAILURE){
            X_TIM_CALC_DELAY = 1;
            return;
        }
        accu += TIM_TimestampDiffNano(&start, &end) - GRANULARITY;
        TIM_freeTimestampData(&start);
        TIM_freeTimestampData(&end);
    }

    accu/=ROUNDS;
    X_TIM_CALC_DELAY = accu;

}

static void X_TIM_initCalcDelayNano(){
    if(X_TIM_CALC_DELAY == 0){
        X_TIM_X_initCalcDelayNano();
    }
}

/*************************************
Initialization
*************************************/

TIM_Delay* TIM_newDelayMillis(unsigned long long int targetMilliseconds){
    return TIM_newDelayNano(targetMilliseconds*TIM_X_MILLISECONDS_IN_NANOSECOND);
}

TIM_Delay* TIM_newDelayMicro(unsigned long long int targetMicroseconds){
    return TIM_newDelayNano(targetMicroseconds*TIM_X_MICROSECONDS_IN_NANOSECOND);
}

TIM_Delay* TIM_newDelayNano(unsigned long long int targetNanoseconds){

    TIM_Delay *delay = malloc(sizeof(TIM_Delay));
    if(delay == NULL){
        return NULL;
    }

    if(TIM_initDelayNano(delay, targetNanoseconds) == TIM_ERROR_FAILURE){
        free(delay);
        return NULL;
    }

    return delay;
}

TIM_Delay* TIM_newDelayFPS(unsigned long long int targetFramesPerSecond){
    return TIM_newDelayNano(TIM_X_NANOSECONDS_IN_SECOND/targetFramesPerSecond);
}

TIM_ERROR_ENUM TIM_initDelayMillis(TIM_Delay *delay, unsigned long long int targetMilliseconds){
    return TIM_initDelayNano(delay, targetMilliseconds*TIM_X_MILLISECONDS_IN_NANOSECOND);
}

TIM_ERROR_ENUM TIM_initDelayMicro(TIM_Delay *delay, unsigned long long int targetMilliseconds){
    return TIM_initDelayNano(delay, targetMilliseconds*TIM_X_MICROSECONDS_IN_NANOSECOND);
}

TIM_ERROR_ENUM TIM_initDelayNano(TIM_Delay *delay, unsigned long long int targetNanoseconds){

    if(TIM_initTimestamp(&delay->last) == TIM_ERROR_FAILURE){
        return TIM_ERROR_FAILURE;
    }

    delay->targetNanoseconds = targetNanoseconds;
    delay->lastDiff = 0;
    delay->totalDiff = 0;

    return TIM_ERROR_SUCCESS;
}

TIM_ERROR_ENUM TIM_initDelayFPS(TIM_Delay *delay, unsigned long long int targetFramesPerSecond){
    return TIM_initDelayNano(delay, TIM_X_NANOSECONDS_IN_SECOND/targetFramesPerSecond);
}

/*************************************
Deallocation
*************************************/

void TIM_freeDelay(TIM_Delay *delay){
    TIM_freeDelayData(delay);
    free(delay);
}

void TIM_freeDelayData(TIM_Delay *delay){
    TIM_freeTimestampData(&delay->last);
    return;
}

/*************************************
Operations
*************************************/
#include <stdio.h>
TIM_ERROR_ENUM TIM_DelaySleep(TIM_Delay *delay){

    TIM_ERROR_ENUM action = TIM_ERROR_SUCCESS;

    X_TIM_initCalcDelayNano();

    TIM_Timestamp cur;
    if(TIM_initTimestamp(&cur) == TIM_ERROR_FAILURE){
        return TIM_ERROR_FAILURE;
    }

    unsigned long long int diff = TIM_TimestampDiffNano(&delay->last, &cur);

    if(diff >= delay->targetNanoseconds){

        delay->lastDiff += diff;
        action = TIM_ERROR_NO_OP;
    }else{

        //printf("LastDif %llu\n", delay->lastDiff);
        if(delay->lastDiff > 0){
            TIM_sleepNano(
                delay->targetNanoseconds - ((delay->lastDiff) %delay->targetNanoseconds)
            );
            delay->totalDiff += delay->lastDiff - delay->targetNanoseconds;
            //puts("delay lag");
        }else{
            TIM_sleepNano(delay->targetNanoseconds - diff);
        }

        //printf("Raw Last: %llu\n", TIM_TimestampInNano(&delay->last));
        delay->lastDiff = 0;
    }

    TIM_freeTimestampData(&cur);
    TIM_freeTimestampData(&delay->last);

    if(TIM_initTimestamp(&delay->last) == TIM_ERROR_FAILURE){
        return TIM_ERROR_FAILURE;
    }

    return action;
}

unsigned long long int TIM_DelayDroppedFrames(TIM_Delay *delay){
    return delay->totalDiff/delay->targetNanoseconds;
}
