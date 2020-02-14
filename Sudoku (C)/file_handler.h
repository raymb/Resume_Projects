#ifndef FILE_HANDLER_H_
#define FILE_HANDLER_H_

#include<stdio.h>
#include "sudoku.h"

/**
 * Loads Sudoku board from file. On success the function returns a pointer to the loaded board,
 * or NULL if memory allocation fails.
 *
 * @param path - path to the file for loading.
 * @return
 * NULL if memory allocation failure occurs
 * Pointer to loaded board on success
 */
Sudoku* load_from_file(const char* path);

/**
 * Saves Sudoku board to file. On success the function returns 1, otherwise returns 0.
 *
 * @param sudoku - The Sudok board to be saved.
 * @param path - The path to the file where the board ill be saved.
 * @return
 * 1 if succeeded, otherwise 0.
 */
int save_to_file(Sudoku* sudoku, const char* path);

#endif /* FILE_HANDLER_H_ */
