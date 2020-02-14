#ifndef GUROBI_HANDLER_H_
#define GUROBI_HANDLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "gurobi_c.h"
#include "sudoku.h"

typedef struct solution_model {
	GRBenv *env;
	GRBmodel *model;
	double *lb;
	char *vtype;
	int optimstatus;
	double objval;
	double *x_val;
	int* ind;
	double* val;
} Solution_model;

int ILPsolve(Sudoku *sudoku);


#endif /* GUROBI_HANDLER_H_ */
