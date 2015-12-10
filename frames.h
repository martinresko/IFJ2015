/**
 * Predmet: IFJ / IAL
 * Subor:     frames.h
 *            Hlavickovy subor ramcov premennych 
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
**/

#ifndef FRAMES_INCLUDED
#define FRAMES_INCLUDED 

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include"error.h"
#include"garbage.h"
#include"bintree.h"
#include"stack.h"
#include"scaner.h"

/* typy ramcov */
typedef enum {
	BASE,
	BLOCK,
	PREPARATION,
	ENDED
}Frame_type;

/* struktura ramca */
typedef struct frame {
	Frame_type type_of_frame; /* typ ramca tj. - zakladny, pripravovany, ukonceny, pre blok */
	TreePointer frame_tree; /* strom premennych v ramci */
} Frame;

typedef union {
	int I;
	double D;
	char *S;
}Var_value;

/* struktura pre premennu v ramci */
typedef struct frame_variable {
	char *frame_var_name;
	int frame_var_type;	
	Var_value frame_var_value;
	int inicialized;
} Frame_variable;

/* prototypy pre ramce */
ERROR_CODE pushFrame(StackPointer *);
void popFrame(StackPointer *);
Frame_variable *insertVariableToFrame(StackPointer*,char*,int);
Frame_variable *searchVariableInFrames(StackPointer *,char *);
Frame_variable *searchVariableInFramesAfterEnded(StackPointer *,char *);
Stack findLastBaseFrame(StackPointer *);
void fromPreparationDoBase(StackPointer *,int);
void destroyFrameWithBase(StackPointer *);
void destroyAllFrames(StackPointer *);
ERROR_CODE setVariableAsInitialized(StackPointer *,char *);
ERROR_CODE setValueVariable(StackPointer *,char *,char*);
void fromBaseDoEnded(StackPointer *);
void destroyFramesToEnded(StackPointer *);
ERROR_CODE copyValue(StackPointer *,Frame_variable *,Frame_variable *);

#endif
