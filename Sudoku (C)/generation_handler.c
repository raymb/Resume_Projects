#include "generation_handler.h"

void flushSet(Set *set, int length){
	int i = 0 ;
	for(i = 0 ; i < length ; i ++){
		set->valid_values[i] = 1 ;
	}
}
Set* createSet(int length){
	Set* new_set = (Set *)malloc(sizeof(Set));
	if(!new_set){
		failure_print_for("malloc");
		return NULL;
	}
	new_set->valid_values = (int *)calloc(length ,sizeof(int));
	if(!new_set->valid_values){
		failure_print_for("calloc");
		return NULL;
	}
	flushSet(new_set, length);
	return new_set;
}
void destroySet(Set *set){
	free(set->valid_values);
	free(set);
}
int not_valid(Board *board,int x, int y ,int z){
	int i ,j , row = board->row,column = board ->column;
	if(z == 0)
		return 0;
	for(i = 0 ; i< row*column;i++){
		if(board->grid[x][i] == z && i != y)
			return 1;
		if(board->grid[i][y] == z && i != x)
				return 1;
	}
	for(i = (x/row)*row; i < (1+ x/row)*row;i ++)
		for(j = (y/column)*column; j < (1 + y/column)*column; j++){
			if(i==x && j==y)
				continue;
			if(board->grid[i][j]== z)
				return 1;
		}
	return 0;
}
void make_valid_values(Set *set,Board *board, int x, int y){
	int i = 0,row = board-> row ,column = board->column;
	for(i=0;i < row*column ; i++)
		if(not_valid(board,x,y,i+1)){
			set->valid_values[i] = 0;
			continue;
		}
}

int get_random_value(Set *set,int length){
	int rand_value = 0 ,i ,counter = 0 ,res;
	int *options;
	options = (int *)calloc(length,sizeof(int));
	for(i = 0; i < length ; i++)
		if(set->valid_values[i]){
				options[counter] = i +1 ;
				counter ++;
			}
	if(counter == 0)
		return -1;
	rand_value = (rand() % counter);
	res = options[rand_value];
	free(options);
	return res ;
}
void clear_matrix(int **mat,int size){
	int i ,j;
	for(i =0 ; i < size ; i ++)
		for( j = 0 ; j < size ; j++)
			if(mat[i][j])
				mat[i][j] = 0;
}
int generator(Sudoku *s,int numtofill){
	int value, xval, yval,filled_cells = 0;
	Set *set = createSet(s->board_size);
	if(!set)
		return 0;
	while(filled_cells < numtofill ){
		yval = rand() % s->board_size ;
		xval = rand() % s->board_size ;
		if(s->board->grid[xval][yval])
			continue;
		make_valid_values(set,s->board,xval,yval);
		value = get_random_value(set,s->board_size);
		if(value == -1){
			clear_matrix(s->board->grid,s->board_size);
			destroySet(set);
			return 0;
		}
		s->board->grid[xval][yval] = value;
		filled_cells ++;
		flushSet(set,s->board_size);
	}
	destroySet(set);
	return 1;
}
