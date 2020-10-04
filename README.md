# C_Timing
C library for conditional compiling to handle thread waits accurately in sub millisecond ranges

Building on Windows:
link with -lwinmm for advanced timings, otherwise delays can be off by 10+ milliseconds

Deficiencies
=
Not intended for very high throughput.

Windows timing system is limited to 2^64/(10^9 * clockFrequency) nanoseconds before rolls over.
Commented attemped, but timeGetTime is currently too inaccurate for milisecond accuracy without further reading.


Code Sample
=
```C

void testSleep_delay(){
    TIM_Timestamp fps_start, fps_end;
    TIM_Delay delay;

    const unsigned int FPS = 111;
    TIM_initDelayFPS(&delay, FPS);
    TIM_initTimestamp(&fps_start);

    int i;
    for(i = 0; i < FPS; i++){
        TIM_DelaySleep(&delay);
        //arbitrary random delay -- 111 is ~9ms per, so up to 8ms random delay
        TIM_sleepMillis((i%7)+1);
    }
    TIM_initTimestamp(&fps_end);
    printf("Diff: %llu ms\n", TIM_TimestampDiffMillis(&fps_start,&fps_end));
    printf("Diff: %llu us\n", TIM_TimestampDiffMicro(&fps_start,&fps_end));
    printf("Diff: %llu ns\n", TIM_TimestampDiffNano(&fps_start,&fps_end));
    
    //make sure to free data in case a platform has handles
    TIM_freeTimestampData(&fps_start);
    TIM_freeTimestampData(&fps_end);
    TIM_freeDelayData(&delay);
}
```
