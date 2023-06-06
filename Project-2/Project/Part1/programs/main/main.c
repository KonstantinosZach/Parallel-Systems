#include "monte_carlo_omp.h"

int main(int argc, char **argv){

    double start, finish, final;
    GET_TIME(start);
    monte_carlo_omp(argv[1], argv[2]);
    GET_TIME(finish);

    final = finish-start;
    printf(" %f\n", final);

    return 0;
}
