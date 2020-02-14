#include "game.h"


void run_game(){
	Sudoku *sudoku = NULL;
	Node *list = NULL;
	int mode = Init, command = -1, erroneous = 0, error = 0;
	Move *move = NULL;
	createHead(&list);
	print_start_of_game();
	enter_command_print();
	while(command != EXIT){
		move = parse(mode);
		if(!move)
			return;
		printf("Move is %d, X=%d, Y=%d, Z=%d, ", move->command,move->x_val, move->y_val, move->z_val);
		if(!move->path)
			printf("NULL in Path\n");
		else
			printf("Path=%s.\n", move->path);
		switch((command = move->command)){
			case SOLVE:
				 if(!(sudoku = solve(sudoku,&list,move)))
					 return;
				mode = Solve;
				break;
			case EDIT:
				if(has_no_path(&move))
					sudoku = createSudoku(3,3);
				else
					sudoku = edit(sudoku,&list,move);
				if(! sudoku)
					return;
				print_board(sudoku);
				mode = Edit;
				sudoku->mark_errors = 1;
				break;
			case MARK_ERRORS:
				sudoku->mark_errors = move->x_val;
				break;
			case PRINT_BOARD:
				print_board(sudoku);
				break;
			case SET:
				error = set(move,sudoku,mode,&list, &erroneous);
				if(error > 1)
					mode = Init;
				break;
			case VALIDATE:
				error = validate(sudoku, &erroneous);
				break;
			case GUESS:

			case GENERATE:
				error = generate(sudoku,move,&list);
				break;
			case UNDO:
				undo(&list,sudoku,0, &erroneous);
				break;
			case REDO:
				redo(&list,sudoku, &erroneous);
				break;
			case SAVE:
				error = save(sudoku,move,mode, &erroneous);
				break;
			case HINT:
				error = hint(sudoku,move, &erroneous);
				break;
			case GUESS_HINT:

			case NUM_SOLUTIONS:
				error = numsolutions(sudoku, &erroneous);
				break;
			case AUTOFILL:
				error = autofill(sudoku,&list, &erroneous);
				break;
			case RESET:
				reset(sudoku,&list, &erroneous);
				break;
			case EXIT :
				destroyMove(move);
				continue;
			case INVALIDCOMMAND:
				invalid_command_print();
				break;
			default:
				break;
		}
		if(error)
			return;
		destroyMove(move);
		enter_command_print();
	}
	exitgame(sudoku,list);

}

