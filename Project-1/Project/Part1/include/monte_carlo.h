#pragma once

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#define GET_TIME(now) { \
   struct timeval t; \
   gettimeofday(&t, NULL); \
   now = t.tv_sec + t.tv_usec/1000000.0; \
}

typedef struct{
    long long start;
    long long end;
    long long darts_in_circle;
}thread_args;

void monte_carlo_linear(char* sum_str);

void monte_carlo_parallel(char* sum_str, char* num_threads_str);