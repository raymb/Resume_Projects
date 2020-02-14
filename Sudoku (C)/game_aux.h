#ifndef GAME_AUX_H_
#define GAME_AUX_H_

#include "stdin_handler.h"
#include "sol_counter.h"
#include "file_handler.h"
#include "gurobi_handler.h"

typedef struct node  {
	int *x;
	int *y;
	int *oldz;
	int *newz;
	int set;
	struct node *next;
	struct node *prev;
} Node;

void createHead(Node **head);

Sudoku* solve(Sudoku*s,Node **list,Move *move);
Sudoku* edit(Sudoku*s,Node **list,Move *move);
void print_board(Sudoku* sud);


int set(Move *move,Sudoku *sudoku, int mode,Node **list, int *erroneous);
int validate(Sudoku* sudoku, int *erroneous);

int generate(Sudoku* s, Move *move,Node **list);
void undo(Node **list, Sudoku* sudoku,int reset, int *erroneous);
void redo(Node **list, Sudoku* sudoku, int *erroneous);

int autofill(Sudoku* sudoku, Node **list, int *erroneous);
Move* next_sudoku_move(Sudoku* sudoku, int mode);
int save(Sudoku *sudoku ,Move *move,int mode, int *erroneous);
int hint(Sudoku *sudoku,Move *move, int *erroneous);
int numsolutions(Sudoku* sudoku, int *erroneous);
void reset(Sudoku *sudoku, Node **list, int *erroneous);
void exitgame(Sudoku *sudoku,Node *list);

#endif /* GAME_AUX_H_ */
