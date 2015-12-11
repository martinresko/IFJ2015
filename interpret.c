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

/* Nacitanie nasledujucej instrukcie */
Instruction * get_next_instr(List NextIP){
	Instruction *Act_Instr = (Instruction *) NextIP->data;
	return Act_Instr;
}

/* Spusta interpretaciu programu v IFJ15 */
ERROR_CODE interpret(Table_symbols *table){
run_error = OK_ERR;

//Inicializujeme zasobnik na ramce
StackPointer *Fr_Stack = NULL;
stackInit(Fr_Stack);

//Inicializujeme zasobnik na postfixove vycislenie
StackPointer *Postfix_stack = NULL;
stackInit(Postfix_stack);

//Vyhladame si instrukcnu pasku funkcie main
Function_GTS *current_function = searchFunction(table, "main");
ListPointer Instr_tape = current_function->instructions;
List NextIP = Instr_tape.first_list_element;

//Ziskame nasledujucu instrukciu
Instruction *Act_Instr = get_next_instr(NextIP);

//aktualny ramec...

//Frame FR_element;

//Premenna ramca...
Frame_variable *Act_Var1 = NULL;
Frame_variable *Act_Var2 = NULL;
Frame_variable *Act_Res = NULL;


	while((run_error == OK_ERR) && (NextIP->next != NULL)){
		switch(Act_Instr->type){
			/* Operacie PRIRADENIA a ZISKANIA HODNOT */
			case iMOV:
				
			break;

			case iGETVALUE:
				
			break;
			///////////////////////////////////////////

			/* ARITMETICKE operatory --> celociselne, desatinne, retazcove */
			case iADD:
			case iSUB:
			case iMUL:
			case iDIV:

				Act_Var2 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);
				Act_Var1 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);

				if((Act_Var1->inicialized == 0) || (Act_Var2->inicialized == 0)){
					run_error = UNINITI_ERR;
					return run_error;
				}

				if((Act_Var1->frame_var_type == sInteger) && (Act_Var2->frame_var_type == sInteger)){
					Act_Res->frame_var_type = sInteger;
					switch(Act_Instr->type){
						case iADD:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.I + Act_Var2->frame_var_value.I;
						break;

						case iSUB:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.I - Act_Var2->frame_var_value.I;
						break;

						case iMUL:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.I * Act_Var2->frame_var_value.I;
						break;

						case iDIV:
							if(Act_Var2->frame_var_value.I == 0){
								run_error = ZERO_DIV_ERR;
								return run_error;
							}
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.I / Act_Var2->frame_var_value.I;
						break;
					}
				}
				else if((Act_Var1->frame_var_type == sInteger) && (Act_Var2->frame_var_type == sDouble)){
					Act_Res->frame_var_type = sDouble;
					switch(Act_Instr->type){
						case iADD:
							Act_Res->frame_var_value.D = (double)Act_Var1->frame_var_value.I + Act_Var2->frame_var_value.D;
						break;

						case iSUB:
							Act_Res->frame_var_value.D = (double)Act_Var1->frame_var_value.I - Act_Var2->frame_var_value.D;
						break;

						case iMUL:
							Act_Res->frame_var_value.D = (double)Act_Var1->frame_var_value.I * Act_Var2->frame_var_value.D;
						break;

						case iDIV:
							if(Act_Var2->frame_var_value.D == 0.0){
								run_error = ZERO_DIV_ERR;
								return run_error;
							}
							Act_Res->frame_var_value.D = (double)Act_Var1->frame_var_value.I / Act_Var2->frame_var_value.D;
						break;
					}
				}
				else if((Act_Var1->frame_var_type == sDouble) && (Act_Var2->frame_var_type == sInteger)){
					Act_Res->frame_var_type = sDouble;
					switch(Act_Instr->type){
						case iADD:
							Act_Res->frame_var_value.D = Act_Var1->frame_var_value.D + (double)Act_Var2->frame_var_value.I;
						break;

						case iSUB:
							Act_Res->frame_var_value.D = Act_Var1->frame_var_value.D - (double)Act_Var2->frame_var_value.I;
						break;

						case iMUL:
							Act_Res->frame_var_value.D = Act_Var1->frame_var_value.D * (double)Act_Var2->frame_var_value.I;
						break;

						case iDIV:
							if(Act_Var2->frame_var_value.I == 0){
								run_error = ZERO_DIV_ERR;
								return run_error;
							}
							Act_Res->frame_var_value.D = Act_Var1->frame_var_value.D / (double)Act_Var2->frame_var_value.I;
						break;
					}
				}
				else if((Act_Var1->frame_var_type == sDouble) && (Act_Var2->frame_var_type == sDouble)){
					Act_Res->frame_var_type = sDouble;
					switch(Act_Instr->type){
						case iADD:
							Act_Res->frame_var_value.D = Act_Var1->frame_var_value.D + Act_Var2->frame_var_value.D;
						break;

						case iSUB:
							Act_Res->frame_var_value.D = Act_Var1->frame_var_value.D - Act_Var2->frame_var_value.D;
						break;

						case iMUL:
							Act_Res->frame_var_value.D = Act_Var1->frame_var_value.D * Act_Var2->frame_var_value.D;
						break;

						case iDIV:
							if(Act_Var2->frame_var_value.D == 0.0){
								run_error = ZERO_DIV_ERR;
								return run_error;
							}
							Act_Res->frame_var_value.D = Act_Var1->frame_var_value.D / Act_Var2->frame_var_value.D;
						break;
					}
				}

				run_error = stackPush(Postfix_stack, Act_Res);
			break;

			//////////////////////////////////////////

			/* RELACNE operatory (logicke) --> vysledok TRUE || FALSE reprezentovane int */
			case iGREATER:
			case iLESS:
			case iEGREATER:
			case iELESS:
			case iEQUAL:
			case iNEQUAL:

				Act_Var2 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);
				Act_Var1 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);

				if((Act_Var1->inicialized == 0) || (Act_Var2->inicialized == 0)){
					run_error = UNINITI_ERR;
					return run_error;
				}

				if((Act_Var1->frame_var_type == sInteger) && (Act_Var2->frame_var_type == sInteger)){
					Act_Res->frame_var_type = sInteger;
					switch(Act_Instr->type){
						case iGREATER:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.I > Act_Var2->frame_var_value.I;
						break;

						case iLESS:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.I < Act_Var2->frame_var_value.I;
						break;

						case iEGREATER:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.I >= Act_Var2->frame_var_value.I;
						break;

						case iELESS:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.I <= Act_Var2->frame_var_value.I;
						break;

						case iEQUAL:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.I == Act_Var2->frame_var_value.I;
						break;

						case iNEQUAL:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.I != Act_Var2->frame_var_value.I;
						break;
					}
				}
				else if((Act_Var1->frame_var_type == sDouble) && (Act_Var2->frame_var_type == sDouble)){
					Act_Res->frame_var_type = sInteger;
					switch(Act_Instr->type){
						case iGREATER:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.D > Act_Var2->frame_var_value.D;
						break;

						case iLESS:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.D < Act_Var2->frame_var_value.D;
						break;

						case iEGREATER:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.D >= Act_Var2->frame_var_value.D;
						break;

						case iELESS:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.D <= Act_Var2->frame_var_value.D;
						break;

						case iEQUAL:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.D == Act_Var2->frame_var_value.D;
						break;

						case iNEQUAL:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.D != Act_Var2->frame_var_value.D;
						break;
					}
				}
				else if((Act_Var1->frame_var_type == sString) && (Act_Var2->frame_var_type == sString)){
					Act_Res->frame_var_type = sInteger;
					switch(Act_Instr->type){
						case iGREATER:
							Act_Res->frame_var_value.I = strcmp(Act_Var1->frame_var_value.S, Act_Var2->frame_var_value.S) > 0;
						break;

						case iLESS:
							Act_Res->frame_var_value.I = strcmp(Act_Var1->frame_var_value.S, Act_Var2->frame_var_value.S) < 0;
						break;

						case iEGREATER:
							Act_Res->frame_var_value.I = strcmp(Act_Var1->frame_var_value.S, Act_Var2->frame_var_value.S) >= 0;
						break;

						case iELESS:
							Act_Res->frame_var_value.I = strcmp(Act_Var1->frame_var_value.S, Act_Var2->frame_var_value.S) <= 0;
						break;

						case iEQUAL:
							Act_Res->frame_var_value.I = strcmp(Act_Var1->frame_var_value.S, Act_Var2->frame_var_value.S) == 0;
						break;

						case iNEQUAL:
							Act_Res->frame_var_value.I = strcmp(Act_Var1->frame_var_value.S, Act_Var2->frame_var_value.S) != 0;
						break;
					}
				}
				else if((Act_Var1->frame_var_type == sInteger) && (Act_Var2->frame_var_type == sDouble)){
					Act_Res->frame_var_type = sInteger;
					switch(Act_Instr->type){
						case iGREATER:
							Act_Res->frame_var_value.I = (double)Act_Var1->frame_var_value.I > Act_Var2->frame_var_value.D;
						break;

						case iLESS:
							Act_Res->frame_var_value.I = (double)Act_Var1->frame_var_value.I < Act_Var2->frame_var_value.D;
						break;

						case iEGREATER:
							Act_Res->frame_var_value.I = (double)Act_Var1->frame_var_value.I >= Act_Var2->frame_var_value.D;
						break;

						case iELESS:
							Act_Res->frame_var_value.I = (double)Act_Var1->frame_var_value.I <= Act_Var2->frame_var_value.D;
						break;

						case iEQUAL:
							Act_Res->frame_var_value.I = (double)Act_Var1->frame_var_value.I == Act_Var2->frame_var_value.D;
						break;

						case iNEQUAL:
							Act_Res->frame_var_value.I = (double)Act_Var1->frame_var_value.I != Act_Var2->frame_var_value.D;
						break;
					}
				}
				else if((Act_Var1->frame_var_type == sDouble) && (Act_Var2->frame_var_type == sInteger)){
					Act_Res->frame_var_type = sInteger;
					switch(Act_Instr->type){
						case iGREATER:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.D > (double)Act_Var2->frame_var_value.I;
						break;

						case iLESS:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.D < (double)Act_Var2->frame_var_value.I;
						break;

						case iEGREATER:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.D >= (double)Act_Var2->frame_var_value.I;
						break;

						case iELESS:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.D <= (double)Act_Var2->frame_var_value.I;
						break;

						case iEQUAL:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.D == (double)Act_Var2->frame_var_value.I;
						break;

						case iNEQUAL:
							Act_Res->frame_var_value.I = Act_Var1->frame_var_value.D != (double)Act_Var2->frame_var_value.I;
						break;
					}
				}

				run_error = stackPush(Postfix_stack, Act_Res);
			break;
			/////////////////////////////////////////

			/* Nepodmienene, podmienene SKOKY a volanie funkcii */
			case iJMP:
				//goto
			break;

			case iJZ:
				//goto
			break;

			case iJNZ:
				//goto
			break;

			case iCALL:

			break;
			/////////////////////////////////////////

			/* Praca so VSTAVANYMI funkciami */
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
			/////////////////////////////////////////

			/* Instrukcie I/O Vstupu a Vystupu */
			case iWRITE:
				/* Potrebujeme zistit co ideme vypisovat */
				if(!(Act_Var1->inicialized)){
					run_error = UNINITI_ERR;
					return run_error;
				}

				switch(Act_Var1->frame_var_type){
					case sInteger:
						printf("%d", Act_Var1->frame_var_value.I);
					break;

					case sDouble:
						printf("%g", Act_Var1->frame_var_value.D);
					break;

					case sString:
						printf("%s", Act_Var1->frame_var_value.S);
					break;
				}
			break;

			case iREAD:

			break;
			////////////////////////////////////////
			
			/* Operacie nad zasobnikom POSTFIX */

			case iDISPSTPOST:
				stackDestroy(Postfix_stack);
			break;

			case iPUSH:
				//Act_Var1 = searchVariableInFrames(Fr_Stack, /* char * */)
				run_error = stackPush(Postfix_stack, Act_Var1);
			break;

			case iPOP:
				stackPop(Postfix_stack);
			break;

			case iTOP:
				Act_Var1 = stackTop(Postfix_stack);
			break;

			case iTOPPOP:
				Act_Var1 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);
			break;
			//////////////////////////////////////

			/* Praca s navestiami instrukcnej pasky */
			case iRET:

			break;

			case iLABEL:
				;
			break;
			//////////////////////////////////////

			/* Praca s FRAME ramcami a zasobnikom ramcov */
			case iPUSHFR:
				run_error = pushFrame(Fr_Stack);
			break;

			case iPOPFR:
				popFrame(Fr_Stack);
			break;

			case iDISPFR:
				destroyFramesWithBase(Fr_Stack);
				destroyFramesToEnded(Fr_Stack);
			break;

			case iINSERT_TO_FR:
				//Act_Var1 = insertVariableToFrame(Fr_Stack, /* char * , int */);
			break;

			case iREADFR:
				//Act_Var1 = searchVariableInFrames(Fr_Stack, /* char * */);
			break;

			case iSETBASEFR:
				//fromPreparationDoBase(Fr_Stack,/* int */, NextIP);
			break;

			case iDISPOSEALL:
				destroyAllFrames(Fr_Stack);
			break;

			case iBASE_TO_END:
				fromBaseDoEnded(Fr_Stack);
			break;

			case iCOPY_VALUE:
				run_error = copyValue(Fr_Stack,Act_Var1, Act_Var2);
			break;
			//////////////////////////////////////
		}
		//Posunieme sa na instrukcnej paske
		NextIP = NextIP->next;
		//Poziadame o nasledujucu instrukciu
		Act_Instr = get_next_instr(NextIP);
	}
	return run_error;
}