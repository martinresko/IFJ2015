#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<stdbool.h>

#define TRUE 1
#define FALSE 0

/* Prvok zoznamu */
typedef struct list {
    struct list *next;
	struct list *prev;
    char *data;
	bool terminal;
} *List;	               

/* zoznam pre spravu zasobnika */
typedef struct { 
    List top_list_element;
    List bottom_list_element;
	List last_terminal;
} ListPointer;

/* Funkcie zasobnika */
void Error();
void InitList (ListPointer *);
int EmptyList(ListPointer *);
List Push(ListPointer *,char *);
void Pop(ListPointer *);
char* Top(ListPointer *);
void PrintList(ListPointer *);
void DestroyList(ListPointer *);
