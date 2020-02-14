#ifndef GAME_AUX_H_
#define GAME_AUX_H_

/**
 *  GAME_AUX summary :
 *
 * A container that holds all auxiliary functions used in the game.
 * Using a given move, the container executes the move's command attribute using the moves parameters.
 * The container supports the following functions :
 *
 * createHead			Creates a head and assign it to a given node pointer.
 * load_new_sudoku		Executes the "solve X"  or "edit X" command, by loading a sudoku from file.
 * set					Executes the "set X Y Z" command
 * validate				Executes the "validate" command.
 * generate				Executes the "generate X Y" command
 * undo					Executes the "undo" command
 * redo					Executes the "redo" command
 * autofill				Executes the "autofill" command.
 * save					Executes the "save X" command
 * hint					Executes the "hint X Y" command.
 * numsolutions			Executes the "num_solution" command.
 * reset				Executes the "reset" command.
 * exitgame				Executes the "exit" command.
 * guess				Executes the "guess X" command.
 * guess_hint			Executes the "guess_hint X Y" command.
 *
 */

#include "stdin_handler.h"
#include "sol_counter.h"
#include "file_handler.h"
#include "gurobi_handler.h"

/**
 * Type used for the representation of all of a given execution changes using a doubly-linked list structure.
 */
typedef struct node  {
	int *x;
	int *y;
	int *oldz;
	int *newz;
	int set;
	struct node *next;
	struct node *prev;
} Node;

/**
 * This function creates a head and assign it to a given node pointer.
 *
 * @param head -a pointer to a node.
 *
 * @return
 * 0 on success
 * 1  otherwise
 */
int createHead(Node **head);

/**
 * This function executes the "solve X"  or "edit X" command, by loading a sudoku from file.
 *
 * @param s -a pointer to the sudoku game.
 * @param list -a pointer to the changes list.
 * @param move -a pointer to the current move attributes.
 *
 *
 * @return
 * 0 on success
 * 1  otherwise
 */
int load_new_sudoku(Sudoku** s,Node **list,Move *move);


/**
 * This function executes the "set X Y Z" command.
 * @param move -a pointer to the current move attributes.
 * @param sudoku -a pointer to the sudoku game.
 * @param mode -a pointer to the current game mode.
 * @param list -a pointer to the changes list.
 * @param erroneous -a pointer to the game erroneous status.
 *
 * @return
 * 0 on success
 * 1  otherwise
 */
int set(Move *move,Sudoku *sudoku, int mode,Node **list, int *erroneous);

/**
 * This function executes the "validate" command.
 * @param sudoku -a pointer to the sudoku game.
 * @param erroneous -a pointer to the game erroneous status.
 *
 * @return
 * 0 on success
 * 1  otherwise
 */
int validate(Sudoku* sudoku, int *erroneous);

/**
 * This function executes the "generate X Y" command.
 * @param s -a pointer to the sudoku game.
 * @param move -a pointer to the current move attributes.
 * @param mode -a pointer to the current game mode.
 * @param list -a pointer to the changes list.
 * @param erroneous -a pointer to the game erroneous status.
 *
 * @return
 * 0 on success
 * 1  otherwise
 */
int generate(Sudoku* s, Move *move,Node **list, int *erroneous);

/**
 * This function executes the "undo" command.
 * @param list -a pointer to the changes list.
 * @param sudoku -a pointer to the sudoku game.
 * @param reset -a bit represents reset mode.
 * @param erroneous -a pointer to the game erroneous status.
 *
 * @return
 * 0 on success
 * 1  otherwise
 */
void undo(Node **list, Sudoku* sudoku,int reset, int *erroneous);

/**
 * This function executes the "redo" command.
 * @param list -a pointer to the changes list.
 * @param sudoku -a pointer to the sudoku game.
 * @param erroneous -a pointer to the game erroneous status.
 *
 * @return
 * 0 on success
 * 1  otherwise
 */
void redo(Node **list, Sudoku* sudoku, int *erroneous);

/**
 * This function executes the "autofill" command.
 * @param sudoku -a pointer to the sudoku game.
 * @param list -a pointer to the changes list.
 * @param erroneous -a pointer to the game erroneous status.
 *
 * @return
 * 0 on success
 * 1  otherwise
 */
int autofill(Sudoku* sudoku, Node **list, int *erroneous);

/**
 * This function executes the "save X" command.
 * @param sudoku -a pointer to the sudoku game.
 * @param move -a pointer to the current move attributes.
 * @param mode -a pointer to the current game mode.
 * @param erroneous -a pointer to the game erroneous status.
 *
 * @return
 * 0 on success
 * 1  otherwise
 */
int save(Sudoku *sudoku ,const char *path,int mode, int *erroneous);

/**
 * This function executes the "hint X Y" command.
 * @param sudoku -a pointer to the sudoku game.
 * @param move -a pointer to the current move attributes.
 * @param erroneous -a pointer to the game erroneous status.
 *
 * @return
 * 0 on success
 * 1  otherwise
 */
int hint(Sudoku *sudoku,Move *move, int *erroneous);

/**
 * This function executes the "num_solution" command.
 * @param sudoku -a pointer to the sudoku game.
 * @param erroneous -a pointer to the game erroneous status.
 *
 * @return
 * 0 on success
 * 1  otherwise
 */
int numsolutions(Sudoku* sudoku, int *erroneous);

/**
 * This function executes the "reset" command.
 * @param sudoku -a pointer to the sudoku game.
 * @param list -a pointer to the changes list.
 * @param erroneous -a pointer to the game erroneous status.
 *
 * @return
 * 0 on success
 * 1  otherwise
 */
void reset(Sudoku *sudoku, Node **list, int *erroneous);

/**
 * This function executes the "exit" command.
 * @param sudoku -a pointer to the sudoku game.
 * @param list -a pointer to the changes list.
 *
 * @return
 * 0 on success
 * 1  otherwise
 */
void exitgame(Sudoku *sudoku,Node **list);

/**
 * This function executes the "guess X" command.
 * @param sudoku -a pointer to the sudoku game.
 * @param threshold - the threshold for the command.
 * @param list -a pointer to the changes list.
 * @param erroneous -a pointer to the game erroneous status.
 *
 * @return
 * 0 on success
 * 1  otherwise
 */
int guess(Sudoku *sudoku, float threshold, Node **list, int *erroneous);

/**
 * This function executes the "guess_hint X Y" command.
 * @param sudoku -a pointer to the sudoku game.
 * @param x - the row coordinate of the desired cell.
 * @param y - the column coordinate of the desired cell.
 *
 * @return
 * 0 on success
 * 1  otherwise
 */
int guess_hint(Sudoku *sudoku, int x, int y, int *erroneous);

#endif /* GAME_AUX_H_ */
