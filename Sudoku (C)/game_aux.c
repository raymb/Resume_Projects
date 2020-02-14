#include "game_aux.h"

Node* createNode(int *x,int *y,int *oldz,int *newz) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->x = x;
	newNode->y = y;
	newNode->oldz = oldz;
	newNode->newz = newz;
	newNode->set = 1;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}
void destroyNode(Node *node){
	free(node->newz);
	free(node->oldz);
	free(node->x);
	free(node->y);
	free(node);
}
int isHead(Node *list){
	if(!list->prev)
		return 1;
	return 0;
}
void deleteTail(Node **list){
	Node *temp = *list;
	if(isHead(*list))
		return;
	while((*list)->next != NULL)
		(*list)= (*list)->next;
	while(temp != *list){
		(*list) = (*list)->prev;
		destroyNode((*list)->next);
		(*list)->next = NULL;
	}
}

void insertNode(Node **list,int **x,int **y,int **oldz,int **newz) {
	Node* newNode = createNode(*x,*y,*oldz,*newz);
	if(!(*list)){
		(*list) = newNode;
		return;
	}
	if((*list)->next != NULL)
		deleteTail(list);
	(*list) -> next = newNode;
	newNode->prev = (*list);
	*list = (*list)->next;
}
void createHead(Node **head){
	int *p = NULL;
	insertNode(head,&p,&p,&p,&p);
}

Sudoku* solve(Sudoku*s,Node **list,Move *move){
	if(s != NULL)
		destroySudoku(s);
	while(!isHead(*list))
			*list = (*list)->prev;
	deleteTail(list);
	move = NULL;
	if(!move)
		return s;
	return load_from_file(move->path);
}
Sudoku* edit(Sudoku*s,Node **list,Move *move){
	if(s)
		destroySudoku(s);
	while(!isHead(*list))
			*list = (*list)->prev;
	deleteTail(list);
	return load_from_file(move->path);
}
void print_board(Sudoku* sud){ /* prints board according to format in PDF */
	int i= 0, j= 0, row = sud->board->row,col = sud->board->column;
	for (i = 0; i < row*col; ++i) { /* loop that runs the amount of board rows */
		if(i % row == 0)
			print_line_seperator(row, col); /* print line seperator in same format as in PDF every m amount of rows */
		for (j = 0; j < row*col; ++j){  /* inner loops to print according to block format.  */
			if(j%col == 0)
				printf("|");
			printf(" ");
			if (sud->board->grid[i][j] != 0)
				printf("%2d", sud->board->grid[i][j]);
			else
				printf(" 0");
			if(sud->fixed->grid[i][j])
				printf(".");
			else
				if(sud->mark_errors && sud->marked_errors->grid[i][j])
					printf("*");
				else
					printf(" ");
		}
		printf("|\n");
	}
	print_line_seperator(row, col);
}

int update_errors(Sudoku *sudoku){
	int i,j,error = 0;
	for(i = 0; i <sudoku->board_size;i++)
		for(j= 0 ; j < sudoku->board_size;j++){
			if((sudoku->marked_errors->grid[i][j] = not_valid(sudoku->board,i,j,sudoku->board->grid[i][j])))
				 error = 1;
		}
	return error;
}
int valid_parameter(int param, int max){
	return (param > 0 && param <= max);
}
int legal_values(Move *move,int n){
	if (valid_parameter(move->x_val,n))
		if(valid_parameter(move->y_val,n))
			if(valid_parameter(move->z_val,n) || move->z_val == 0)
				return 1;
			else{
				print_invalid_parameter(1);
			}
		else{
			print_invalid_parameter(2);
			}
	else{
		print_invalid_parameter(3);
		}
	return 0;
}

