#ifndef STDIN_HANDLER_H_
#define STIDN_HANDLER_H_

/**
 * STDIN_HANDLER Summary :
 *
 * A container that reads a new line that represent a command, and interpret it.
 * A valid move can be one of the following :
 * solve X,
 * edit X,
 * mark_errors X,
 * print_board,
 * set X Y Z ,
 * validate,
 * guess X,
 * generate X Y,
 * undo,
 * redo,
 * save X,
 * hint X Y,
 * guess_hint X Y,
 * num_solutions
 * autofill
 * reset,
 * exit .
 * By the line interpretation, the container creates the next move for the player.
 * The container supports the following functions :
 *
 * destroyMove		- Frees all memory resources associated with a Move.
 * parse			- Reads a new line and returns a new move by the line interpretation.
 *
 */

#include <string.h>
#include <stddef.h>
#include "main_aux.h"

#define To_Int(x) x-'0'
#define DELIMETER  " \t\n\r"

/**
 * Type used for the representation of a command.
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
	INVALIDCOMMAND,
	INVALIDMODECOMMAND

} COMMAND;

/**
 * Type used for the representation of a move.
 */
typedef struct move {
	COMMAND command;
	int x_val;
	int y_val;
	int z_val;
	float threshold;
	char* path;
} Move;

/**
 * This function frees all memory allocations associated with a move.
 *
 * @param move - pointer to the requested memory allocation to free.
 * @return
 */
void destroyMove(Move *move);

/**
 * This function checks if a move has a NULL path.
 *
 * @param move -a pointer to a given move to check.
 *
 * @return
 * 1 if move has a NULL path or o  otherwise.
 */
int has_no_path(Move **move);

/**
 * This function reads a new line and creates a new move by interpreting the line.
 * On success the function returns a pointer to the desired move. If an error occurs,
 * the function terminates.
 *
 * @param
 * @return NULL if memory allocation failure occurs or reached EOF.
 * pointer to the move otherwise.
 *
 * */
Move* parse(int mode);



#endif /* SDTIN_HANDLER_H_ */
