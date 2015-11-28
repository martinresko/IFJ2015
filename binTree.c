#include"binTree.h"

//int main()
//{
//	struct a{
//		int x;
//	}struktura={99};
//
//	//int a=49;
//	Tree test;
//	TreePointer strom;
//	treeInit(&strom);
//	treeInsert(&strom,"a",&struktura);
//	test=treeSearch(&strom,"a");
//	//printf("%d\n",((struct a*)(test->data))->x);
//	//struct a *y=test->data;
//	//printf("%d\n",y->x);
//	treeDestroy(&strom);
//	return 0;
//}

/* vyhlada polozku pomocou kluca vo vsetkych podstromoch daneho uzlu
 * return - vrati polozku ktora bola hladana
 * node   - ukazatel na polozku v strome
 * key    - kluc podla ktoreho sa vyhladava */
Tree treeNodesSearch(Tree node,char *key)
{
	if(node!=NULL)	
	{
		int compare = strcmp(node->key,key);	/* porovnam hladany kluc s klucom v prvku */
		if(compare)
		{
			if(compare>0)	
				return treeNodesSearch(node->right,key);
			else /* compare je mensie ako nula */
				return treeNodesSearch(node->left,key);
		}
		else /* compare == 0, cize su kluce rovnake */
			return node; 
	}
	else /* nic som nenasiel */
		return NULL;
}
/* vymaze vsetky podstromy daneho uzlu 
 * node - ukazatel na polozku v strome */
void treeNodesDestroy(Tree node)
{		
	if(node->left!=NULL)
		treeNodesDestroy(node->left);
	if(node->right!=NULL)
		treeNodesDestroy(node->right);
	//free(node->data); // POZOR znamena ze mozem do data ukladat len pointer na alokovanu pamat ak by som do dat dal pointer na int napriklad tak hodi chybu lebo nema byt aka pamat uvolnena */
	free(node);
}
/*funkcia inicializuje strom
 * Tre - ukazatel na strom */
void treeInit(TreePointer *Tre)
{
	Tre->root=NULL;
}

/* funkcia odstrani strom a uvolni vsetky jeho prvky 
 * Tre - ukazatel na strom */
void treeDestroy(TreePointer *Tre)
{
	if(Tre->root!=NULL)
	{
		treeNodesDestroy(Tre->root);
		Tre->root=NULL;
	}
}

/* funkcia najde polozku v strome podla zadaneho kluca v lavo su kluce mensie v pravo vacsie 
 * return - ukazatel na prislusnu polozku alebo NULL ak nic nenasiel
 * Tre 	  - ukazatel na strom
 * key 	  - kluc vyhladavania */
Tree treeSearch(TreePointer *Tre,char *key)
{
	if((Tre!=NULL) && (key!=NULL))
		return treeNodesSearch(Tre->root,key);
	return NULL;
}

/* funkcia vlozi do stromu novy polozku a ulozi ju na spravne miesto v strome 
 * Tre	- ukazatel na strom
 * key 	- kluc vkladaneho prvku
 * data - ukazatel na cokolvek co cheme uchovavat v prvku stromu */
void treeInsert(TreePointer *Tre, char *key,void *data)
{
	if(Tre->root!=NULL) /* strom nieje prazdny */
	{
		int compare;
		Tree helpful_pointer=Tre->root;
		while(helpful_pointer!=NULL)
		{
			compare=strcmp(helpful_pointer->key,key);
			if(compare>0)
			{
				if(helpful_pointer->right==NULL) /* je to list */
				{
					helpful_pointer->right=malloc(sizeof(struct tree));
					if(helpful_pointer->right!=NULL)
					{
						helpful_pointer->right->left=NULL;
						helpful_pointer->right->right=NULL;
						helpful_pointer->right->data=data;
						helpful_pointer->right->key=key;
					}
					else
						printf("nedostal som pamat\n");
				}
				else /* list to nieje tak musim sa posunut dalej */
					helpful_pointer=helpful_pointer->right;
			}
			else if(compare<0)
			{
				if(helpful_pointer->left==NULL) /* je to list */
				{
					helpful_pointer->left=malloc(sizeof(struct tree));
					if(helpful_pointer->left!=NULL)
					{
						helpful_pointer->left->left=NULL;
						helpful_pointer->left->right=NULL;
						helpful_pointer->left->data=data;
						helpful_pointer->left->key=key;
					}
					else
						printf("nedostal som pamat\n");
				}
				else /* list to nieje tak musim sa posunut dalej */
					helpful_pointer=helpful_pointer->right;
			}
			else /*compare == 0 */
			{
				helpful_pointer->data=data; /* aktualizujem data */
				return;/*nasiel som co som hladal*/
			}
		}
	}
	else /* strom je prazdny tj. vkladam koren */
	{
		Tre->root=malloc(sizeof(struct tree));	
		if(Tre->root!=NULL)
		{
			Tre->root->left=NULL;
			Tre->root->right=NULL;
			Tre->root->data=data;
			Tre->root->key=key;
		}
		else
			printf("nedostal som pamat\n");
	}
}
