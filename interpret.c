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

/*
* Nacitame si instrukcnu pasku funkcie main, Pristupime
* k prvej instrukcii. Inicializujeme zasobnik ramcov.
* Zavolame hlavnu funkciu interpretu.
*/
ERROR_CODE init_interpret(Table_Symbols *table){
	ListPointer Instr_tape;
	List NextIP;
	Instr_tape = table.actual_function->instructions;
	NextIP = Instr_tape->first_list_element;
	StackPointer *Stack;
	stackInit(Stack);
	run_error = interpret()
}

ERROR_CODE interpret(){
run_error = OK_ERR;
Act_Instr = NextIP->data;

	while((run_error == OK_ERR) && (NextIP->next != NULL)){
		switch(Act_Instr->type){
			case iMOV:
				
			break;

			case iADD:

			break;

			case iSUB:

			break;

			case iMUL:

			break;

			case iDIV:

			break;

			case iGREATER:

			break;

			case iLESS:

			break;

			case iEGREATER;

			break;

			case iELESS:

			break;

			case iEQUAL:

			break;

			case iNEQUAL:

			break;

			case iJMP:

			break;

			case iJZ:

			break;

			case iJNZ:

			break;

			case iCALL:

			break;

			case iSORT:

			break;

			case iLEN:

			break;

			case iSUBSTR:

			break;

			case iFIND:

			break;

			case iCONCAT:

			break;

			case iWRITE:

			break;

			case iREAD:

			break;

			case iPUSH:

			break;

			case iPOP:

			break;

			case iTOP:

			break;

			case iTOPPOP:

			break;

			case iEMPTY:

			break;

			case iINIT:

			break;

			case iRET:

			break;

			case iLABEL:

			break;

			case iINITFR:

			break;

			case iDISPFR:

			break;

			case iWRITEFR:

			break;

			case iREADFR:

			break;
		}
		Act_Instr = Act_Instr->next;
	}
}