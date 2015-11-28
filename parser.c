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

 #include <stdlib.h>
 #include "scaner.h"
 #include "error.h"

ERROR_CODE prog();
ERROR_CODE body();
ERROR_CODE function();
ERROR_CODE typ();
ERROR_CODE prototype_of_definition();
ERROR_CODE params();
ERROR_CODE multi_params();
ERROR_CODE stat_list();

tToken token; // aktualny token

/**
 * @info:<prog> ->  <body> ; $
 */

ERROR_CODE prog()
{
	ERROR_CODE error = ERR_OK;
	token = get_Token();

    switch (token.id) {
        case sError: // chyba v lexikalnom analyzatore
             error = LEX_ERR;
             break;
        case sEndofFile: // koniec suboru
            error = SYN_ERR;
            break;
        default :
            error = body();
            break;
    }

    if (error != ERR_OK) {
        return error;
    }

    return ERR_OK;
}

/**
 * @info:<body> -> <function><body> /empty
 */

ERROR_CODE body()
{
    ERROR_CODE error;

    switch (token.id) {
        /* chyba v lexikalnom analyzatore*/
        case sError :
             error = LEX_ERR; 
             break;
        /*koniec suboru po uz jedej funkcii min */     
        case sEndofFile :
            error = ERR_OK;
            break;
        default :
            error = function();
            break;
    }

    error = body();

    if (error != ERR_OK) {
        return error;
    }

    return ERR_OK;
}

/**
 * @info:function> -> <typ> id (<params>) <protype_or_definition>
 */

ERROR_CODE function()
{
    ERROR_CODE error;

    error = typ();

    if (error != ERR_OK){
        return error;
    }

    token = get_Token();
    /*kontrola ci je Lex_ERR*/
    if (token.id == sError) {
        error = LEX_ERR;
        return error;
    }
    /*kontrola ci je to ID*/
    if (token.id != sIdent) {
        error = SYN_ERR;
        return error;
    }

    token = get_Token();
    /*kontrola ci je Lex_ERR*/
    if (token.id == sError) {
        error = LEX_ERR;
        return error;
    }
    /*kontrola ci je to "("*/
    if (token.id != sLParenth) {
        error = SYN_ERR;
        return error;
    }

    error = params();

    if (error != ERR_OK){
        return error;
    }
    /*kontrola ci je to ")"*/
   /*token = get_Token();

    if (token.id != sRParenth) {
        error = SYN_ERR;
        return error;
    }
*/
    error = prototype_of_definition();

    if (error != ERR_OK){
        return error;
    }

    return ERR_OK;
}

/**
 * @info:<typ> -> int / string / double
 */

ERROR_CODE typ()
{
    ERROR_CODE error;

    if(strcmp(token.attribute, "int")){
        error = ERR_OK;
        return error;
    }
    else if(strcmp(token.attribute, "double")){
        error = ERR_OK;
        return error;
    } else if(strcmp(token.attribute, "string")){
        error = ERR_OK;
        return error;
    } else {
        error = SYN_ERR;
        return error;
    }

    return error;
}

/**
 * @info:<params> -> <typ> id <multi_params> / empty
 */

ERROR_CODE params()
{
    ERROR_CODE error;

    token = get_Token();
    /*kontrola ci je Lex_ERR*/
    if (token.id == sError) {
        error = LEX_ERR;
        return error;
    }

    error = typ();
    /*kontrola ci ma funkcia nejaky parameter*/
    if (error != ERR_OK) {
        /*kontrola je v spravnom tvare*/
        if (token.id != sRParenth) {
            error = SYN_ERR;
            return error;
        }
        /*ak nema funkcia ziadny parameter*/
        else {
            error = ERR_OK;
            return error;
        }
    }  /*ak ma funkcia parameter*/
    else {

        token = get_Token();
        /*kontrola ci je Lex_ERR*/
        if (token.id == sError) {
            error = LEX_ERR;
            return error;
        }

        if (token.id != sIdent) {
            error = SYN_ERR;
            return error;
        }

        error = multi_params();
    }

    if (error != ERR_OK) {
        return error;
    }

    return ERR_OK;
}

