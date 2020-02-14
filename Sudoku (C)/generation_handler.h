#ifndef GENERATION_HANDLER_H_
#define GENERATION_HANDLER_H_

/**
 * GENERATION_HANDLER Summary :
 *
 * A container that generates valid values into a given sudoku game.
 * The container supports the following functions :
 *
 * flushSet				- Flushes a set of used values
 * createSet			- Creates a new valid values set and initialize it.
 * destroySet			- Frees all memory allocations associated with a set.
 * make_valid_values	- Finds all possible values for a given cell
 * clear_matrix			- Sets all entries of a given integer matrix to zero
 * generator			- Generates and inserts random valid values into a given sudoku game.
 */

#include "sudoku.h"

/**
 * Type used for representing a set of valid numbers for a given cell.
 */
typedef struct set{
	int* valid_values;
}Set;

/**
 * This function flushes a set of used values. All sets values will be valid again.
 *
 * @param set - a pointer to the set to be flushed of used values
 * @param length - denotes length of set
 * @return
 */
void flushSet(Set *set, int length);

/**
 * This function creates a new set and initialize it. On success the function returns a pointer
 * to the new set. If a memory allocation failure occurs, the function terminate.
 *
 * @param length - denotes the length of set
 * @return
 * NULL  if a memory allocation failure occurs
 * pointer to the new set otherwise.
 */
Set* createSet(int length);

/*
 * This Function frees all memory allocations associated with a set.
 *
 * @param *set - a pointer to set
 * @return
 */
void destroySet(Set *set);

/*
 * This function finds all possible values for a given cell .
 *
 * @param set - a pointer to the set of the desired cell
 * @param board - a pointer to the Sudoku game board
 * @param x -  row coordinate of the desired cell
 * @param y -  column coordinate of the desired cell
 *
 */
void make_valid_values(Set *set,Board *board, int x, int y);

/**
 * This function sets all entries of a given integer matrix to zero.
 *
 * @param set - a pointer to an integer matrix.
 * @param size - denotes size of matrix.
 * @return
 */
void clear_matrix(int **mat,int size);

/**
 * This function generates and inserts random valid values into a given sudoku game.
 *
 * @param s - a pointer to the sudoku game
 * @param numtofill - denotes number of cells to fill
 * @return
 *
 * 1 on success,
 * 0 otherwise.
 */
int generator(Sudoku *s,int numtofill);

#endif /* GENERATION_HANDLER_H_ */
