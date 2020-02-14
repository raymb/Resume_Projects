#include "gurobi_handler.h"

/**
 * This Function frees all memory allocations associated with a single solver's array.
 *
 * @param arr - pointer to the requested memory allocation to free
 * @return
 */
void destroy_array(void *arr){
	if(!arr)
		free(arr);
}

/**
 * This Function frees all memory allocations associated with all solver's arrays.
 *
 * @param lb - pointer to the requested memory allocation to free
 * @param vtype - pointer to the requested memory allocation to free
 * @param x - pointer to the requested memory allocation to free
 * @param ind - pointer to the requested memory allocation to free
 * @param val - pointer to the requested memory allocation to free
 * @param ub - pointer to the requested memory allocation to free
 * @param obj - pointer to the requested memory allocation to free
 * @return
 */
void destroy_ILP_arrays(double* lb, char* vtype, double *x,int* ind, double* val, double *ub, double *obj){
	destroy_array(lb);
	destroy_array(ub);
	destroy_array(vtype);
	destroy_array(x);
	destroy_array(ind);
	destroy_array(val);
	destroy_array(obj);
}

/**
 * This Function frees all memory allocations associated with a Gurobi solver.
 *
 * @param arr - pointer to the requested memory allocation to free
 * @return
 */
void destroy_Solution_model(Solution_model *model){
	destroy_ILP_arrays(model->lb, model->vtype, model->x_val, model->ind, model->val, model->ub, model->obj);
	GRBfreemodel(model->model);
	GRBfreeenv(model->env);
	free(model);
}

/**
 * This function values of a given entry of a constraint array.
 *
 * @param ind - pointer to the indices array
 * @param val - pointer to the values array
 * @param ind_val - pointer to the requested memory allocation to free
 * @param index - pointer to the requested memory allocation to free
 *
 * @return
 */
void update_values(int **ind, double **val, int ind_val, int index){
	(*ind)[index] = ind_val;
	(*val)[index] = 1.0;
}

/**
 * This function adds a constraint to the solver.
 *
 * @param model - pointer to the solver
 * @param dim - the sudoku board size
 * @param i - pointer to the variables index
 * @param j - pointer to the variables index
 * @param k - pointer to the variables index
 * @param x - pointer to the variables index
 * @param y - pointer to the variables index
 * @param z - pointer to the variables index
 * @param LP - the type of the problem
 *
 * @return
 * 0 on success or 1 otherwise.
 */
int addconstaints(Solution_model* model, int dim, int *i, int *j, int *k, int *x, int *y, int *z){
	for ((*i) = 0; (*i) < dim; (*i)++)
		for ((*j) = 0; (*j) < dim; (*j)++) {
			for ((*k) = 0; (*k) < dim; (*k)++)
				update_values(&(model->ind), &(model->val),((*x)*dim*dim)+((*y)*dim)+(*z),(*k));
			if(GRBaddconstr(model->model, dim, model->ind, model->val, GRB_EQUAL, 1.0, NULL))
					return 1;
		}

	return 0;
}

/**
 * This function adds all sudoku constraints to the solver.
 *
 * @param sud - a pointer to a sudoku game
 * @param model - pointer to the solver
 * @param LP - the type of the problem
 *
 * @return
 * 0 on success or 1 otherwise.
 */
int add_Sudoku_Constr(Sudoku *sud, Solution_model* model, int LP){

	int i = 0, j = 0, k = 0, l, t, count;
	int m = (sud->board->row);
	int n = (sud->board->column);
	int dim = m * n;
	/*Cell Constraints*/
	if(addconstaints(model, dim, &i,&j,&k,&i,&j,&k))
		return 1;
	/*Row Constraints*/
	if(addconstaints(model, dim, &k,&j,&i,&i,&j,&k))
		return 1;
	/*Column Constraints*/
	if(addconstaints(model, dim, &k,&i,&j,&i,&j,&k))
		return 1;
	/*Block Constraints*/
	for (k = 0; k < dim; k++) {
		for (l = 0; l < n; l++) {
			for (t = 0; t < m; t++) {
				count = 0;
				for (i = l*m; i < (l+1)*m; i++)
					for (j = t*n; j < (t+1)*n; j++) {
						update_values(&(model->ind), &(model->val),i*dim*dim+j*dim+k, count);
						count++;
					}
				if(!LP){
					if(GRBaddconstr(model->model, dim, model->ind, model->val, GRB_EQUAL, 1.0, NULL))
						return 1;
				}
				else
					if(GRBaddconstr(model->model, dim, model->ind, model->val, GRB_EQUAL, 1.0, NULL))
						return 1;
			}
		}
	}
	return 0;
}

