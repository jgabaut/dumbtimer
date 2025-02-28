// jgabaut @ github.com/jgabaut
// SPDX-License-Identifier: GPL-3.0-only
/*
    Copyright (C) 2025 jgabaut

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef DUMBTIMER_H_
#define DUMBTIMER_H_
#include <time.h>
#ifdef _WIN32
#include <profileapi.h>		//Used for QueryPerformanceFrequency(), QueryPerformanceCounter()
#endif

#define DUMBTIMER_MAJOR 0 /**< Represents current major release.*/
#define DUMBTIMER_MINOR 1 /**< Represents current minor release.*/
#define DUMBTIMER_PATCH 0 /**< Represents current patch release.*/

static const int DUMBTIMER_API_VERSION_INT =
    (DUMBTIMER_MAJOR * 1000000 + DUMBTIMER_MINOR * 10000 + DUMBTIMER_PATCH * 100);

struct DumbTimer {
#ifndef _WIN32
    struct timespec start_time, end_time;
#else
    LARGE_INTEGER start_time, end_time, frequency;
#endif
};

typedef struct DumbTimer DumbTimer;

#ifndef _WIN32
#define DumbTimer_Fmt "DumbTimer { start_time: %.9f, end_time: %.9f }"
#define DumbTimer_Arg(dt) \
    ((dt)->start_time.tv_sec + (dt)->start_time.tv_nsec / 1e9), \
    ((dt)->end_time.tv_sec + (dt)->end_time.tv_nsec / 1e9)
#else
#define DumbTimer_Fmt "DumbTimer { start_time: %.7f, end_time: %.7f, frequency: %.7f }"
#define DumbTimer_Arg(dt) \
    ((double)(dt)->start_time.QuadPart / (dt)->frequency.QuadPart), \
    ((double)(dt)->end_time.QuadPart / (dt)->frequency.QuadPart), \
    ((double)(dt)->frequency.QuadPart)
#endif

#define DUMBTIMER_TIMED(func, res, elapsed, ...) do { \
    DumbTimer dt__ = dt_new(); \
    (*res) = (func)(__VA_ARGS__); \
    (*elapsed) = dt_stop(&dt__); \
} while (0)

DumbTimer dt_new(void);
double dt_elapsed(DumbTimer* dt);
double dt_stop(DumbTimer* dt);
#endif // DUMBTIMER_H_

#ifdef DUMBTIMER_IMPLEMENTATION
DumbTimer dt_new(void)
{
    DumbTimer self = {0};
#ifndef _WIN32
    clock_gettime(CLOCK_MONOTONIC, &(self.start_time));
#else
    QueryPerformanceFrequency(&(self.frequency));
    QueryPerformanceCounter(&(self.start_time));
#endif // _WIN32
    return self;
}

double dt_elapsed(DumbTimer* dt)
{
#ifndef _WIN32
    struct timespec end = {0};
    clock_gettime(CLOCK_MONOTONIC, &end);	// %.9f
    double elapsed_time =
        (end.tv_sec - dt->start_time.tv_sec) + (end.tv_nsec -
            dt->start_time.tv_nsec) / 1e9;
#else
    LARGE_INTEGER end = {0};
    QueryPerformanceCounter(&end);	// %.7f
    double elapsed_time =
        (double)(end.QuadPart -
                 dt->start_time.QuadPart) / dt->frequency.QuadPart;
#endif
    return elapsed_time;
}

double dt_stop(DumbTimer* dt)
{
#ifndef _WIN32
    clock_gettime(CLOCK_MONOTONIC, &(dt->end_time));
    double elapsed_time =
        (dt->end_time.tv_sec - dt->start_time.tv_sec) + (dt->end_time.tv_nsec -
            dt->start_time.tv_nsec) / 1e9;
#else
    QueryPerformanceCounter(&(dt->end_time));
    double elapsed_time =
        (double)(dt->end_time.QuadPart -
                 dt->start_time.QuadPart) / dt->frequency.QuadPart;
#endif
    return elapsed_time;
}

#endif // DUMBTIMER_IMPLEMENTATION
