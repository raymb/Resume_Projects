#ifndef SUDOKU_H_
#define SUDOKU_H_

/**
 * Sudoku Summary :
 *
 * A container that represents the Sudoku puzzle . Sudoku is a number-placement puzzle where the objective is to fill a 9x9 grid with digits so that
 * each column, each row, and each 3x3 block contains all digits from 1 to 9. Since the Sudoku grid constraints are defined by column, row and block,
 * we chose to represent the grid, i.e. the Sudoku play board, by a matrix of Integers. A Sudoku game is being represented by a play board, a solution
 * board, an errors board and a fixed board which defines the initial play board.
 * The container supports the following functions:
 *
 * createBoard						 - Creates a Sudoku board.
 * copyBoard 						 - Copies a Sudoku board.
 * destroyBoard 					 - Frees all memory resources associated with a Sudoku board.
 * createGrid						 - Creates a Sudoku board grid.
 * createSudoku						 - Create a Sudoku game.
 * destroyGrid 						 - Frees all memory associated with Sudoku board grid.
 * destroySudoku					 - Frees all memory associated with Sudoku game.
 * print_board						 - Prints the game board.
 * not_valid						 - Checksif a value is valid in a given cell.
 *
 */

#include <stddef.h>
#include <string.h>
#include "main_aux.h"


/**
 * Type used for the representation of a board.
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

/**
 * This Function prints the game board.
 *
 * @param sudoku - pointer to the sudoku game.
 * @return
 */
void print_board(Sudoku* sud, int solution);

/*
 * This function checks if z is valid value for the (x,y) cell in a given board.
 *
 * @param board - pointer to a board game
 * @param x - the row coordinate of the desired cell to check
 * @param y - the column coordinate of the desired cell to check
 * @param z - value to check
 * @return
 * 1 if invalid or 0 otherwise
 */
int not_valid(Board *board,int x, int y ,int z);

#endif /* SUDOKU_H_ */
