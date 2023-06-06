# paths
INCLUDE = ../../include
MODULES = ../../modules

# compiler
CC = gcc

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -Wall -Werror -g -fopenmp	-I$(INCLUDE)
FLAGS =	-lpthread	-lm	-fopenmp

# Αρχεία .o
OBJS = main.o $(MODULES)/monte_carlo_omp.o

# Το εκτελέσιμο πρόγραμμα
EXEC = prog

ARGS = 10^8 6

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(FLAGS)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

val:
	valgrind ./$(EXEC) $(ARGS)