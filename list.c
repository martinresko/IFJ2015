/**
 * Predmet: IFJ / IAL
 * Subor:     list.c
 *            Implementacia dvojsmerne viazaneho zoznamu
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
 */

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
	List push_element = memmalloc(sizeof(struct list)); /* vytvorenie mem pre novy prvok */
	if (push_element==NULL)
		return NULL;
	else /* memmalloc sa podaril */
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
			memfree(Lis->last_list_element->data);
			memfree(Lis->last_list_element);
			Lis->last_list_element=NULL;
			Lis->first_list_element=NULL;
		}
		else
		{
			Lis->last_list_element=Lis->last_list_element->prev; /* nastav vrchol listu na predposledny prvok */
			memfree(Lis->last_list_element->next->data);
			memfree(Lis->last_list_element->next);
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
		;
		//printf("list je prazdny\n");
	else
	{
		List help = Lis->first_list_element;
		while (help!=NULL)
		{
			//printf(" %s ",help->data);
			help=help->next;
		}
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
		memfree(Lis->first_list_element->data);
		memfree(Lis->first_list_element);
		Lis->first_list_element=helpful_pointer;
	}
	Lis->first_list_element=NULL;
	Lis->last_list_element=NULL;
}
