#include <stdio.h>
#include <unistd.h>
#define DUMBTIMER_IMPLEMENTATION
#include "dumbtimer.h"

int foo(int bar, char* baz) {
    sleep(2);
    return bar;
}

int main(int argc, char** argv)
{
    DumbTimer timer = dt_new();
    printf("" DumbTimer_Fmt "\n", DumbTimer_Arg(&timer));
    sleep(3);
    double elapsed = dt_elapsed(&timer);
    printf("elapsed: %.7f\n", elapsed);
    sleep(3);
    elapsed = dt_stop(&timer);
    printf("" DumbTimer_Fmt ", elapsed: %.7f\n", DumbTimer_Arg(&timer), elapsed);

    DumbTimer timer_real = dt_new();
    int voider = 0;
    for (int i=0; i<10000; i++) {
       voider++;
    }

    double elapsed_real = dt_stop(&timer_real);
    printf("Real timer {%i}: " DumbTimer_Fmt ", elapsed: %.7f\n", voider, DumbTimer_Arg(&timer_real), elapsed_real);

    DumbTimer timer_fast = dt_new();

    double elapsed_fast = dt_elapsed(&timer_fast);

    printf("elapsed (fast): %.7f\n", elapsed_fast);

    double elapsed_timed = 0;

    int res = 0;
    DUMBTIMER_TIMED(foo, &res, &elapsed, 42, "BAR");

    printf("foo() res {%i} took {%.7f}\n", res, elapsed_timed);
}
