#include "monte_carlo_omp.h"

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

void monte_carlo_omp(char* sum_str, char* num_threads_str){

    int base, power, thread_count;
    parse(sum_str, &base, &power);
    thread_count = strtol(num_threads_str, NULL, 10);

    long long num_darts = pow(base, power);
    long long thread_portion = num_darts/thread_count; //number of iterations for each thread
    long long darts_in_circle = 0;

    #pragma omp parallel num_threads(thread_count) reduction(+: darts_in_circle) firstprivate(thread_portion)
    {

        //all variables that are declared inside the omp block are already private//
        //thread_portion was made private for clarity, there was no case of race condition

        //seed that its altered based on the thread_num
        unsigned int seed = time(NULL) ^ omp_get_thread_num();

        for(long long i=0; i<thread_portion; i++){
            double x = drandom_r(1,-1, &seed);
            double y = drandom_r(1,-1, &seed);
            double dist = x*x + y*y;
            
            //does not need omp critical section cause we use reduction
            if(dist <=1)
                darts_in_circle++;
        }
    }

    //computes the result
    double result = 4 * darts_in_circle/((double)num_darts);
    printf("%f", result);

    return;
}