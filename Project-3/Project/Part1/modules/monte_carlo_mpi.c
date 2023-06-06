#include "monte_carlo_mpi.h"

//returns a random (double) number between [min, max]
static double drandom_r(int max, int min, unsigned int* seed){
    return (max - min) * (double)rand_r(seed)/(double)(RAND_MAX) + min;
}

//parses the 1st command line input
static void parse(char* sum, int* base, int* power){

    char* copy = malloc(sizeof(char) * (strlen(sum) + 1));
    strcpy(copy, sum);

    char* power_str;
    char* base_str = strtok_r(copy, "^", &power_str);
    
    (*base) = atoi(base_str);
    (*power) = atoi(power_str);

    free(copy);

    return;
}

//returns the number of iterations for each block
long long get_portion(char* sum_str, int comm_sz, long long* total_num_darts){
    int base, power;
    parse(sum_str, &base, &power);
    *total_num_darts = pow(base, power);
    return  *total_num_darts/comm_sz;
}

long long get_num_darts_in(long long portion, int rank){
    //seed that its altered based on the rank
    unsigned int seed = time(NULL) ^ rank;

    long long darts_in_circle = 0;
    for(long long i=0; i<portion; i++){
        double x = drandom_r(1,-1, &seed);
        double y = drandom_r(1,-1, &seed);
        double dist = x*x + y*y;
        
        if(dist <=1)
            darts_in_circle++;
    }

    return darts_in_circle;
}