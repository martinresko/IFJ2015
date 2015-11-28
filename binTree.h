#ifndef BINTREE_INCLUDED
#define BINTREE_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

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
void treeInsert(TreePointer *,char *,void *);
Tree treeNodesSearch(Tree, char *key);
void treeNodesDestroy(Tree);

#endif
