#include "main_aux.h"

void line_too_long_error(){
	printf("ERROR: Line is too long.\n");
}

void command_not_valid_in_current_mode_error(char* command){
	printf("ERROR: The command %s is not valid in current mode.\n", command);
}

void missing_path_parameter(){
	printf("ERROR: Missing path parameter.\n");
}

void print_invalid_parameter(int param_num){
	printf("ERROR: The %d parameter is not valid.\n", param_num);

}

void print_line_seperator(int row, int col){
	int i = 0;
	int n = row * col;
	for (i = 0; i < (4*n+row+1); ++i) {
		printf("-");
	}
	printf("\n");
}

void print_start_of_game(){
	printf("Sudoku Game\n------\n");
}
void invalid_command_print(){
	printf("ERROR: Invalid command!\n");
}
void file_open_error_print_solve(){
	printf("ERROR: File doesn't exist or cannot be opened.\n");
}
void file_open_error_print_edit(){
	printf("ERROR: File cannot be opened.\n");
}
void mark_errors_value_print(){
	printf("ERROR: Field \"marked errors\" value should be 0 or 1.\n");
}
void guess_value_print(){
	printf("ERROR: Field \"guess\" value should be 0 or 1.\n");
}
void set_cell_value_error_print(int n){
	printf("ERROR: Given value is not in range 0-%d.\n", n);
}

void fixed_cell_error_print(){
	printf("ERROR: Cell's value is fixed!\n");
}

void erroneous_solution_print(){
	printf("The current puzzle solution is erroneous.\n");
}

void successful_solution_print(){
	printf("Puzzle solved successfully!\n");
}

void enter_command_print(){
	printf("Enter your command: ");
}

void errors_in_board_print(){
	printf("ERROR: Current board contains erroneous values!\n");
}

void validation_is_good_print(){
	printf("Validation passed: board is solvable!\n");
}

void validation_failed_print(){
	printf("Validation failed: board is unsolvable.\n");
}

void illegal_X_value(int x){
	printf("ERROR: The given value is not in range 0-%d.\n", x);
}

void generate_error_nonempty_board(){
	printf("ERROR: The board is not empty!\n");
}

void generator_failed_x1000(){
	printf("ERROR: Puzzle generator failed.\n");
}


void no_moves_2_undo(){
	printf("ERROR: No moves to undo.\n");
}

void undo_print(int x,int y,int z1,int z2){
	if(z1 == 0)
		printf("Undo %d,%d: from _ to %d\n",x,y,z2);
	else if(z2 == 0)
		printf("Undo %d,%d: from %d to _\n",x,y,z1);
	else
		printf("Undo %d,%d: from %d to %d\n",x,y,z1,z2);
}

void no_moves_2_redo(){
	printf("ERROR: no moves to redo.\n");
}
void redo_print(int x,int y,int z1,int z2){
	if(z1 == 0)
		printf("Redo %d,%d: from _ to %d.\n",x,y,z2);
	else if (z2 == 0)
		printf("Redo %d,%d: from %d to _.\n",x,y,z1);
	else
		printf("Redo %d,%d: from %d to %d.\n",x,y,z1,z2);
}

void edit_mode_validation_error(){
	printf("ERROR: Board without a solution cannot be saved.\n");
}

void file_cannot_be_opened(){
	printf("ERROR: File specified cannot be opened.\n");
}

void file_cannot_be_modified_error(char* path){
	printf("ERROR: File specified %s cannot be modified.\n", path);
}

void game_saved_to_file_print(const char* str){
	printf("Game saved to file: %s\n", str);
}

void error_hint_cell_index_print(int n){
	printf("ERROR: Cell's index is not in range 1-%d\n",n);
}

void cell_contains_already_value_error(){
	printf("ERROR: The cell already contains a value!\n");
}

void hint_set_to_cell_print(int z){
	printf("Hint: set cell to %d\n",z);
}

void board_is_unsolvable_print(){
	printf("ERROR: The current board is unsolvable!\n");
}

void print_number_of_solutions(int sol_num){
	printf("Number of possible solutions: %d\n", sol_num);
}

void one_solution_message(){
	printf("This is a good board - there is only one solution!\n");
}

void multiple_solutions_message(){
	printf("The puzzle has more than 1 solution, try to edit it further!\n");
}

void cell_set_in_autofill_message(int x, int y, int z){
	printf("Cell <%d,%d> is set to %d.\n",x,y,z);
}

void board_reset_message(){
	printf("Board is reset.\n");
}

void exit_message(){
	printf("Exiting...\n");
}

void failure_print_for(char *s){
	printf("ERROR: %s has failed",s);
	perror(" ");
	printf("Please try again.\n");
}

void guess_hint_print(int *w, double *c, int len, int x, int y){
	int i;
	printf("The following scores were calculated for the cell <%d,%d>:\n",x ,y);
	for(i = 0 ; i < len; i ++)
		if(w[i])
			printf("     The value %d with the score of %f.\n ",w[i], c[(w[i] - 1)]);
}
