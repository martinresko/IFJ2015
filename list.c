#include"list.h"

/* inicializuje list */
/* Lis ukazatel na list */
void InitList (ListPointer *Lis)
{
	Lis->last_list_element = NULL;
	Lis->first_list_element = NULL; 
}
/* vrati 0 ak list nieje prazdny */
/* Lis ukazatel na list */
int EmptyList(ListPointer *Lis)
{
	return(Lis->first_list_element==NULL ? TRUE : FALSE);
}
/* vlozi novy prvok na vrchol listu + jeho data */
/* vrati ukazatel na vlozeny prvok ak */
/* Lis ukazatel na list */
/* data znak ktory prvkok obsahuje */
List InsertLast(ListPointer *Lis,void *data)
{
	List push_element = malloc(sizeof(struct list)); /* vytvorenie mem pre novy prvok */
	if (push_element==NULL)
		return NULL;
	else /* malloc sa podaril */
	{
		/* inicializacia vkladaneho prvku */
		push_element->data=data;
		push_element->next=NULL;
		push_element->prev=NULL;
		if (EmptyList(Lis)) /* ak je list je prazdny */ 
		{
			Lis->last_list_element=push_element;
			Lis->first_list_element=push_element;
		}
	 	else
		{
			push_element->prev=Lis->last_list_element; /* nastavenie ukazatela a stary vrchol listu */
			Lis->last_list_element->next=push_element; /* prvok na vrchole listu ukazuje na vkladany prvok */
			Lis->last_list_element=push_element; /* novy vrchol listu */
		}
	}
	return push_element;
}
/* odstrani prvok na vrchole listu */
/* Lis ukazatel na list */
void DeleteLast(ListPointer *Lis)
{
	if(EmptyList(Lis))
		return;
	else
	{
		if(Lis->last_list_element==Lis->first_list_element) /* ak list ma iba jeden element */
		{
			free(Lis->last_list_element->data);
			free(Lis->last_list_element);
			Lis->last_list_element=NULL;
			Lis->first_list_element=NULL;
		}
		else
		{
			Lis->last_list_element=Lis->last_list_element->prev; /* nastav vrchol listu na predposledny prvok */
			free(Lis->last_list_element->next->data);
			free(Lis->last_list_element->next);
			Lis->last_list_element->next=NULL;
		}
	}
}
/* vrati data na vrchole listu */
/* Lis ukazatel na list */
void* GetLast(ListPointer *Lis)
{
	return Lis->last_list_element->data;
}
// testovaci print listu
void PrintList(ListPointer *Lis)
{
	if (EmptyList(Lis))
		printf("list je prazdny\n");
	else
	{
		List help = Lis->first_list_element;
		while (help!=NULL)
		{
			//printf(" %s ",help->data);
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
	
	while(Lis->first_list_element!=NULL)
	{
		helpful_pointer = Lis->first_list_element->next;
		free(Lis->first_list_element->data);
		free(Lis->first_list_element);
		Lis->first_list_element=helpful_pointer;
	}
	Lis->first_list_element=NULL;
	Lis->last_list_element=NULL;
}
