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

/* Hlavickove subory */
#include "error.h"
#include "table_of_symbols.h"
#include "parser.h"
#include "stack.h"
#include "list.h"
#include "binTree.h"

/* definujeme si premennu na uchovanie chyby */
ERROR_CODE run_error;

typedef enum{
//Presun
	iMOV,
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
	iPUSP,
	iPOP,
	iTOP,
	iTOPPOP,
	iEMPTY,
	iINIT,
//Navratova hodnota
	iRET,
//Navestia
	iLABEL,
//Operacie s ramcami
	iINITFR,
	iDISPFR,
	iWRITEFR,
	iREADFR
} tInstrT;
