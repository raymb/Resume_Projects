#ifndef SUDOKU_H_
#define SUDOKU_H_

#include <stddef.h>
#include <string.h>
#include "main_aux.h"

/**
 * Type used for the representation of a Sudoku game board.
 */
typedef struct board{
	int row;
	int column;
	int** grid;
} Board;

/**
 * Type used for the representation of a Sudoku game.
 */
typedef struct sudoku{
	Board *board;
	Board *solution;
	Board *fixed;
	int mark_errors;
	Board *marked_errors;
	int board_size;
	int empty_cells;
} Sudoku;

/**
 * Creates a new Sudoku game board. On success the function returns a pointer
 * to the new board. If a memory allocation failure occurs , the function terminates.
 *
 * @param row - row size for the block matrix and row size for each block in the matrix
 * @param column - column sizes for the block matrix and column size for each block in the matrix
 * @return
 * NULL if memory allocation failure occurs
 * pointer to new_board otherwise
 */
Board* createBoard(int row, int column);

/**
 * This function copies a Sudoku game board. On success the function returns a pointer
 * to the new copied board. If a memory allocation failure occurs , the function terminates.
 *
 * @param board - the desired board to copy
 * @return
 * NULL if memory allocation failure occurs
 * pointer to the new board otherwise
 */
Board* copyBoard(Board *board);

/**
 * This Function frees all memory allocations associated with a Sudoku game board.
 *
 * @param board - pointer to the requested memory allocation to free
 * @return
 */
void destroyBoard(Board *board);

/**
 * This function creates a matrix of integers. On success the function returns the
 * new matrix. If a memory allocation failure occurs , the function terminates.
 *
 * @param row ,column - sizes for the matrix (row*column)*(row*column)
 * @return
 * NULL if memory allocation failure occurs
 * pointer to the new matrix otherwise
 *
 * */
int** createGrid(int row,int column);

/**
 * This function creates a new Sudoku game . On success the function returns the
 * new Sudoku game. If a memory allocation failure occurs , the function terminates.
 *
 * @param row - row size for the Sudoku game boards
 * @param column - column size for the Sudoku game boards
 * @return
 * NULL if memory allocation failure occurs
 * pointer to new_sudoku otherwise
 */
Sudoku* createSudoku(int block_row,int block_column);

/**
 * This Function frees all memory allocations associated with a Sudoku game board grid.
 *
 * @param mat - pointer to the requested memory allocation to free,n - size of the grid (nXn)
 * @return
 */
void destroyGrid(int **mat,int size);

/**
 * This Function frees all memory allocations associated with a Sudoku game.
 *
 * @param sudoku - pointer to the requested memory allocation to free
 * @return
 */
void destroySudoku(Sudoku *sudoku);

#endif /* SUDOKU_H_ */
