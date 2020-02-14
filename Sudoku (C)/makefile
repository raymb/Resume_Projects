CC = gcc
OBJS = main.o main_aux.o sudoku.o stdin_handler.o generation_handler.o file_handler.o \
sol_counter.o gurobi_handler.o game_aux.o game.o 
EXEC = sudoku-console
COMP_FLAG = -O3 -ansi -Wall -Wextra \
-Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c game.h SPBufferset.h
	$(CC) $(COMP_FLAG) $(GUROBI_LIB) $(GUROBI_COMP) -c $*.c -lm
main_aux.o: main_aux.c main_aux.h
	$(CC) $(COMP_FLAG) -c $*.c 
sudoku.o: sudoku.c sudoku.h main_aux.h
	$(CC) $(COMP_FLAG) -c $*.c 
stdin_handler.o: stdin_handler.c stdin_handler.h main_aux.h 
	$(CC) $(COMP_FLAG) -c $*.c 
generation_handler.o: generation_handler.c generation_handler.h sudoku.h main_aux.h
	$(CC) $(COMP_FLAG) -c $*.c 
file_handler.o : file_handler.c file_handler.h sudoku.h main_aux.h
	$(CC) $(COMP_FLAG) -c $*.c 
sol_counter.o : sol_counter.c sol_counter.h generation_handler.h 
	$(CC) $(COMP_FLAG) -c $*.c 
gurobi_handler.o: gurobi_handler.c gurobi_handler.h sudoku.h main_aux.h
	$(CC) $(COMP_FLAG) $(GUROBI_LIB) $(GUROBI_COMP) -c $*.c -lm
game_aux.o : game_aux.c game_aux.h stdin_handler.h sol_counter.h generation_handler.h file_handler.h gurobi_handler.h 
	$(CC) $(COMP_FLAG) $(GUROBI_LIB) $(GUROBI_COMP) -c $*.c -lm
game.o : game.c game.h game_aux.h
	$(CC) $(COMP_FLAG) $(GUROBI_LIB) $(GUROBI_COMP) -c $*.c -lm

clean:
	rm -f $(OBJS) $(EXEC)