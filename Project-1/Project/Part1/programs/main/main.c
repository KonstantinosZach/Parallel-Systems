#define _POSIX_C_SOURCE 199309L //needed for CLOCK_MONOTONIC
#include "monte_carlo.h"

int main(int argc, char **argv){

    double start, finish, final;

    /* first we run the monte carlo linearly */
    printf("Linear approach:\n");
    GET_TIME(start);
    monte_carlo_linear(argv[1]);
    GET_TIME(finish);

    final = finish-start;
    printf("\tTime = %f\n", final);

    /* now we run the monte carlo parellel */
    printf("Multithreaded approach:\n");
    GET_TIME(start);
    monte_carlo_parallel(argv[1], argv[2]);
    GET_TIME(finish);

    final = finish-start;
    printf("\tTime = %f\n", final);

    return 0;
}
