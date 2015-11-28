#ifndef TABLEOFSYMBOLS_INCLUDED
#define TABLEOFSYMBOLS_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"binTree.h"
#include"stack.h"
//#include"binTree.c"
//#include"stack.c"

/* Struktura funkcie v globalnej tabulke symbolov, len pre funkcie */
typedef struct function_GTS {
	TreePointer params; /* TS parametrov funkcie */
	StackPointer symbol_table_of_block; /* zasobnik TS blokov */
	void *pointer_to_instructions; /* ukazatel na instrukcnu pasku !! treba doplnit upravit ukazatel na co typicky ukazatel na dvojsmerne viazany zozname alebo jeho prvu polozku ... */
	int defined; /* funkcia definovana ? */
	int return_type; /* navratova hodnota funkcie */
} Function_GTS;

/* Struktura pre jednotlive premenne || paremetre v TS */
typedef struct variable {
	int typ; /* int,double,string */
} Variable;

typedef struct table_symbols{
	TreePointer functions; /* strom funkcii */
} Table_symbols;

/* funkcie tabulky symbolov */
void globalTableOfSymbolsInit(Table_symbols *);
void insertFunction(Table_symbols *,char *,int);
Function_GTS *searchFunction(Table_symbols *,char *);
void insertFunctionParam(Function_GTS *,char *,int);
Variable *searchFunctionParam(Function_GTS *,char *);
void pushBlock(Function_GTS *);
void popBlock(Function_GTS *);
void insertFunctionVariableToStack(Function_GTS *, char *,int);
Variable *searchFunctionVariableInStack(Function_GTS *,char *);

void SymbolTableStackDestoy(Function_GTS *);
void functionInGlobalTableDestroy(Function_GTS *);
void traversalGTS(Table_symbols *);
void traversalTree(TreePointer *);
void nodesTree(Tree);
void destroyGTS(Table_symbols *);

#endif
