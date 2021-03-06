/*
 * Predmet: IFJ / IAL
 * Subor:     parser.c
 *            Implementacia parsru (rekurzivny zostup)
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
 */
 
#include "parser.h"

#define AUTO 9
#define TAKEN_FIRST_TOKEN 1
#define NOT_TAKEN_FIRST_TOKEN 0
#define TRUE 1
#define FALSE 0 
#define CONTINUE 2

tToken token; // aktualny token z lexikalneho analyzatora
extern tToken token_expression; // token vracany z expression
extern Table_symbols symbol_table; // globalna tabulka symbolov
extern int typ_pre_auto; // navratovy typ z expression pri odvodzovani auto

tState type_of_element_to_table_of_symbols; // typ pre tabulku symbolov
int type_for_expression; // typ id pre expression kvoli kontrolovaniu semantickej analyzy
char * identificator_for_generating_instruction_in_assign_on_left_side;
int LABEL = 0;
tToken very_helpfull_variable;

/* hlavna funkcia na spustenie parseru
 * @info:<prog> ->  <body> ; $
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE prog()
{
    globalTableOfSymbolsInit(&symbol_table);
    ERROR_CODE error = OK_ERR;
	token = get_Token();

    switch (token.id) {
        // chyba v lexikalnom analyzatore
        case sError: 
            error = LEX_ERR;
            return error;
        break;
        // koniec suboru
        case sEndofFile: 
            error = SYN_ERR;
        break;
        // ak dostanem hocico ine
        default :
            error = body();
        break;
    }

    if (error != OK_ERR) {
        return error;
    }
    // osetrenie ci main bol deklarovany a ci su vsetky funkcie definovane
    error =  finalFunctionCheckout(&symbol_table);

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info:<body> -> <function><body> /empty
 * @navratova hodnota return: ERROR CODE
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
            error = OK_ERR;
            return error;
        break;
        default :
            error = function();
            if (error != OK_ERR) {
                return error;
            }
        break;
    }

    token = get_Token();

    if (token.id == sError) {
        error = LEX_ERR;
        return error;
    }

    error = body();

    if (error != OK_ERR) {
        return error;
    }
    return OK_ERR;
}

/*
 * @info:function> -> <typ> id (<params>) <protype_or_definition>
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE function()
{
    // skontrolovat este rezzervovane slova
    int main_detection = FALSE;
    ERROR_CODE error;

    error = typ();

    if (error != OK_ERR){
        return error;
    }

    token = get_Token();
    /*kontrola ci je Lex_ERR*/
    if (token.id == sError) {
        error = LEX_ERR;
        return error;
    }
    /*kontrola ci je to rezervovane slovo*/
    if (token.id == sResWord) {
        error = SEM_UNDEF_ERR;
        return error;
    }
    /*kontrola ci je to ID*/
    if (token.id != sIdent) {
        error = SYN_ERR;
        return error;
    }
    if(!(strcmp(token.attribute, "main"))) {
        main_detection = TRUE;
    }
    // kontrola na redefiniciu funkcie
    Function_GTS *function_for_searching = searchFunction(&symbol_table,token.attribute);

    if (function_for_searching == NULL) {
        // vlozim do tabulky symbolov 
        error = insertFunction(&symbol_table,token.attribute,type_of_element_to_table_of_symbols);
    }
    else {
    	symbol_table.actual_function = function_for_searching; // nastavim este aktualnu funkciu na najdenu
    }

    if (error != OK_ERR){
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
    error = params(function_for_searching);

    if (error != OK_ERR){
        return error;
    }
    error = prototype_of_definition(main_detection);

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info:<typ> -> int / string / double 
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE typ()
{
    ERROR_CODE error = OK_ERR;

    if (token.id == sKeyWord) {

        if(!(strcmp(token.attribute, "int"))) { 
            type_of_element_to_table_of_symbols = sInteger;
            error = OK_ERR;
            return error;
        }
        if(!(strcmp(token.attribute, "double"))) {
            type_of_element_to_table_of_symbols = sDouble;
            error = OK_ERR;
            return error;
        }
        if(!(strcmp(token.attribute, "string"))) {
            type_of_element_to_table_of_symbols = sString;
            error = OK_ERR;
            return error;
        }
        if(!(strcmp(token.attribute, "auto"))) {
            error = SYN_ERR;
            return error;
        }
    }
    else {
        error = SYN_ERR;
        return error;
    }

    return error;
}

/*
 * @info:<params> -> <typ> id <multi_params> / empty
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE params(Function_GTS * previous_function_id)
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
    if (error != OK_ERR) {
        /*kontrola je v spravnom tvare*/
        if (token.id != sRParenth) {
            error = SYN_ERR;
            return error;
        }
        /*ak nema funkcia ziadny parameter*/
        else {
            if (getFunctionParam(symbol_table.actual_function,TRUE) != NULL) {
                error = SEM_UNDEF_ERR;
                return error;
            }

            getFunctionParam(symbol_table.actual_function,FALSE);
            error = OK_ERR;
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
        // 
        if (previous_function_id == NULL) {
            if (searchFunction(&symbol_table,token.attribute) != NULL) {
                error = SEM_UNDEF_ERR;
                return error;
            }
            //vkladanie prametrov do tabulky symbolov
            error = insertFunctionParam(symbol_table.actual_function,token.attribute,type_of_element_to_table_of_symbols);

            if (error != OK_ERR) {
                return error;
            }
        }
        else {
            // ziskam prvy parameter funkcie a kontrooluje mena a typy parametrov
            Variable *The_First = getFunctionParam(symbol_table.actual_function,TRUE);
            if (The_First != NULL) {
                if ((The_First->typ != (int)(type_of_element_to_table_of_symbols)) || (strcmp(The_First->name,token.attribute))) {
                    error = SEM_UNDEF_ERR;
                    return error;
                }
            }
            else {
                error = SEM_UNDEF_ERR;
                return error;
            }
        }

        error = multi_params(previous_function_id);
    }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info:<multi_params> -> , <typ> id <multi_params> / empty
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE multi_params(Function_GTS * previous_function_id)
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
            getFunctionParam(symbol_table.actual_function,FALSE);
            error = OK_ERR;
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

            if (error != OK_ERR) {
                error = SYN_ERR;
                return error;
            }
            /*zoberiem dalsi token ktory ma byt ID*/
            token = get_Token();

            if (token.id == sError) {
                error = LEX_ERR;
                return error;
            }

            if (token.id != sIdent) {
                error = SYN_ERR;
                return error;
            }
            // ziskam prvy parameter funkcie a kontrooluje mena a typy parametrov
            if (previous_function_id == NULL) {
                 // kontrola ci na redefiniciu
                if (searchFunction(&symbol_table,token.attribute) != NULL) {
                    error = SEM_UNDEF_ERR;
                    return error;
                }

                if(findInList(&symbol_table.actual_function->params,token.attribute) == NULL){
                    //vkladanie prametrov do tabulky symbolov
                    error = insertFunctionParam(symbol_table.actual_function,token.attribute,type_of_element_to_table_of_symbols);
                    
                    if (error != OK_ERR) {
                        return error;
                    }
                }
                else {
                    error = SEM_UNDEF_ERR;
                    return error;
                }                
            }
            else {
                // kontrolujem dalsi parameter funkcie
                Variable *The_First = getFunctionParam(symbol_table.actual_function,CONTINUE);
                if (The_First != NULL) {
                    if ((The_First->typ != (int)(type_of_element_to_table_of_symbols)) || (strcmp(The_First->name,token.attribute))) {
                        error = SEM_UNDEF_ERR;
                        return error;
                    }
                }
                else {
                    error = SEM_UNDEF_ERR;
                    return error;
                }
            }
            
            error = multi_params(previous_function_id);

            if (error != OK_ERR) {
                return error;
            }
        break;
        default :
            error = SYN_ERR;
            return error;
        break;
    }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info:<prototype_or_definition> -> { <stat_list> } / ;
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE prototype_of_definition(int main_detection)
{
    ERROR_CODE error;
    token = get_Token();
    if (main_detection == TRUE) {
        error = insertFunctionInstruction(symbol_table.actual_function, iPUSHFR, NULL, NULL, NULL); 
        InsVal Val;
        Val.VarType = sInteger;
        Val.Inum = symbol_table.actual_function->return_type;

        InsVal Val2;
        Val2.Void = NULL;
        error = insertFunctionInstruction(symbol_table.actual_function, iSETBASEFR, NULL, &Val, &Val2);

        if (error != OK_ERR) {
            return error;
        }
    }

    switch (token.id) {
        /*pokila lexer vrati lex error*/
        case sError : 
            error = LEX_ERR;
            return error;
        break;    
        case sSemicolon :
            // ak dostanem ; tak nieje funkcia definovana
            error = OK_ERR;
            return error;
        break;
        case sLSetPar :
            if (symbol_table.actual_function->defined == TRUE) {
                error = SEM_UNDEF_ERR;
                return error;
            }
            // ak dostanem { tak je uz funkcia definovana
            symbol_table.actual_function->defined = TRUE;
            // pushnem blok
            error = pushBlock(symbol_table.actual_function);
            if (error != OK_ERR) {
                return error;
            }

            error = stat_list(main_detection);
        break;
        default :
            error = SYN_ERR;
            return error;
        break;
    }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info:<stat_list> -> <command> <stat_list> /empty
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE stat_list(int main_detection)
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
            // popujem zasobnik
            popBlock(symbol_table.actual_function);

            if( (stackEmpty(&(symbol_table.actual_function->symbol_table_of_block))) && (symbol_table.actual_function->return_occured == FALSE) ) {
            	error = UNINITI_ERR;
            	return error;
            }

            error = OK_ERR;
            return error;
        break;
        case sEndofFile :
            error = SYN_ERR;
            return error;
        break;
        default :
           error = command(main_detection);
            if (error != OK_ERR) {
                return error;
            }
        break;
    }

    error = stat_list(main_detection);

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info: command
 * <assign>
 * <funkcia_ priradenie>
 *  ;
 *  id<funkcia_priradenie>;
 *  return expresion ;
 *   if ( expresion ) {<stat_list>} else { <stat_list>}
 *  for (<for_definice> ; expresion ; <premenna>) { <stat_list>}
 *  cin >> id <multi_cin>;
 *  cout << <term> <multi_cout>;
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE command(int main_detection)
{
    main_detection = main_detection;
    ERROR_CODE error;
    InsVal val;
    switch (token.id) {
        /*pokila lexer vrati lex error*/
        case sError: 
            error = LEX_ERR;
            return error;
        break;
        /*pokial dostaanem ";"*/
        case sSemicolon :
            error = SYN_ERR;
            return error;
        break;
        /*
         *  { <stat_list> }
        */
        case sLSetPar :
        	error = pushBlock(symbol_table.actual_function);

            if (error != OK_ERR) {
                return error;
            }
        	error = stat_list(main_detection);

        	if (error != OK_ERR) {
                return error;
            }
        break;
        /*
         * id<funkcia_priradenie>;
        */
        case sIdent :
            identificator_for_generating_instruction_in_assign_on_left_side = token.attribute;          
            ;
            Variable *premenna_na_odvodenie_typu = searchFunctionVariableInStack(symbol_table.actual_function,token.attribute);

            if (premenna_na_odvodenie_typu != NULL) {
            	type_for_expression = premenna_na_odvodenie_typu->typ;
            }
            error = funkcia_priradenie(token.attribute);

            if (error != OK_ERR) {
                return error;
            }

            switch (token.id) {
                /*pokial lexer vrati lex error*/
                case sError : 
                    error = LEX_ERR;
                    return error;
                break;
                /*pokial dostaanem ";"*/
                case sSemicolon :
                    //error = insertFunctionInstruction(Function_GTS *, iMOV, void *, void *, identificator_for_generating_instruction_in_assign_on_left_side); // pridane generovanie
                    error = OK_ERR;
                    return error;
                break;
                default : 
                    error = SYN_ERR;
                    return error;
                break;

            }
        break;
        case sResWord :
            // nekontrolujem typ 
            error = build_in_function(FALSE);

            if (error != OK_ERR) {
                return error;
            }
            token = get_Token();

            switch (token.id) {
                /*pokial lexer vrati lex error*/
                case sError : 
                    error = LEX_ERR;
                    return error;
                break;
                /*pokial dostaanem ";"*/
                case sSemicolon :
                    error = OK_ERR;
                    return error;
                break;
                default :
                 error = SYN_ERR;
                    return error;
                break;
            }
        break;
        case sKeyWord : 
            /*
             *return expresion ;
             */
            if(!(strcmp(token.attribute, "return"))){
                symbol_table.actual_function->return_occured = TRUE;
                // predam do expression aktualny typ funkcie
                error = expression(NOT_TAKEN_FIRST_TOKEN, symbol_table.actual_function->return_type);
                token = token_expression;

                if (error != OK_ERR) {
                    return error;
                } 

                // z prediktivnej som uz dostal token
                /*kontrola ci je Lex_ERR*/
                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                // ak bol zadany ;
                } else if (token.id == sSemicolon) {
                    if (main_detection == TRUE) {
                        val.VarType = sInteger;
                        val.Inum = TRUE;
                        error = insertFunctionInstruction(symbol_table.actual_function, iRET, NULL, &val, NULL);

                        if (error != OK_ERR) {
                            return error;
                        }
                    }
                    error = OK_ERR;
                    return error;
                // inak chyba
                } else {
                    error = SYN_ERR;
                    return error;
                }
            }
            /*
             *if ( expresion ) {<stat_list>} else { <stat_list>}
             */
            else if(!(strcmp(token.attribute, "if"))){
                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }
                // ak dostanem (
                if(token.id != sLParenth) {
                    error = SYN_ERR;
                    return error;
                }
                error = expression(NOT_TAKEN_FIRST_TOKEN, sInteger); // expression
                token = token_expression;
                
                if (error != OK_ERR) { 
                    return error;
                } 
                // zo prediktivnej som uz dostal token
                //ak dostanem )
                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }

                if(token.id != sRParenth) {
                    error = SYN_ERR;
                    return error;
                }
                // ak dostanem {
                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }

                if(token.id != sLSetPar) {
                    error = SYN_ERR;
                    return error;
                }
                // pushnem blok
                error = pushBlock(symbol_table.actual_function);

                if (error != OK_ERR) {
                    return error;
                }


                error = stat_list(main_detection);

                if (error != OK_ERR) {
                    return error;
                }
                // ak dostanem elese
                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }

                if((strcmp(token.attribute, "else"))){

                    error = SYN_ERR;
                    return error;
                }


                // ak dostanem {
                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }

                if(token.id != sLSetPar) {

                    error = SYN_ERR;
                    

                    return error;
                }

                	error = pushBlock(symbol_table.actual_function);

		            if (error != OK_ERR) {
		                return error;
		            }

                error = stat_list(main_detection);

                if (error != OK_ERR) {
                    return error;
                }

                return OK_ERR;
            }
            /*
             *<command> -> cin >> id <multi_cin>;
             */   
            else if(!(strcmp(token.attribute, "cin"))){
                token = get_Token ();

                switch (token.id) {
                    /*pokial lexer vrati lex error*/
                    case sError: 
                        error = LEX_ERR;
                        return error;
                    break;
                    case sCin :
                        token = get_Token ();

                        switch (token.id) {
                            /*pokial lexer vrati lex error*/
                            case sError: 
                                error = LEX_ERR;
                                return error;
                            break;
                            case sIdent :
                                if(searchFunctionVariableInStack(symbol_table.actual_function,token.attribute) == NULL) {
                                    error = SEM_UNDEF_ERR;
                                    return error;
                                }
                                error = OK_ERR;
                                //error = insertFunctionInstruction(symbol_table.actual_function, iREAD, NULL, token.attribute, NULL);

                                if (error != OK_ERR) {
                                    return error;
                                }
                            break;
                            default :
                                error = SYN_ERR;
                                return error;
                            break;
                        }

                        if (error != OK_ERR) {
                            return error;
                        }
                    break;
                    default :
                        error = SYN_ERR;
                        return error;
                    break;
                }

                if (error != OK_ERR) {
                    return error;
                }

                error = multi_cin();

                if (error != OK_ERR) {
                    return error;
                }

                return OK_ERR;
            }
            /*
             * for (<for_definice> ; expresion ; id = expression) { <stat_list>}
             */ 
            else if(!(strcmp(token.attribute, "for"))){

            	// pushnem blok
                error = pushBlock(symbol_table.actual_function);

                if (error != OK_ERR) {
                    return error;
                }

                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }
                // ak dostanem (
                if(token.id != sLParenth) {
                    error = SYN_ERR;
                    return error;
                }
                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }
                error = for_definition();

                if (error != OK_ERR) {
                    return error;
                }

                // ak dostanem ;
                if(token.id != sSemicolon) {
                    error = SYN_ERR;
                    return error;
                }

                error = expression(NOT_TAKEN_FIRST_TOKEN, sInteger); // expression opravit este
                token = token_expression;

                if (error != OK_ERR) {
                    return error;
                }

                if(token.id != sSemicolon) {
                    error = SYN_ERR;
                    return error;
                }

                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }

                if(token.id != sIdent) {
                    error = SYN_ERR;
                    return error;
                }
                Variable *variable_for_search = searchFunctionVariableInStack(symbol_table.actual_function,token.attribute);
                if(variable_for_search == NULL) {
                	error = SEM_UNDEF_ERR;
                	return error;
                }

                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }

                if(token.id != sAssign) {
                    error = SYN_ERR;
                    return error;
                }

                error = expression(NOT_TAKEN_FIRST_TOKEN,variable_for_search->typ); // expression opravit este
                token = token_expression;

                if (error != OK_ERR) {
                    return error;
                }
                // ak dostanem (
                if(token.id != sRParenth) {
                    error = SYN_ERR;
                    return error;
                }

                // ak dostanem {
                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }

                if(token.id != sLSetPar) {
                    error = SYN_ERR;
                    return error;
                }

                error = stat_list(main_detection);

                if (error != OK_ERR) {
                    return error;
                }

                return OK_ERR;
            }
            /*
             * cout << <term> <multi_cout>;
             */
            else if(!(strcmp(token.attribute, "cout"))){
                token = get_Token ();
                //token pre count na ziskanie hodnoty tokenu z term
                tToken token_for_emmiting_instruction;
                switch (token.id) {
                    /*pokial lexer vrati lex error*/
                    case sError: 
                        error = LEX_ERR;
                        return error;
                    break;
                    case sCout :
                        error = term(&token_for_emmiting_instruction);

                        if (error != OK_ERR) {
                            return error;
                        }

                        //===========================================================================
                                            char *c = token_for_emmiting_instruction.attribute;
                                          InsVal val;
                        char * err = NULL;
                        int i = (int)strtoll(c, &err, 0);
                        if (*err) {
                            double d = strtod(c, &err);
                            if (*err) {
                                val.VarType = sString;
                                val.Str = c;
                            } else {
                                val.VarType = sDouble;
                                val.Dnum = d;
                            }
                        } else {
                            val.VarType = sInteger;
                                                val.Inum = i;
                                            }
                        //===========================================================================
                        error = insertFunctionInstruction(symbol_table.actual_function, iWRITE, NULL, &val, NULL);

                        if (error != OK_ERR) {
                            return error;
                        }

                    break;
                    default :
                        error = SYN_ERR;
                        return error;
                    break;
                }

                if (error != OK_ERR) {
                    return error;
                }
                error = multi_cout();

                if (error != OK_ERR) {
                    return error;
                }

                return OK_ERR;
            } else {
                error = assign();

                if (error != OK_ERR) {
                    return error;
                }
            }
        break;
        default :
            error = SYN_ERR;
            return error;
        break;
    }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info:  <funkcia_ priradenie>
 *  (<arguments>)
 *  <deklaracia>
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE funkcia_priradenie(char *previous_token_atributte)
{

    ERROR_CODE error;
    token = get_Token();

    
    switch (token.id) {
        /*pokial lexer vrati lex error*/
        case sError: 
            error = LEX_ERR;
            return error;
        break;
        /*ak dostanem ( idem do arguments*/
        case sLParenth :
            if (searchFunction(&symbol_table,previous_token_atributte) == NULL) {
                error = SEM_UNDEF_ERR;
                return error;
            }
            error = arguments(previous_token_atributte);

            if (error != OK_ERR) {
                return error;
            }

            token = get_Token();
        break;
        /*ak som dostal = tak idem do deklaracia*/
        case sAssign :
            if (searchFunctionVariableInStack(symbol_table.actual_function,previous_token_atributte) == NULL) {
                error = SEM_UNDEF_ERR;
                return error;
            }

            error = declaration();

            if (error != OK_ERR) {
                return error;
            }

        break;
        default :
            error = SYN_ERR;
            return error;
        break;
    }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;

}

