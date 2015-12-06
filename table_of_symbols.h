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
	ListPointer instructions; /* ukazatel na instrukcnu pasku */
	int defined; /* funkcia definovana ? */
	int return_type; /* navratova hodnota funkcie */
	int return_occured; /* kontrola ci funkcia obsahuje return */
} Function_GTS;

/* Struktura pre jednotlive premenne v TS */
typedef struct variable {
	int typ; /* int,double,string */
	char *name;
} Variable;

/* struktura instrukcie */
typedef struct instruction {
	int type;
	void *destination;
	void *source1;
	void *source2;
} Instruction;

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
Variable *getFunctionParam(Function_GTS *,int);
ERROR_CODE pushBlock(Function_GTS *);
void popBlock(Function_GTS *);
ERROR_CODE insertFunctionVariableToStack(Function_GTS *, char *,int);
Variable *searchFunctionVariableInStack(Function_GTS *,char *);
Variable *findInList(ListPointer *, char *);
Variable *getFromList(ListPointer *,int);
ERROR_CODE insertFunctionInstruction(Function_GTS *, int, void *, void *, void *);

void SymbolTableStackDestoy(Function_GTS *);
void functionInGlobalTableDestroy(Function_GTS *);
ERROR_CODE traversalGTS(Table_symbols *,int);
ERROR_CODE traversalTree(TreePointer *,int);
ERROR_CODE nodesTree(Tree,int);
void destroyGTS(Table_symbols *);

ERROR_CODE finalFunctionCheckout(Table_symbols *);
#endif
