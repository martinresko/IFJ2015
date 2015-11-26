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
 #include "error.h"

FILE *file;

int main(int argc, char **argv){

	if(argc != 2){
		fprintf(stderr, "Bol zadany nespravny pocet argumentov.\n");
		return INTERN_ERR;
	}

	if(!(file = fopen(argv[1], "r"))){
		fprintf(stderr, "Subor sa nepodarilo otvorit.\n");
		return INTERN_ERR;
	}

return ERR_OK;
}