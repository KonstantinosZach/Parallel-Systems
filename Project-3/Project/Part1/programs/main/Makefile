INCLUDE = ../../include
MODULES = ../../modules

CC = mpicc

CFLAGS = -Wall -Werror -g -I$(INCLUDE)
FLAGS =	-lpthread -lm

OBJS = main.o	$(MODULES)/monte_carlo_mpi.o

EXEC = prog

ARGS = 10^8

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(FLAGS)

clean:
	rm -f $(OBJS) $(EXEC)

run_hosts: $(EXEC)
	mpiexec -hosts linux02,linux03,linux04,linux05 -n 4 ./$(EXEC) $(ARGS)

run_machines: $(EXEC)
	mpiexec -f machines -n 4 ./$(EXEC) $(ARGS)

val:
	valgrind ./$(EXEC) $(ARGS)