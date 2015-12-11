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

static void expand_str(int c, int *i){
 	if((*i) > 0){
 		if((str_value = (char *) memrealloc(str_value, (*i) + 2))){
 			str_value[(*i) + 1] = '\0';
 			str_value[(*i)] = c;
 			(*i)++;
 		} else{
 			//memfree(token.attribute);
 			run_error = INTERN_ERR;
 		}
 	} else{
 			str_value[(*i) + 1] = '\0';
 			str_value[(*i)] = c;
 			(*i)++;
 	}
}

static void init_string(int *i){
 	//memfree(PomUk);
 	 str_value = (char *) memmalloc((*i)+2);
 	if(str_value == NULL){
 		run_error = INTERN_ERR;
 	} else{
 		str_value[(*i)] = '\0';
 	}
 }



/* Spusta interpretaciu programu v IFJ15 */
ERROR_CODE interpret(Table_symbols *table){
run_error = OK_ERR;
int ret_value_func = 0;
int var_type = 0;

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

//Premenna ramca...
Frame_variable *Act_Var1 = NULL;
Frame_variable *Act_Var2 = NULL;
Frame_variable *Act_Res = NULL; 

//Pomocne premenne
char *res_str = NULL;
int res_int;

	while((run_error == OK_ERR) && (NextIP->next != NULL)){
		switch(Act_Instr->type){
			/* Operacie PRIRADENIA a ZISKANIA HODNOT */
			case iMOV:
			// source1 = meno termu pravej strany
			// source2 = meno termu do ktorej chceme priradit
				Act_Var1 = searchVariableInFrames(Fr_Stack, Act_Instr->source1);
				if(Act_Var1->inicialized == 0){
					run_error = UNINITI_ERR;
					return run_error;
				}

				Act_Var2 = searchVariableInFrames(Fr_Stack, Act_Instr->destination);
				run_error = copyValue(Fr_Stack, Act_Var1, Act_Var2);
			break;

			///////////////////////////////////////////

			/* ARITMETICKE operatory --> celociselne, desatinne, retazcove */
			case iADD:
			case iSUB:
			case iMUL:
			case iDIV:
				//Ocakava len samotnu instrukciu, vie co treba robit ak tak

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

				//Ocakava len samotnu instrukciu {iGREATER, iLESS atd} nic ineho !!!!!!!!!

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
				//ocakava navestie typu List kam treba skocit
			break;

			case iJZ:
				//Ocakava navestie typu List kam treba skocit, ked sa bude pracovat s IF, FOR
			break;

			case iJNZ:
				//Ocakava navestie typu List kam treba skocit, ked sa bude pracovat s IF, FOR
			break;

			case iCALL:
				//source1 = meno funkcie ktora sa ide vykonavat.
				current_function = searchFunction(table, Act_Instr->source1);
				Instr_tape = current_function->instructions;
				NextIP = Instr_tape.first_list_element;
			break;
			/////////////////////////////////////////

			/* Praca so VSTAVANYMI funkciami */
			case iSORT:
				//source1 = meno premennej - 1. parameter vstavanej funkcie
				//destination = meno premennej do ktorej ukladame navratovu hodnotu vstavanej funkcie
				Act_Var1 = searchVariableInFrames(Fr_Stack, Act_Instr->source1);

				if(Act_Var1->inicialized == 0){
					run_error = UNINITI_ERR;
					return run_error;
				}

				res_str = shell(Act_Var1->frame_var_value.S);
				Act_Res = searchVariableInFrames(Fr_Stack, Act_Instr->destination);

				run_error = setValueVariable(Fr_Stack, Act_Res->frame_var_name, res_str);
			break;

			case iLEN:
				//source1 = meno premennej - 1. parameter vstavanej funkcie
				//destination = meno premennej do ktorej ukladame navratovu hodnotu vstavanej funkcie
				Act_Var1 = searchVariableInFrames(Fr_Stack, Act_Instr->source1);

				if(Act_Var1->inicialized == 0){
					run_error = UNINITI_ERR;
					return run_error;
				}

				res_int = length_of_string(Act_Var1->frame_var_value.S);
				Act_Res = searchVariableInFrames(Fr_Stack, Act_Instr->destination);

				sprintf(res_str, "%d", res_int);

				run_error = setValueVariable(Fr_Stack, Act_Res->frame_var_name, res_str);
			break;

			case iSUBSTR:
				//ked vratu substr NULL, tak je to chyba 10
			break;

			case iFIND:
				//source1 = meno premennej - 1. parameter vstavanej funkcie
				//source2 = meno premennej - 2. parameter vst. funkcie
				//destination = meno premennej do ktorej ukladame navratovu hodnotu vstavanej funkcie
				Act_Var1 = searchVariableInFrames(Fr_Stack, Act_Instr->source1);
				Act_Var2 = searchVariableInFrames(Fr_Stack, Act_Instr->source2);

				if((Act_Var1->inicialized == 0) || (Act_Var2->inicialized == 0)){
					run_error = UNINITI_ERR;
					return run_error;
				}

				res_int = KMP_Find(Act_Var1->frame_var_value.S, Act_Var2->frame_var_value.S);
				Act_Res = searchVariableInFrames(Fr_Stack, Act_Instr->destination);

				sprintf(res_str, "%d", res_int);

				run_error = setValueVariable(Fr_Stack, Act_Res->frame_var_name, res_str);
			break;

			case iCONCAT:
				//source1 = meno premennej - 1. parameter vstavanej funkcie
				//source2 = meno premennej - 2. parameter vst. funkcie
				//destination = meno premennej do ktorej ukladame navratovu hodnotu vstavanej funkcieT:

				Act_Var1 = searchVariableInFrames(Fr_Stack, Act_Instr->source1);
				Act_Var2 = searchVariableInFrames(Fr_Stack, Act_Instr->source2);

				if((Act_Var1->inicialized == 0) || (Act_Var2->inicialized == 0)){
					run_error = UNINITI_ERR;
					return run_error;
				}

				res_str = concatenation(Act_Var1->frame_var_value.S, Act_Var2->frame_var_value.S);
				Act_Res = searchVariableInFrames(Fr_Stack, Act_Instr->destination);

				run_error = setValueVariable(Fr_Stack, Act_Res->frame_var_name, res_str);
			break;	
			/////////////////////////////////////////

			/* Instrukcie I/O Vstupu a Vystupu */
			case iWRITE:
				//source1 = meno toho co ideme vypisovat.
				Act_Var1 = searchVariableInFrames(Fr_Stack, Act_Instr->source1);

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
				//source1 = Meno toho do coho ideme zapisovat z cin

				/*+++++++++++++++++++++*/
				str_value = NULL;
				int state_of_str = sStart;
				/*+++++++++++++++++++++*/
				bool cont = true;
				int state = sStart;
				int c;
				int pom = 0;
				init_string(&pom);

				while((cont) && (c = getc(stdin))){
					switch(state){
						case sStart:
							if(isalpha(c)) 			state = sInteger;
							else if(c == '.')		state = sIsDouble;
							else if(isspace(c)){
								state = sStart;
								break;
							}
							else					state = sString;

						expand_str(c, &pom);
						break;

						case sInteger:
							if(isdigit(c)){
 								state = sInteger;
 								expand_str(c, &pom);
 							} else if(c == '.'){		//desatinne cislo
 								state = sIsDouble;
 								expand_str(c, &pom);
 							} else if((c == 'e') || (c == 'E')){	//cele cislo s exp.
 								state = sIsExpo;
 								expand_str(c, &pom);
 							} else{
 								//Nacitali sme ine ako cislo, posielame str int.
 								state_of_str = sInteger;
 								state = sEnd;
 								ungetc(c, stdin);
 							}
						break;

						case sIsDouble:
							if(isdigit(c)){
 								state = sDouble;
 								expand_str(c, &pom);
 							} else {
 								state_of_str = sDouble;
 								state = sEnd;
 								ungetc(c, stdin);
 							}
						break;

						case sDouble:
							if(isdigit(c)){
 								state = sDouble;
 								expand_str(c, &pom);
 							} else if((c == 'e') || (c == 'E')){
 								//desatinne cislo s exponentom
 								state = sIsExpo;
 								expand_str(c, &pom);
 							} else{
 								state_of_str = sDouble;
 								state = sEnd;
 								ungetc(c, stdin);
 							}
						break;		

						case sIsExpo:
							if((c == '+') || (c == '-')){
 								state = sIsExpo2;
 								expand_str(c, &pom);
 							} else if(isdigit(c)){
 								state = sExpo;
 								expand_str(c, &pom);
 							} else{
 								state_of_str = sDouble;
 								state = sEnd;
 								ungetc(c, stdin);
 							}
						break;

						case sIsExpo2:
							if(isdigit(c)){
 								state = sExpo;
 								expand_str(c, &pom);
 							} else{
 								//Chyba, po znamienku nasleduje iny znak ako cislo.
 								state_of_str = sDouble;
 								state = sEnd;
 								ungetc(c, stdin);
 							}
						break;

						case sExpo:
							if(isdigit(c)){
 								state = sExpo;
 								expand_str(c, &pom);
 							} else{
 								state_of_str = sDouble;
 								state = sEnd;
 								ungetc(c, stdin);
 							}
						break;

						case sString:
							if(!(isspace(c))){
								state = sString;
								expand_str(c, &pom);
							} else{
								state_of_str = sString;
								state = sEnd;
							}
						break;

						case sEnd:
							ungetc(c, stdin);
							cont = false;
						break;
					}
				}

				Act_Var1 = searchVariableInFrames(Fr_Stack, Act_Instr->source1);

				switch(state_of_str){
					case sInteger:
						if(Act_Var1->frame_var_type == sInteger){
							run_error = setValueVariable(Fr_Stack, Act_Var1->frame_var_name, str_value);
						} else{
							run_error = NUMERIC_ERR;
							return run_error;
						}
					break;

					case sDouble:
						if(Act_Var1->frame_var_type == sDouble){
							run_error = setValueVariable(Fr_Stack, Act_Var1->frame_var_name, str_value);
						} else{
							run_error = NUMERIC_ERR;
							return run_error;
						}
					break;

					case sString:
						if(Act_Var1->frame_var_type == sString){
							run_error = setValueVariable(Fr_Stack, Act_Var1->frame_var_name, str_value);
						} else{
							run_error = NUMERIC_ERR;
							return run_error;
						}
					break;
				}

			break;
			////////////////////////////////////////
			
			/* Operacie nad zasobnikom POSTFIX */

			case iDISPSTPOST:
				//Len instrukcia
				stackDestroy(Postfix_stack);
			break;

			case iPUSH:
				/*
				* source1 = meno premennej
				* source2 = ??
				* destination = ?? 
				*/
					Act_Var1 = searchVariableInFrames(Fr_Stack, Act_Instr->source1);
					run_error = stackPush(Postfix_stack, Act_Var1);
			break;

			case iPOP:
				//Len instrukcia - POSTFIX zasobnik
				stackPop(Postfix_stack);
			break;

			case iTOP:
				//Len instrukcia
				Act_Var1 = stackTop(Postfix_stack);
			break;

			case iTOPPOP:
				//Len instrukcia
				Act_Var1 = stackTop(Postfix_stack);
				stackPop(Postfix_stack);
			break;
			//////////////////////////////////////

			/* Praca s navestiami instrukcnej pasky */
			case iRET:
				//len instrukcia

				//next_instruction je asi len v base Frame
				NextIP = Act_Var1->next_instruction;
			break;

			case iLABEL:
				//Este neviem lebo lable :(
				;
			break;
			//////////////////////////////////////

			/* Praca s FRAME ramcami a zasobnikom ramcov */
			case iPUSHFR:
				//len instrukcia
				run_error = pushFrame(Fr_Stack);
			break;

			case iPOPFR:
				//Len instrukcia
				popFrame(Fr_Stack);
			break;

			case iDISPFR:
				//Len instrukcia
				destroyFramesToEnded(Fr_Stack);
			break;

			case iINSERT_TO_FR:
				//source1 = meno premennej ktoru ideme predavat do ramca
				//source2 = integer - akeho typu je premenna ktoru ideme pridavat {sInteger, sDouble, sString}
				var_type = *((int *)(Act_Instr->source2));
				Act_Var1 = insertVariableToFrame(Fr_Stack, Act_Instr->source1 , var_type);
			break;

			case iREADFR:
				//source1 = meno toho co ideme vyhladavat
				Act_Var1 = searchVariableInFrames(Fr_Stack, Act_Instr->source1);
			break;

			case iSETBASEFR:
				//NextIP - premenna typu List
				//ret_value_func - navratovy typ - integer
				fromPreparationDoBase(Fr_Stack, ret_value_func, NextIP);
			break;

			case iDISPOSEALL:
				//len instrukcia
				destroyAllFrames(Fr_Stack);
			break;

			case iBASE_TO_END:
				//len instrukcia
				fromBaseDoEnded(Fr_Stack);
			break;

			case iCOPY_VALUE:
				//source1 = z coho ideme kopirovat
				//source2 = do coho ideme kopirovat
				Act_Var1 = searchVariableInFrames(Fr_Stack, Act_Instr->source1);
				Act_Var2 = searchVariableInFrames(Fr_Stack, Act_Instr->source2);
				run_error = copyValue(Fr_Stack, Act_Var1, Act_Var2);
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
