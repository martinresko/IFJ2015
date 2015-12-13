/**
 * Predmet: IFJ / IAL
 * Subor:     main.c
 *            Main funkcia programu
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
 #include "parser.h"
 #include "ial.h"
 #include "garbage.h"

 #define OK_ERR 0

ERROR_CODE error;
FILE *file;
Table_symbols symbol_table;

int main(int argc, char **argv){

	meminitialization();/* init garbage */

	if(argc != 2){
		fprintf(stderr, "Bol zadany nespravny pocet argumentov.\n");
		memallfree(); /* volanie garbage aby upratal */
		return INTERN_ERR;
	}

	if(!(file = fopen(argv[1], "r"))){
		fprintf(stderr, "Subor sa nepodarilo otvorit.\n");
		memallfree(); /* volanie garbage aby upratal */
		return INTERN_ERR;
	}
	
	error = prog();
	
    printf("PARSER OK\n");
    
    if (error == OK_ERR) {
        error = interpret(&symbol_table);
    }

	printf("koniec programu navratovi kod je %d\n",error );

	destroyGTS(&symbol_table);
	fclose(file);
	
	memallfree(); /* volanie garbage aby upratal */
return error;
}
