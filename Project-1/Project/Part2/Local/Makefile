INCLUDE = ./

# compiler
CC = gcc

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -Wall -Werror -g -I$(INCLUDE)
FLAGS = -lpthread -lm

# Αρχεία .o
OBJS = local.o

# Το εκτελέσιμο πρόγραμμα
EXEC = prog

ARGS = 4 8000000 8

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(FLAGS)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

val:
	valgrind ./$(EXEC) $(ARGS)