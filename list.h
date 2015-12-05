/**
 * Predmet: IFJ / IAL
 * Subor:     list.h
 *            Hlavickovy subor dvojsmerne viazaneho zoznamu
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
 */

#ifndef LIST_INCLUDED
#define LIST_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<stdbool.h>
#include"garbage.h"

#define TRUE 1
#define FALSE 0

/* Prvok zoznamu */
typedef struct list {
    struct list *next;
	struct list *prev;
    void *data;
} *List;	               

/* zoznam pre spravu obojsmerne viazaneho zoznamu */
typedef struct { 
    List last_list_element;
    List first_list_element;
	List last_terminal;
} ListPointer;

/* Funkcie obojsmerne viazeneho zoznamu */
void Error();
void InitList (ListPointer *);
int EmptyList(ListPointer *);
List InsertLast(ListPointer *,void *);
void DeleteLast(ListPointer *);
void* GetLast(ListPointer *);
void PrintList(ListPointer *);
void DestroyList(ListPointer *);

#endif
