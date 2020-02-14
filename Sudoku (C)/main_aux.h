#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

/**
 *  MAIN_AUX summary :
 *
 * A container that holds all print used in the game
 *
 */

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


#define MAX_LENGTH 1024

/**
 * Type used for the representation of the game mode.
 */
typedef enum mode {
	Init,
	Edit,
	Solve
}MODE;

void line_too_long_error();
void missing_path_parameter();
void command_not_valid_in_current_mode_error(char* command);
void print_invalid_parameter(int param_num);
void print_line_seperator(int row, int col);
void print_start_of_game();
void invalid_command_print();
void file_open_error_print_solve();
void file_open_error_print_edit();
void mark_errors_value_print();
void guess_value_print();
void set_cell_value_error_print(int n);
void fixed_cell_error_print();
void erroneous_solution_print();
void successful_solution_print();
void enter_command_print();
void errors_in_board_print();
void validation_is_good_print();
void validation_failed_print();
void illegal_X_value(int x);
void generate_error_nonempty_board();
void generator_failed_x1000();
void no_moves_2_undo();
void undo_print(int x,int y,int z1,int z2);
void no_moves_2_redo();
void redo_print(int x,int y,int z1,int z2);
void edit_mode_validation_error();
void file_cannot_be_opened();
void file_cannot_be_modified_error();
void game_saved_to_file_print(const char* str);
void error_hint_cell_index_print(int n);
void cell_contains_already_value_error();
void hint_set_to_cell_print(int z);
void board_is_unsolvable_print();
void print_number_of_solutions(int sol_num);
void one_solution_message();
void multiple_solutions_message();
void cell_set_in_autofill_message(int x, int y, int z);
void board_reset_message();
void exit_message();
void failure_print_for(char *s);
void guess_hint_print(int *w, double *c, int len, int x, int y);

#endif /* MAIN_AUX_H_ */
