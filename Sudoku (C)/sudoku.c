#include "sudoku.h"


Board* createBoard(int row, int column){
	Board *new_board= (Board *)malloc(sizeof(Board));
	int **grid;

	if (!new_board){
		failure_print_for("malloc");
		return NULL;
	}

	grid = createGrid(row,column);
	if(!grid)
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
	grid = (int **)calloc(row*column, sizeof(int*));

	if(!grid){
		failure_print_for("calloc");
		return NULL;
	}

	for(i = 0; i < row*column; i++){
		grid[i] =(int *)calloc(row*column,sizeof(int));
		if(!grid[i]){
			failure_print_for("calloc");
			return NULL;
		}
	}

	return grid;
}

Sudoku* createSudoku(int block_row, int block_column){
	Sudoku *new_sudoku;
	Board *new_board , *new_solution, *new_fixed, *new_marked_errors;

	new_sudoku = (Sudoku *)malloc(sizeof(Sudoku));
	if(!new_sudoku){
		failure_print_for("malloc");
		return NULL;
	}

	new_fixed = createBoard(block_row, block_column);
	if(!new_fixed)
		return NULL;

	new_marked_errors = createBoard(block_row, block_column);
	if(!new_marked_errors)
		return NULL;

	new_board = createBoard(block_row, block_column);
	if(!new_board)
		return NULL;

	new_solution = createBoard(block_row,block_column);
	if(!new_solution)
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


