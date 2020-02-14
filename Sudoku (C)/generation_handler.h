#ifndef GENERATION_HANDLER_H_
#define GENERATION_HANDLER_H_

#include <time.h>
#include "sudoku.h"


typedef struct set{
	int* valid_values;
}Set;

void flushSet(Set *set, int length);
Set* createSet(int length);
void destroySet(Set *set);
int not_valid(Board *board,int x, int y ,int z);
void make_valid_values(Set *set,Board *board, int x, int y);
void clear_matrix(int **mat,int size);
int generator(Sudoku *s,int numtofill);

#endif /* GENERATION_HANDLER_H_ */
