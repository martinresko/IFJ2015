/**
 * Predmet: IFJ / IAL
 * Subor:     bintree.c
 *            Implementacia binarneho stromu
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
**/

#include"bintree.h"

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
		if(compare!=0)
		{
			if(compare>0)	
			{
				return treeNodesSearch(node->right,key);
			}
			else /* compare je mensie ako nula */
			{
				return treeNodesSearch(node->left,key);
			}
		}
		else /* compare == 0, cize su kluce rovnake */
		{
			return node; 
		}
	}
	else /* nic som nenasiel */
	{
		return NULL;
	}
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
	memfree(node->data); // POZOR znamena ze mozem do data ukladat len pointer na alokovanu pamat ak by som do dat dal pointer na int napriklad tak hodi chybu lebo nema byt aka pamat uvolnena */
	memfree(node);
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
ERROR_CODE treeInsert(TreePointer *Tre, char *key,void *data)
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
					helpful_pointer->right=memmalloc(sizeof(struct tree));
					if(helpful_pointer->right!=NULL)
					{
						helpful_pointer->right->left=NULL;
						helpful_pointer->right->right=NULL;
						helpful_pointer->right->data=data;
						helpful_pointer->right->key=key;
					}
					else
						return INTERN_ERR;
				}
				else /* list to nieje tak musim sa posunut dalej */
					helpful_pointer=helpful_pointer->right;
			}
			else if(compare<0)
			{
				if(helpful_pointer->left==NULL) /* je to list */
				{
					helpful_pointer->left=memmalloc(sizeof(struct tree));
					if(helpful_pointer->left!=NULL)
					{
						helpful_pointer->left->left=NULL;
						helpful_pointer->left->right=NULL;
						helpful_pointer->left->data=data;
						helpful_pointer->left->key=key;
					}
					else
						return INTERN_ERR;
				}
				else /* list to nieje tak musim sa posunut dalej */
					helpful_pointer=helpful_pointer->left;
			}
			else /*compare == 0 */
			{
				helpful_pointer->data=data; /* aktualizujem data */
				return OK_ERR;/*nasiel som co som hladal*/
			}
		}
	}
	else /* strom je prazdny tj. vkladam koren */
	{
		Tre->root=memmalloc(sizeof(struct tree));	
		if(Tre->root!=NULL)
		{
			Tre->root->left=NULL;
			Tre->root->right=NULL;
			Tre->root->data=data;
			Tre->root->key=key;
		}
		else
			return INTERN_ERR;
	}
	return OK_ERR;
}
