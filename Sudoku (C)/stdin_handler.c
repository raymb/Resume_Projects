#include "stdin_handler.h"

/**
 * This function creates a new move. On success the function returns a pointer
 * to the new move. If a memory allocation failure occurs , the function terminates.
 *
 * @param command - the command for the move
 * @param x - first argument for the move 	, coordinate for the row
 * @param y - second argument for the move 	, coordinate for the column
 * @param z - third  argument for the move 	, value to set
 * @return
 * NULL if memory allocation failure occurs
 * pointer to new_move otherwise
 */
Move* create_move(COMMAND command, int x, int y, int z, char filepath[MAX_LENGTH]) {
	Move* new_move = (Move *)malloc(sizeof(Move));
	if (!new_move) {/*failure check*/
		failure_print_for("malloc");
		return NULL;
	}
	new_move->command = command;
	new_move->x_val = x;
	new_move->y_val = y;
	new_move->z_val = z;
	if ( (!filepath) || !strcmp(filepath, "")) {
		new_move->path = NULL;
		return new_move;
	}
	new_move->path = (char *)malloc(sizeof(char)*MAX_LENGTH);
	if (!new_move->path) {/*failure check*/
		failure_print_for("malloc");
		return NULL;
	}
	strcpy(new_move->path, filepath);
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
 * This function gets the desired command from the string input. On success the function returns
 *  a valid command. If the input is invalid , the function returns the invalid command.
 *
 * @param input - a string which represents the desired command
 * @return
 * SET / HINT / VALIDATE / RESTART / EXIT if the input is
 * "set" / "hint" / "validate" / "restart" / "exit" respectively
 * INVALIDCOMMAND		otherwise
 */
int parsed_command_is_valid(char *input1, char *input2) {
	return !strcmp(input1, input2);
}
COMMAND get_command(char *input, int mode) { /*check word before mode && print accordingly*/
	if (!input)
		return INVALIDCOMMAND;
	switch (input[0]) {
	case 'a':
		if (mode == Solve)
			command_not_valid_in_current_mode_error(input);
			if (parsed_command_is_valid(input, "autofill"))
				return AUTOFILL;break;
	case 'e':
		if (parsed_command_is_valid(input, "edit"))
			return EDIT;
		if (parsed_command_is_valid(input, "exit"))
			return EXIT; break;
	case 'g':
		if (mode == Edit)
			if (parsed_command_is_valid(input, "generate"))
				return GENERATE; break;
		if (mode == Solve)
			if (parsed_command_is_valid(input, "guess"))
				return GUESS; break;
			if (parsed_command_is_valid(input, "guess_hint"))
				return GUESS_HINT; break;
	case 'h':
		if (mode == Solve)
			if (parsed_command_is_valid(input, "hint"))
				return HINT; break;
	case 'm':
		if (mode == Solve)
			if (parsed_command_is_valid(input, "mark_errors"))
				return MARK_ERRORS; break;
	case 'n':
		if (mode != Init)
			if (parsed_command_is_valid(input, "num_solutions"))
				return NUM_SOLUTIONS; break;
	case 'p':
		if (mode != Init)
			if (parsed_command_is_valid(input, "print_board"))
				return PRINT_BOARD;
		break;
	case 'r':
		if (mode == Init) break;
		if (parsed_command_is_valid(input, "reset"))
			return RESET;
		if (parsed_command_is_valid(input, "redo"))
			return REDO; break;
	case 's':
		if (parsed_command_is_valid(input, "solve"))
			return SOLVE;
		if (mode == Init) break;
		if (parsed_command_is_valid(input, "set"))
			return SET;
		if (parsed_command_is_valid(input, "save"))
			return SAVE; break;
	case 'u':
		if (mode != Init)
			if (parsed_command_is_valid(input, "undo"))
				return UNDO; break;
	case 'v':
		if (mode != Init)
			if (parsed_command_is_valid(input, "validate"))
				return VALIDATE; break;
	}
	return INVALIDCOMMAND;
}
/**
 * This function gets the next token from the input , set by the defined DELIMETER .
 *
 * @return the next token of input
 * */
char* next_token() {
	return strtok(NULL, DELIMETER);
}
int to_int(char* str) {
	int i, s = 0;
	if (!str)
		return -1;
	for (i = 0; i < (int)strlen(str); i++) {
		s *= 10;
		s += To_Int(str[i]);
	}
	return s;
}

float to_float(char* str) {
	int i = 0, position = 1;
	float result = 0, digit = 1;
	if (!str)
		return -1;
	while (i < (int)strlen(str)) {
		if(str[i] == '.'){
			position = -1;
			i++;
			continue;
		}
		if(position < 0)
			digit = digit / 10;
		else
			result *= 10;
		result += digit * To_Int(str[i]);
		i++;
	}
	return result;
}

int get_next_char(){
	int ch;
	ch = getchar();
    if (feof(stdin)) {
    	failure_print_for("getchar");
    	return -1;
    }
    return ch;
}

int next_int_param(char *token){
	if (!token)
		return -1;
	return to_int(token);
}

int not_valid_parameter(int x){
	return (x < 0);
}

int param_insert(int *param){
	return not_valid_parameter((*param = next_int_param((next_token()))));
}

int has_no_path(Move **move){
	return !((*move)->path) || !strcmp((*move)->path,"");
}

void update_parameters(Move *move, char *input, int mode){
	char *token, *filepath;
	COMMAND command;
	int x = 0, y = 0, z = 0, invalid = 0;
	token = strtok(input, DELIMETER);/* first token */
	    if (!token)
	    	return;
	command = get_command(token, mode);
	filepath = NULL;
	switch (command) {
	case SOLVE:
	case SAVE:
	case EDIT:
		if (mode == Init && command == SAVE){
			command_not_valid_in_current_mode_error(command);
			invalid = 1; break;
		}
		token = next_token();
		if (!token) {
			if (command == EDIT)
				break;
			invalid = 1;
			missing_path_parameter();
			break;
		}
		filepath = token; break;
	case GENERATE:
	case GUESS_HINT:
	case HINT:
	case MARK_ERRORS:
	case SET:
		if((invalid = param_insert(&x))){
			print_invalid_parameter(1);
			break;
		}
		if(command == MARK_ERRORS){
			if(x > 1 || x < 0){
				mark_errors_value_print();
				command = INVALIDCOMMAND;
			}
			break;
		}
		if((invalid = param_insert(&y))){
			print_invalid_parameter(2);
			break;
		}
		if(command == HINT)
			break;
		if(command == SET)
			if((invalid = param_insert(&z)))
				print_invalid_parameter(3);
		break;
	default:
		break;
	}
	if((token = next_token())){
		line_too_long_error();
		invalid = 1;
	}
	if(invalid)
		move->command = INVALIDCOMMAND;
	else
		move->command = command;
	move->path = filepath;
	move->x_val = x;
	move->y_val = y;
	move->z_val = z;

}


Move* parse(int mode) {
	char curr_ch, input[MAX_LENGTH], *str_from_inupt;
	Move *move = create_move(INVALIDCOMMAND, 0, 0, 0, NULL);
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
    if (!(str_from_inupt = (char*)malloc(sizeof(char)*length))){
    	failure_print_for("malloc");
    	return NULL;
    }
    strcpy(str_from_inupt, input);
	update_parameters(move, str_from_inupt, mode);
	return move;
}


