#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "sudoku.h"

int save_to_file(Sudoku* sudoku, const char* path){
	FILE* sudoku_file =  fopen(path, "w");
	int row = sudoku->board->row, col = sudoku->board->column;
	int i, j;

	if(!sudoku_file){
		file_cannot_be_modified_error();
		return 0;
	}

	/* First line */
	if(fprintf(sudoku_file, "%d %d\n", row, col) != 2){
		failure_print_for("scanf");
		return 0;
	}

	/* Sudoku board */
	for(i = 0; i < row*col; ++i){
		/* Print row */
		for(j = 0; j < row*col; ++j){
			if(sudoku->board->grid[i][j]){
				fprintf(sudoku_file, "%d", sudoku->board->grid[i][j]);
				if(sudoku->fixed->grid[i][j])
					fprintf(sudoku_file, ".");
			}
			else {
				fprintf(sudoku_file, "0");
			}
			fprintf(sudoku_file, " ");
		}
		fprintf(sudoku_file, "\n");
	}
	fclose(sudoku_file);
	return 1;
}

/* Don't forget to free any previous board before loading a new board! */
Sudoku* load_from_file(const char* path){
	FILE* sudoku_file;
	Sudoku* sudoku = NULL;
	int row, col, i, j, cur_cell;
	char dot;
	if(!(sudoku_file =  fopen(path, "r"))){
		file_cannot_be_modified_error();
		return NULL;
	}

	if(fscanf(sudoku_file, "%d", &row) !=1 )
		failure_print_for("fscanf");
	if(fscanf(sudoku_file, "%d", &col) != 1)
		failure_print_for("fscanf");
	sudoku = createSudoku(row, col);
	for(i = 0; i < row*col; ++i){
		for(j = 0; j < row*col; ++j){
			if(fscanf(sudoku_file, "%d", &cur_cell)!= 1){
				failure_print_for("fscanf");
			};
			if(cur_cell != 0)
				sudoku->board->grid[i][j] = cur_cell;

			dot = fgetc(sudoku_file);
			if(dot == EOF)
				failure_print_for("fgetc");
			if(dot == '.')
				sudoku->fixed->grid[i][j] = 1;
		}
	}
	fclose(sudoku_file);
	return sudoku;


}
