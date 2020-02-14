#include "gurobi_handler.h"

Solution_model* init_model(int dim){
	Solution_model* new = (Solution_model *)malloc(sizeof(Solution_model));
	new->env   = NULL;
	new->model = NULL;
	if((new->lb = (double *)calloc(dim*dim*dim, sizeof(double))))
		if(( new->vtype = (char *)calloc(dim*dim*dim, sizeof(char))))
			if(( new->x_val = (double *)calloc(dim*dim*dim, sizeof(double))))
				if(( new->ind = (int *)calloc(dim, sizeof(int))))
					if(( new->val = (double *)calloc(dim, sizeof(double))))
						return new;
	failure_print_for("calloc");
	return NULL;
}

void destroy_array(void *arr){
	if(!arr)
		free(arr);
}

void destroy_ILP_arrays(double* lb, char* vtype, double *x,int* ind, double* val){
	destroy_array(lb);
	destroy_array(vtype);
	destroy_array(x);
	destroy_array(ind);
	destroy_array(val);
}

void destroy_Solution_model(Solution_model *model){
	destroy_ILP_arrays(model->lb, model->vtype, model->x_val, model->ind, model->val);
	GRBfreemodel(model->model);
	GRBfreeenv(model->env);
	free(model);
}

void update_values(int *ind, double *val, int i, int j, int k, int index){
	ind[index] = i + j + k;
	val[index] = 1.0;
}


int addconstaints(Solution_model* model, int dim, int i, int j, int k, int x, int y, int z, int index){

	for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++) {
			for (k = 0; k < dim; k++)
				update_values(model->ind, model->val,x*dim*dim,y*dim,z,index);
			if(GRBaddconstr(model->model, dim, model->ind, model->val, GRB_EQUAL, 1.0, NULL))
				return 1;
		}
		return 0;
}

int add_Sudoku_Constr(Sudoku *sud, Solution_model* model){

	int i = 0, j = 0, k = 0, l, t, count;
	int m = (sud->board->row);
	int n = (sud->board->column);
	int dim = m * n;
	/*Cell Constraints*/
	if(addconstaints(model, dim, i,j,k,i,j,k,k))
		return 1;
	/*Row Constraints*/
	if(addconstaints(model, dim, k,j,i,i,j,k,i))
		return 1;
	/*Column Constraints*/
	if(addconstaints(model, dim, k,i,j,i,j,k,j))
		return 1;
	/*Block Constrainta*/
	for (k = 0; k < dim; k++) {
		for (l = 0; l < n; l++) {
			for (t = 0; t < m; t++) {
				count = 0;
				for (i = l*m; i < (l+1)*m; i++)
					for (j = t*n; j < (t+1)*n; j++) {
						update_values(model->ind, model->val,i*dim*dim,j*dim,k, j);
						count++;
					}
				if(GRBaddconstr(model->model, dim, model->ind, model->val, GRB_EQUAL, 1.0, NULL))
					return 1;
			}
		}
	}
	return 0;
}

void update_lb_vtype(Sudoku *sudoku, Solution_model *model, int dim){
	int i, j, k;
	for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++) {
			for (k = 0; k < dim; k++) {
				if (sudoku->board->grid[i][j] == k+1)
					model->lb[i*dim*dim+j*dim+k] = 1;
				else
					model->lb[i*dim*dim+j*dim+k] = 0;
				model->vtype[i*dim*dim+j*dim+k] = GRB_BINARY;
			}
		}
}

void update_solution_grid(Sudoku *sudoku, double *x_val, int dim){
	int i, j, k;
	for (i = 0; i < dim; i++)
			for (j = 0; j < dim; j++)
				for (k = 0; k < dim; k++)
					if (x_val[i*dim*dim+j*dim+k] == 1)
						sudoku->solution->grid[i][j] = k+1;

}


int ILPsolve(Sudoku *sudoku){

	int dim = (sudoku->board->row)*(sudoku->board->column);
	int res = -1;
	Solution_model* solver = init_model(dim);

	update_lb_vtype(sudoku,solver,dim);

	if (!(GRBloadenv(&(solver->env), "sudoku.log")))
		if (!(GRBnewmodel(solver->env, &(solver->model), "sudoku", dim*dim*dim, NULL, solver->lb, NULL, solver->vtype, NULL)))
			if(!(GRBsetintparam(GRBgetenv(solver->model), "OutputFlag", 0)))
				if(!(add_Sudoku_Constr(sudoku, solver)))
					if (!(GRBoptimize(solver->model)))
						if (!(GRBwrite(solver->model, "sudoku.lp")))
							if (!(GRBgetintattr(solver->model, GRB_INT_ATTR_STATUS, &(solver->optimstatus))))
								if (!(GRBgetdblattr(solver->model, GRB_DBL_ATTR_OBJVAL, &(solver->objval))))
									if(!(GRBsetintparam(GRBgetenv(solver->model), "OutputFlag", 0))){
										if (solver->optimstatus == GRB_INF_OR_UNBD){
											destroy_Solution_model(solver);
											return 1;
										}
										if(!(res = GRBgetdblattrarray(solver->model, GRB_DBL_ATTR_X, 0, dim*dim*dim, solver->x_val)))
											update_solution_grid(sudoku, solver->x_val, dim);
									}
	destroy_Solution_model(solver);
	return res;

}

