/**
 * Predmet: IFJ / IAL
 * Subor:     stack.c
 *            Implementacia zasobnika
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
**/

#include"stack.h"


/* inicializuje zasobnik */
/* Stac - ukazatel na zasobnik */
void stackInit(StackPointer *Stac)
{
	Stac->top_of_stack = NULL;
}

/* vlozi na zasobnik novy prvok
 * Stac - ukazatel na zasobnik
 * data_struct ukazatel na datovu strukturu */
ERROR_CODE stackPush(StackPointer *Stac, void *data_struct)
{
	Stack helpful_pointer = memmalloc(sizeof(struct stack));
	if(helpful_pointer!=NULL)
	{
		helpful_pointer->data=data_struct;
		helpful_pointer->left_stack_element=Stac->top_of_stack;
		Stac->top_of_stack=helpful_pointer;
		return OK_ERR;
	}
	else
		return INTERN_ERR;
}

/* odstani prvok z vrcholu zasobniku 
 * Stac - ukazatel na zasobnik */
void stackPop(StackPointer *Stac)
{
	Stack helpful_pointer = NULL;
	if(!stackEmpty(Stac))
	{	
		helpful_pointer = Stac->top_of_stack;
		Stac->top_of_stack=Stac->top_of_stack->left_stack_element;
		memfree(helpful_pointer->data); // POZOR to iste ako v stact destroy 
		memfree(helpful_pointer);
	}
}
/* vrati vrchol zasobniku 
 * return - ukazatel na cokolvek
 * Stac - ukazatel na zasobnik */
void *stackTop(StackPointer *Stac)
{
	return Stac->top_of_stack->data;
}

/* funkcia ktora zisti ci je zasobnik prazdny 
 * return - 0(zasobnik nieje prazdny) 1(zasonik nieje prazdny)
 * Stac - ukazatel na zasobnik */
int stackEmpty(StackPointer *Stac)
{
	return (Stac->top_of_stack==NULL)?1:0;
}
 
/* prejde cely zasobnik a postupne uvolny vsetky jeho prvky */
/* Stac - ukazatel na zasobnik */
void stackDestroy(StackPointer *Stac)
{
	Stack helpful_pointer = NULL;
	while(Stac->top_of_stack!=NULL)
	{
		helpful_pointer = Stac->top_of_stack;
		Stac->top_of_stack = helpful_pointer->left_stack_element;
		memfree(helpful_pointer->data); // POZOR znamena to ze zasobnik moze obsahovat len to co sa ulozi na hromade inak to hodi error lebo nema co uvolnit 
		memfree(helpful_pointer);
	}
}
