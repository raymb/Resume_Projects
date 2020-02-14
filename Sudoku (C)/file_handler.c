#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "sudoku.h"

int save_to_file(Sudoku* sudoku, const char* path, int mode){
	FILE* sudoku_file;
	int row = sudoku->board->row, col = sudoku->board->column;
	int i, j;
	if(!(sudoku_file  =  fopen(path, "w"))){
		file_cannot_be_modified_error(path);
		return 0;
	}
	if(fprintf(sudoku_file, "%d %d\n", row, col) < 0){ failure_print_for("fprintf"); return 0;}
	for(i = 0; i < row*col; ++i){
		/* Print row */
		for(j = 0; j < row*col; ++j){
			if(sudoku->board->grid[i][j]){
				if(fprintf(sudoku_file, "%d", sudoku->board->grid[i][j]) < 0){ failure_print_for("fprintf"); return 0;}
				if(mode == Edit)
					if(fprintf(sudoku_file, ".") < 0){ failure_print_for("fprintf"); return 0;}
			}
			else {
				if(fprintf(sudoku_file, "0") < 0){ failure_print_for("fprintf"); return 0;}
			}
			if(fprintf(sudoku_file, " ") < 0) { failure_print_for("fprintf"); return 0;}
		}
		if(fprintf(sudoku_file, "\n") < 0) { failure_print_for("fprintf"); return 0;}
	}
	fclose(sudoku_file);
	return 1;
}

Sudoku* load_from_file(const char* path){
	FILE* sudoku_file;
	int row, col, i, j, cur_cell = 0;
	char dot;
	Sudoku *new = NULL;
	if(!(sudoku_file =  fopen(path, "r"))){
		file_cannot_be_opened();
		return NULL;
	}
	if(!fscanf(sudoku_file, "%d", &row)){ failure_print_for("fscanf"); return NULL;}
	if(!fscanf(sudoku_file, "%d", &col)){ failure_print_for("fscanf"); return NULL;}
	if(!(new = createSudoku(row, col)))
		return NULL;
	for(i = 0; i < row*col; ++i){
		for(j = 0; j < row*col; ++j){
			if(!fscanf(sudoku_file, "%d", &cur_cell)){ failure_print_for("fscanf"); return NULL;}
			if(cur_cell){
				new->board->grid[i][j] = cur_cell;
				new->empty_cells --;
			}
			dot = fgetc(sudoku_file);
		    if (feof(sudoku_file)) { new->empty_cells = -1; failure_print_for("fgetc");return new;}
			if(dot == '.')
				new->fixed->grid[i][j] = 1;
		}
	}
	fclose(sudoku_file);
	return new;


}
