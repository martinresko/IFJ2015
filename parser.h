/*
 * Predmet: IFJ / IAL
 * Subor:     parser.h
 *            Hlavickovy subor parsru (rekurzivny zostup)
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
 */
#ifndef PARSER_INCLUDED
#define PARSER_INCLUDED

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 #include "error.h"
 #include "expression.h"
 #include "ial.h"
 #include "scaner.h"
 #include "interpret.h"

 ERROR_CODE prog();
 ERROR_CODE body();
 ERROR_CODE function();
 ERROR_CODE typ();
 ERROR_CODE prototype_of_definition(int);
 ERROR_CODE params(Function_GTS *);
 ERROR_CODE multi_params(Function_GTS *);
 ERROR_CODE stat_list(int);
 ERROR_CODE command(int);
 ERROR_CODE funkcia_priradenie(char *);
 ERROR_CODE assign();
 ERROR_CODE arguments(char *);
 ERROR_CODE declaration();
 ERROR_CODE multi_arguments(char *);
 ERROR_CODE multi_cin();
 ERROR_CODE hodnota_priradenia();
 ERROR_CODE multi_cout();
 ERROR_CODE term(tToken *);
 ERROR_CODE fun_auto();
 ERROR_CODE inicialization();
 ERROR_CODE declaration_or();
 ERROR_CODE for_definition();
 ERROR_CODE for_deklaration();
 ERROR_CODE foo();
 ERROR_CODE build_in_function(int);

#endif
