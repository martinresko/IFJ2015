/**
 * Predmet: IFJ / IAL
 * Subor:     interpret.h
 *            Implementacia interpretu
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
 */

#ifndef INTERPRET_INCLUDED
#define INTERPRET_INCLUDED

/* Hlavickove subory */
#include <stdio.h>
#include <string.h>

#include "error.h"
#include "parser.h"
#include "stack.h"
#include "list.h"
#include "frames.h"
#include "scaner.h"
#include "ial.h"
#include "garbage.h"


/* definujeme si premennu na uchovanie chyby */
ERROR_CODE run_error;

char *str_value;

typedef enum{
//Presun
	iMOV=0,
//Aritmeticke operacie
	iADD, 			//01
	iSUB,			//02
	iMUL,			//03
	iDIV,			//04
//Logicke operacie
	iGREATER,		//05
	iLESS,			//06
	iEGREATER,		//07
	iELESS,			//08
	iEQUAL,			//09
	iNEQUAL,		//10
//Skoky
	iJMP,			//11
	iJZ,			//12
	iJNZ,			//13
//Volania funkcii
	iCALL,			//14
//Vstavane funkcie	
	iSORT,			//15
	iLEN,			//16
	iSUBSTR,		//17
	iFIND,			//18
	iCONCAT,		//19
//Vstupno-vystupne operacie
	iWRITE, 		//20
	iREAD,			//21
//Zasobnikove operacie
	iDISPSTPOST,	//22
	iPUSH,			//23
	iPOP,			//24
	iTOP,			//25
	iTOPPOP,		//26
//Navratova hodnota
	iRET,			//27
//Navestia
	iLABEL,			//28
//Operacie s ramcami
	iPUSHFR,		//29
	iPOPFR,			//30
	iDISPFR,		//31
	iINSERT_TO_FR,	//32
	iREADFR,		//33
	iSETBASEFR,		//34
	iDISPOSEALL,	//35
	iBASE_TO_END,	//36
	iCOPY_VALUE,	//37
	iSETVALUE		//38
} tInstrT;	

ERROR_CODE interpret(Table_symbols *table);

#endif
