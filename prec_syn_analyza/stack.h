#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<stdbool.h>

#define TRUE 1
#define FALSE 0

/* Prvok zoznamu */
typedef struct stack {
    struct stack *next;
	struct stack *prev;
    char *data;
	bool terminal;
} *Stack;	               

/* zoznam pre spravu zasobnika */
typedef struct { 
    Stack top_stack_element;
    Stack bottom_stack_element;
	Stack last_terminal;
} StackPointer;

/* Funkcie zasobnika */
void Error();
void InitStack (StackPointer *);
int EmptyStack(StackPointer *);
Stack Push(StackPointer *,char *);
void Pop(StackPointer *);
char* Top(StackPointer *);
void PrintStack(StackPointer *);
void DestroyStack(StackPointer *);
