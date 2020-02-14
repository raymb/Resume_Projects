#include "stdin_handler.h"

/**
 * This function creates a new move. On success the function returns a pointer
 * to the new move. If a memory allocation failure occurs , the function terminates.
 *
 * @param command - the command for the move.
 * @param x - first argument for the move, coordinate for the row.
 * @param y - second argument for the move, coordinate for the column.
 * @param z - third argument for the move.
 * @param filepath - a path for the move.
 * @param threshold - a threshold for the move.
 *
 * @return
 * NULL if memory allocation failure occurs or
 * a pointer to new_move otherwise.
 */
Move* create_move(COMMAND command, int x, int y, int z, char filepath[MAX_LENGTH], float threshold) {
	Move* new_move = (Move *)malloc(sizeof(Move));
	if (!new_move) {/*failure check*/
		failure_print_for("malloc");
		return NULL;
	}
	new_move->command = command;
	new_move->x_val = x;
	new_move->y_val = y;
	new_move->z_val = z;
	new_move->threshold = threshold;
	if ( (!filepath) || !strcmp(filepath, "")) {
		new_move->path = NULL;
		return new_move;
	}
	new_move->path = filepath;
	return new_move;
}

void destroyMove(Move *move) {
	if(!move)
		return;
	if(!move->path)
		free(move->path);
	free(move);
	return;
}

/**
 * This function checks if the given input is a vaild command.
 *
 * @param input - a string which represents the desired command.
 * @param valid_command - a string which represents the desired command.
 * @return
 * 1 if the input is a valid command or
 * 0 otherwise.
 */
int parsed_command_is_valid(char *input, char *valid_command) {
	return !strcmp(input, valid_command);
}

/**
 * This function gets the desired command from the string input. On success the function returns
 *  a valid command. If the input is invalid , the function returns a proper invalid command.
 *
 * @param input - a string which represents the desired command
 * @return
 * COMMAND type stating the input given in the current mode.
 */
COMMAND get_command(char *input, int mode) {
	int valid_mode_command = 1;
	if (!input)
		return INVALIDCOMMAND;
	switch (input[0]) {
	case 'a':
		if (parsed_command_is_valid(input, "autofill"))
			if ((valid_mode_command = (mode == Solve)))
				return AUTOFILL;
		break;
	case 'e':
		if (parsed_command_is_valid(input, "edit"))
			return EDIT;
		if (parsed_command_is_valid(input, "exit"))
			return EXIT; break;
	case 'g':
		if (parsed_command_is_valid(input, "generate"))
			if ((valid_mode_command = (mode == Edit)))
				return GENERATE;
		if (parsed_command_is_valid(input, "guess"))
			if ((valid_mode_command = (mode == Solve)))
				return GUESS;
		if (parsed_command_is_valid(input, "guess_hint"))
			if ((valid_mode_command = (mode == Solve)))
				return GUESS_HINT;
		break;
	case 'h':
		if (parsed_command_is_valid(input, "hint"))
			if ((valid_mode_command = (mode == Solve)))
				return HINT;
		break;
	case 'm':
		if (parsed_command_is_valid(input, "mark_errors"))
			if ((valid_mode_command = (mode == Solve)))
				return MARK_ERRORS;
		break;
	case 'n':
		if (parsed_command_is_valid(input, "num_solutions"))
			if ((valid_mode_command = (mode != Init)))
				return NUM_SOLUTIONS;
		break;
	case 'p':
		if (parsed_command_is_valid(input, "print_board"))
			if ((valid_mode_command = (mode != Init)))
				return PRINT_BOARD;
		break;
	case 'r':
		if (parsed_command_is_valid(input, "reset"))
			if ((valid_mode_command = (mode != Init)))
				return RESET;
		if (parsed_command_is_valid(input, "redo"))
			if ((valid_mode_command = (mode != Init)))
				return REDO;
		break;
	case 's':
		if (parsed_command_is_valid(input, "solve"))
			return SOLVE;
		if (parsed_command_is_valid(input, "set"))
			if ((valid_mode_command = (mode != Init)))
				return SET;
		if (parsed_command_is_valid(input, "save"))
			if ((valid_mode_command = (mode != Init)))
				return SAVE;
		break;
	case 'u':
		if (parsed_command_is_valid(input, "undo"))
			if ((valid_mode_command = (mode != Init)))
				return UNDO;
		break;
	case 'v':
		if (parsed_command_is_valid(input, "validate"))
			if ((valid_mode_command = (mode != Init)))
				return VALIDATE;
		break;
	}
	if(!valid_mode_command){
		command_not_valid_in_current_mode_error(input);
		return INVALIDMODECOMMAND;
	}
	return INVALIDCOMMAND;
}

/*
 * This function gets the next token from the input , set by the defined DELIMETER.
 *
 * @return the next token of input.
 */
