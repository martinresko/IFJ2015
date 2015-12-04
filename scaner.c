/**
 * Predmet: IFJ / IAL
 * Subor:     scaner.c
 *            Implementacia lexikálnej analýzy
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
 */


#include "scaner.h"

#define NUM_OF_KEYWORDS 10	//pocet klucovych slov
#define NUM_OF_RESWORDS 5 	//pocet rezervovanych slov

tToken token; //globalna premenna reprezentujuca token
int row = 0;  //globalna premenna reprezentujuca aktualny riadok
FILE *file;

/* Tabulka klucovych slov */
 const char *keywords[NUM_OF_KEYWORDS] = {
 	"auto\0", "cin\0", "cout\0", "double\0", "else\0",
 	"for\0", "if\0", "int\0", "return\0", "string\0"
 };

/* 
* Tabulka rezervovanych slov 
* Mena vstavanych funkcii jazyka IFJ15
*/
 const char *reswords[NUM_OF_RESWORDS] = {
 	"lenght\0", "substr\0", "concat\0",
 	"find\0", "sort\0"
 };

/*
* funkcia ma dva parametre: aktualny znak a
* ukazatel na pocet nacitanych znakov.
* Funkcia realokuje pamat pre atribut tokenu a rozsiri ho 
* o preve nacitany znak. 
*/
 static void expand_token(int c, int *i){
 	if((*i) > 0){
 		if((token.attribute = (char *) realloc(token.attribute, (*i) + 2))){
 			token.attribute[(*i) + 1] = '\0';
 			token.attribute[(*i)] = c;
 			(*i)++;
 		} else{
 			free(token.attribute);
 			scaner_error = INTERN_ERR;
 		}
 	} else{
 			token.attribute[(*i) + 1] = '\0';
 			token.attribute[(*i)] = c;
 			(*i)++;
 	}
 } 


/*
* Funkcia prevadza hexadecimalne
* cislo na cislo v desiatkovej sustave.
*/
int HextoDec(char *hexa){
 	int i;
 	int value = 0;
 	int base = 16;

	for (i = 0; i < 2; i++){
 		//Druhe cislo nenasobime 16.
 		if(i > 0){
 			base = 1;
 		}

 		if((hexa[i] >= '0') && (hexa[i] <= '9')){
 			value += ((int)hexa[i] - 48) * base;
 		} else if((hexa[i] >= 'a') && (hexa[i] <= 'f')){
 			value += ((int)hexa[i] - 55) * base;
 		} else if((hexa[i] >= 'A') && (hexa[i] <= 'F')){
 			value += ((int)hexa[i] - 87) * base;
 		}
 	}

 	return value;
}

/*
* funkcia ma parameter prave nacitany znak c.
* Ak znak nie je biely, tak ho funkcia vrati
* spat do toku znakov, spravi opak funkcie getc().
*/
static void undo_c(int c){
	if(!(isspace(c))){
 		ungetc(c, file);
 	}
 }

/*
* Funkcia prejde tabulku kluovych a rezervovanych slov
* a postupne porovnava nacitany retazec s klucovymi slovami.
*/
 static tState check_keyword(char *s){
int j;

 	for(j = 0; j < NUM_OF_KEYWORDS; j++){
 		if(!(strcmp(s,keywords[j]))){
 			return sKeyWord;
 		}
 	}

 	for(j = 0; j < NUM_OF_RESWORDS; j++){
 		if(!(strcmp(s,reswords[j]))){
 			return sResWord;
 		}
 	}

 	return sIdent;		//nenastala zhoda, vracia sa obycajny identifikator
 }

/*
* doplni id tokenu.
*/
 static void fill_token(tState state){
 	token.id = state;
}

