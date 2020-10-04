#include <stdio.h>
#include <stdlib.h>

#include "c_timing.h"

void testSleep_timestamp(){
    TIM_Timestamp start;
    TIM_initTimestamp(&start);
    int i;
    for(i = 0; i < 10000000; i++){
        i++;
    }
    TIM_Timestamp end;
    TIM_initTimestamp(&end);
    printf("Diff: %llu ms\n", TIM_TimestampDiffMillis(&start,&end));
}

void testSleep_decay(){
    TIM_Timestamp start,end;
    int i;
    for(i = 0; i < 150; i++){
        TIM_initTimestamp(&start);
        TIM_sleepMillis(i);
        TIM_initTimestamp(&end);
        printf("Diff: %llu ms\n", TIM_TimestampDiffMillis(&start,&end));
    }
}

void testSleep_fps(){
    TIM_Timestamp fps_start, fps_end;
    TIM_initTimestamp(&fps_start);
    const unsigned int FPS = 60;
    int i;
    for(i = 0; i < FPS; i++){
        TIM_sleepFPS(FPS);
    }
    TIM_initTimestamp(&fps_end);
    printf("Diff: %llu ms\n", TIM_TimestampDiffMillis(&fps_start,&fps_end));

    TIM_freeTimestampData(&fps_start);
    TIM_freeTimestampData(&fps_end);
}

void testSleep_delay(){
    TIM_Timestamp fps_start, fps_end;
    TIM_Delay delay;

    const unsigned int FPS = 111;
    TIM_initDelayFPS(&delay, FPS);
    TIM_DelaySleep(&delay);
    TIM_initTimestamp(&fps_start);

    int i;
    for(i = 0; i < FPS; i++){
        TIM_DelaySleep(&delay);
        TIM_sleepMillis((i%7)+1);
    }
    TIM_initTimestamp(&fps_end);
    printf("Diff: %llu ms\n", TIM_TimestampDiffMillis(&fps_start,&fps_end));

    TIM_freeTimestampData(&fps_start);
    TIM_freeTimestampData(&fps_end);
    TIM_freeDelayData(&delay);
}


int main()
{

    printf("Hello world!\n");

    testSleep_fps();
    testSleep_delay();

    return 0;
}
