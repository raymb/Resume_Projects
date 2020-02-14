#ifndef SOL_COUNTER_H_
#define SOL_COUNTER_H_

#include <stddef.h>
#include <string.h>
#include "generation_handler.h"

/**
 * Type used for the representation of a stack for the backtracking process.
 */
typedef struct stack{
	int *x;
	int *y;
	int *values;
	int top;
	int capacity;
} Stack;

/**
 * Counts how many possible solutions exists for the Sudoku board.
 *
 * @param sudoku - The Sudoku board to count
 * @return
 * NULL if memory allocation failure occurs
 * pointer to new_board otherwise
 */
int count(Sudoku *sudoku);

#endif /* SOL_COUNTER_H_ */