/**
 * @info:<prototype_or_definition> -> { <stat_list> } / ;
 */

ERROR_CODE prototype_of_definition()
{
    ERROR_CODE error;

    token = get_Token();

    switch (token.id) {
        /*pokila lexer vrati lex error*/
        case sError : 
            error = LEX_ERR;
            return error;
            break;    
        case sSemicolon :
            error = ERR_OK;
            return error;
            break;
        case sLSetPar :
           error = stat_list();
           break;
    }

    if (error != ERR_OK) {
        return error;
    }

    return ERR_OK;
}

/**
 * @info:<multi_params> -> , <typ> id <multi_params> / empty
 */

ERROR_CODE multi_params()
{
    token = get_Token();
    ERROR_CODE error;
    
    switch (token.id) {
        /*pokila lexer vrati lex error*/
        case sError : 
            error = LEX_ERR;
            return error;
            break;
        /*pokila dostanem ")" je len jedne parameter a vraciam sa*/
        case sRParenth :
            error = ERR_OK;
            return error;
            break;
        /*pokila dostanem ciarku tak moze byt viac parametrov a pokracujem*/
        case sComma :
            token = get_Token();
            /*kontrola ci je Lex_ERR*/
            if (token.id == sError) {
                error = LEX_ERR;
                return error;
            }
            error = typ();

            if (error != ERR_OK) {
                error = SYN_ERR;
                return error;
            }
            /*zoberiem dalsi token ktory ma byt ID*/
            token = get_Token();

            if (token.id != sIdent) {
                error = SYN_ERR;
                return error;
            }

            error = multi_params();

            if (error != ERR_OK) {
                return error;
            }
            break;
        default :
            error = SYN_ERR;
            return error;
            break;
    }

    if (error != ERR_OK) {
        return error;
    }

    return ERR_OK;
}

/**
 * @info:<stat_list> -> <command> <stat_list> /empty
 */

ERROR_CODE stat_list();
{
    token = get_Token();
    ERROR_CODE error;
    
    switch (token.id) {
        /*pokila lexer vrati lex error*/
        case sError : 
            error = LEX_ERR;
            return error;
            break;
        /*ak som dostal "}"*/
        case sRSetPar :
            error = ERR_OK;
            return error;
            break;
        default :
            error = command();
    }

    error = stat_list();

    if (error != ERR_OK) {
        return error;
    }

    return ERR_OK;
}
/**
 * @info: 
    <assign>
    <funkcia_ priradenie>
    ;
    id<funkcia_priradenie>;
    return expresion ;
    if ( expresion ) {<stat_list>} else { <stat_list>}
    for (<for_definice> ; expresion ; <premenna>) { <stat_list>}
    cin >> id <multi_cin>;
    cout << <term> <multi_cout>;
 */
ERROR_CODER command()
{
    ERROR_CODE error;

    switch (token.id) {
        /*pokila lexer vrati lex error*/
        case sError : 
            error = LEX_ERR;
            return error;
            break;
        /*pokial dostaanem ";"*/
        case sSemicolon :
            error = ERR_OK;
            return error;
            break;
        /*ked dostanem ID*/
        case sIdent :
            error = funkcia_priradenie();

            token = get_Token();
            /*kontrola ci je Lex_ERR*/
            if (token.id == sError) {
                error = LEX_ERR;
                return error;
            // ak dostanem ID
            } else if (token.id == sSemicolon) {
                error = ERR_OK;
                return error;
                break;

            }
        default : 
            // ak dostanem return
            if(strcmp(token.attribute, "return")){
                // zavolat expression
                token = get_Token()
            }
            else if(strcmp(token.attribute, "double")){
                error = ERR_OK;
                return error;
            } else if(strcmp(token.attribute, "string")){
                error = ERR_OK;
                return error;
            } else {
                error = SYN_ERR;
                return error;
            }

    }

    if (error != ERR_OK) {
        return error;
    }

    return ERR_OK;
}

ERROR_CODER funkcia_priradenie()
{

}