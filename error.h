/**
 * Predmet: IFJ / IAL
 * Subor:     error.h
 *            Hlavickovy subor zachytavania errorov
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
 */

 /**
 * @info: chybove stavy
 */

#ifndef ERROR_INCLUDED
#define ERROR_INCLUDED

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
 
typedef enum error_code{
    OK_ERR			= 0,
    LEX_ERR 		= 1,
    SYN_ERR			= 2,
    SEM_UNDEF_ERR	= 3, //sém. chyba–nedefin fc./prem., redefi fc./perm.
    SEM_TYPE_ERR	= 4, //sém. chyba typ kompatibility v arit.,retaz. ...
    DATA_TYPE_ERR	= 5,
    SEM_OTHER_ERR	= 6,
    NUMERIC_ERR		= 7,
    UNINITI_ERR		= 8,
    ZERO_DIV_ERR	= 9,
    RUNNING_ERR		= 10,
    INTERN_ERR		= 99

} ERROR_CODE;

#endif