/*
 * @info:  <arguments> 
 * id <multi_arguments>
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE arguments(char *previous_token_atributte)
{
    ERROR_CODE error;
    token = get_Token();
    switch (token.id) {
        /*pokial lexer vrati lex error*/
        case sError: 
            error = LEX_ERR;
            return error;
        break;
        case sIdent :
            ;
            // vyhladame prvy parameter funkcie
            Variable *The_First = getFunctionParam(searchFunction(&symbol_table,previous_token_atributte),TRUE);
            //ak nejaky ma
            if (The_First != NULL) {
                //zistim ci je ID platne
                Variable* argument_ID = searchFunctionVariableInStack(symbol_table.actual_function, token.attribute);
                if (argument_ID == NULL) {
                   error = SEM_UNDEF_ERR;
                   return error;
                }
                if (typeControl(The_First->typ, argument_ID->typ)!=OK_ERR) {
                    error = SEM_TYPE_ERR;
                    return error;
                }
            }
            else {
                error = SEM_TYPE_ERR;
                return error;
            }
            error = multi_arguments(previous_token_atributte);

            if (error != OK_ERR) {
                return error;
            }
        break;
        case sRParenth :
            if (getFunctionParam(searchFunction(&symbol_table,previous_token_atributte),TRUE) != NULL) {
                error = SEM_TYPE_ERR;
                return error;
            }

            error = OK_ERR;
            getFunctionParam(searchFunction(&symbol_table,previous_token_atributte),FALSE);
        break;
        default :
            if((token.id == sInteger) || (token.id == sDouble) || (token.id == sString) ) {
                // vyhladame prvy parameter funkcie
                Variable *The_First = getFunctionParam(searchFunction(&symbol_table,previous_token_atributte),TRUE);
                //ak nejaky ma
                if (The_First != NULL) {
                    if (typeControl(The_First->typ,token.id)!=OK_ERR) {
                        error = SEM_TYPE_ERR;
                        return error;
                    }
                }
                else{
                    error = SEM_TYPE_ERR;
                    return error;
                }
                error = OK_ERR;
            }
            else {
                error = SYN_ERR;
                return error;
            }
            error = multi_arguments(previous_token_atributte);

            if (error != OK_ERR) {
                return error;
            }
        break;
    }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info: <multi_arguments>
 *  , id <multi_arguments>
 * empty
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE multi_arguments(char *previous_token_atributte)
{
    ERROR_CODE error;
    token = get_Token();
    switch (token.id) {
        /*pokial lexer vrati lex error*/
        case sError: 
            error = LEX_ERR;
            return error;
        break;
        // ak som dostal ciarku
        case sComma :
            token = get_Token();
            switch (token.id) {
                /*pokial lexer vrati lex error*/
                case sError: 
                    error = LEX_ERR;
                    return error;
                break;
                case sIdent :
                    ;
                    // vyhladame prvy parameter funkcie
                    Variable *The_First = getFunctionParam(searchFunction(&symbol_table,previous_token_atributte),CONTINUE);
                    //ak nejaky ma
                    if (The_First != NULL) {
                        //zistim ci je ID platne
                        Variable* argument_ID = searchFunctionVariableInStack(symbol_table.actual_function, token.attribute);
                        if (argument_ID == NULL) {
                           error = SEM_TYPE_ERR;
                           return error;
                        }
                        if ((typeControl(The_First->typ, argument_ID->typ)!=OK_ERR)) {
                            error = SEM_TYPE_ERR;
                            return error;
                        }
                    }
                    else {
                        error = SEM_TYPE_ERR;
                        return error;
                    }
                    error = OK_ERR;
                break;
                default :
                    if((token.id == sInteger) || (token.id == sDouble) || (token.id == sString) ) {
                        // vyhladame prvy parameter funkcie
                        Variable *The_First = getFunctionParam(searchFunction(&symbol_table,previous_token_atributte),CONTINUE);
                        //ak nejaky ma
                        if (The_First != NULL) {
                            if (typeControl(The_First->typ, token.id)!=OK_ERR) {
                                error = SEM_TYPE_ERR;
                                return error;
                            }
                        }
                        else {
                            error = SEM_TYPE_ERR;
                            return error;
                        }
                        error = OK_ERR;
                    } 
                    else {
                        error = SYN_ERR;
                        return error;
                    }
                break;
            }

            if (error != OK_ERR) {
                return error;
            }
        break;
        // ak som dostal )
        case sRParenth :

            if(getFunctionParam(searchFunction(&symbol_table,previous_token_atributte),CONTINUE) != NULL ) {
                error = SEM_TYPE_ERR;
                return error;
            }

            getFunctionParam(searchFunction(&symbol_table,previous_token_atributte),FALSE);
            error = OK_ERR;
            return error;
        break;
        default :
            error = SYN_ERR;
            return error;
        break;
    }

    error = multi_arguments(previous_token_atributte);

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info: <multi_cin>  
 *  >> id <multi_cin>
 *  empty
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE multi_cin()
{
    ERROR_CODE error;
    token = get_Token ();

    switch (token.id) {
        /*pokial lexer vrati lex error*/
        case sError: 
            error = LEX_ERR;
            return error;
        break;
        case sSemicolon :
            error = OK_ERR;
            return error;
        break;
        case sCin :
            token = get_Token ();

            switch (token.id) {
                /*pokial lexer vrati lex error*/
                case sError: 
                    error = LEX_ERR;
                    return error;
                break;
                case sIdent :
                    if(searchFunctionVariableInStack(symbol_table.actual_function,token.attribute) == NULL) {
                        error = SEM_UNDEF_ERR;
                        return error;
                    }
                    error = OK_ERR;
                break;
                default :
                    error = SYN_ERR;
                    return error;
                break;
            }

            if (error != OK_ERR) {
                return error;
            }

        break;
        default :
            error = SYN_ERR;
            return error;
        break;
    }

    if (error != OK_ERR) {
        return error;
    }
    error = multi_cin();

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}


/*
 * @info: <multi_cout> 
 *  << <term> <multi_count>
 *  empty
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE multi_cout()
{
    ERROR_CODE error;
    token = get_Token ();
    tToken token_for_emmiting_instruction;
    InsVal val;

    switch (token.id) {
        /*pokial lexer vrati lex error*/
        case sError: 
            error = LEX_ERR;
            return error;
        break;
        case sSemicolon :
            error = OK_ERR;
            return error;
        break;
        case sCout :
            error = term(&token_for_emmiting_instruction);

            if (error != OK_ERR) {
                return error;
            }
            val.VarType=token_for_emmiting_instruction.id;
            val.Str = token_for_emmiting_instruction.attribute;
            error = insertFunctionInstruction(symbol_table.actual_function, iWRITE, NULL, &val, NULL);

            if (error != OK_ERR) {
                return error;
            }
        break;
        default :
            error = SYN_ERR;
            return error;
        break;
    }

    if (error != OK_ERR) {
        return error;
    }
    error = multi_cout();

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info: <assign> 
 *  <auto>;
 *  <inicializacia>;
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE assign()
{
    ERROR_CODE error;

    error = typ();
    if (error == OK_ERR) {

        error = inicialization();

        if (error != OK_ERR) {
            return error;
        } 
    }
    else {
        error = fun_auto();

        if (error != OK_ERR) {
            return error;
        } 
    	
    }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info: <deklaracia>
 *  = <hodnota_priradenia>
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE declaration()
{
    ERROR_CODE error;

   

    if (token.id != sAssign) {
        error = SYN_ERR;
        return error;
    }
    error = hodnota_priradenia();

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info: <hodnota_priradenia> 
 *  expression
 *  id( < arguments > ) 
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE hodnota_priradenia()
{	
    InsVal val;

	
    ERROR_CODE error;
    token = get_Token();
    // ak dostanem lexiklanu chybu LEX_ERR
    switch (token.id) {
        /*pokial lexer vrati lex error*/
        case sError : 
            error = LEX_ERR;
            return error;
        break;
        case sIdent :
        	;
            // premenna pre arguments
            char * token_attribute_for_arguments = token.attribute;
            // konrola typu pri volani funkcie
            Function_GTS *function_control_type = searchFunction(&symbol_table,token.attribute);

			if (function_control_type == NULL) {

        		error = expression(TAKEN_FIRST_TOKEN,type_for_expression); 
                token = token_expression;

                if (token.id != sSemicolon) {
                    error = SYN_ERR;
                    return error;
                }

                val.Str = very_helpfull_variable.attribute;
                val.VarType = very_helpfull_variable.id;
                error = insertFunctionInstruction(symbol_table.actual_function, iTOPPOP, NULL, &val, NULL);

                if (error != OK_ERR) {
                    return error;
                }


                if (error != OK_ERR) {
                    return error;
                }
        	}
        	else {
                // kontrola typov pri volani funkcii
                
                if (typeControl(function_control_type->return_type, type_for_expression) != OK_ERR) {
                    error = SEM_TYPE_ERR;
                    return error;
                }

        		token = get_Token();

        		if (token.id == sError) {
			        error = LEX_ERR;
			        return error;
			    }

			    if (token.id != sLParenth) {
			        error = SYN_ERR;
			        return error;
			    }

			    error = arguments(token_attribute_for_arguments);

			    if (error != OK_ERR) {
			        return error;
			    }
			    token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }
        	}
        break;
        case sResWord :
            error = build_in_function(TRUE);
            if (error != OK_ERR) {
                    return error;
                }
            token = get_Token();
        break;
        default :
        	error = expression(TAKEN_FIRST_TOKEN,type_for_expression); 
            token = token_expression;


            if (token.id != sSemicolon) {
                error = SYN_ERR;
                return error;
            }

            val.Str = very_helpfull_variable.attribute;
            val.VarType = very_helpfull_variable.id;
            error = insertFunctionInstruction(symbol_table.actual_function, iTOPPOP, NULL, &val, NULL);

            if (error != OK_ERR) {
                return error;
            }

        break;
    }    

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info: <term> 
 * @param: hodnota/id/string pre cout
 * id / cislo / retazec 
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE term(tToken* token_for_count)
{
    ERROR_CODE error;
    token = get_Token ();
    // 
    token_for_count->id = token.id;
    token_for_count->attribute = token.attribute;
    token_for_count = token_for_count;
    // ak dostanem lexiklanu chybu LEX_ERR
    if (token.id == sError) {
        error = LEX_ERR;
        return error;
    // ak dostanem ocakavany vstup OK_ERR
    } 
    else if(token.id == sIdent) {
        if(searchFunctionVariableInStack(symbol_table.actual_function,token.attribute) == NULL) {
            error = SEM_UNDEF_ERR;
            return error;
        }
        error = OK_ERR;
    } 
    else if ((token.id == sInteger) || (token.id == sDouble) || (token.id == sString)) {
        error = OK_ERR;
        return error;
    }
    // inak ak dostanem nieco ine SYN_ERR
    else {
        error = SYN_ERR;
        return error;
    }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info: <auto> 
 *  auto id = expression 
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE fun_auto()
{
    ERROR_CODE error;
    
    

    if(!(strcmp(token.attribute, "auto"))) {
        token = get_Token();

        if (token.id == sError) {
            error = LEX_ERR;
            return error;
        }

        if(token.id != sIdent) {
            error = SYN_ERR;
            return error;
        }

        if (searchFunction(&symbol_table,token.attribute) != NULL) {
            error = SEM_UNDEF_ERR;
            return error;
        }

        Variable *premenna_auto = searchFunctionVariableInActualLevel(symbol_table.actual_function,token.attribute);

        if (premenna_auto != NULL){
            error = SEM_UNDEF_ERR;
            return error;
        }

        // vlozime premennu na zasobnik
        error = insertFunctionVariableToStack(symbol_table.actual_function,token.attribute,AUTO);
        
        premenna_auto = searchFunctionVariableInStack(symbol_table.actual_function,token.attribute);
        
        if (error != OK_ERR) {
            return error;
        }

        token = get_Token();

        if (token.id == sError) {
            error = LEX_ERR;
            return error;
        }

        if(token.id != sAssign) {
            error = DATA_TYPE_ERR;
            return error;
        }
        error = expression(NOT_TAKEN_FIRST_TOKEN,AUTO);
        
        if (error != OK_ERR) {
           	return error;
        }
        premenna_auto->typ = typ_pre_auto;

        token = token_expression;
        // zavolam precedencnu expression

    } 
    else {
        error = SYN_ERR;
        return error;
    }

    switch (token.id) {
        /*pokial lexer vrati lex error*/
        case sError: 
            error = LEX_ERR;
            return error;
        break;
        case sSemicolon :
            error = OK_ERR;
            return error;
        break;
        default :
            error = SYN_ERR;
            return error;
        break;
        }

        if (error != OK_ERR) {
            return error;
        }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info: <inicialization> 
 *  <typ> id <deklaracia alebo ;>
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE inicialization()
{
    ERROR_CODE error;
    InsVal val;
    error = typ();
    type_for_expression = type_of_element_to_table_of_symbols;
    very_helpfull_variable.id = type_of_element_to_table_of_symbols;

    if (error != OK_ERR) {
        error = SYN_ERR;
        return error;
    }

    token = get_Token();

    if (token.id == sError) {
        error = LEX_ERR;
        return error;
    }

    if(token.id != sIdent) {
        error = SYN_ERR;
        return error;
    }
    if (searchFunction(&symbol_table,token.attribute) != NULL) {
        error = SEM_UNDEF_ERR;
        return error;
    }
    very_helpfull_variable.attribute = token.attribute;
    Variable *premenna_inicialization = searchFunctionVariableInActualLevel(symbol_table.actual_function,token.attribute);

    if (premenna_inicialization != NULL){
        error = SEM_UNDEF_ERR;
        return error;
    }
    // volozenie instrukcia pre vlozenie premmenej do ramca
    val.VarType = type_of_element_to_table_of_symbols;
    val.Str = token.attribute;
    error = insertFunctionInstruction(symbol_table.actual_function, iINSERT_TO_FR, NULL, &val, NULL);
    if (error != OK_ERR) {
        return error;
    }
    // vkladam do tabulky symbolov typ a id
    error = insertFunctionVariableToStack(symbol_table.actual_function,token.attribute,type_of_element_to_table_of_symbols);

    if (error != OK_ERR) {
        return error;
    }
    error = declaration_or();

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info: <declaration_or> 
 *  <deklaracia>
 *  empty
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE declaration_or()
{
    ERROR_CODE error;
    token = get_Token();

    if (token.id == sError) {
        error = LEX_ERR;
        return error;
    }

    if (token.id == sSemicolon) {
        error = OK_ERR;
        return error;
    }
    else {
        error = declaration();

        if (error != OK_ERR) {
            return error;
        }
    }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info: <for_definition>
 *  = <hodnota_priradenia>
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE for_definition()
{
    ERROR_CODE error;

    if(!(strcmp(token.attribute, "auto"))){
        error = fun_auto();

        if (error != OK_ERR) {
            return error;
        }
    }
    else {
        error = for_deklaration();

        if (error != OK_ERR) {
            return error;
        }
    }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info: <for_deklaration>
 *  id <deklaracia>
 *  <typ> id <deklaracia>
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE for_deklaration() 
{
    ERROR_CODE error;
    error = typ();

    if (error == OK_ERR) {
        token = get_Token();

        if (token.id == sError) {
            error = LEX_ERR;
            return error;
        }

        if(token.id != sIdent) {
            error = SYN_ERR;
            return error;
        }
        // vkladam do tabulky symbolov typ a id
        error = insertFunctionVariableToStack(symbol_table.actual_function,token.attribute,type_of_element_to_table_of_symbols);

        if (error != OK_ERR) {
            return error;
        }

        token = get_Token();

        if (token.id == sError) {
            error = LEX_ERR;
            return error;
        }
        error = foo();

        if (error != OK_ERR) {
            return error;
        }    
    }
    /*ak som nedostal typ testujem id*/
    else {
        error = SYN_ERR;
        return error;
    }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/*
 * @info: <foo>
 *  empty
 *  = expresion
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE foo() 
{
    ERROR_CODE error;

    switch (token.id) {
        /*pokial lexer vrati lex error*/
        case sError: 
            error = LEX_ERR;
            return error;
        break;
        case sSemicolon :
            error = OK_ERR;
            return error;
        break;
        case sAssign :
        	// volam expression s parametrom typu ID volanej naposledy
            error = expression(NOT_TAKEN_FIRST_TOKEN,type_of_element_to_table_of_symbols);
        	
        	if (error != OK_ERR) {
                    return error;
                }
        	
        	token = token_expression;
        break;
        default :
            error = SYN_ERR;
            return error;
        break;
        }

        if (error != OK_ERR) {
            return error;
        }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR; 
}

/*
 * @info: <build_in_function>
 *  length
 *  substr
 *  find
 *  sort
 * type_control znaci ci sa bude kontrolovat typ TRUE/FALSE
 * type_for_build_in_function
 * @navratova hodnota return: ERROR CODE
 */

ERROR_CODE build_in_function(int type_control)
{
    ERROR_CODE error;
    /*
    *Vstavana funkcia length
    */
    if(!(strcmp(token.attribute, "length"))){

        if(type_control == TRUE) {

            if (typeControl(type_for_expression, sInteger) != OK_ERR) {
                    error = SEM_TYPE_ERR;
                    return error;
                }
        }
        token = get_Token();

        switch (token.id) {
            /*pokial lexer vrati lex error*/
            case sError: 
                error = LEX_ERR;
                return error;
            break;
            case sLParenth:
                token = get_Token();

                switch (token.id) {
                    case sError:
                        error = LEX_ERR;
                        return error;
                    break;
                    case sString :
                        error = OK_ERR;
                    break;
                    // ak dostanem ID pozriem sa do tabulky symbolov ci bola definovana
                    case sIdent :
                        ;
                        Variable * identifier_of_parameter = searchFunctionVariableInStack(symbol_table.actual_function,token.attribute);

                        if (identifier_of_parameter == NULL) {
                            error = SEM_UNDEF_ERR;
                            return error;
                        }
                        else if (identifier_of_parameter->typ != sString) {
                            error = SEM_TYPE_ERR;
                            return error;
                        }
                        else {
                            error = OK_ERR;
                        }
                    break;
                    default :
                        error = SYN_ERR;
                        return error;
                    break;
                }

                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }
                // dostal som )
                if (token.id != sRParenth) {
                    error = SYN_ERR;
                    return error;
                }
            break;
            default :
                error = SYN_ERR;
                return error;
            break;
        }
    }
    /*
    *Vstavana funkcia substring
    */
    else if(!(strcmp(token.attribute, "substr"))){

        if(type_control == TRUE) {

            if (typeControl(type_for_expression, sString) != OK_ERR) {
                    error = SEM_TYPE_ERR;
                    return error;
                }
        }

        token = get_Token();

        switch (token.id) {
            /*pokial lexer vrati lex error*/
            case sError: 
                error = LEX_ERR;
                return error;
            break;
            case sLParenth:
                token = get_Token();

                switch (token.id) {
                    case sError:
                        error = LEX_ERR;
                        return error;
                    break;
                    case sString :
                        error = OK_ERR;
                    break;
                    // ak dostanem ID pozriem sa do tabulky symbolov ci bola definovana
                    case sIdent :
                        ;
                        Variable * identifier_of_parameter = searchFunctionVariableInStack(symbol_table.actual_function,token.attribute);

                        if (identifier_of_parameter == NULL) {
                            error = SEM_UNDEF_ERR;
                            return error;
                        }
                        else if (identifier_of_parameter->typ != sString) {
                            error = SEM_TYPE_ERR;
                            return error;
                        }
                        else {
                            error = OK_ERR;
                        }
                    break;
                    default :
                        error = SYN_ERR;
                        return error;
                    break;
                }

                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }
                // dostal som ,
                if (token.id != sComma) {
                    error = SYN_ERR;
                    return error;
                }
                token = get_Token();

                switch (token.id) {
                    case sError:
                        error = LEX_ERR;
                        return error;
                    break;
                    case sInteger :
                        error = OK_ERR;
                    break;
                    case sDouble :
                        error = OK_ERR;
                    break;
                    // ak dostanem ID pozriem sa do tabulky symbolov ci bola definovana
                    case sIdent :
                        ;
                        Variable * identifier_of_parameter = searchFunctionVariableInStack(symbol_table.actual_function,token.attribute);

                        if (identifier_of_parameter == NULL) {
                            error = SEM_UNDEF_ERR;
                            return error;
                        }
                        else if (identifier_of_parameter->typ != sInteger) {
                            error = SEM_TYPE_ERR;
                            return error;
                        }
                        else {
                            error = OK_ERR;
                        }
                    break;
                    default :
                        error = SYN_ERR;
                        return error;
                    break;
                }
                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }
                // dostal som ,
                if (token.id != sComma) {
                    error = SYN_ERR;
                    return error;
                }
                token = get_Token();

                switch (token.id) {
                    case sError:
                        error = LEX_ERR;
                        return error;
                    break;
                    case sInteger :
                        error = OK_ERR;
                    break;
                    case sDouble :
                        error = OK_ERR;
                    break;
                    // ak dostanem ID pozriem sa do tabulky symbolov ci bola definovana
                    case sIdent :
                        ;
                        Variable * identifier_of_parameter = searchFunctionVariableInStack(symbol_table.actual_function,token.attribute);

                        if (identifier_of_parameter == NULL) {
                            error = SEM_UNDEF_ERR;
                            return error;
                        }
                        else if (identifier_of_parameter->typ != sInteger) {
                            error = SEM_TYPE_ERR;
                            return error;
                        }
                        else {
                            error = OK_ERR;
                        }
                    break;
                    default :
                        error = SYN_ERR;
                        return error;
                    break;
                }

                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }
                // dostal som )
                if (token.id != sRParenth) {
                    error = SYN_ERR;
                    return error;
                }
            break;
            default :
                error = SYN_ERR;
                return error;
            break;
        }
    }
    /*
    *Vstavana funkcia concat
    */
    else if(!(strcmp(token.attribute, "concat"))){
        if(type_control == TRUE) {

            if (typeControl(type_for_expression, sString) != OK_ERR) {
                error = SEM_TYPE_ERR;
                return error;
            }
        }

        token = get_Token();

        switch (token.id) {
            /*pokial lexer vrati lex error*/
            case sError: 
                error = LEX_ERR;
                return error;
            break;
            case sLParenth:
                token = get_Token();

                switch (token.id) {
                    case sError:
                        error = LEX_ERR;
                        return error;
                    break;
                    case sString :
                        error = OK_ERR;
                    break;
                    // ak dostanem ID pozriem sa do tabulky symbolov ci bola definovana
                    case sIdent :
                        ;
                        Variable * identifier_of_parameter = searchFunctionVariableInStack(symbol_table.actual_function,token.attribute);

                        if (identifier_of_parameter == NULL) {
                            error = SEM_UNDEF_ERR;
                            return error;
                        }
                        else if (identifier_of_parameter->typ != sString) {
                            error = SEM_TYPE_ERR;
                            return error;
                        }
                        else {
                            error = OK_ERR;
                        }
                    break;
                    default :
                        error = SYN_ERR;
                        return error;
                    break;
                }

                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }
                // dostal som ,
                if (token.id != sComma) {
                    error = SYN_ERR;
                    return error;
                }
                token = get_Token();

                switch (token.id) {
                    case sError:
                        error = LEX_ERR;
                        return error;
                    break;
                    case sString :
                        error = OK_ERR;
                    break;
                    // ak dostanem ID pozriem sa do tabulky symbolov ci bola definovana
                    case sIdent :
                        ;
                        Variable * identifier_of_parameter = searchFunctionVariableInStack(symbol_table.actual_function,token.attribute);

                        if (identifier_of_parameter == NULL) {
                            error = SEM_UNDEF_ERR;
                            return error;
                        }
                        else if (identifier_of_parameter->typ != sString) {
                            error = SEM_TYPE_ERR;
                            return error;
                        }
                        else {
                            error = OK_ERR;
                        }
                    break;
                    default :
                        error = SYN_ERR;
                        return error;
                    break;
                }
                
                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }
                // dostal som )
                if (token.id != sRParenth) {
                    error = SYN_ERR;
                    return error;
                }
            break;
            default :
                error = SYN_ERR;
                return error;
            break;
        }
    }
    /*
    *Vstavana funkcia find
    */
    else if(!(strcmp(token.attribute, "find"))){

        if(type_control == TRUE) {

            if (typeControl(type_for_expression, sInteger) != OK_ERR) {
                error = SEM_TYPE_ERR;
                return error;
            }
        }

        token = get_Token();

        switch (token.id) {
            /*pokial lexer vrati lex error*/
            case sError: 
                error = LEX_ERR;
                return error;
            break;
            case sLParenth:
                token = get_Token();

                switch (token.id) {
                    case sError:
                        error = LEX_ERR;
                        return error;
                    break;
                    case sString :
                        error = OK_ERR;
                    break;
                    // ak dostanem ID pozriem sa do tabulky symbolov ci bola definovana
                    case sIdent :
                        ;
                        Variable * identifier_of_parameter = searchFunctionVariableInStack(symbol_table.actual_function,token.attribute);

                        if (identifier_of_parameter == NULL) {
                            error = SEM_UNDEF_ERR;
                            return error;
                        }
                        else if (identifier_of_parameter->typ != sString) {
                            error = SEM_TYPE_ERR;
                            return error;
                        }
                        else {
                            error = OK_ERR;
                        }
                    break;
                    default :
                        error = SYN_ERR;
                        return error;
                    break;
                }

                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }
                // dostal som ,
                if (token.id != sComma) {
                    error = SYN_ERR;
                    return error;
                }
                token = get_Token();

                switch (token.id) {
                    case sError:
                        error = LEX_ERR;
                        return error;
                    break;
                    case sString :
                        error = OK_ERR;
                    break;
                    // ak dostanem ID pozriem sa do tabulky symbolov ci bola definovana
                    case sIdent :
                        ;
                        Variable * identifier_of_parameter = searchFunctionVariableInStack(symbol_table.actual_function,token.attribute);

                        if (identifier_of_parameter == NULL) {
                            error = SEM_UNDEF_ERR;
                            return error;
                        }
                        else if (identifier_of_parameter->typ != sString) {
                            error = SEM_TYPE_ERR;
                            return error;
                        }
                        else {
                            error = OK_ERR;
                        }
                    break;
                    default :
                        error = SYN_ERR;
                        return error;
                    break;
                }
                
                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }
                // dostal som )
                if (token.id != sRParenth) {
                    error = SYN_ERR;
                    return error;
                }
            break;
            default :
                error = SYN_ERR;
                return error;
            break;
        }
    }
    /*
    *Vstavana funkcia length
    */
    else if(!(strcmp(token.attribute, "sort"))){

        if(type_control == TRUE) {

            if (typeControl(type_for_expression, sString) != OK_ERR) {
                error = SEM_TYPE_ERR;
                return error;
            }
        }

        token = get_Token();

        switch (token.id) {
            /*pokial lexer vrati lex error*/
            case sError: 
                error = LEX_ERR;
                return error;
            break;
            case sLParenth:
                token = get_Token();

                switch (token.id) {
                    case sError:
                        error = LEX_ERR;
                        return error;
                    break;
                    case sString :
                        error = OK_ERR;
                    break;
                    // ak dostanem ID pozriem sa do tabulky symbolov ci bola definovana
                    case sIdent :
                        ;
                        Variable * identifier_of_parameter = searchFunctionVariableInStack(symbol_table.actual_function,token.attribute);

                        if (identifier_of_parameter == NULL) {
                            error = SEM_UNDEF_ERR;
                            return error;
                        }
                        else if (identifier_of_parameter->typ != sString) {
                            error = SEM_TYPE_ERR;
                            return error;
                        }
                        else {
                            error = OK_ERR;
                        }
                    break;
                    default :
                        error = SYN_ERR;
                        return error;
                    break;
                }

                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }
                // dostal som )
                if (token.id != sRParenth) {
                    error = SYN_ERR;
                    return error;
                }
            break;
            default :
                error = SYN_ERR;
                return error;
            break;
        }
    }
    // toto by nemalo nikdy nastat
    else {
        error = SYN_ERR;
        return error;
    }

    if (error != OK_ERR) {
        return error;
    }
    return OK_ERR;
}
