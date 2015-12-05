/**
 * Predmet: IFJ / IAL
 * Subor:     error.c
 *            Implementacia zachytavanie errorov
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
 */
#include "error.h"

void call_error(ERROR_CODE tmp, ERROR_MESSAGE pom){

	switch (tmp) {
		case OK_ERR :
			call_error_message(pom);
			exit(0);
		case LEX_ERR :
			call_error_message(pom);
			exit(1);
		case SYN_ERR :
			call_error_message(pom);
			exit(2);
		case SEM_UNDEF_ERR :
			call_error_message(pom);
			exit(3);
		case SEM_TYPE_ERR :
			call_error_message(pom);    // vsade doplnit funkciu na mazannie globalnych veci...
			exit(4);
		case DATA_TYPE_ERR :
			call_error_message(pom);
			exit(5);
		case SEM_OTHER_ERR :
			call_error_message(pom);
			exit(6);
		case NUMERIC_ERR :
			call_error_message(pom);
			exit(7);
		case UNINITI_ERR :
			call_error_message(pom);
			exit(8);
		case ZERO_DIV_ERR :
			call_error_message(pom);
			exit(9);
		case RUNNING_ERR :
			call_error_message(pom);
			exit(10);
		case INTERN_ERR :
			call_error_message(pom);
			exit(99);
		}

}

void call_error_message(ERROR_MESSAGE pom) {

	switch (pom) {
		case WRONG_SCALE :
			fprintf(stderr,"Zle zvoleny rozsah\n");
			break;
		case FAILED_MALLOC :
			fprintf(stderr,"Funkcia malloc zlyhala\n");
			break;
		}
}
