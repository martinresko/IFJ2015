/**
 * Predmet: IFJ / IAL
 * Subor:     interpret.c
 *            Implementacia interpretu
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
 */

//Hlavickovy subor pre interpret
#include "interpret.h"

ERROR_CODE init_interpret(Table_Symbols *table){
	StackPointer *Stack;
	stackInit(Stack);
	run_error = interpret()
}

