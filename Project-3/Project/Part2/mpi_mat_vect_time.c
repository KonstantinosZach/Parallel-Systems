/* File:     mpi_mat_vect_time.c
 *
 * Purpose:  Implement parallel matrix-vector multiplication using
 *           one-dimensional arrays to store the vectors and the
 *           matrix.  Vectors use block distributions and the
 *           matrix is distributed by block rows.  This version
 *           generates a random matrix A and a random vector x.
 *           It prints out the run-time.
 *
 * Compile:  mpicc -g -Wall -o mpi_mat_vect_time mpi_mat_vect_time.c
 * Run:      mpiexec -n <number of processes> ./mpi_mat_vect_time
 *
 * Input:    Dimensions of the matrix (m = number of rows, n
 *              = number of columns)
 * Output:   Elapsed time for execution of the multiplication
 *
 * Notes:     
 *    1. Number of processes should evenly divide both m and n
 *    2. Define DEBUG for verbose output, including the product
 *       vector y
 *
 * IPP:  Section 3.6.2 (pp. 122 and ff.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void Check_for_error(int local_ok, char fname[], char message[], 
      MPI_Comm comm);
void Get_dims(int* m_p, int* local_m_p, int* n_p, int* local_n_p,
      int my_rank, int comm_sz, MPI_Comm comm);
void Allocate_arrays(double** local_A_pp, double** local_x_pp, 
      double** local_y_pp, int m, int n, MPI_Comm comm);
void Generate_matrix(double local_A[], int local_m, int n);
void Generate_vector(double local_x[], int local_n);
void Print_matrix(char title[], double local_A[], int m,
      int n, int my_rank, MPI_Comm comm);
void Print_vector(char title[], double local_vec[], int n, int my_rank, MPI_Comm comm);
void Print_splited_matrix(double* matrix, int m, int n, int my_rank);

/*-------------------------------------------------------------------*/
int main(void) {
   double* local_A;
   double* local_x;
   double* local_y;
   int m, local_m, n, local_n;
   int my_rank, comm_sz;
   MPI_Comm comm;
   double start, finish;

   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &comm_sz);
   MPI_Comm_rank(comm, &my_rank);

   Get_dims(&m, &local_m, &n, &local_n, my_rank, comm_sz, comm);

   if(my_rank == 0){
      Allocate_arrays(&local_A, &local_x, &local_y, m, n, comm);
      srandom(my_rank);
      Generate_matrix(local_A, m, n);
      Generate_vector(local_x, n);

      Print_matrix("A", local_A, m, n, my_rank, comm);
      Print_vector("x", local_x, n, my_rank, comm);
   }

   if(my_rank == 0){
      //start timer
      start = MPI_Wtime();

      double* temp_maxtrix = malloc(sizeof(double) * (m*local_n));
      double* temp_vector = malloc(sizeof(double) * (local_n));

      //for all the processes send the partitioned vector and the partitioned matrix
      for(int i=1; i<comm_sz; i++){
         //for each process create the matrix and vector to send (spedific collums)
         int temp_maxtrix_c = 0;
         int temp_vector_c = 0;

         for(int col=i*local_n; col<(i+1)*local_n; col++){
            temp_vector[temp_vector_c] = local_x[col];
            temp_vector_c++;
            for(int row=0; row<m; row++){
               temp_maxtrix[temp_maxtrix_c] = local_A[col + row*n];
               temp_maxtrix_c++;
            }
         }

         //send the partioned vector
         MPI_Send(temp_vector, temp_vector_c, MPI_DOUBLE, i, 0, comm);
         //send the partitioned matrix
         MPI_Send(temp_maxtrix, temp_maxtrix_c, MPI_DOUBLE, i, 0, comm);
         
      }
      free(temp_maxtrix);
      free(temp_vector);

      //root compute his part of the matrix
      for(int col=0; col<local_n; col++){
         for(int row=0; row<m; row++){
            local_y[row]  += local_A[col + row*m]*local_x[col];
         }
      }
      //Print_vector("x", local_y, m, my_rank, comm);

   }
   else{
      Allocate_arrays(&local_A, &local_x, &local_y, m, local_n, comm);

      //receive portion of vector x
      MPI_Recv(local_x, n, MPI_DOUBLE, 0, 0, comm, MPI_STATUS_IGNORE);
      //Print_vector("x", local_x, n, my_rank, comm);

      //receive portion of matrix A
      MPI_Recv(local_A, m*local_n, MPI_DOUBLE, 0, 0, comm, MPI_STATUS_IGNORE);
      //Print_splited_matrix(local_A, m, local_n, my_rank);
      
      //each process computes its part of the matrix
      for (int i = 0; i < local_n; i++){
         for (int j = 0; j < m; j++){
            local_y[j]  += local_A[i*m+j]*local_x[i];
         }
      }
      //Print_vector("x", local_y, m, my_rank, comm);
   }

   //sum up all the smaller arrays to this global array for the final result
   double* global_y = calloc(m, sizeof(double));
   MPI_Reduce(local_y, global_y, m, MPI_DOUBLE, MPI_SUM, 0, comm );

   if(my_rank == 0){
      //end timer
      finish = MPI_Wtime();
      Print_vector("y", global_y, m, my_rank, comm);  //output result
      printf("Elapsed time = %f\n", finish-start);    //output time
   }

   //free arrays
   free(local_A);
   free(local_x);
   free(local_y); 
   free(global_y);

   MPI_Finalize();
   return 0;
}  /* main */


