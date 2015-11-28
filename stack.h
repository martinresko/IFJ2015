#ifndef STACK_INCLUDED
#define STACK_INCLUDED

#include<stdio.h>
#include<stdlib.h>

/* prvok zasobniku */
typedef struct stack {
	struct stack *left_stack_element;
	void *data;
} *Stack;

/* struktura zasobnika */
typedef struct {
	Stack top_of_stack;
} StackPointer;

/* funkcie zasobnika */
void stackInit(StackPointer *);
void stackPush(StackPointer *,void *);
void stackPop(StackPointer *);
void *stackTop(StackPointer *);
int stackEmpty(StackPointer *);
void stackDestroy(StackPointer *);

#endif
