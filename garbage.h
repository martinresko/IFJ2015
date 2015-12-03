/**
 * Predmet: IFJ / IAL
 * Subor:     ial.c
 *            Implementacia spravy pamete
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "strings.h"
#define MMU_SIZE 32


typedef enum typeofitem{
    mem_memory,
    mem_notdefined,
    mem_file,
} gmemtype;

typedef struct memtableitem{
    intptr_t key;
    gmemtype type;
    void* ptr;
    unsigned long allocated;

    struct memtableitem* next;
} *gmemtableitemptr, gmemtableitem;

typedef struct memtable{
    unsigned int size;
    gmemtableitem** data;
} gmemtable;

typedef struct memory_counters{
    gmemtable* table;
    unsigned long mallocs;
    unsigned long reallocs;
    unsigned long callocs;
    unsigned long fopens;
    unsigned long frees;
    unsigned long fcloses;
    unsigned long allocated;
} gmemcount;

extern gmemcount memtable;

//prototypy
void meminitialization();
void* memmalloc(size_t size);
void* memrealloc(void* ptr, size_t size);
void* memcalloc(size_t num, size_t size);
void* memfopen(const char* filename, const char* mode);
void memfree(void* ptr);
void memfclose(void* ptr);
void memallfree();
gmemtable* memorytablecreate();
gmemtableitem* memtitemcreate(intptr_t key);
void memtdestroy(gmemtable* T);
void memtitemdestroy(gmemtableitem* item);
void memtableinicialization(gmemtable* T, size_t size);
void memtabledispose(gmemtable* T);
gmemtableitem* memfindout(gmemtable* T, intptr_t key);
size_t hash(intptr_t key, size_t size);
