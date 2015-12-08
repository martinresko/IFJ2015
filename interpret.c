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

Instruction * get_next_instr(List NextIP){
	Instruction *Act_Instr = (Instruction *) NextIP->data;
	return Act_Instr;
}

ERROR_CODE interpret(Table_symbols *table){
run_error = OK_ERR;
tFrame *frame;
//Inicializujeme zasobnik na ramce
StackPointer *Fr_Stack = NULL;
stackInit(Fr_Stack);
//Inicializujeme zasobnik na postfixove vycislenie
StackPointer *Postfix_stack = NULL;
stackInit(Postfix_stack);
Function_GTS *current_function = searchFunction(table, "main"); //Vyhladame funckiu main v TS
ListPointer Instr_tape = current_function->instructions; //Nacitana instrukcna paska
//Prvy prvok na instrukcnej paske
List NextIP = Instr_tape.first_list_element;
//Dostaneme prvu instrukciu z instrukcnej pasky
Instruction *Act_Instr = get_next_instr(NextIP);
tPostFixNum *op1, *op2, *res = NULL;

	while((run_error == OK_ERR) && (NextIP->next != NULL)){
		switch(Act_Instr->type){
			case iMOV:
				
			break;

			case iGETNUM:
				run_error = stackPush(Postfix_stack, Act_Instr->source1);
				if(!(run_error)){
					return run_error;
				}
			break;

			case iADD:
				op2 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);
				op1 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);

				res->data->iData = op1->data->iData + op2->data->iData;
				run_error = stackPush(Postfix_stack, res);
			break;

			case iSUB:
				op2 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);
				op1 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);

				res->data->iData = op1->data->iData - op2->data->iData;
				run_error = stackPush(Postfix_stack, res);
			break;

			case iMUL:
				op2 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);
				op1 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);

				res->data->iData = op1->data->iData * op2->data->iData;
				run_error = stackPush(Postfix_stack, res);
			break;

			case iDIV:
				op2 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);
				op1 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);

				res->data->iData = op1->data->iData / op2->data->iData;
				run_error = stackPush(Postfix_stack, res);
			break;

			case iGREATER:
				op2 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);
				op1 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);

				res->data->iData = op1->data->iData > op2->data->iData;
				run_error = stackPush(Postfix_stack, res);
			break;

			case iLESS:
				op2 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);
				op1 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);

				res->data->iData = op1->data->iData < op2->data->iData;
				run_error = stackPush(Postfix_stack, res);
			break;

			case iEGREATER:
				op2 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);
				op1 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);
				res->data->iData = op1->data->iData >= op2->data->iData;
				run_error = stackPush(Postfix_stack, res);
			break;

			case iELESS:
				op2 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);
				op1 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);

				res->data->iData = op1->data->iData <= op2->data->iData;
				run_error = stackPush(Postfix_stack, res);
			break;

			case iEQUAL:
				op2 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);
				op1 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);

				res->data->iData = op1->data->iData == op2->data->iData;
				run_error = stackPush(Postfix_stack, res);
			break;

			case iNEQUAL:
				op2 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);
				op1 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);

				res->data->iData = op1->data->iData != op2->data->iData;
				run_error = stackPush(Postfix_stack, res);
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
				frame = Act_Instr->source1;
				run_error = stackPush(Fr_Stack, frame);
				if(!(run_error)){
					return run_error;
				}
			break;

			case iPOP:
				stackPop(Fr_Stack);
			break;

			case iTOP:
				frame = stackTop(Fr_Stack);
			break;

			case iTOPPOP:
				frame = stackTop(Fr_Stack);
				stackPop(Fr_Stack);
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
		//Posunieme sa na instrukcnej paske
		NextIP = NextIP->next;
		//Poziadame o nasledujucu instrukciu
		Act_Instr = get_next_instr(NextIP);
	}
	return run_error;
}