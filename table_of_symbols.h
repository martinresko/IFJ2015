/**
 * Predmet: IFJ / IAL
 * Subor:     table_of_symbols.h
 *            Hlavickovy subor tabulky symbolov
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
**/

#ifndef TABLEOFSYMBOLS_INCLUDED
#define TABLEOFSYMBOLS_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "bintree.h"
#include "stack.h"
#include "list.h"
#include "garbage.h"

/* Struktura funkcie v globalnej tabulke symbolov, len pre funkcie */
typedef struct function_GTS {
	ListPointer params; /* TS parametrov funkcie */
	StackPointer symbol_table_of_block; /* zasobnik TS blokov */
	void *pointer_to_instructions; /* ukazatel na instrukcnu pasku !! treba doplnit upravit ukazatel na co typicky ukazatel na dvojsmerne viazany zozname alebo jeho prvu polozku ... */
	int defined; /* funkcia definovana ? */
	int return_type; /* navratova hodnota funkcie */
	int return_occured; /* kontrola ci funkcia obsahuje return */
} Function_GTS;

/* Struktura pre jednotlive premenne v TS */
typedef struct variable {
	int typ; /* int,double,string */
	char *name;
} Variable;

typedef struct table_symbols{
	TreePointer functions; /* strom funkcii */
	Function_GTS *actual_function; /* ukazatel na aktualnu funkciu */
} Table_symbols;

/* funkcie tabulky symbolov */
void globalTableOfSymbolsInit(Table_symbols *);
ERROR_CODE insertFunction(Table_symbols *,char *,int);
Function_GTS *searchFunction(Table_symbols *,char *);
ERROR_CODE insertFunctionParam(Function_GTS *,char *,int);
Variable *searchFunctionParam(Function_GTS *,char *);
ERROR_CODE pushBlock(Function_GTS *);
void popBlock(Function_GTS *);
ERROR_CODE insertFunctionVariableToStack(Function_GTS *, char *,int);
Variable *searchFunctionVariableInStack(Function_GTS *,char *);
Variable *findInList(ListPointer *, char *);

void SymbolTableStackDestoy(Function_GTS *);
void functionInGlobalTableDestroy(Function_GTS *);
void traversalGTS(Table_symbols *);
void traversalTree(TreePointer *);
void nodesTree(Tree);
void destroyGTS(Table_symbols *);

#endif