char* next_token() {
	return strtok(NULL, DELIMETER);
}

/*
 * This function returns the integer represented by the given string str.
 * @param str - a string represents an integer.
 * @return
 * integer represented in the string if successfull or
 * (-1) otherwise.
 */
int to_int(char* str) {
	float result = 0;
	if (!str)
		return -1;
	result = atoi(str);
	return result;
}

/*
 * This function returns the float represented by the given string str.
 * @param str - a string represents a float.
 * @return
 * float represented in the string if successful or
 * (-1) otherwise.
 */
float to_float(char* str) {
	float result;
	if (!str)
		return -1;
	result = atof(str);
	return result;
}

/*
 * This function gets a char from stdin.
 *
 * @return
 * next char from stdin or
 * (-1) otherwise.
 */
int get_next_char(){
	int ch;
	ch = getchar();
    if (feof(stdin)) {
    	failure_print_for("getchar");
    	return -1;
    }
    return ch;
}

/*
 * This function returns the integer represented by the given token.
 * @param token - a string represents an integer.
 * @return
 * integer represented in the token if successful or
 * (-1) otherwise.
 */
int next_int_param(char *token){
	if (!token)
		return -1;
	return to_int(token);

}

/*
 * This function returns the float represented by the given token.
 * @param token - a string represents an integer.
 * @return
 * float represented in the token if successful.
 * (-1) otherwise.
 */
float next_float_param(char *token){
	if (!token)
		return -1;
	return to_float(token);
}

/*
 * This function checks if a non-negative parameter x is invalid.
 * @param x - a non-negative integer.
 * @return
 * 1 if invalid or
 * 0 otherwise
 */
int not_valid_parameter(int x){
	return (x < 0);
}

/*
 * This function read an integer from input and assign it to a given parameter.
 * @param param - a param to assign.
 * @return
 * 0 if assigned successfully or
 * 1 otherwise.
 */
int int_param_insert(int *param){
	return not_valid_parameter((*param = next_int_param((next_token()))));
}

/*
 * This function read a float from input and assign it to a given parameter.
 * @param param - a param to assign.
 * @return
 * 0 if assigned successfully or
 * 1 otherwise.
 */
int float_param_insert(float *param){
	return not_valid_parameter((*param = next_float_param((next_token()))));
}

int has_no_path(Move **move){
	return !((*move)->path) || !strcmp((*move)->path,"");
}

/*
 * This function updates a given moves parameters using a given line and the game mode.
 * @param move - a move to update.
 * @param input - stdin line to interpret.
 * @param mode - the current game mode.
 *
 * @return
 */
void update_parameters(Move *move, char *input, int mode){
	char *token;
	COMMAND command;
	int x = 0, y = 0, z = 0, invalid = 0;
	float threshold = -1;
	token = strtok(input, DELIMETER);/* set input for token */
	if (!token)
		return;
	command = get_command(token, mode);
	move->path = NULL;
	switch (command) {
	case SOLVE:
	case SAVE:
	case EDIT:
		token = next_token();
		if (!token) {
			if (command == EDIT) break;
			invalid = 1;
			missing_path_parameter();
			break;
		}
		move->path = token;
		break;
	case GUESS:
		if((invalid = float_param_insert(&threshold)))
			print_invalid_parameter(1);
		break;
	case GENERATE:
	case GUESS_HINT:
	case HINT:
	case MARK_ERRORS:
	case SET:
		if((invalid = int_param_insert(&x))){
			print_invalid_parameter(1);break;
		}
		if(command == MARK_ERRORS)
			break;
		if((invalid = int_param_insert(&y))){
			print_invalid_parameter(2);break;
		}
		if(command == SET)
			if((invalid = int_param_insert(&z)))
				print_invalid_parameter(3);
		break;
	default:
		break;
	}
	if((token = next_token()))
		invalid = 1;
	if(invalid)
		move->command = INVALIDCOMMAND;
	else
		move->command = command;
	move->x_val = x;
	move->y_val = y;
	move->z_val = z;
	move->threshold = threshold;
}

Move* parse(int mode) {
	char curr_ch, input[MAX_LENGTH];
	Move *move = create_move(INVALIDCOMMAND, 0, 0, 0, NULL, 0);
	int length;
    for(length = 0; ((curr_ch = get_next_char())!='\n') && (length < MAX_LENGTH) ; length++ ) {
    	if (curr_ch < 0)
    		return NULL;
    	input[length] = (char) curr_ch;
    	input[length + 1] = '\0';
    }
    if(length == 0){
    	destroyMove(move);
    	return parse(mode);
    }
    if (length == MAX_LENGTH){
    	while ((curr_ch = get_next_char()) != '\n');
    	line_too_long_error();
    	return move;
    }
	update_parameters(move, input, mode);
	return move;
}


