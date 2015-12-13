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

	switch (error) {
		case OK_ERR:
        	error = interpret(&symbol_table);
        //CHYBA V LEXIKALNEJ ANALYZE
        case LEX_ERR: 
        	fprintf(stderr,"ERROR CHYBA V LEXIKALNEJ ANALYZE\n");
        break;
        //CHYBA V SYNTAKTICKEJ ANALYZE
        case SYN_ERR: 
        	fprintf(stderr,"ERROR CHYBA V SYNTAKTICKEJ ANALYZE\n");
        break;
        //CHYBA V SEMANTICKEJ ANALYZE
        case SEM_UNDEF_ERR: 
        	fprintf(stderr,"ERROR CHYBA V SEMANTICKEJ ANALYZE NEDEFINOVANA/REDEFINOVANA FUKCIA/PREMENNA\n");
        break;
        //CHYBA V SEMANTICKEJ ANALYZE
        case SEM_TYPE_ERR: 
        	fprintf(stderr,"ERROR CHYBA V SEMANTICKEJ ANALYZE ZLA TYPOVA KOMPATIBILITA\n");
        break;
        //CHYBA V SEMANTICKEJ ANALYZE
        case DATA_TYPE_ERR: 
        	fprintf(stderr,"ERROR CHYBA V SEMANTICKEJ ANALYZE CHYBA PRI ODVODZOVANI DATOEHO TYPU\n");
        break;
        //CHYBA V SEMANTICKEJ ANALYZE
        case SEM_OTHER_ERR: 
        	fprintf(stderr,"ERROR CHYBA V SEMANTICKEJ ANALYZE OSTATNE SEMANTICKE CHYBY\n");
        break;
        //BEHOVA CHYBA
        case NUMERIC_ERR: 
        	fprintf(stderr,"ERROR BEHOVA CHYBA PRI NACITANI CISELNEJ HODNOTY ZO VSTUPU\n");
        break;
        //BEHOVA CHYBA
        case UNINITI_ERR: 
        	fprintf(stderr,"ERROR BEHOVA CHYBA PRI PRACI S NEINIICILIZOVANOU PREMENNOU\n");
        break;
        //BEHOVA CHYBA
        case ZERO_DIV_ERR: 
        	fprintf(stderr,"ERROR BEHOVA CHYBA PRI DELENNI NULOU\n");
        break;
        //BEHOVA CHYBA
        case RUNNING_ERR: 
        	fprintf(stderr,"ERROR OSTATNE BEHOVE CHYBY\n");
        break;
        case INTERN_ERR: 
        	fprintf(stderr,"ERROR INTERNNA CHYBA INTERPRETU\n");
        break;
    }
	

	printf("koniec programu navratovi kod je %d\n",error );

	destroyGTS(&symbol_table);
	fclose(file);
	
	memallfree(); /* volanie garbage aby upratal */
return error;
}
