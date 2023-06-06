## Monte Carlo Algorithm for Computing π - Parallel Computing

This repository contains implementations of the Monte Carlo algorithm for computing the value of π (pi) using parallel computing techniques. The algorithm is implemented using different parallel programming models, including serial, Pthreads, OpenMP, and MPI. Each implementation demonstrates how parallelization can improve the performance and efficiency of the algorithm.

Monte Carlo Algorithm
The Monte Carlo algorithm is a statistical method that approximates the value of π by generating random points within a square and determining the ratio of points falling within a quarter of a circle to the total number of generated points. The formula for estimating π using this method is:

π ≈ 4 * (number of points inside the circle) / (total number of points)

The Monte Carlo algorithm is an embarrassingly parallel problem, making it an ideal candidate for parallel computing.

## Implementations
### **Serial Implementation**
The serial implementation is a straightforward implementation of the Monte Carlo algorithm using a single thread. It serves as a baseline for performance comparison with parallel versions.

### **Pthreads Implementation**
The Pthreads implementation demonstrates the usage of POSIX threads to parallelize the Monte Carlo algorithm. It divides the workload across multiple threads, with each thread independently generating a portion of the random points. The results are then combined to compute the final estimate of π.

### **OpenMP Implementation**
The OpenMP implementation demonstrates the utilization of shared memory parallelism using OpenMP directives. It employs parallel loops and workload distribution techniques to distribute the computation across multiple threads, allowing for efficient parallel execution of the Monte Carlo algorithm.

### **MPI Implementation** 
The MPI (Message Passing Interface) implementation showcases the usage of distributed memory parallelism to accelerate the Monte Carlo algorithm. It utilizes multiple processes running on different nodes of a cluster, each generating a subset of random points. MPI communication primitives are used to synchronize the results and compute the final estimate of π.

## Other performance improvements

### Avoiding False-Sharing with **Padding** or **Local-Variables**
### Efficient matrix-vector multiplication using the **OpenMP-Scheduler**
### Efficient matrix-vector multiplication using **MPI_Send** and **MPI_Reduce**