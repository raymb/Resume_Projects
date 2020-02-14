#include "game_aux.h"

/**
 * This function creates a new node and initialize it. On success the function returns a pointer
 * to the new node. If a memory allocation failure occurs, the function terminate.
 *
 * @param x - denotes the x coordinate changes
 * @param y - denotes the y coordinate changes
 * @param oldz - denotes the old value.
 * @param newz - denotes the new value.
 *
 * @return
 * NULL  if a memory allocation failure occurs
 * pointer to the new node otherwise.
 */
Node* createNode(int *x,int *y,int *oldz,int *newz) {
	Node* newNode;
	if(!(newNode = (Node*)malloc(sizeof(Node)))){
		failure_print_for("malloc");
		return NULL;
	}
	newNode->x = x;
	newNode->y = y;
	newNode->oldz = oldz;
	newNode->newz = newz;
	newNode->set = 1;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

/*
 * This function frees all memory allocations associated with a node.
 *
 * @param *node - a pointer to node
 * @return
 */
void destroyNode(Node *node){
	free(node->newz);
	free(node->oldz);
	free(node->x);
	free(node->y);
	free(node);
}

/*
 * This function checks if a given node is the head of the list.
 *
 * @param *list - a pointer to a node
 * @return
 *  1 if head, 0 otherwise.
 */
int isHead(Node *list){
	if(!list->prev)
		return 1;
	return 0;
}

/*
 * This function deletes a given node tail by freeing all memory associated with the tail nodes.
 *
 * @param *node - a pointer to node
 * @return
 */
void deleteTail(Node **list){
	Node *temp = *list;
	while((*list)->next != NULL)
		(*list)= (*list)->next;
	while(temp != *list){
		(*list) = (*list)->prev;
		destroyNode((*list)->next);
		(*list)->next = NULL;
	}
}

/*
 * This function inserts a new node as next to a given one.
 *
 * @param node - a pointer to node.
 * @param x - denotes the x coordinate changes
 * @param y - denotes the y coordinate changes
 * @param oldz - denotes the old value.
 * @param newz - denotes the x new value.
 *
 * @return
 * 0 on sucess or 1 otherwise
 */
int insertNode(Node **list,int **x,int **y,int **oldz,int **newz) {
	Node* newNode = createNode(*x,*y,*oldz,*newz);
	if(!newNode)
		return 1;
	if(!(*list)){/*head*/
		(*list) = newNode;
		return 0;
	}
	if((*list)->next != NULL)
		deleteTail(list);
	(*list) -> next = newNode;
	newNode->prev = (*list);
	*list = (*list)->next;
	return 0;
}

int createHead(Node **head){
	int *p = NULL;
	return insertNode(head,&p,&p,&p,&p);
}

int load_new_sudoku(Sudoku** s,Node **list,Move *move){
	if((*s))
		destroySudoku((*s));
	while(!isHead(*list))
			*list = (*list)->prev;
	deleteTail(list);
	if(!move)
		return 1;
	(*s) = load_from_file(move->path);
	if(!(*s))
		return 1;
	if((*s)->empty_cells < 0)
		return 2;
	return 0;
}


/*
 * This function update errors in a given sudoku board.
 *
 * @param sudoku - a pointer to a sudoku games
 * @return
 * 0 if no errors found or 1 otherwise
 */
int update_errors(Sudoku *sudoku){
	int i,j,error = 0;
	for(i = 0; i <sudoku->board_size;i++)
		for(j= 0 ; j < sudoku->board_size;j++){
			if((sudoku->marked_errors->grid[i][j] = not_valid(sudoku->board,i,j,sudoku->board->grid[i][j])))
				 error = 1;
		}
	return error;
}

/*
 * This function checks if a given positive parameter is valid.
 *
 * @param param - a positive parameter.
 * @param max - max value for the parameter.
 * @return
 * 1 if valid or 0 otherwise
 */
int valid_parameter(int param, int max){
	return (param > 0 && param <= max);
}

/*
 * This function validates a set move attributes.
 *
 * @param move - a pointer to a set move.
 * @param n - max value for the parameters.
 * @return
 * 1 if valid or 0 otherwise
 */
int legal_values(Move *move,int n){
	if (valid_parameter(move->x_val,n- 1))
		if(valid_parameter(move->y_val,n- 1))
			if(valid_parameter(move->z_val,n) || move->z_val == 0)
				return 1;
			else
				print_invalid_parameter(3);
		else
			print_invalid_parameter(2);
	else
		print_invalid_parameter(1);
	return 0;
}

/*
 * This function inserts a single value to a given parameter.
 *
 * @param p - a pointer to the parameter.
 * @param val - the value to assign.
 * @return
 * 1 if an error occured or 0 otherwise.
 */
int insert_single_value(int val,int **p){
	if(!(*p = (int *)malloc(sizeof(int)))){
		failure_print_for("malloc");
		return 1;
	}
	*(*p) = val;
	return 0;
}

/*
 * This function assign a set change attributes using a given move.
 *
 * @param move - a pointer to the set move.
 * @param x - the row coordinate to assign.
 * @param y - the column coordinate to assign.
 * @param newz - the new value to assign.
 *
 * @return
 * 1 if error occured or 0 otherwise.
 */
int set_insert_values(Move *move,int **x, int **y, int **newz){
	if(!insert_single_value(move->x_val,x))
		if(!insert_single_value(move->y_val,y))
			if(!insert_single_value(move->z_val,newz))
				return 0;
	return 1;
}

int set(Move *move,Sudoku *sudoku, int mode,Node **list, int *erroneous){
	int size = sudoku->board_size;
	int *x = NULL ,*y = NULL,*newz = NULL ,*oldz = NULL;
	if(mode != Edit)
		if(sudoku->fixed->grid[move->x_val -1][move->y_val -1]){
			fixed_cell_error_print();
			return 0;
		}
	if(!legal_values(move,size))
		return 0;
	if(set_insert_values(move,&x,&y,&newz))
		return 1;
	if(insert_single_value(sudoku->board->grid[(*x)-1][(*y)-1],&oldz))
		return 1;
	if(insertNode(list,&x,&y,&oldz,&newz))
		return 1;
	sudoku->board->grid[(*x)-1][(*y)-1] = *newz;
	if((!*oldz && *newz)){
			sudoku->empty_cells--;
	}
	if((*oldz && !*newz)){
		if(*newz)
			sudoku->empty_cells--;
	}
	*erroneous = update_errors(sudoku);
	(*list)->set = 1;
	print_board(sudoku,0);
	if(sudoku->empty_cells == 0 && mode == Solve){
		if(!(*erroneous)){
			successful_solution_print();
			return 2;
		}
		else
			erroneous_solution_print();
	}

	return 0;
}

int validate(Sudoku* sudoku, int *erroneous){
	int result;
	if((*erroneous = update_errors(sudoku))){
		errors_in_board_print();
		return 0;
	}
	if((result = Gurobi_solve(sudoku, 0, 0, 0, 0)))
		if(result != 1){
			failure_print_for("Gurobi tool");
			return 1;
		}
		else
			validation_failed_print();
	else
		validation_is_good_print();
	return 0;
}

/*
 * This function checks if a given board is not empty.
 *
 * @param board - a pointer to a board.
 * @return
 * 0 if empty found or 1 otherwise.
 */
int is_not_empty(Board *board){
	int i ,j, n =board->column * board -> row;
	for(i =0 ; i < n ; i ++)
		for( j = 0 ; j < n ; j++)
			if(board->grid [i][j])
				return 1;
	return 0;
}

/**
 * This function creates changes arrays for a node and initialize it.
 *
 * @param x - denotes the x coordinates change.
 * @param y - denotes the y coordinates change.
 * @param oldz - denotes the old values.
 * @param newz - denotes the x new values.
 * @param size - denotes the max size of changes per node.
 * @return
 * 0  if a memory allocation failure occurs
 * 1 otherwise.
 */
int create_changes_array(int **x,int **y, int **oldz, int **newz,int size){
	if(!(*x = (int *)calloc(size, sizeof(int))))
		return 0;
	if(!(*y = (int *)calloc(size, sizeof(int))))
			return 0;
	if(!(*oldz = (int *)calloc(size, sizeof(int))))
			return 0;
	if(!(*newz = (int *)calloc(size, sizeof(int))))
			return 0;
	return 1;
}

/**
 * This function fills board state for a generation node.
 *
 * @param x - denotes the x coordinates.
 * @param y - denotes the y coordinates.
 * @param oldz - denotes the old values.
 * @param newz - denotes the x new values.
 * @param size - denotes the max size of changes per node.
 * @return
 * 0  if a memory allocation failure occurs
 * 1 otherwise.
 */
void fill_board_state(int **x,int **y, int **oldz, int **newz, Board *board){
	int i, j, size = board->column*board->row , index = 0;
	for( i = 0 ;i < size; i++)
		for( j = 0 ; j < size ; j ++)
			if(board->grid[i][j]){
				(*x)[index] = i;
				(*y)[index] = j;
				(*oldz)[index] = board->grid[i][j];
				(*newz)[index] = 0;
				index++;
			}
}

/**
 * This function update fix board of a given sudoku game.
 *
 * @param sudoku - a pointer to a sudoku game.
 * @return
 */
void update_fixed(Sudoku *sudoku){
	int i,j;
	for(i = 0; i < sudoku->board_size ; i++)
		for(j = 0 ; j < sudoku ->board_size ; j ++)
			if(sudoku->board->grid[i][j])
				sudoku->fixed->grid[i][j] = 1;
}

/**
 * This function reverts a board to a given original.
 * @param board - a pointer to a board.
 * @param original - a pointer to the original board.
 * @return
 */
void revert_board(Board *b1, Board *original){
	int dim = b1->column*b1->row;
	int j , i;
	for(i = 0; i < dim ; i++)
		for(j = 0 ; j < dim ; j ++)
			b1->grid[i][j] = original->grid[i][j];
}

/**
 * This function updates changes arrays using a given board.
 *
 * @param x_val - denotes the x coordinates change.
 * @param y_val - denotes the y coordinates change.
 * @param oldz - denotes the old values.
 * @param newz - denotes the x new values.
 * @param original - a pointer to a board.
 *
 * @return
 * number of changes updated.
 */
int update_node_values(Board *original, int *x_val, int *y_val, int* oldz, int *newz){
	int dim = original->column*original->row;
	int j , i;
	int result = 0;
	for(i = 0; i < dim ; i++)
		for(j = 0 ; j < dim ; j ++)
			if(original->grid[i][j]){
					x_val[result] = i+1;
					y_val[result]= j+1;
					oldz[result] = original->grid[i][j];
					newz[result] = 0;
					result++;
			}
	return result;
}

int generate(Sudoku* s, Move *move,Node **list, int *erroneous){
	int counter = 0, xval, yval, i= 0, changes_index;
	int *x,*y,*oldz,*newz,result , dim = (s->board_size)*(s->board_size);
	Board *original = copyBoard(s->board);
	if( move->x_val > (s->empty_cells)){
		set_cell_value_error_print((s->empty_cells));
		return 0;
	}
	if(move->y_val > move->x_val){
		set_cell_value_error_print(move->x_val);
		return 0;
	}
	if(!create_changes_array(&x,&y,&oldz,&newz,dim)){
		failure_print_for("malloc");
		return 1;
	}
	while(counter < 1000){
		if(!generator(s,move->x_val)){
			revert_board(s->board, original);
			counter++;
			continue;
		}
		if((*erroneous = update_errors(s))){
			revert_board(s->board, original);
			counter++;
			continue;
		}
		changes_index = update_node_values(original, x, y,oldz, newz);
		if((result = Gurobi_solve(s, 0, 0, 0, 0))){
			if(result != 2){
				failure_print_for("Gurobi tool");
				return 1;
			}
			revert_board(s->board, original);
			counter++;
			continue;
		}
		clear_matrix(s->board->grid,s->board_size);
		i = 0;
		while(i < move->y_val){
			yval = rand() % s->board_size ;
			xval = rand() % s->board_size ;
			if(s->board->grid[xval][yval])
				continue;
			x[changes_index + i] = xval +1;
			y[changes_index + i] = yval +1;
			oldz[changes_index + i] = 0;
			newz[changes_index + i] = s->solution->grid[xval][yval];
			s->board->grid[xval][yval] = s->solution->grid[xval][yval];
			s->empty_cells--;
			i++;
		}
		break;
	}
	if(counter == 1000){
		generator_failed_x1000();
		return 0;
	}
	if(insertNode(list,&x,&y,&oldz,&newz))
		return 1;
	(*list)->set = changes_index + move->y_val;
	destroyBoard(original);
	print_board(s,0);
	return 0;
}

void undo(Node **list, Sudoku* sudoku,int reset, int *erroneous){
	Node *current = *list;
	int x ,y,oldz, len = current->set,i, newz;
	if(isHead(current)){
		no_moves_2_undo();
		return;
	}
	for(i = 0 ; i < len; i++){
		x = current->x[i]-1 , y=current->y[i]-1,oldz=current->oldz[i], newz = current->newz[i];
		if(x == -1)
			continue;
		sudoku->board->grid[x][y] = oldz ;
		if(oldz && !newz)
			sudoku->empty_cells--;
		if(!oldz && newz)
			sudoku->empty_cells++;
		if(!(current->set) &&  !oldz)
			sudoku->fixed->grid[x][y] = 0;
	}
	*erroneous = update_errors(sudoku);
	if(reset)
		return;
	for( i = 0 ; i < len ; i ++)
		if((current->x[i]-1) != -1)
			undo_print(current->x[i],current->y[i],current->newz[i],current->oldz[i]);
	print_board(sudoku, 0);
	*list = (*list)->prev;
}

void redo(Node **list, Sudoku* sudoku, int *erroneous){
	Node *current = *list;
	int x ,y,newz, len,i, oldz;
	if(!current->next){
		no_moves_2_redo();
		return;
	}
	current = current-> next;
	len = current->set ;
	for(i = 0 ; i < len; i++){
		x = current->x[i]-1 , y=current->y[i]-1, newz=current->newz[i], oldz = current->oldz[i];
		if(x == -1)
			continue;
		sudoku->board->grid[x][y] = newz ;
		if(!oldz && newz)
			sudoku->empty_cells--;
		if(oldz && !newz)
			sudoku->empty_cells++;
		if(current->set == 0 && newz > 0)
			sudoku->fixed->grid[x][y] = 1;
	}
	*erroneous = update_errors(sudoku);
	for( i = 0 ; i < len ; i ++)
		if((current->x[i]-1) != -1)
			redo_print(current->x[i],current->y[i],current->oldz[i],current->newz[i]); /* TODO PRINT */
	*list = (*list)->next;
	print_board(sudoku, 0);

}

/*
 * This function checks if a given set has a single valid value.
 *
 * @param set - a pointer to a set.
 * @param size - size of the set.
 * @return
 * 1 if has single value or 0 otherwise
 */
int has_single_legal_value(Set *set, int size){
	int counter = 0,i,value = 0;
	for(i = 0; i < size ; i++)
		if(set->valid_values[i]){
			counter++;
			value = i+1 ;
		}
	if(counter == 1)
		return value;
	return 0;
}

int autofill(Sudoku* sudoku, Node **list, int *erroneous){
	Set *set = createSet(sudoku->board_size);
	int i,j,value = 0,current_cell = 0,*x,*y,*oldz,*newz,dim = sudoku->board_size, size = dim*dim,success = 0;
	if(*erroneous){
		errors_in_board_print();
		return 0;
	}
	if(!create_changes_array(&x,&y,&oldz,&newz,size)){
		failure_print_for("malloc");
		return 1;
	}
	for(i =0; i < dim;i++)
		for( j = 0 ; j < dim ; j ++){
			if(sudoku->board->grid[i][j])
				continue;
			make_valid_values(set,sudoku->board,i,j);
			if((value = has_single_legal_value(set,sudoku->board_size))){
				x[current_cell] = i+1;
				y[current_cell] = j+1;
				oldz[current_cell] = 0;
				newz[current_cell] = value;
				current_cell++;
				success = 1;
			}
			flushSet(set,sudoku->board_size);
		}
	destroySet(set);
	for( i = 0 ; i < current_cell ; i ++){
		cell_set_in_autofill_message(x[i],y[i],newz[i]);
		sudoku->board->grid[(x[i])-1][(y[i])-1] = newz[i];
		sudoku->empty_cells--;
	}
	if(success){
		*erroneous = update_errors(sudoku);
		print_board(sudoku, 0);
		if(insertNode(list,&x,&y,&oldz,&newz))
			return 1;
		(*list)->set = current_cell;
	}
	return 0;
}

int save(Sudoku *sudoku ,const char *path,int mode, int *erroneous){
	int result;
	if(mode == Edit){
		if((*erroneous) == 1){
			errors_in_board_print();
			return 0;
		}
		if((result = Gurobi_solve(sudoku, 0, 0, 0, 0))){
			if (result  != 2){
				failure_print_for("Gurobi tool");
				return 1;
			}
			else{
				edit_mode_validation_error();
				return 0;
			}
		}
	}
	if(!save_to_file(sudoku, path, mode))
		return 1;
	game_saved_to_file_print(path);
	return 0;
}

int hint(Sudoku *sudoku,Move *move, int *erroneous){
	int x = move ->x_val - 1, y = move ->y_val- 1 ,result, dim = sudoku->board_size;
	if(!((valid_parameter(move->x_val,dim)) && (valid_parameter(move->y_val,dim)))){
		error_hint_cell_index_print(dim);
		return 0;
	}
	if(*erroneous){
		errors_in_board_print();
		return 0;
	}
	if(sudoku->fixed->grid[x][y]){
		fixed_cell_error_print();
		return 0;
	}
	if(sudoku->board->grid[x][y]){
		cell_contains_already_value_error();
		return 0;
	}
	;
	if((result = Gurobi_solve(sudoku , 0 , 0, 0, 0))){
		if(result == 1){
			failure_print_for("Gurobi tool");
			return 1;
			}
		board_is_unsolvable_print();
	}
	if(result == 0)
		hint_set_to_cell_print(sudoku->solution->grid[x][y]);
	return 0;
}

int numsolutions(Sudoku* sudoku, int *erroneous){
	int solutions;
	if((*erroneous)){
		errors_in_board_print();
		return 0;
	}
	if((solutions = count(sudoku)) < 0)
		return 1;
	print_number_of_solutions(solutions);
	if(solutions == 1)
		one_solution_message();
	if(solutions > 1)
		multiple_solutions_message();
	return 0;
}

void reset(Sudoku *sudoku, Node **list, int *erroneous){
	while(!isHead(*list)){
		undo(list,sudoku,1, erroneous);
		*list = (*list)->prev;
	}
	board_reset_message();
	print_board(sudoku, 0);
}

void exitgame(Sudoku *sudoku,Node **list){
	if(sudoku != NULL)
		destroySudoku(sudoku);
	if(!(*list))
		return;
	while(!isHead(*list))
		*list = (*list)->prev;
	deleteTail(list);
	destroyNode(*list);
	exit_message();
}

int guess(Sudoku *sudoku, float threshold, Node **list, int *erroneous){
	int dim = sudoku->board_size;
	int i, j, *x, *y, *oldz, *newz, counter = 0, value;
	int error;
	if(!create_changes_array(&x,&y,&oldz,&newz,dim*dim)){
		failure_print_for("malloc");
		return 1;
	}
	if((*erroneous = update_errors(sudoku))){
		errors_in_board_print();
		return 0;
	}
	if((error = Gurobi_solve(sudoku , 1, threshold , 0, 0))){
		if(error == 1){
			failure_print_for("Gurobi tool");
			return 1;
			}
		board_is_unsolvable_print();
		return 0;
	}
	for(i = 0; i< dim; i++)
		for(j = 0; j < dim ; j++){
			if((value = sudoku->solution->grid[i][j]))
				if(!sudoku->board->grid[i][j]){
					x[counter] = i+1;
					y[counter] = j+1;
					oldz[counter] = 0;
					newz[counter] = value;
					sudoku->board->grid[i][j] = value ;
					sudoku->empty_cells--;
					counter++;
			}
		}
	print_board(sudoku, 0);
	if(insertNode(list,&x,&y,&oldz,&newz))
		return 1;
	(*list)->set = counter;
	return 0;
}

int guess_hint(Sudoku *sudoku, int x_val, int y_val, int *erroneous){

	int dim = sudoku->board_size;
	int  *x, *y, *oldz, *newz;
	int error;
	if(!create_changes_array(&x,&y,&oldz,&newz,dim*dim)){
			failure_print_for("malloc");
			return 1;
	}
	if((*erroneous = update_errors(sudoku))){
		errors_in_board_print();
		return 0;
	}
	if(sudoku->fixed->grid[x_val- 1][y_val -1]){
		fixed_cell_error_print();
		return 0;
	}
	if(sudoku->board->grid[x_val- 1][y_val -1]){
		cell_contains_already_value_error();
		return 0;
	}
	if((error = Gurobi_solve(sudoku , 1 , -1, x_val - 1, y_val - 1))){
		if(error == 1){
			failure_print_for("Gurobi tool");
			return 1;
			}
		board_is_unsolvable_print();
	}
	return 0;
}
