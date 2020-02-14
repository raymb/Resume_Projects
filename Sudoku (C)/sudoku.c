#include "sudoku.h"


Board* createBoard(int row, int column){
	Board *new_board;
	int **grid;
	if (!(new_board= (Board *)malloc(sizeof(Board)))){
		failure_print_for("malloc");
		return NULL;
	}
	if(!(grid = createGrid(row,column)))
		return NULL;
	new_board->row = row;
	new_board->column = column;
	new_board->grid = grid;
	return new_board;
}

Board * copyBoard(Board *board){
	int i, j;
	int row = board->row;
	int column = board->column;
	Board *copy = createBoard(row, column);
	for(i = 0; i < row*column; i++)
		for(j = 0; j < row*column; j++)
			copy->grid[i][j] = board->grid[i][j];
	return copy;
}

int** createGrid(int row,int column){
	int **grid ,i;
	if(!(grid = (int **)calloc(row*column, sizeof(int*)))){
		failure_print_for("calloc");
		return NULL;
	}
	for(i = 0; i < row*column; i++)
		if(!(grid[i] =(int *)calloc(row*column,sizeof(int)))){
			failure_print_for("calloc");
			return NULL;
		}
	return grid;
}

Sudoku* createSudoku(int block_row, int block_column){
	Sudoku *new_sudoku;
	Board *new_board , *new_solution, *new_fixed, *new_marked_errors;
	if(!(new_sudoku = (Sudoku *)malloc(sizeof(Sudoku)))){
		failure_print_for("malloc");
		return NULL;
	}
	if(!(new_fixed = createBoard(block_row, block_column)))
		return NULL;
	if(!(new_marked_errors = createBoard(block_row, block_column)))
		return NULL;
	if(!(new_board = createBoard(block_row, block_column)))
		return NULL;
	if(!(new_solution = createBoard(block_row,block_column)))
		return NULL;

	new_sudoku->board = new_board;
	new_sudoku->solution = new_solution;
	new_sudoku->fixed = new_fixed;
	new_sudoku->mark_errors = 1;
	new_sudoku->marked_errors = new_marked_errors;
	new_sudoku->board_size = block_row*block_column;
	new_sudoku->empty_cells = (block_row*block_column)*(block_row*block_column);
	return new_sudoku;
}

void destroyGrid(int **grid,int size){
	int i;
	for(i = 0; i < size; i++)
		free(grid[i]);
	free(grid);
}

void destroyBoard(Board *board){
	if(!board)
		return ;
	destroyGrid(board->grid, board->row*board->column);
	free(board);
	return;

}

void destroySudoku(Sudoku *sudoku){
	if(!sudoku)
		return;
	destroyBoard(sudoku->board);
	destroyBoard(sudoku->solution);
	destroyBoard(sudoku->fixed);
	destroyBoard(sudoku->marked_errors);
	free(sudoku);
}

void print_board(Sudoku* sud, int solution){
	int i= 0, j= 0, row = sud->board->row,col = sud->board->column;
	int value;
	for (i = 0; i < row*col; ++i) {
		if(i % row == 0)
			print_line_seperator(row, col);
		for (j = 0; j < row*col; ++j){
			if(j%col == 0)
				printf("|");
			printf(" ");
			value = sud->board->grid[i][j];
			if(solution)
				value = sud->solution->grid[i][j];
			if (value)
				printf("%2d", value);
			else
				printf("  ");

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