/**
 * This function updates the variables bounds and types attributes for the problem.
 *
 * @param sud - a pointer to a sudoku game
 * @param model - pointer to the solver
 * @param LP - the type of the problem
 *
 * @return
 * 0 on success or 1 otherwise.
 */
void update_up_lb_vtype(Sudoku *sudoku, Solution_model *model, int dim, int LP){
	int i, j, k;
	for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++) {
			for (k = 0; k < dim; k++) {
				if (sudoku->board->grid[i][j] == k+1)
					model->lb[i*dim*dim+j*dim+k] = 1.0;
				else
					model->lb[i*dim*dim+j*dim+k] = 0.0;
				if(LP)
					model->vtype[i*dim*dim+j*dim+k] = GRB_CONTINUOUS;
				else
					model->vtype[i*dim*dim+j*dim+k] = GRB_BINARY;
				model->ub[i*dim*dim+j*dim+k] = 1.0;
			}
		}
}

/**
 * This function returns all possible values for a given cell.
 *
 * @param sud - a pointer to a sudoku game
 * @param x - row coordinate of the cell
 * @param y - column coordinate for the cell
 * @param values - boards possible values
 * @param dim - the size of the sudoku board
 *
 * @return
 * 0 on success or 1 otherwise.
 */
double* all_possible_cell_values(Sudoku *sudoku, int x, int y, double *values, int dim){
	double *cell_values;
	int i , j , index = 0;
	if(!(cell_values = (double *)malloc(dim*sizeof(double)))){
		failure_print_for("malloc");
		return NULL;
	}
	for(i = 0; i <= x; i ++)
		for(j = 0; j <= y; j++)
			if(!sudoku->board->grid[i][j])
				index++;

	for( i = 0 ; i < dim ; i ++ )
		cell_values[i] = values[x*dim*dim+y*dim+i];
	return cell_values;
}

/**
 * This function randomly chooses a value for a given cell.
 *
 * @param sudoku - a pointer to a sudoku game
 * @param values - boards possible values
 * @param threshold - threshold for "guess" command.
 * @param x - row coordinate of the cell
 * @param y - column coordinate for the cell
 * @param dim - the size of the sudoku board
 * @param print_hint -if the command is "guess_hint"
 *
 * @return
 * 0 on success or 1 otherwise.
 */
int randomly_best_value(Sudoku *sudoku, double *values, double threshold, int x, int y, int dim, int print_hint){

	int i, index = 0, result = 0;
	double rand_value;
	double *cell_values;
	int *weighted_cell_values;
	if(!(cell_values = all_possible_cell_values(sudoku, x, y, values, dim)))
		return -1;
	if(sudoku->board->grid[x][y])
		return 0;
	if(!(weighted_cell_values = (int *)malloc(dim*sizeof(int)))){
		failure_print_for("malloc");
		return -1;
	}
	rand_value = (rand() % 10)/ 10;
	for(i = 0 ; i < dim; i ++)
		weighted_cell_values[i] = 0;

	for(i = 0 ; i < dim; i ++)
		if(cell_values[i] > threshold){
			if(not_valid(sudoku->board,x,y ,i + 1))
				continue;
			if(cell_values[i] > rand_value){
				weighted_cell_values[index] = i+1;
				index ++;
			}
		}
	if(threshold < 0)
		if(print_hint)
			guess_hint_print(weighted_cell_values,cell_values,index,x+1,y+1);

	if(index > 1)
		result = weighted_cell_values[(rand() % index)];
	else
		result = weighted_cell_values[0];
	free(weighted_cell_values);
	free(cell_values);
	return result;
}

/**
 * This function upgrades the solution board.
 *
 * @param sudoku - a pointer to a sudoku game
 * @param x_val - boards possible values
 * @param threshold - threshold for "guess" command.
 * @param hint_x - row coordinate of the cell
 * @param hint_y - column coordinate for the cell
 * @param dim - the size of the sudoku board
 * @param print_hint -if the command is "guess_hint"
 * @param LP - the type of the problem.
 *
 * @return
 * 0 on success or 1 otherwise.
 */
