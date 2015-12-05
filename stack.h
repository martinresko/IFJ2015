/**
 * Predmet: IFJ / IAL
 * Subor:     stack.h
 *            Hlavickovy subor zasobnika
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
**/

#ifndef STACK_INCLUDED
#define STACK_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include"error.h"
#include"garbage.h"

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
ERROR_CODE stackPush(StackPointer *,void *);
void stackPop(StackPointer *);
void *stackTop(StackPointer *);
int stackEmpty(StackPointer *);
void stackDestroy(StackPointer *);

#endif