/*
* Funkcia inicializuje retazec pre token.
*/
 static void init_string(int *i){
 	free(token.attribute);
 	token.attribute = (char *) malloc((*i)+2);
 	if(token.attribute == NULL){
 		free(token.attribute);
 		scaner_error = INTERN_ERR;
 	} else{
 		token.attribute[(*i)] = '\0';
 	}
 }

/* Funkcia spusti konecny automat na spracovanie lexemu */
 tToken get_Token(){
 	tState state = sStart;
 	int i = 0;		// pocitadlo nacitanych znakov
 	int c;			// prave nacitany znak
 	int escap;		//pomocna premenna na urcenie escape sekvencii
 	int escap2;		// -||-
 	bool cont = true;
 	char hexa[3];		//spracovanie hexadecimalneho cisla.

/* inicializacia tokenu */
 	token.id = sStart;
 	init_string(&i);

/* Cyklus while ktory reprezentuje DKA */
 	while((c = getc(file)) && (cont)){
 		switch(state){
 			case sStart:{
 				if((isalpha(c)) || (c == '_'))				state = sIdent;
 				else if(c == '0')							state = sNull;
 				else if((c > '0') && (c <= '9'))			state = sInteger;
 				else if(c == '=')							state = sAssign;
 				else if(c == '/')							state = sDivide;
 				else if(c == '!')							state = sExclam;
 				else if(c == '>')							state = sGreater;
 				else if(c == '<')							state = sLess;
 				else if(c == ';')							state = sSemicolon;
 				else if(c == '(')							state = sLParenth;
 				else if(c == ')')							state = sRParenth;
 				else if(c == '{')							state = sLSetPar;
 				else if(c == '}')							state = sRSetPar;
 				else if(c == '+')							state = sPlus;
 				else if(c == '-')							state = sMinus;
 				else if(c == '*')							state = sMult;
 				else if(c == ',')							state = sComma;
 				else if(c == EOF)							state = sEndofFile;
 				
 				else if(c == '"'){						
 					state = sString;
 					break;
 				} 	

 				//Ak sa jedna o biely znak
 				else if(isspace(c)){
 					state = sStart;
 					break;
 				}
 				//Nejedna sa o lexem jazyka IFJ15
 				else{
 					state = sError;
 					break;
 				}

 				//rozsir token o prve nacitany znak
 				expand_token(c, &i);
 				break;
 			}

 			//prvy nacitany znak bol 0
 			case sNull:{		
 				if(c == '0'){
 					state = sNull;
 				} else if((c > '0') && (c <= '9')){
 					state = sInteger;
 					expand_token(c, &i);	//rozsirime token o jeden znak
 				} else{
					state = sInteger;
 					fill_token(state);		//prepiseme id tokenu
 					state = sEnd;
 					undo_c(c);		//posledny nacitany znak vratime spat
 				}
 				break;
 			}

 			//prvy nacitany znak bol '_' alebo pismeno
 			case sIdent:{
 				if((isdigit(c)) || (isalpha(c)) || c == '_'){
 					state = sIdent;
 					expand_token(c, &i);
 				} else{
 					/* 
 					*  kedze sa jedna o identifikator, musime sa
 					*  uistit, ci nejde o klucove alebo o rezervovane slovo
 					*/
 					token.id = check_keyword(token.attribute);
 					state = sEnd;
 					undo_c(c);
 				}
 				break;
 			}

 			//Prvy nacitany znak bol numericky.
 			case sInteger:{
 				if(isdigit(c)){
 					state = sInteger;
 					expand_token(c, &i);
 				} else if(c == '.'){		//desatinne cislo
 					state = sIsDouble;
 					expand_token(c, &i);
 				} else if((c == 'e') || (c == 'E')){	//cele cislo s exp.
 					state = sIsExpo;
 					expand_token(c, &i);
 				} else{
 					//Nacitali sme ine ako cislo, posielame token int.
 					fill_token(state);
 					state = sEnd;
 					undo_c(c);
 				}
 				break;
 			}

 			//skontrolujem ci po desatinnej bodke nasleduje cislo
 			case sIsDouble:{
 				if(isdigit(c)){
 					state = sDouble;
 					expand_token(c, &i);
 				} else {
 					state = sError;
 					undo_c(c);
 				}
 				break;
 			}

 			//desatinne cislo
 			case sDouble:{
 				if(isdigit(c)){
 					state = sDouble;
 					expand_token(c, &i);
 				} else if((c == 'e') || (c == 'E')){
 					//desatinne cislo s exponentom
 					state = sIsExpo;
 					expand_token(c, &i);
 				} else{
 					fill_token(state);
 					state = sEnd;
 					undo_c(c);
 				}
 				break;
 			}

 			/* skontrolujeme ci je zadane znamienko pre exponent
 			*  alebo hned nasleduje cislo, v opacnom pripade sa jedna
 			*  o chybu. */
 			case sIsExpo:{
 				if((c == '+') || (c == '-')){
 					state = sIsExpo2;
 					expand_token(c, &i);
 				} else if(isdigit(c)){
 					state = sExpo;
 					expand_token(c, &i);
 				} else{
 					state = sError;
 					undo_c(c);
 				}
 				break;
 			}

 			//skontrolujeme ci po znamienku je cislo
 			case sIsExpo2:{
 				if(isdigit(c)){
 					state = sExpo;
 					expand_token(c, &i);
 				} else{
 					//Chyba, po znamienku nasleduje iny znak ako cislo.
 					state = sError;
 					undo_c(c);
 				}
 			}

 			//nacitavanie cislic pre exponent.
 			case sExpo:{
 				if(isdigit(c)){
 					state = sExpo;
 					expand_token(c, &i);
 				} else{
 					state = sDouble;
 					fill_token(state);
 					state = sEnd;
 					undo_c(c);
 				}
 				break;
 			}

 			//Ak bola nacitana uvodzovka, jedna sa o retazec.
 			case sString:{
 				if(c == '"'){
 					fill_token(state);
 					state = sEnd;
 				} else if((c == '\n') || (c == EOF)){
 					state = sError;
 				} else if(c == 92){
 					state = sEscSeq;
 				} else{
 					expand_token(c, &i);
 					state = sString;
 				}
 				break;
 			}

 			//Escape sekvencia v retazci.
 			case sEscSeq:{
 				if(c == '"'){
 					state = sString;
 					expand_token(c, &i);
 				} else if(c == 'n'){
 					escap = '\n';
 					state = sString;
 					expand_token(escap, &i);
 				} else if(c == 't'){
 					escap = '\t';
 					state = sString;
 					expand_token(escap, &i);
 				} else if(c == 92){
 					state = sString;
 					expand_token(c, &i);
 				} else if(c == 'x'){
 					escap = 'x';
 					state = sEscHex;
 				} else{
 					state = sString;
 					expand_token(92, &i);
 					expand_token(c, &i);
 				}
 				break;
 			}

 			/* 
 			* Zadavanie znakov pomocou escape
 			* sekvencie vramci retazcoveho literalu
 			*/
 			case sEscHex:{
 				if(isxdigit(c)){
 					state = sEscHex2;
 					escap2 = c;
 					hexa[0] = (char)c;
 				} else{
 					state = sString;
 					expand_token(92, &i);
 					expand_token(escap, &i);
 					expand_token(c, &i);
 				}
 				break;
 			}

 			/*
 			* Pokracovanie hex. v escape sekvecii
 			*/
 			case sEscHex2:{
 				if(isxdigit(c)){
 					state = sString;
 					hexa[1] = (char)c;
 					hexa[2] = '\0';
 					escap2 = HextoDec(hexa);
 					expand_token(escap2, &i);
 				} else{
 					state = sString;
 					expand_token(92, &i);
 					expand_token(escap, &i);
 					expand_token(escap2, &i);
 					expand_token(c, &i);
 				}
 				break;
 			}

 			//prvy znak bol '='
 			case sAssign:{
 				if(c == '='){
 					state = sEqual;
 					expand_token(c, &i);
 				} else{
 					fill_token(state);
 					state = sEnd;
 					undo_c(c);
 				}
 				break;
 			}

 			//prvy znak bol '/'
 			case sDivide:{
 				if(c == '/'){
 					state = sLComment;
 				} else if(c == '*'){
 					state = sBComment;
 				} else{
 					fill_token(state);
 					state = sEnd;
 					undo_c(c);
 				}
 				break;
 			}

 			//jedna sa o riadkovy komentar
 			case sLComment:{
 				if(c == '\n'){
 					state = sStart;
 					i = 0;
 				} else if (c == EOF){
 					state = sEndofFile;
 				} else{
 					state = sLComment;
 				}
 				break;
 			}

 			//Blokovy komenta /*
 			case sBComment:{
 				if(c == '*'){
 					state = sBlockEnd;
 				} else if(c == EOF){
 					state = sEndofFile;
 				} else{
 					state = sBComment;
 				}
 				break;
 			}

 			//Zisti ci sa blokovy komentar spravny dokoncil
 			case sBlockEnd:{
 				if(c == '/'){
 					state = sStart;
 					i = 0;
 				} else if(c == EOF){
 					state = sEndofFile;
 				} else{
 					state = sBComment;
 				}
 				break;
 			}

 			//prvy nacitany znak bol !
 			case sExclam:{
 				if(c == '='){
 					state = sNotEq;
 					expand_token(c, &i);
 				} else{
 					// ! samotny nie je lexem jazyka IFJ15
 					state = sError;
 					undo_c(c);
 				}
 				break;
 			}

 			// prvy nacitany znak je >
 			case sGreater:{
 				if(c == '='){ // >=
 					state = sGrorEq;
 					expand_token(c, &i);
 				} else if(c == '>'){ // >>
 					state = sCin;
 					expand_token(c, &i);
 				} else{ // >>
 					fill_token(state);
 					state = sEnd;
 					undo_c(c);
 				}
 				break;
 			}

 			//Prvy nacitany znak bol <
 			case sLess:{
 				if(c == '='){ //<=
 					state = sLeorEq;
 					expand_token(c, &i);
 				} else if(c == '<'){ // <<
 					state = sCout;
 					expand_token(c, &i);
 				} else{ // <
 					fill_token(state);
 					state = sEnd;
 					undo_c(c);
 				}
 				break;
 			}

 			case sEqual:		// ==
 			case sNotEq:		// !=
 			case sGrorEq:		// >=
 			case sLeorEq:		// <=
 			case sSemicolon:	// ;
 			case sLParenth:		// (
 			case sRParenth:		// )
 			case sLSetPar:		// {
 			case sRSetPar:		// }
 			case sPlus:			// +
 			case sMinus:		// -
 			case sMult:			// *
 			case sEndofFile:	// EOF
 			case sComma:		// ,
 			case sCout:			// <<
 			case sCin:			// >>
 			case sKeyWord:
 			case sResWord:{
 				fill_token(state);
 				state = sEnd;
 				undo_c(c);
 				break;
 			}

 			//Nastala lexikalna chyba
 			case sError:{
 				scaner_error = LEX_ERR;
 				fill_token(state);
 				cont = false;
 				break;
 			}

 			//Koncovy stav DKA
 			case sEnd:{
				undo_c(c);
				undo_c(c);
 				cont = false;
 				break;
 			}
 		}

 		//Nastala lexikalna chyba
 		if(scaner_error){
 			break;
 		}

 		/* Ak bol precitany znak noveho riadku
 		*  inkrementujeme pocitadlo riadkov. */
 		if(c == '\n'){
 			row++;
 		}
 	}

 /* Vratime token parseru */
 	return token;
 }
