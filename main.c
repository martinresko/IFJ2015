/**
 * Predmet: IFJ / IAL
 * Subor:     ial.c
 *            Implementacia vstavaných funkcií
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
 */

//Hlavickove subory
 #include <stdlib.h>
 #include <stdio.h>
 #include "scaner.h"

 #define INTER_ERR 99
 #define OK_ERR 0


FILE *file;

int main(int argc, char **argv){

	if(argc != 2){
		fprintf(stderr, "Bol zadany nespravny pocet argumentov.\n");
		return INTER_ERR;
	}

	if(!(file = fopen(argv[1], "r"))){
		fprintf(stderr, "Subor sa nepodarilo otvorit.\n");
		return INTER_ERR;
	}

return OK_ERR;
}