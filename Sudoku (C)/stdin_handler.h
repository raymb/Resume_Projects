#ifndef STDIN_HANDLER_H_
#define STIDN_HANDLER_H_

/**
 * STDIN_HANDLER Summary :
 *
 * A container that reads a new line that represent a command, and interpret it.
 * A valid move can be one of the following :
 * 	set X Y Z ,
 * 	hint X Y,
 * 	validate,
 * 	restart,
 * 	exit .
 * By the interpretation of the line, the container creates the next move for the
 * player of the Sudoku game.
 * The container supports the following functions .
 *
 * destroyMove		- Frees all memory resources associated with a Move
 * parse			- Reads a new line and creates a new move by interpreting the line
 *
 */

#include <string.h>
#include <stddef.h>
#include "main_aux.h"


#define MAX_LENGTH 1024
#define To_Int(x) x-'0'
#define DELIMETER  " \t\n\r"

typedef enum mode {
	Init,
	Edit,
	Solve
}MODE;

/**
 * Type used for the representation of a command
 */
typedef enum command {
	SOLVE,
	EDIT,
	MARK_ERRORS,
	PRINT_BOARD,
	SET,
	VALIDATE,
	GUESS,
	GENERATE,
	UNDO,
	REDO,
	SAVE,
	HINT,
	GUESS_HINT,
	NUM_SOLUTIONS,
	AUTOFILL,
	RESET,
	EXIT,
	INVALIDCOMMAND

} COMMAND;
/**
 * Type used for the representation of a move
 */
typedef struct move {
	COMMAND command;
	int x_val;
	int y_val;
	int z_val;
	char* path;
} Move;
Move* create_move(COMMAND command, int x, int y, int z, char filepath[MAX_LENGTH]);
/**
 * This function frees all memory allocations associated with a move.
 *
 * @param move - pointer to the requested memory allocation to free
 * @return
 */
void destroyMove(Move *move);
/**
 * This function reads a new line and creates a new move by interpreting the line.
 * On success the function returns a pointer to the desired move. If an error occurs ,
 * the function terminates.
 *
 * @param
 * @return NULL if memory allocation failure occurs or reached EOF
 * pointer to the move otherwise
 *
 * */
int has_no_path(Move **move);
Move* parse(int mode);



#endif /* SDTIN_HANDLER_H_ */
