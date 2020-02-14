#ifndef FILE_HANDLER_H_
#define FILE_HANDLER_H_

/**
 *  FILE_HANDLER summary :
 *
 * A container that loads a sudoku game from a file or saves a sudoku game to a file.
 * The container supports the following functions :
 *
 * load_from_file		- Loads a sudoku game from a file.
 * save_to_file			- Saves a sudoku game to a file.
 */

#include <stdio.h>
#include "sudoku.h"

/**
 * This function loads a sudoku game from a file.
 *
 * @param path - path to the file.
 * @paramsudoku -a sudoku game.
 *
 * @return
 * 0 on success, 2 if EOF reached,1 otherwise.
 */
Sudoku* load_from_file(const char* path);

/**
 * This function saves a sudoku game from to file.
 *
 * @param sudoku - a pointer to the sudoku game.
 * @param path - path to the file.
 * @param mode - the current game mode.
 *
 * @return
 * 1 if saved successfully or 0 otherwise.
 */
int save_to_file(Sudoku* sudoku, const char* path , int mode);

#endif /* FILE_HANDLER_H_ */
