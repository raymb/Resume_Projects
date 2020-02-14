#include "sol_counter.h"

/**
 * Creates a new stack structure. On success the function returns a pointer
 * to the new stack. If a memory allocation failure occurs, the function terminates.
 *
 * @param int - The size of the stack.
 * @return
 * NULL if memory allocation failure occurs
 * pointer to new stack otherwise
 */
Stack* createStack(int n){
	Stack *s = (Stack *)malloc(sizeof(Stack));
	int *x,*y,*values;
	if(!s){
		failure_print_for("malloc");
		return NULL;
	}
	x = (int *)calloc(n,sizeof(int));
	y = (int *)calloc(n,sizeof(int));
	values= (int *)calloc(n,sizeof(int));
	if(!x || !y || !values){
		failure_print_for("calloc");
		return NULL;
	}
	s->x = x;
	s->y = y;
	s->top = 0;
	s->capacity = n;
	s->values =values;
	return s;
}

/**
 * Gets the "x" value (first index) of the last element in the stack.
 *
 * @param s - The stack.
 * @return
 * "x" value (first index) of the last element in the stack.
 */
int getX(Stack *s){
	return s->x[s->top];
}

/**
 * Gets the "y" value (second index) of the last element in the stack.
 *
 * @param s - The stack.
 * @return
 * "y" value (second index) of the last element in the stack.
 */
int getY(Stack *s){
	return s->y[s->top];
}

/**
 * This Function frees all memory allocations associated with a stack structure.
 *
 * @param s - pointer to the requested memory allocation to free
 * @return
 */
void destroyStack(Stack *s){
	free(s->x);
	free(s->y);
	free(s->values);
	free(s);
}

/**
 * Gets the last element in the stack.
 *
 * @param s - The stack.
 * @return
 * The last element in the stack.
 */
int top(Stack *s){
	return s->values[s->top];
}

/**
 * Gets the last element in the stack, and remove it from the stack.
 *
 * @param s - The stack.
 * @return
 * The last element in the stack.
 */
int pop(Stack *s){
	int n = top(s);
	s->values[s->top] = 0;
	s->top --;
	return n;
}

/**
 * Insert a new element (value) to the stack.
 *
 * @param s - The stack.
 * @param n - The new element.
 * @return
 */
void push(Stack *s,int n){
	s->values[s->top] = n;
	s->top ++;
}

/**
 * Updates the index arrays (x, y) with the indexes of the non-fixed (empty) cells indexes.
 *
 * @param s - The stack.
 * @param size - The size of the stack
 * @param fixed - The fixed cells board.
 * @return
 */
void update_index_arrays(Stack *s,int size , int **fixed){
	int i,j;
	for(i = 0; i < size ; i++)
		for(j = 0; j < size ; j++)
			if(!fixed[i][j]){
				s->x[s->top] = i;
				s->y[s->top] = j;
				s->top++;
			}
	s->capacity = s->top;
	s->top = 0;

}

/**
 * Checks if the value val is valid in the board, at the x, y cell. The function returns 0 if
 * the value is not valid, otherwise returns 1.
 *
 * @param board - The Sudoku board.
 * @param x - first index of the cell.
 * @param y - second index of the cell.
 * @param val - The value to be checked.
 * @return
 * 1 if the value is valid in the x,y index, otherwise returns 0.
 */
int is_valid(Board *board, int x, int y, int val){
	int i, j;
	int row = board->row;
	int column = board ->column;
	if(val == 0)
		return 0;

	for(i = 0; i < row*column; i++){
		if(board->grid[x][i] == val && i != y)
			return 0;
		if(board->grid[i][y] == val && i != x)
			return 0;
	}
	for(i = (x/row) * row; i < (1 + x/row) * row; i++)
		for(j = (y/column) * column; j < (1 + y/column) * column; j++){
			if(i==x && j==y)
				continue;
			if(board->grid[i][j]== val)
				return 0;
		}
	return 1;
}

/**
 * Check the next valid value in the board at the x,y cell.
 *
 * @param s - The Sudoku board.
 * @param x - first index of the cell.
 * @param y - second index of the cell.
 * @param current - The current value, to start the ckecking its next.
 * @return
 *the next valid value if exists, otherwise returns 0.
 */
int next_valid_value(Sudoku *s ,int x, int y, int current){
	int i;
		for(i = current; i < s->board_size; i++){
			if(is_valid(s->board, x, y, i+1))
				return i+1;
		}
		return 0;
}

int count(Sudoku *sudoku){
	int counter =0 , size = sudoku->board_size,direction = 1;
	int x,y ,value;
	Stack *stack = createStack(size * size);
	update_index_arrays(stack,size,sudoku->board->grid);
	while(stack->top != -1){
			if(stack->top == stack->capacity){
				counter++;
				pop(stack);
				direction = 0 ;
			}
			x = getX(stack);
			y = getY(stack);
			if(direction == 1){
				value=next_valid_value(sudoku,x,y,0);
				if( value == 0){
					sudoku->board->grid[x][y] = 0;
					direction = 0 ;
					continue;
				}
			}
			else{
				value=next_valid_value(sudoku,x,y,top(stack));
				if( value == 0){
					sudoku->board->grid[x][y] = 0;
					pop(stack);
					direction = 0 ;
					continue;
				}
			}
			sudoku->board->grid[x][y] = value;
			push(stack,value);
			direction = 1;
		}
		destroyStack(stack);
		return counter;

	return counter;
}
