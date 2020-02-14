#ifndef GUROBI_HANDLER_H_
#define GUROBI_HANDLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "gurobi_c.h"
#include "sudoku.h"

/**
 * GUROBI_HANDLER Summary :
 *
 * A container that solves a sudoku puzzle game using the Gurobi tool. By the interpretation of the puzzle to a problem,
 * (LP problem or ILP problem), the container creates decides if the board is solvable and updates the games solution.
 * The container supports the following functions :
 *
 * Gurobi_solve		- Solves a given sudoku puzzle.
 *
 */

/**
 * Type used for the representation of a Gurobi solver.
 */
typedef struct solution_model {
	GRBenv *env;
	GRBmodel *model;
	double *lb;
	double *ub;
	char *vtype;
	int optimstatus;
	double objval;
	double *obj;
	double *x_val;
	int* ind;
	double* val;
} Solution_model;

/**
 * This function solves a sudoku puzzle game using the Gurobi tool.
 *
 * @param sudoku 		- a pointer to a sudoku game
 * @param LP 			- the type of problem to solve (0 for ILP)
 * @param threshold 	- threshold value for guess command
 * @param hint_x 		- x coordinate for guess_hint command
 * @param hint_y 		- y coordinate for guess_hint command
 *
 * @return
 * 0 on success,2 if the givem sudoku game is not solvable,1 otherwise.
 *
 * */
int Gurobi_solve(Sudoku *sudoku, int LP, float threshold, int hint_x, int hint_y);

#endif /* GUROBI_HANDLER_H_ */
