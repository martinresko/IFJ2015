/* Fukncia je volana pri chybe */	
void Error()
{
    printf ("*ERROR* The program has performed an illegal operation.\n");
}

/* inicializuje list */
/* Lis ukazatel na list */
void InitList (ListPointer *Lis)
{
	Lis->top_list_element = NULL;
	Lis->bottom_list_element = NULL; 
	Lis->last_terminal = NULL;
}
/* vrati 0 ak list nieje prazdny */
/* Lis ukazatel na list */
int EmptyList(ListPointer *Lis)
{
	return(Lis->bottom_list_element==NULL ? TRUE : FALSE);
}
/* vlozi novy prvok na vrchol listu + jeho data */
/* vrati ukazatel na vlozeny prvok */
/* Lis ukazatel na list */
/* data znak ktory prvkok obsahuje */
List Push(ListPointer *Lis,char *data)
{
	List push_element = malloc(sizeof(struct list)); /* vytvorenie mem pre novy prvok */
	if (push_element==NULL)
		Error();
	else /* malloc sa podaril */
	{
		/* inicializacia vkladaneho prvku */
		push_element->data=data;
		push_element->next=NULL;
		push_element->prev=NULL;
		push_element->terminal=false;
		if (EmptyList(Lis)) /* ak je list je prazdny */ 
		{
			Lis->top_list_element=push_element;
			Lis->bottom_list_element=push_element;
		}
	 	else
		{
			push_element->prev=Lis->top_list_element; /* nastavenie ukazatela a stary vrchol listu */
			Lis->top_list_element->next=push_element; /* prvok na vrchole listu ukazuje na vkladany prvok */
			Lis->top_list_element=push_element; /* novy vrchol listu */
		}
	}
	return push_element;
}
/* odstrani prvok na vrchole listu */
/* Lis ukazatel na list */
void Pop(ListPointer *Lis)
{
	if(EmptyList(Lis))
	{
		Error(); /* chyba nieje co odstranovat */
	}
	else
	{
		if(Lis->top_list_element==Lis->bottom_list_element) /* ak list ma iba jeden element */
		{
			free(Lis->top_list_element);
			Lis->top_list_element=NULL;
			Lis->bottom_list_element=NULL;
		}
		else
		{
			Lis->top_list_element=Lis->top_list_element->prev; /* nastav vrchol listu na predposledny prvok */
			free(Lis->top_list_element->next);
			Lis->top_list_element->next=NULL;
		}
	}
	
}
/* vrati data na vrchole listu */
/* Lis ukazatel na list */
char* Top(ListPointer *Lis)
{
	return Lis->top_list_element->data;
}
// testovaci print listu
void PrintList(ListPointer *Lis)
{
	if (EmptyList(Lis))
		printf("list je prazdny\n");
	else
	{
		List help = Lis->bottom_list_element;
		while (help!=NULL)
		{
			printf(" %s ",help->data);
			help=help->next;
		}
		printf("\n");
	}
}
/* zmaze list a uvolni pamat */
/* Lis ukazatel na list */
void DestroyList(ListPointer *Lis)
{
	List helpful_pointer = NULL;
	
	while(Lis->bottom_list_element!=NULL)
	{
		helpful_pointer = Lis->bottom_list_element->next;
		free(Lis->bottom_list_element);
		Lis->bottom_list_element=helpful_pointer;
	}
	Lis->bottom_list_element=NULL;
	Lis->top_list_element=NULL;
	Lis->last_terminal=NULL;
}
