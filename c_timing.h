#ifndef C_TIMING_H_INCLUDED
#define C_TIMING_H_INCLUDED

#include <stdlib.h>

#ifdef _WIN32

    #include <windows.h>

#elif _POSIX_C_SOURCE >= 199309L

    #include <sys/time.h>
    #include <time.h>

#else

    #include <unistd.h>

#endif

#define TIM_X_MICROSECONDS_PER_MILLISECOND 1000LLU

#define TIM_X_NANOSECONDS_IN_SECOND 1000000000LLU
#define TIM_X_MICROSECONDS_IN_SECOND 1000000LLU
#define TIM_X_MILLISECONDS_IN_SECOND 1000LLU

#define TIM_X_MICROSECONDS_IN_NANOSECOND 1000LLU
#define TIM_X_MILLISECONDS_IN_NANOSECOND 1000000LLU
#define TIM_X_SECONDS_IN_NANOSECOND 1000000000LLU

typedef enum TIM_ERROR_ENUM{
    TIM_ERROR_NO_OP = 0,
    TIM_ERROR_SUCCESS,
    TIM_ERROR_FAILURE
}TIM_ERROR_ENUM;

/*************************************
Gobals
*************************************/

TIM_ERROR_ENUM TIM_sleepMillis(unsigned int milliseconds);
TIM_ERROR_ENUM TIM_sleepMicro(unsigned int milliseconds);
TIM_ERROR_ENUM TIM_sleepNano(unsigned int nanoseconds);
TIM_ERROR_ENUM TIM_sleepFPS(unsigned int framesPerSecond);


/**************************************************************************
Holds a precise point in time with respect to OS storage and call requirements
Not intended to use for date and time differences
**************************************************************************/

typedef struct TIM_Timestamp{
#ifdef _WIN32

    LARGE_INTEGER mark;
    DWORD time;

#elif _POSIX_C_SOURCE >= 199309L

    struct timespec mark;

#else

    struct timeval mark;

#endif
}TIM_Timestamp;

/*************************************
Initialization
*************************************/

TIM_Timestamp* TIM_newTimestamp();
TIM_ERROR_ENUM TIM_initTimestamp(TIM_Timestamp *timestamp);

/*************************************
Deallocation
*************************************/

void TIM_freeTimestamp(TIM_Timestamp *timestamp);
void TIM_freeTimestampData(TIM_Timestamp *timestamp);

/*************************************
Operations
*************************************/

unsigned long long int TIM_TimestampDiff(TIM_Timestamp *start, TIM_Timestamp *end, unsigned int unitsPerSecond);
unsigned long long int TIM_TimestampDiffMillis(TIM_Timestamp *start, TIM_Timestamp *end);
unsigned long long int TIM_TimestampDiffMicro(TIM_Timestamp *start, TIM_Timestamp *end);
unsigned long long int TIM_TimestampDiffNano(TIM_Timestamp *start, TIM_Timestamp *end);

unsigned long long int TIM_TimestampInUnits(TIM_Timestamp *mark, unsigned int unitsPerSecond);
unsigned long long int TIM_TimestampInMillis(TIM_Timestamp *mark);
unsigned long long int TIM_TimestampInMicro(TIM_Timestamp *mark);
unsigned long long int TIM_TimestampInNano(TIM_Timestamp *mark);

#include "c_timing_delay.h"


#endif /* C_TIMING_H_INCLUDED */
