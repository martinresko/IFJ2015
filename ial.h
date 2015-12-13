/**
 * Predmet: IFJ / IAL
 * Subor:     ial.h
 *            Hlavickovy subor ial.h
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
 */ 

#ifndef IAL_INCLUDED
#define IAL_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "garbage.h"
#include "error.h"
#include "stack.h"
#include "list.h"

/* STRUKTURY binarneho stromu */

/* element stromu */
typedef struct tree {
	struct tree *left;
	struct tree *right;
	void *data;
	char *key;
} *Tree;

/* struktura stromu */
typedef struct {
	Tree root;
} TreePointer;

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


typedef struct 
{
 	union
	{
		int Inum;
		double Dnum;
		char * Str;
		void * Void;
	};	
	int VarType;
}InsVal; 

/* struktura instrukcie */
typedef struct instruction {
	int type;
	InsVal des;
	InsVal src1;
	InsVal src2;
} Instruction;

typedef struct table_symbols{
	TreePointer functions; /* strom funkcii */
	Function_GTS *actual_function; /* ukazatel na aktualnu funkciu */
} Table_symbols;


/**Prototypy funkcii**/
int KMP_Find(char *string, char *pattern);
int length_of_string(char * string);
char* concatenation(char *string1, char *string2);
char* shell(char *items);
char* substring(char *string, int i, int n);

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
Variable *searchFunctionVariableInActualLevel(Function_GTS *,char *);
Variable *findInList(ListPointer *, char *);
Variable *getFromList(ListPointer *,int);
ERROR_CODE insertFunctionInstruction(Function_GTS *, int, InsVal *, InsVal *, InsVal *);

void SymbolTableStackDestoy(Function_GTS *);
void functionInGlobalTableDestroy(Function_GTS *);
ERROR_CODE traversalGTS(Table_symbols *,int);
ERROR_CODE traversalTree(TreePointer *,int);
ERROR_CODE nodesTree(Tree,int);
void destroyGTS(Table_symbols *);

ERROR_CODE finalFunctionCheckout(Table_symbols *);

/* funkcie binarneho stromu */
void treeInit(TreePointer *);
void treeDestroy(TreePointer *);
Tree treeSearch(TreePointer *, char *);
ERROR_CODE treeInsert(TreePointer *,char *,void *);
Tree treeNodesSearch(Tree, char *key);
void treeNodesDestroy(Tree);

#endif
