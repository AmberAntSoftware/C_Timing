#include "c_timing.h"

#ifdef _WIN32
    static LARGE_INTEGER X_TIM_FREQUENCY = {0,0,0,0,0};
    static void X_TIM_initFrequency(){
        if(X_TIM_FREQUENCY.QuadPart == 0){
            if(QueryPerformanceFrequency(&X_TIM_FREQUENCY) == 0){
                X_TIM_FREQUENCY.QuadPart = 1;
            }
        }
    }
#endif

TIM_Timestamp* TIM_newTimestamp(){
    TIM_Timestamp *timestamp = malloc(sizeof(TIM_Timestamp));
    if(timestamp == NULL){
        return NULL;
    }
    if(TIM_initTimestamp(timestamp) == TIM_ERROR_FAILURE){
        free(timestamp);
        return NULL;
    }
    return timestamp;
}

TIM_ERROR_ENUM TIM_initTimestamp(TIM_Timestamp *timestamp){
    /**TODO include windows support**/
#ifdef _WIN32
    timestamp->time = timeGetTime();
    if(QueryPerformanceCounter(&timestamp->mark) == 0){
        return TIM_ERROR_FAILURE;
    }
#else

    #if _POSIX_C_SOURCE >= 199309L
        if(clock_gettime(CLOCK_REALTIME_ALARM, &timestamp->mark) != 0){
            return TIM_ERROR_FAILURE;
        }
    #else
        if(gettimeofday(&timestamp->mark, NULL) !=0){
            return TIM_ERROR_FAILURE;
        }
    #endif
#endif
    return TIM_ERROR_SUCCESS;
}

void TIM_freeTimestamp(TIM_Timestamp *timestamp){
    TIM_freeTimestampData(timestamp);
    free(timestamp);
}

void TIM_freeTimestampData(TIM_Timestamp *timestamp){
#ifdef _WIN32
#else
#if _POSIX_C_SOURCE >= 199309L
#else
#endif
#endif
    return;
}




unsigned long long int TIM_TimestampDiff(TIM_Timestamp *start, TIM_Timestamp *end, unsigned int unitsPerSecond){
#ifdef _WIN32

    //*
    X_TIM_initFrequency();
    unsigned long long int diff = end->mark.QuadPart;
    diff -= start->mark.QuadPart;
    diff *= TIM_X_NANOSECONDS_IN_SECOND;
    diff /= (TIM_X_NANOSECONDS_IN_SECOND/unitsPerSecond);
    diff /= X_TIM_FREQUENCY.QuadPart;

    /*/

    X_TIM_initFrequency();

    unsigned long long int diffMillis;
    unsigned long long int diffNano;
    unsigned long long int diff;

    diffMillis = end->time - start->time;
    if(end->mark.QuadPart < start->mark.QuadPart){
        diffNano = start->mark.QuadPart - end->mark.QuadPart;
    }else{
        diffNano = end->mark.QuadPart - start->mark.QuadPart;
    }
    diffNano %= TIM_X_MILLISECONDS_IN_NANOSECOND;
    diffMillis*=TIM_X_MICROSECONDS_IN_SECOND;
    diffMillis+=diffNano;
    diff = diffMillis;
    diff *= TIM_X_NANOSECONDS_IN_SECOND;
    diff /= (TIM_X_NANOSECONDS_IN_SECOND/unitsPerSecond);
    diff /= X_TIM_FREQUENCY.QuadPart;

    diff = (end->time - start->time);//*1000000  / (TIM_X_NANOSECONDS_IN_SECOND/unitsPerSecond);

    //*/

#else

    #if _POSIX_C_SOURCE >= 199309L
        unsigned long long int diff = end->mark.tv_nsec;
        diff -= start->mark.tv_nsec;
        unsigned long long int tmp = end->mark.tv_sec;
        tmp -= start->mark.tv_sec;
        tmp *= TIM_X_NANOSECONDS_IN_SECOND;
        diff += tmp;
        diff /= (TIM_X_NANOSECONDS_IN_SECOND/unitsPerSecond);
    #else
        unsigned long long int diff = end->mark.tv_usec;
        diff -= start->mark.tv_usec;
        diff*=1000;
        unsigned long long int tmp = end->mark.tv_sec;
        tmp -= start->mark.tv_sec;
        tmp *= 1000000000;
        diff += tmp;
        diff /= (1000000000/unitsPerSecond);
    #endif

#endif

    return diff;
}

unsigned long long int TIM_TimestampDiffMillis(TIM_Timestamp *start, TIM_Timestamp *end){
    return TIM_TimestampDiff(start, end, TIM_X_MILLISECONDS_IN_SECOND);
}

unsigned long long int TIM_TimestampDiffMicro(TIM_Timestamp *start, TIM_Timestamp *end){
    return TIM_TimestampDiff(start, end, TIM_X_MICROSECONDS_IN_SECOND);
}

unsigned long long int TIM_TimestampDiffNano(TIM_Timestamp *start, TIM_Timestamp *end){
    return TIM_TimestampDiff(start, end, TIM_X_NANOSECONDS_IN_SECOND);
}



TIM_ERROR_ENUM TIM_sleepMillis(unsigned int milliseconds) {
    return TIM_sleepNano(milliseconds*TIM_X_MILLISECONDS_IN_NANOSECOND);
}

TIM_ERROR_ENUM TIM_sleepMicro(unsigned int microseconds){
    return TIM_sleepNano(microseconds*TIM_X_MICROSECONDS_IN_NANOSECOND);
}


TIM_ERROR_ENUM TIM_sleepNano(unsigned int nanoseconds) {

    if(nanoseconds == 0){
        return TIM_ERROR_NO_OP;
    }

#ifdef WIN32

   // X_TIM_initFrequency();

    unsigned int microseconds = nanoseconds/TIM_X_MICROSECONDS_IN_NANOSECOND;
    LARGE_INTEGER start, end;
    QueryPerformanceCounter(&start);
    if(microseconds >= 1000){
        MMRESULT gran = timeBeginPeriod(1);
        _sleep(microseconds/1000);
        timeEndPeriod(gran);
    }

    //**
    X_TIM_initFrequency();


    do{
        QueryPerformanceCounter(&end);
    }while( (((end.QuadPart - start.QuadPart)*TIM_X_MICROSECONDS_IN_SECOND)/X_TIM_FREQUENCY.QuadPart) < microseconds);
    //**/
#else
    #if _POSIX_C_SOURCE >= 199309L
        struct timespec timestamp;
        timestamp.tv_sec = nanoseconds / TIM_X_NANOSECONDS_IN_SECOND;
        timestamp.tv_nsec = (nanoseconds - (timestamp.tv_sec * TIM_X_NANOSECONDS_IN_SECOND));
        if(nanosleep(&timestamp, NULL) == -1){
            return TIM_ERROR_FAILURE;
        }
    #else
        nanoseconds/=1000;
        if(nanoseconds == 0){
            nanoseconds = 1;
        }
        if(usleep(nanoseconds) == -1){
            return TIM_ERROR_FAILURE;
        }
    #endif
#endif

    return TIM_ERROR_SUCCESS;
}


TIM_ERROR_ENUM TIM_sleepFPS(unsigned int framesPerSecond){
    unsigned int nanoseconds = TIM_X_NANOSECONDS_IN_SECOND/framesPerSecond;
    return TIM_sleepNano(nanoseconds);
}
