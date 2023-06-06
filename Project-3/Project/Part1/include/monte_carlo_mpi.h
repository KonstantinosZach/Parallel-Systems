#pragma once

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <mpi.h>

long long get_portion(char* sum_str, int comm_sz, long long* total_num_darts);

long long get_num_darts_in(long long portion, int rank);