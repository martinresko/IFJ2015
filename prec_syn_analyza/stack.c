//#include "stack.h"

//int main()
//{
//	StackPointer zasobnik;
//	InitStack(&zasobnik);
//	Push(&zasobnik,'i');
//	Pop(&zasobnik);
//	return EXIT_SUCCESS;
//}

/* Fukncia je volana pri chybe */	
void Error()
{
    printf ("*ERROR* The program has performed an illegal operation.\n");
}

/* inicializuje zasobnik */
/* Stac ukazatel na zasobnik */
void InitStack (StackPointer *Stac)
{
	Stac->top_stack_element = NULL;
	Stac->bottom_stack_element = NULL; 
	Stac->last_terminal = NULL;
}
/* vrati 0 ak zasobnik nieje prazdny */
/* Stac ukazatel na zasobnik */
int EmptyStack(StackPointer *Stac)
{
	return(Stac->bottom_stack_element==NULL ? TRUE : FALSE);
}
/* vlozi novy prvok na vrchol zasobniku + jeho data */
/* vrati ukazatel na vlozeny prvok */
/* Stac ukazatel na zasobnik */
/* data znak ktory prvkok obsahuje */
Stack Push(StackPointer *Stac,char *data)
{
	Stack push_element = malloc(sizeof(struct stack)); /* vytvorenie mem pre novy prvok */
	if (push_element==NULL)
		Error();
	else /* malloc sa podaril */
	{
		/* inicializacia vkladaneho prvku */
		push_element->data=data;
		push_element->next=NULL;
		push_element->prev=NULL;
		push_element->terminal=false;
		if (EmptyStack(Stac)) /* ak je zasobnik je prazdny */ 
		{
			Stac->top_stack_element=push_element;
			Stac->bottom_stack_element=push_element;
		}
	 	else
		{
			push_element->prev=Stac->top_stack_element; /* nastavenie ukazatela a stary vrchol zasobnika */
			Stac->top_stack_element->next=push_element; /* prvok na vrchole zasobniku ukazuje na vkladany prvok */
			Stac->top_stack_element=push_element; /* novy vrchol zasobniku */
		}
	}
	return push_element;
}
/* odstrani prvok na vrchole zasobnika */
/* Stac ukazatel na zasobnik */
void Pop(StackPointer *Stac)
{
	if(EmptyStack(Stac))
	{
		Error(); /* chyba nieje co odstranovat */
	}
	else
	{
		if(Stac->top_stack_element==Stac->bottom_stack_element) /* ak zasobnik ma iba jeden element */
		{
			free(Stac->top_stack_element);
			Stac->top_stack_element=NULL;
			Stac->bottom_stack_element=NULL;
		}
		else
		{
			Stac->top_stack_element=Stac->top_stack_element->prev; /* nastav vrchol zasobniku na predposledny prvok */
			free(Stac->top_stack_element->next);
			Stac->top_stack_element->next=NULL;
		}
	}
	
}
/* vrati data na vrchole zasobnika */
/* Stac ukazatel na zasobnik */
char* Top(StackPointer *Stac)
{
	return Stac->top_stack_element->data;
}
// testovaci print zasobniku
void PrintStack(StackPointer *Stac)
{
	if (EmptyStack(Stac))
		printf("zasobnik je prazdny\n");
	else
	{
		Stack help = Stac->bottom_stack_element;
		while (help!=NULL)
		{
			printf(" %s ",help->data);
			help=help->next;
		}
		printf("\n");
	}
}
/* zmaze zasobnik a uvolni pamat */
/* Stac ukazatel na zasobnik */
void DestroyStack(StackPointer *Stac)
{
	Stack helpful_pointer = NULL;
	
	while(Stac->bottom_stack_element!=NULL)
	{
		helpful_pointer = Stac->bottom_stack_element->next;
		free(Stac->bottom_stack_element);
		Stac->bottom_stack_element=helpful_pointer;
	}
	Stac->bottom_stack_element=NULL;
	Stac->top_stack_element=NULL;
	Stac->last_terminal=NULL;
}