int insert_single_value(int val,int **p){
	*p = (int *)malloc(sizeof(int));
	if(!*p){
		failure_print_for("malloc");
		return 0;
	}
	*(*p) = val;
	return 1;
}
int set_insert_values(Move *move,int **x, int **y, int **newz){
	if(insert_single_value(move->x_val,x))
		if(insert_single_value(move->y_val,y))
			if(insert_single_value(move->z_val,newz))
				return 1;
	return 0;
}
int set(Move *move,Sudoku *sudoku, int mode,Node **list, int *erroneous){
	int size = sudoku->board_size;
	int *x = NULL ,*y = NULL,*newz = NULL ,*oldz = NULL;
	if(mode != EDIT)
		if(sudoku->fixed->grid[move->x_val -1][move->y_val -1]){
			fixed_cell_error_print();
			return 0;
		}
	if(!legal_values(move,size))
		return 0;
	if(!set_insert_values(move,&x,&y,&newz))
		return 1;
	if(!insert_single_value(sudoku->board->grid[(*x)-1][(*y)-1],&oldz))
		return 1;
	insertNode(list,&x,&y,&oldz,&newz);
	sudoku->board->grid[(*x)-1][(*y)-1] = *newz;
	if(*newz > 0)
		sudoku->empty_cells--;
	else
		sudoku->empty_cells++;
	*erroneous = update_errors(sudoku);
	print_board(sudoku);
	if(sudoku->empty_cells == 0 && mode == SOLVE){
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
	int result = 0;
	if((*erroneous = update_errors(sudoku))){
		errors_in_board_print();
		return 0;
	}
	result = ILPsolve(sudoku);
	if(result == -1){
		failure_print_for("Gurobi");
		return 1;
	}
	if(result == 0)
		validation_is_good_print();
	else
		validation_failed_print();
	return 0;
}
int is_not_empty(Board *board){
	int i ,j, n =board->column * board -> row;
	for(i =0 ; i < n ; i ++)
		for( j = 0 ; j < n ; j++)
			if(board->grid [i][j])
				return 1;
	return 0;
}

int create_changes_array(int **x,int **y, int **oldz, int **newz,int size){
	*x = (int *)malloc(size*sizeof(int));
	*y = (int *)malloc(size*sizeof(int));
	*oldz = (int *)malloc(size*sizeof(int));
	*newz = (int *)malloc(size*sizeof(int));
	if(!*x || !*y || !*oldz || !*newz)
		failure_print_for("malloc");
		return 0;
	return 1;
}

void fill_board_state(int **x,int **y, int **oldz, int **newz, Board *board){
	int i, j, size = board->column*board->row , index = 0;
	for( i = 0 ;i < size; i++)
		for( j = 0 ; j < size ; j ++)
			if(board->grid[i][j]){
				*x[index] =i;
				*y[index]=j;
				*oldz[index]=board->grid[i][j];
				*newz[index]=0;
				index++;
			}
}

int generate(Sudoku* s, Move *move,Node **list){
	int counter = 0,xval,yval,i= 0;
	int *x,*y,*oldz,*newz,result;


	if(move->y_val > move->x_val || move->x_val > (s->board_size*s->board_size)){
		set_cell_value_error_print(s->board_size*s->board_size);
		return 0;
	}
	if(!create_changes_array(&x,&y,&oldz,&newz,move->y_val + move->x_val + 1)){
		failure_print_for("malloc");
		return 1;
	}
	if (is_not_empty(s ->board)){
		generate_error_nonempty_board();
		return 0;
	}
	while(counter < 1000){
		if(!generator(s,move->x_val)){
			counter++;
			continue;
		}
		if(update_errors(s)){
			clear_matrix(s->board->grid,s->board_size);
			counter++;
			continue;
		}
		result = ILPsolve(s);
		if(result == -1){
			failure_print_for("Gurobi");
			return 1;
		}
		if(result == 1){
			clear_matrix(s->board->grid,s->board_size);
			counter++;
			continue;
		}
		clear_matrix(s->board->grid,s->board_size);
		while(i < move->y_val){
			yval = rand() % s->board_size ;
			xval = rand() % s->board_size ;
			if(s->board->grid[xval][yval])
				continue;
			x[i] = xval +1;
			y[i] = yval +1;
			oldz[i] = 0;
			newz[i] = s->solution->grid[xval][yval];
			s->board->grid[xval][yval] = newz[i];
			s->empty_cells--;
			i++;
		}
		oldz[0] = move->y_val;
		break;
	}
	if(counter == 1000){
		generator_failed_x1000();
		return 0;
	}
	update_fixed(s);
	print_board(s);
	insertNode(list,&x,&y,&oldz,&newz);
	(*list)->set = 0;
	return 0;
}


void undo(Node **list, Sudoku* sudoku,int reset, int *erroneous){/*add undo/redo autofill command*/
	Node *current = *list;
	int x ,y,oldz, len = 1,i;
	if(isHead(current)){
		no_moves_2_undo();
		return;
	}
	if(!(current->set)){
		len = current->oldz[0];
		current->oldz[0] = 0;
	}
	for(i = 0 ; i < len; i++){
		x = current->x[i]-1 , y=current->y[i]-1,oldz=current->oldz[i];
		if(x == -1)
			continue;
		sudoku->board->grid[x][y] = oldz ;
		if(oldz)
			sudoku->empty_cells--;
		else
			sudoku->empty_cells++;
		if(!(current->set) && len>1)
			sudoku->fixed->grid[x][y] = 0;
	}
	if(reset)
		return;
	*erroneous = update_errors(sudoku);
	for( i = 0 ; i < len ; i ++)
		if(current->x[i]-1)
			undo_print(current->x[i],current->y[i],current->newz[i],current->oldz[i]);
	print_board(sudoku);
	if(len > 1)
		current->oldz[0] = len;
	*list = (*list)->prev;
}
void redo(Node **list, Sudoku* sudoku, int *erroneous){
	Node *current = *list;
	int x ,y,newz, len = 1,i;
	if(!current->next){
		no_moves_2_redo();
		return;
	}
	current = current-> next;
	if(!(current->set)){
		len = current->oldz[0];
		current->oldz[0] = 0;
	}
	for(i = 0 ; i < len; i++){
		x = current->x[i]-1 , y=current->y[i]-1, newz=current->newz[i];
		if(x == -1)
			continue;
		sudoku->board->grid[x][y] = newz ;
		if(newz)
			sudoku->empty_cells--;
		else
			sudoku->empty_cells++;
		if(current->set == 0 && newz > 0)
			sudoku->fixed->grid[x][y] = 1;
	}
	*erroneous = update_errors(sudoku);
	for( i = 0 ; i < len ; i ++)
		if(current->x[i]-1)
			redo_print(current->x[i],current->y[i],current->oldz[i],current->newz[i]);
	if(len > 1)
		current->oldz[0] = len;
	*list = (*list)->next;
	print_board(sudoku);

}

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
int autofill(Sudoku* sudoku, Node **list, int *erroneous){/*add undo/redo command*/
	Set *set = createSet(sudoku->board_size);
	int i,j,value = 0,current_cell = 0,*x,*y,*oldz,*newz,size= sudoku->empty_cells,success = 0;
	if(*erroneous){
		errors_in_board_print();
		return 0;
	}
	if(!create_changes_array(&x,&y,&oldz,&newz,size)){
		failure_print_for("malloc");
		return 1;
	}
	for(i =0; i < sudoku->board_size;i++)
		for( j = 0 ; j < sudoku ->board_size ; j ++){
			if(sudoku->board->grid[i][j])
				continue;
			make_valid_values(set,sudoku->board,i,j);
			if((value = has_single_legal_value(set,sudoku->board_size))){
				x[current_cell] = i+1;
				y[current_cell] = j+1;
				oldz[current_cell] = 0;
				newz[current_cell] = value;
				success = 1;
			}
			else{
				x[current_cell] = 0;
				y[current_cell] = 0;
			}
			current_cell++;
			flushSet(set,sudoku->board_size);
		}
	oldz[0] = current_cell ;
	destroySet(set);
	for( i = 0 ; i < current_cell ; i ++){
		if(!x[i])
			continue;
		cell_set_in_autofill_message(x[i],y[i],newz[i]);
		sudoku->board->grid[(x[i])-1][(y[i])-1] = value;
		sudoku->empty_cells--;

	}
	if(success){
		*erroneous = update_errors(sudoku);
		print_board(sudoku);
		insertNode(list,&x,&y,&oldz,&newz);
		(*list)->set = 0;
	}
	return 0;
}

void update_fixed(Sudoku *sudoku){
	int i,j;
	for(i = 0; i < sudoku->board_size ; i++)
		for(j = 0 ; j < sudoku ->board_size ; j ++)
			if(sudoku->board->grid[i][j])
				sudoku->fixed->grid
				[i][j] = 1;
}

int save(Sudoku *sudoku ,Move *move,int mode, int *erroneous){
	int result = 6;
	if(mode == Edit){
		if((*erroneous)){
			errors_in_board_print();
			return 0;
		}
		result = ILPsolve(sudoku);
		if(result == -1){
			failure_print_for("Gurobi");
			return 1;
		}
		if(result == 1){
			edit_mode_validation_error();
			return 0;
		}
		update_fixed(sudoku);
	}
	if(!save_to_file(sudoku, move->path))
		return 1;
	game_saved_to_file_print(move->path);
	return 0;

}
int hint(Sudoku *sudoku,Move *move, int *erroneous){

	int x = move ->x_val, y = move ->y_val ,result;
	if(!legal_values(move , sudoku->board_size)){
		error_hint_cell_index_print(sudoku->board_size);
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
	result = ILPsolve(sudoku);
	if(result == -1){
		failure_print_for("Gurobi");
		return 1;
	}
	if(result == 0)
		hint_set_to_cell_print(sudoku->solution->grid[x][y]);
	else
		board_is_unsolvable_print();
	return 0;
}
int numsolutions(Sudoku* sudoku, int *erroneous){
	int solutions;
	if((*erroneous)){
		errors_in_board_print();
		return 0;
	}
	solutions = count(sudoku);
	if(solutions < 0)
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
}
void exitgame(Sudoku *sudoku,Node *list){
	if(sudoku != NULL)
		destroySudoku(sudoku);
	if(!list)
		return;
	while(!isHead(list))
		list = list->prev;
	deleteTail(&list);
	destroyNode(list);
	exit_message();
}