/*-------------------------------------------------------------------*/
void Check_for_error(
      int       local_ok   /* in */, 
      char      fname[]    /* in */,
      char      message[]  /* in */, 
      MPI_Comm  comm       /* in */) {
   int ok;

   MPI_Allreduce(&local_ok, &ok, 1, MPI_INT, MPI_MIN, comm);
   if (ok == 0) {
      int my_rank;
      MPI_Comm_rank(comm, &my_rank);
      if (my_rank == 0) {
         fprintf(stderr, "Proc %d > In %s, %s\n", my_rank, fname, 
               message);
         fflush(stderr);
      }
      MPI_Finalize();
      exit(-1);
   }
}  /* Check_for_error */


/*-------------------------------------------------------------------*/
void Get_dims(
      int*      m_p        /* out */, 
      int*      local_m_p  /* out */,
      int*      n_p        /* out */,
      int*      local_n_p  /* out */,
      int       my_rank    /* in  */,
      int       comm_sz    /* in  */,
      MPI_Comm  comm       /* in  */) {
   int local_ok = 1;

   if (my_rank == 0) {
      printf("Enter the number of rows\n");
      scanf("%d", m_p);
      printf("Enter the number of columns\n");
      scanf("%d", n_p);
   }
   MPI_Bcast(m_p, 1, MPI_INT, 0, comm);
   MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
   if (*m_p <= 0 || *n_p <= 0 || *m_p % comm_sz != 0 
         || *n_p % comm_sz != 0) local_ok = 0;
   Check_for_error(local_ok, "Get_dims",
      "m and n must be positive and evenly divisible by comm_sz", 
      comm);

   *local_m_p = *m_p/comm_sz;
   *local_n_p = *n_p/comm_sz;
}  /* Get_dims */

/*-------------------------------------------------------------------*/
void Allocate_arrays(
      double**  local_A_pp  /* out */, 
      double**  local_x_pp  /* out */, 
      double**  local_y_pp  /* out */, 
      int       m           /* in  */, 
      int       n           /* in  */,   
      MPI_Comm  comm        /* in  */) {

   *local_A_pp = malloc(m*n*sizeof(double));
   *local_x_pp = malloc(n*sizeof(double));
   *local_y_pp = calloc(sizeof(double), m);

}  /* Allocate_arrays */


/*-------------------------------------------------------------------*/
void Generate_matrix(
      double local_A[]  /* out */, 
      int    local_m    /* in  */, 
      int    n          /* in  */) {
   int i, j;

   for (i = 0; i < local_m; i++)
      for (j = 0; j < n; j++) 
         local_A[i*n + j] = ((double) random())/((double) RAND_MAX);
}  /* Generate_matrix */

/*-------------------------------------------------------------------*/
void Generate_vector(
      double local_x[] /* out */, 
      int    local_n   /* in  */) {
   int i;

   for (i = 0; i < local_n; i++)
      local_x[i] = ((double) random())/((double) RAND_MAX);
}  /* Generate_vector */

/*-------------------------------------------------------------------*/
void Print_matrix(
      char      title[]    /* in */,
      double    local_A[]  /* in */, 
      int       m          /* in */, 
      int       n          /* in */,
      int       my_rank    /* in */,
      MPI_Comm  comm       /* in */) {
      int i, j;

   printf("\nThe matrix %s\n", title);
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++)
         printf("%f ", local_A[i*n+j]);
      printf("\n");
   }
   printf("\n");

}  /* Print_matrix */

void Print_splited_matrix(double* matrix, int m, int n, int my_rank){
   printf("Split matrix for proc %d\n", my_rank);
   for (int i = 0; i < n; i++){
      for (int j = 0; j < m; j++){
         printf("%f ", matrix[i*m+j]);
      }
      printf("\n");
   }
}

/*-------------------------------------------------------------------*/
void Print_vector(
      char      title[]     /* in */, 
      double    local_vec[] /* in */, 
      int       n           /* in */,
      int       my_rank     /* in */,
      MPI_Comm  comm        /* in */) {
      int i;

   printf("\nThe vector %s\n", title);
   for (i = 0; i < n; i++)
      printf("%f ", local_vec[i]);
   printf("\n");
   
}  /* Print_vector */
