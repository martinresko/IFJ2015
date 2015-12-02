#include"stack.h"

//struct x
//{
//	int a;
//	char *b;
//} struktura_x = {4,"ahoj"};
//int main()
//{
//	printf("%d cislo a\n",struktura_x.a);
//	
//	StackPointer zasobnik;
//	stackInit(&zasobnik);
//	stackPush(&zasobnik,&struktura_x);
//	// plati pre premennu
//	//printf("%d cislo a zo zasobniku",*((int *)(zasobnik.top_of_stack->data)));
//	// plati pre strukturu
//	//printf("%d cislo a zo zasobniku",((struct x*)(zasobnik.top_of_stack->data))->a);
//	stackPop(&zasobnik);
//	return 0;
//}

/* inicializuje zasobnik */
/* Stac - ukazatel na zasobnik */
void stackInit(StackPointer *Stac)
{
	Stack->top_of_stack = NULL;
}

/* vlozi na zasobnik novy prvok
 * Stac - ukazatel na zasobnik
 * data_struct ukazatel na datovu strukturu */
ERROR_CODE stackPush(StackPointer *Stac, void *data_struct)
{
	Stack helpful_pointer = malloc(sizeof(struct stack));
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
		free(helpful_pointer->data); // POZOR to iste ako v stact destroy 
		free(helpful_pointer);
	}
	else
		printf("Pop z prazdneho zasobniku \n");
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
		free(helpful_pointer->data); // POZOR znamena to ze zasobnik moze obsahovat len to co sa ulozi na hromade inak to hodi error lebo nema co uvolnit 
		free(helpful_pointer);
	}
}
