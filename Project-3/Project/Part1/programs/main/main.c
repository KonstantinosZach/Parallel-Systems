#include "monte_carlo_mpi.h"

int main(int argc, char* argv[]) {
   int comm_sz;                        //Number of processes
   int my_rank;                        //My process rank
   double start_time = 0;              //only in root
   double end_time = 0;                //only in root
   long long total_num_darts = 0;      //only in root
   long long portion = 0;              //sent by root received by all the processes

   //Start up MPI
   MPI_Init(NULL, NULL); 

   //Get the number of processes
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   //Get my rank among all the processes
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
   
   if(my_rank == 0){
      //start timer on root
      start_time = MPI_Wtime();

      //broadcast number of iterations (portion of work) to other processes
      portion = get_portion(argv[1], comm_sz, &total_num_darts);
      MPI_Bcast(&portion, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
   }
   else{
      //each process receives the portion from root
      MPI_Bcast(&portion, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
   }

   //each process (root also) computes the num_darts_in
   long long local_num_darts_in = 0;
   local_num_darts_in = get_num_darts_in(portion, my_rank);

   // Reduce all of the local sums into the global sum to the root
   long long global_num_darts_in = 0;
   MPI_Reduce(&local_num_darts_in, &global_num_darts_in, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

   //root will calculate the final results and the time needed to perform the task
   if(my_rank == 0){
      //computes the result
      double result = 4 * global_num_darts_in/((double)total_num_darts);
      end_time = MPI_Wtime();
      printf("Result:%f    Time:%f\n",result, end_time-start_time);
   }

   //Shut down MPI
   MPI_Finalize(); 

   return 0;
}
