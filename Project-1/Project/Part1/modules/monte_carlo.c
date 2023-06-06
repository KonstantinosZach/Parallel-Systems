#include "monte_carlo.h"

//thread safe rand
static double drandom_r(int max, int min, unsigned int* seed){
    return (max - min) * (double)rand_r(seed)/(double)(RAND_MAX) + min;
}

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

static void* thread_job(void* args){

    unsigned int seed = time(NULL); //thread safe
    thread_args* t_args = (thread_args*)args;

    for(long long i=t_args->start; i<t_args->end; i++){
        double x = drandom_r(1,-1, &seed);
        double y = drandom_r(1,-1, &seed);
        double dist = x*x + y*y;

        if(dist <=1)
            t_args->darts_in_circle++;
    }

    pthread_exit(0);
}

void monte_carlo_parallel(char* sum_str, char* num_threads_str){

    int base, power, num_threads;
    parse(sum_str, &base, &power);
    num_threads = atoi(num_threads_str);

    long long num_darts = pow(base, power);
    long long start = 0, end = 0;

    //create the threads and split the workload
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    thread_args** args = malloc(num_threads * sizeof(thread_args*));

    long long num_darts_t = num_darts;
    for(int i=0; i<num_threads; i++){
        long long workload = num_darts_t / (num_threads - i);
        num_darts_t -= workload;

        end += workload;

        args[i] = (thread_args*)malloc(sizeof (thread_args));
        args[i]->start = start;
        args[i]->end = end;
        args[i]->darts_in_circle = 0LL;

        pthread_create(&threads[i], NULL, thread_job, args[i]);

        start += workload;
    }

    //wait for each thread to finish and get the total darts in circle
    long long darts_in_circle = 0LL;
    for(int i=0; i<num_threads; i++){
        pthread_join(threads[i], NULL);
        darts_in_circle += args[i]->darts_in_circle;
        free(args[i]);
    }

    double result = 4 * darts_in_circle/((double)num_darts);
    printf("\tResult = %f\n", result);

    free(threads);
    free(args);

    return;
}

void monte_carlo_linear(char* sum){

    int base, power;
    parse(sum, &base, &power);

    long long darts_in_circle = 0;
    long long num_darts = pow(base, power);
    unsigned int seed = time(NULL); //thread safe

    for(long long throw=0; throw < num_darts; throw++){
        double x = drandom_r(1,-1, &seed);
        double y = drandom_r(1,-1, &seed);
        double dist = x*x + y*y;

        if(dist <=1)
            darts_in_circle++;
    }

    double result = 4 * darts_in_circle/((double)num_darts);
    printf("\tResult = %f\n", result);

    return;
}