int update_solution_grid(Sudoku *sudoku, double *x_val, int dim, int LP, float threshold, int hint_x, int hint_y){
	int i, j, k;
	for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++)
			if(!LP){
				for (k = 0; k < dim; k++)
					if (x_val[i*dim*dim+j*dim+k] == 1.0)
							sudoku->solution->grid[i][j] = k+1;
			}
			else{
				if(i == hint_x && j == hint_y)
					sudoku->solution->grid[i][j] = randomly_best_value(sudoku, x_val, threshold, i, j, dim, 1);
				else
					sudoku->solution->grid[i][j] = randomly_best_value(sudoku, x_val, threshold, i, j, dim, 0);
				if(sudoku->solution->grid[i][j] < 0)
					return 1;
			}
	return 0;

}

/**
 * This function initiate the Gurobi solver.
 *
 * @param sudoku - a pointer to the sudoku game
 * @param di	 - the sudoku game board size
 * @param LP	 - the type of the problem
 * @return
 * NULL if memory allocation failure occurs
 * pointer to the new solver
 */
Solution_model* init_model(Sudoku *sudoku, int dim, int LP){
	Solution_model* new = (Solution_model *)malloc(sizeof(Solution_model));
	new->env   = NULL;
	new->model = NULL;
	if((new->lb = (double *)malloc(dim*dim*dim * sizeof(double))))
		if((new->ub = (double *)malloc(dim*dim*dim * sizeof(double))))
			if(( new->vtype = (char *)malloc(dim*dim*dim * sizeof(char))))
				if(( new->x_val = (double *)malloc(dim*dim*dim * sizeof(double))))
					if(( new->ind = (int *)malloc(dim * sizeof(int))))
						if(( new->val = (double *)malloc(dim * sizeof(double))))
							if(( new->obj = (double *)malloc(dim*dim*dim * sizeof(double)))){
								update_up_lb_vtype(sudoku,new,dim,LP);
								return new;
							}
	failure_print_for("calloc");
	return NULL;
}

/**
 * This function creates an objective function for the solver.
 *
 * @param sudoku - a pointer to the sudoku game
 * @param dim	 - the sudoku game board size
 * @return
 */
void create_objective_function(Solution_model *model, Sudoku *sudoku, int dim){
	int i, j, k;
	for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++) {
			for (k = 0; k < dim; k++) {
				if (sudoku->board->grid[i][j] == k+1){
					model->obj[i*dim*dim+j*dim+k] = 0.0;
				}
				else{
					model->obj[i*dim*dim+j*dim+k] = 1.0;
				}

			}
		}
}


int Gurobi_solve(Sudoku *sudoku, int LP, float threshold, int hint_x, int hint_y){

	int dim = (sudoku->board->row)*(sudoku->board->column);
	int error;

	Solution_model* solver = init_model(sudoku,dim, LP);
	if (!(error = GRBloadenv(&(solver->env), "sudoku.log"))){
		if(!LP){
			if((error = GRBnewmodel(solver->env, &(solver->model), "sudoku", dim*dim*dim, solver->obj, solver->lb, NULL, solver->vtype, NULL))){
				destroy_Solution_model(solver);
				return 1;
			}
		}
		else{
			create_objective_function(solver,sudoku, dim);
			if((error = GRBnewmodel(solver->env, &(solver->model), "sudoku", dim*dim*dim, solver->obj, solver->lb, solver->ub, solver->vtype, NULL))){
				destroy_Solution_model(solver);
				return 1;
			}
		}
		if(LP)
			if((error = GRBsetintattr(solver->model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE))){
				destroy_Solution_model(solver);
				return 1;
			}
		if(!(error = GRBsetintparam(GRBgetenv(solver->model), "OutputFlag", 0)))
			if(!(error = add_Sudoku_Constr(sudoku, solver, LP)))
				if (!(error = GRBoptimize(solver->model)))
					if (!(error = GRBwrite(solver->model, "sudoku.lp")))
						if (!(error = GRBgetintattr(solver->model, GRB_INT_ATTR_STATUS, &(solver->optimstatus))))
							if (!(error = GRBgetdblattr(solver->model, GRB_DBL_ATTR_OBJVAL, &(solver->objval))))
								if(!(error = GRBsetintparam(GRBgetenv(solver->model), "OutputFlag", 0))){
									if (solver->optimstatus == GRB_INF_OR_UNBD){
										destroy_Solution_model(solver);
										return 2;
									}
									if(!(error = GRBgetdblattrarray(solver->model, GRB_DBL_ATTR_X, 0, dim*dim*dim, solver->x_val)))
										error = update_solution_grid(sudoku, solver->x_val, dim, LP, threshold, hint_x, hint_y);
								}
	}
	destroy_Solution_model(solver);
	if(!error)
		return 0;
	return 1;

}
