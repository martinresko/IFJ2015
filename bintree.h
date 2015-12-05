/**
 * Predmet: IFJ / IAL
 * Subor:     bintree.h
 *            Hlavickovy subor binarneho stromu
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
**/

#ifndef BINTREE_INCLUDED
#define BINTREE_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include"error.h"
#include"garbage.h"

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

/* funkcie binarneho stromu */
void treeInit(TreePointer *);
void treeDestroy(TreePointer *);
Tree treeSearch(TreePointer *, char *);
ERROR_CODE treeInsert(TreePointer *,char *,void *);
Tree treeNodesSearch(Tree, char *key);
void treeNodesDestroy(Tree);

#endif
