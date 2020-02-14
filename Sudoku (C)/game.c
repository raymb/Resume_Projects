#include "game.h"


void run_game(){
	Sudoku *sudoku = NULL;
	Node *list = NULL;
	int mode = Init, command = -1, erroneous = 0, error = 0;
	Move *move = NULL;
	char path[MAX_LENGTH];
	if(createHead(&list))
		return;
	print_start_of_game();
	enter_command_print();
	while(command != EXIT){
		if(!(move = parse(mode)))
			return;
		if(feof(stdin)){
			move->command =EXIT;
			destroyMove(move);
			continue;
		}
		switch((command = move->command)){
			case SOLVE:
				if((error = load_new_sudoku(&sudoku,&list,move))){
					if(error >1){
						command = EXIT;
						destroyMove(move);
						continue;
					}
					return;
				}
				print_board(sudoku, 0);
				mode = Solve;
				break;
			case EDIT:
				if(!move->path){
					sudoku = createSudoku(3,3);
				}
				else {
					if((error = load_new_sudoku(&sudoku,&list,move))){
						if(error > 1){
							command = EXIT;
							destroyMove(move);
							continue;
						}
						return;
					}
				}
				print_board(sudoku, 0);
				mode = Edit;
				sudoku->mark_errors = 1;
				break;
			case MARK_ERRORS:
				if(move->x_val > 1 || move->x_val < 0)
					mark_errors_value_print();
				else{
					sudoku->mark_errors = move->x_val;
					print_board(sudoku, 0);
				}
				break;
			case PRINT_BOARD:
				print_board(sudoku, 0);
				break;
			case SET:
				if((error = set(move,sudoku,mode,&list, &erroneous)) > 1){
					mode = Init;
					error = 0;
				}
				break;
			case VALIDATE:
				error = validate(sudoku, &erroneous);
				break;
			case GUESS:
				if(move->threshold > 1 || move->threshold < 0)
					guess_value_print();
				else
					error = guess(sudoku, move->threshold, &list, &erroneous);
				break;
			case GENERATE:
				error = generate(sudoku,move,&list, &erroneous);
				break;
			case UNDO:
				undo(&list,sudoku,0, &erroneous);
				break;
			case REDO:
				redo(&list,sudoku, &erroneous);
				break;
			case SAVE:
				strcpy(path, (move)->path);
				error = save(sudoku,path,mode, &erroneous);
				break;
			case HINT:
				error = hint(sudoku,move, &erroneous);
				break;
			case GUESS_HINT:
				error = guess_hint(sudoku, move->x_val, move->y_val, &erroneous);
				break;
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
			case INVALIDMODECOMMAND:
			default:
				break;
		}
		if(error){
			return;
		}
		destroyMove(move);
		enter_command_print();
	}
	exitgame(sudoku,&list);
}

