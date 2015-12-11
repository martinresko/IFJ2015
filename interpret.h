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
#include "table_of_symbols.h"
#include "parser.h"
#include "stack.h"
#include "list.h"
#include "bintree.h"
#include "frames.h"
#include "scaner.h"
#include "ial.h"
#include "garbage.h"


/* definujeme si premennu na uchovanie chyby */
ERROR_CODE run_error;

char *str_value;

typedef enum{
//Presun
	iMOV,
	iGETVALUE,
//Aritmeticke operacie
	iADD,
	iSUB,
	iMUL,
	iDIV,
//Logicke operacie
	iGREATER,
	iLESS,
	iEGREATER,
	iELESS,
	iEQUAL,
	iNEQUAL,
//Skoky
	iJMP,
	iJZ,
	iJNZ,
//Volania funkcii
	iCALL,
//Vstavane funkcie
	iSORT,
	iLEN,
	iSUBSTR,
	iFIND,
	iCONCAT,
//Vstupno-vystupne operacie
	iWRITE,
	iREAD,
//Zasobnikove operacie
	iDISPSTPOST,
	iPUSH,
	iPOP,
	iTOP,
	iTOPPOP,
//Navratova hodnota
	iRET,
//Navestia
	iLABEL,
//Operacie s ramcami
	iPUSHFR,
	iPOPFR,
	iDISPFR,
	iINSERT_TO_FR,
	iREADFR,
	iSETBASEFR,
	iDISPOSEALL,
	iBASE_TO_END,
	iCOPY_VALUE
} tInstrT;

ERROR_CODE interpret(Table_symbols *table);

#endif
