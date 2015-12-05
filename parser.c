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
**/
 
#include "parser.h"

#define AUTO 9
#define TAKEN_FIRST_TOKEN 1
#define NOT_TAKEN_FIRST_TOKEN 0

tToken token; // aktualny token
extern tToken token_expression;
extern Table_symbols symbol_table;
extern int typ_pre_auto;
tState type_of_element_to_table_of_symbols;
int type_for_expression;



/**
 * @info:<prog> ->  <body> ; $
 */

ERROR_CODE prog()
{
    globalTableOfSymbolsInit(&symbol_table);
    printf("som v prog\n");
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
             printf("koniec1\n"); 
            error = SYN_ERR;
            break;
        // ak dostanem hocico ine
        default :
            printf("Body\n");
            error = body();
            break;
    }
    printf("Program vystup %d\n", error );
    if (error != OK_ERR) {
        return error;
    }
    

    return OK_ERR;
}

/**
 * @info:<body> -> <function><body> /empty
 */

ERROR_CODE body()
{
    printf("som v body\n");
    ERROR_CODE error;

    switch (token.id) {
        /* chyba v lexikalnom analyzatore*/
        case sError :
             error = LEX_ERR; 
             break;
        /*koniec suboru po uz jedej funkcii min */     
        case sEndofFile :
            printf("koniec2\n");
            error = OK_ERR;
            return error;
            break;
        default :
             printf("idem do funkcion\n");
            error = function();
            if (error != OK_ERR) {
                return error;
            }
            break;
    }

    token = get_Token();
    error = body();

    if (error != OK_ERR) {
        return error;
    }
    printf("Program vystup %d\n", error );

    return OK_ERR;
}

/**
 * @info:function> -> <typ> id (<params>) <protype_or_definition>
 */

ERROR_CODE function()
{
    // skontrolovat este rezzervovane slova
    printf("som vo funkcion\n");
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
        printf("chyba pokus o redefiniciu vstavanej funkcie\n");
        error = SEM_UNDEF_ERR;
        return error;
    }
    /*kontrola ci je to ID*/
    if (token.id != sIdent) {
        printf("Nieje id\n");
        error = SYN_ERR;
        return error;
    }
    // vlozim do tabulky symbolov 
    error = insertFunction(&symbol_table,token.attribute,type_of_element_to_table_of_symbols);

    if (error != OK_ERR){
        return error;
    }
    printf("je id\n");
    token = get_Token();
    /*kontrola ci je Lex_ERR*/
    if (token.id == sError) {
        error = LEX_ERR;
        return error;
    }
    /*kontrola ci je to "("*/
    if (token.id != sLParenth) {
        printf("nieje to (\n");
        error = SYN_ERR;
        return error;
    }
    printf("je to (\n");
    error = params();

    if (error != OK_ERR){
        return error;
    }
   	printf("idem do prototype_of_definition\n");
    error = prototype_of_definition();

    if (error != OK_ERR){
        return error;
    }

    return OK_ERR;
}

/**
 * @info:<typ> -> int / string / double
 */

ERROR_CODE typ()
{
    printf("som v typ\n");
    printf("%s\n",token.attribute );
    ERROR_CODE error = OK_ERR;

    if (token.id == sKeyWord) {

        if(!(strcmp(token.attribute, "int"))) { 
            printf("je to int\n");
            type_of_element_to_table_of_symbols = sInteger;
            error = OK_ERR;
            return error;
        }
        if(!(strcmp(token.attribute, "double"))) {
            printf("je to double\n");
            type_of_element_to_table_of_symbols = sDouble;
            error = OK_ERR;
            return error;
        }
        if(!(strcmp(token.attribute, "string"))) {
            printf("je to string\n");
            type_of_element_to_table_of_symbols = sString;
            error = OK_ERR;
            return error;
        }
        if(!(strcmp(token.attribute, "auto"))) {
            printf("je to auto\n");
            error = 20;
            return error;
        }
    }
    else {
        printf("je to zly typ\n");
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
    printf("som v prams\n");
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
            printf("nieje )\n");
            error = SYN_ERR;
            return error;
        }
        /*ak nema funkcia ziadny parameter*/
        else {
            printf("je )\n");
            error = OK_ERR;
        }
    }  /*ak ma funkcia parameter*/
    else {
        printf("ma viac parametrov\n");
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
        //vkladanie prametrov do tabulky symbolov
        error = insertFunctionParam(symbol_table.actual_function,token.attribute,type_of_element_to_table_of_symbols);

        if (error != OK_ERR) {
            return error;
        }
        error = multi_params();
    }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/**
 * @info:<multi_params> -> , <typ> id <multi_params> / empty
 */

ERROR_CODE multi_params()
{
    printf("som v multi params\n");
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
            printf("je to )\n");
            error = OK_ERR;
            return error;
            break;
        /*pokila dostanem ciarku tak moze byt viac parametrov a pokracujem*/
        case sComma :
            printf("je to ciarka\n");
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
            //vkladanie prametrov do tabulky symbolov
            error = insertFunctionParam(symbol_table.actual_function,token.attribute,type_of_element_to_table_of_symbols);
        
            if (error != OK_ERR) {
                return error;
            }
            printf("je to ID\n");
            error = multi_params();

            if (error != OK_ERR) {
                return error;
            }
            break;
        default :
        	printf("je to zle\n");
            error = SYN_ERR;
            return error;
            break;
    }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/**
 * @info:<prototype_or_definition> -> { <stat_list> } / ;
 */

ERROR_CODE prototype_of_definition()
{
    printf("som v prototype_of_definition\n");
    ERROR_CODE error;

    token = get_Token();

    switch (token.id) {
        /*pokila lexer vrati lex error*/
        case sError : 
            error = LEX_ERR;
            return error;
            break;    
        case sSemicolon :
        	printf("dostal som ; je to prototyp\n");
            // ak dostanem ; tak nieje funkcia definovana
            symbol_table.actual_function->defined = FALSE;
            error = OK_ERR;
            return error;
            break;
        case sLSetPar :
            // ak dostanem { tak je uz funkcia definovana
            symbol_table.actual_function->defined = TRUE;
            // pushnem blok
            printf("IDEM pushnut blok \n");
            error = pushBlock(symbol_table.actual_function);
            if (error != OK_ERR) {
                return error;
            }

            printf("idem do stat list\n");
            error = stat_list();
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

/**
 * @info:<stat_list> -> <command> <stat_list> /empty
 */

ERROR_CODE stat_list()
{
    printf("som v stat list\n");
    token = get_Token();
    ERROR_CODE error;

    if(stackEmpty(&(symbol_table.actual_function->symbol_table_of_block)))
        	printf("PRAZDNY zasobnik v stat list \n");

    switch (token.id) {
        /*pokila lexer vrati lex error*/
        case sError : 
            error = LEX_ERR;
            return error;
            break;
        /*ak som dostal "}"*/
        case sRSetPar :
            printf("dostal som }\n");
            // popujem zasobnik
            popBlock(symbol_table.actual_function);
            error = OK_ERR;
            return error;
            break;
        case sEndofFile :
            printf("som na konci suboru\n");
            error = SYN_ERR;
            return error;
            break;
        default :
            printf("idem do command\n");
			if(stackEmpty(&(symbol_table.actual_function->symbol_table_of_block)))
        		printf("PRAZDNY zasobnik v stat na IDEM DO COMMAND \n");
            error = command();
            if (error != OK_ERR) {
                return error;
            }
            break;
    }

    error = stat_list();

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/**
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
 */

ERROR_CODE command()
{
    printf("som v command\n");
    ERROR_CODE error;
    if(stackEmpty(&(symbol_table.actual_function->symbol_table_of_block)))
        	printf("PRAZDNY zasobnik v command \n");

    switch (token.id) {
        /*pokila lexer vrati lex error*/
        case sError: 
            error = LEX_ERR;
            return error;
            break;
        /*pokial dostaanem ";"*/
        case sSemicolon :
            printf("dostal som ;\n");
            error = OK_ERR;
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
        	printf("dostal som {\n");
        	error = stat_list();

        	if (error != OK_ERR) {
                return error;
            }
        	break;
        /*
         * id<funkcia_priradenie>;
        */
        case sIdent :
            printf("idem do funkcia_priradenie\n");
			if(stackEmpty(&(symbol_table.actual_function->symbol_table_of_block)))
        		printf("PRAZDNY zasobnik IDEM do FUNKCIA PRIRADENIE\n");
            
            Variable *premenna_na_odvodenie_typu = searchFunctionVariableInStack(symbol_table.actual_function,token.attribute);
            if(premenna_na_odvodenie_typu!=NULL){
            	type_for_expression = premenna_na_odvodenie_typu->typ;
            }
            error = funkcia_priradenie();

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
                    printf("dostal som ;\n");
                    error = OK_ERR;
                    return error;
                    break;
                default : 
                    printf("chyba v command v ID\n");               
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
                printf("volam expression\n");
                // predam do expression aktualny typ funkcie
                printf("navratova hodnota funkcie   %d\n",symbol_table.actual_function->return_type );
                error = expression(NOT_TAKEN_FIRST_TOKEN, symbol_table.actual_function->return_type);
                token = token_expression;

                if (error != OK_ERR) {
                    return error;
                } 

                // zo prediktivnej som uz dostal token
                /*kontrola ci je Lex_ERR*/
                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                // ak bol zadany ;
                } else if (token.id == sSemicolon) {
                	printf("dostal som ;\n");
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
                printf("som v if\n");
                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }
                // ak dostanem (
                if(token.id != sLParenth) {
                    printf("nieje (\n");
                    error = SYN_ERR;
                    return error;
                }
                printf("je (\n");

                error = expression(NOT_TAKEN_FIRST_TOKEN, sEnd); // expression
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
                    printf("nieje )\n");
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
                    printf("nieje {\n");
                    error = SYN_ERR;
                    return error;
                }
                // pushnem blok
                error = pushBlock(symbol_table.actual_function);

                if (error != OK_ERR) {
                    return error;
                }
                printf("je {\n");

                error = stat_list();

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
                    printf("nieje else\n");
                    error = SYN_ERR;
                    return error;
                }
                printf("je else\n");

                // ak dostanem {
                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }

                if(token.id != sLSetPar) {
                    printf("nieje {\n");
                    error = SYN_ERR;
                    

                    return error;
                }
                printf("e {\n");
                	error = pushBlock(symbol_table.actual_function);

		            if (error != OK_ERR) {
		                return error;
		            }
                   if(stackEmpty(&(symbol_table.actual_function->symbol_table_of_block)))
        		   printf("PRAZDNY zasobnik v ELSE\n");

                error = stat_list();

                if (error != OK_ERR) {
                    return error;
                }

                return OK_ERR;
            }
            /*
             *<command> -> cin >> id <multi_cin>;
             */   
            else if(!(strcmp(token.attribute, "cin"))){
            	printf("som v cin\n");
                token = get_Token ();

                switch (token.id) {
                    /*pokial lexer vrati lex error*/
                    case sError: 
                        error = LEX_ERR;
                        return error;
                        break;
                    case sCin :
                    	printf("dostal som >>\n");
                        token = get_Token ();

                        switch (token.id) {
                            /*pokial lexer vrati lex error*/
                            case sError: 
                                error = LEX_ERR;
                                return error;
                                break;
                            case sIdent :
                            	printf("dostal som ID\n");
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
                    	printf("chyba v cin\n");
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

            	printf("som vo for\n");
                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }
                // ak dostanem (
                if(token.id != sLParenth) {
                    printf("nieje (\n");
                    error = SYN_ERR;
                    return error;
                }
                printf("dostal som (\n");
                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }
                printf("idem do for definition\n");
                error = for_definition();

                if (error != OK_ERR) {
                    return error;
                }

                // ak dostanem ;
                if(token.id != sSemicolon) {
                    printf("nieje ;\n");
                    error = SYN_ERR;
                    return error;
                }
                printf("dostal som ;\n");

                error = expression(NOT_TAKEN_FIRST_TOKEN, sEnd); // expression opravit este
                token = token_expression;

                if (error != OK_ERR) {
                    return error;
                }

                if(token.id != sSemicolon) {
                    printf("nieje ;\n");
                    error = SYN_ERR;
                    return error;
                }
                printf("dostal som ;\n");

                token = get_Token();

                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                }

                if(token.id != sIdent) {
                    printf("nieje ID\n");
                    error = SYN_ERR;
                    return error;
                }
                printf("je ID\n");
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
                    printf("nieje =\n");
                    error = SYN_ERR;
                    return error;
                }
                printf("dostal som =\n");

                error = expression(NOT_TAKEN_FIRST_TOKEN,variable_for_search->typ); // expression opravit este
                token = token_expression;

                if (error != OK_ERR) {
                    return error;
                }
                // ak dostanem (
                if(token.id != sRParenth) {
                    printf("nieje )\n");
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
                    printf("nieje {\n");
                    error = SYN_ERR;
                    return error;
                }
                printf("je {\n");

                error = stat_list();

                if (error != OK_ERR) {
                    return error;
                }

                return OK_ERR;

            }
            /*
             * cout << <term> <multi_cout>;
             */
            else if(!(strcmp(token.attribute, "cout"))){
            	printf("som v cout\n");
                token = get_Token ();

                switch (token.id) {
                    /*pokial lexer vrati lex error*/
                    case sError: 
                        error = LEX_ERR;
                        return error;
                        break;
                    case sCout :
                    	printf("dostal som <<\n");
                    	printf("idem do term\n");
                        error = term();

                        if (error != OK_ERR) {
                            return error;
                        }
                        break;
                    default :
                    	printf("chyba v cout\n");
                        error = SYN_ERR;
                        return error;
                        break;
                }

                if (error != OK_ERR) {
                    return error;
                }
                printf("idem do multi_cout\n");
                error = multi_cout();

                if (error != OK_ERR) {
                    return error;
                }

                return OK_ERR;
            } else {
            	printf("idem do assign\n");
                error = assign();

                if (error != OK_ERR) {
                    return error;
                }
            }
            break;

    }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/**
 * @info:  <funkcia_ priradenie>
 *  (<arguments>)
 *  <deklaracia>
 */

ERROR_CODE funkcia_priradenie()
{
	printf("som vo funkcia_priradenie\n");

    ERROR_CODE error;
    token = get_Token();

    if(stackEmpty(&(symbol_table.actual_function->symbol_table_of_block)))
        		printf("PRAZDNY zasobnik v stat na FUNKCIA PRIRADENIA\n");

    switch (token.id) {
        /*pokial lexer vrati lex error*/
        case sError: 
            error = LEX_ERR;
            return error;
            break;
        /*ak dostanem ( idem do arguments*/
        case sLParenth :
        	printf("je to (\n");
        	printf("idem do arguments\n");
            error = arguments();

            if (error != OK_ERR) {
                return error;
            }
            //majo
            // asi nebude token = get_Token();

            switch (token.id) {
                /*pokial lexer vrati lex error*/
                case sError: 
                    error = LEX_ERR;
                    return error;
                    break;
                // dostal som ) 
                case sRParenth :
                	printf("dostal som )\n");
                    error = OK_ERR;
                    return error;
                    break;
                default :
                	printf("chyba v hodnota_priradenia\n");
                    error = SYN_ERR;
                    return error;
                    break;
            }
            break;
        /*ak som dostal = tak idem do deklaracia*/
        case sAssign :
        	printf("idem do declaration\n");
            error = declaration();

            if (error != OK_ERR) {
                return error;
            }

            break;
        default :
        	printf("chyba v hodnota_priradenia\n");
            error = SYN_ERR;
            return error;
            break;
    }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;

}

/**
 * @info:  <arguments> 
 * id <multi_arguments>
 */

ERROR_CODE arguments()
{
	printf("som v arguments\n");
    ERROR_CODE error;
    token = get_Token();

    switch (token.id) {
        /*pokial lexer vrati lex error*/
        case sError: 
            error = LEX_ERR;
            return error;
            break;
        case sIdent :
        	printf("dostal som ID\n");
        	printf("idem do multi_arguments\n");
            error = multi_arguments();

            if (error != OK_ERR) {
                return error;
            }
            break;
        default :
        	printf("chyba v arguments\n");
            error = SYN_ERR;
            return error;
            break;
    }

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/**
 * @info: <multi_arguments>
 *  , id <multi_arguments>
 * empty
**/

ERROR_CODE multi_arguments()
{
	printf("som v multi_arguments\n");
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
        	printf("dostal som ,\n");
            token = get_Token();

            switch (token.id) {
                /*pokial lexer vrati lex error*/
                case sError: 
                    error = LEX_ERR;
                    return error;
                    break;
                case sIdent :
                	printf("dostal som ID\n");
                    error = OK_ERR;
                    break;
                 default :
                 	printf("dostal som chybu\n");
                    error = SYN_ERR;
                    return error;
                    break;
            }

            break;
        // ak som dostal )
        case sRParenth :
        	printf("dostal som )\n");
            error = OK_ERR;
            return error;
            break;
        default :
            error = SYN_ERR;
            return error;
            break;
    }
    printf("idem do multi multi_arguments\n");
    error = multi_arguments();

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/**
 * @info: <multi_cin>  
 *  >> id <multi_cin>
 *  empty
**/

ERROR_CODE multi_cin()
{
	printf("som v multi_cin\n");
    ERROR_CODE error;
    token = get_Token ();

    switch (token.id) {
        /*pokial lexer vrati lex error*/
        case sError: 
            error = LEX_ERR;
            return error;
            break;
        case sSemicolon :
        	printf("dostal som ;\n");
            error = OK_ERR;
            return error;
        case sCin :
        	printf("dostal som >>\n");
            token = get_Token ();

            switch (token.id) {
                /*pokial lexer vrati lex error*/
                case sError: 
                    error = LEX_ERR;
                    return error;
                    break;
                case sIdent :
                	printf("dostal som ID\n");
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
    printf("idem do multi_cin\n");
    error = multi_cin();

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}


/**
 * @info: <multi_cout> 
 *  << <term> <multi_count>
 *  empty
**/

ERROR_CODE multi_cout()
{
	printf("som v multi_cout\n");
    ERROR_CODE error;
    token = get_Token ();

    switch (token.id) {
        /*pokial lexer vrati lex error*/
        case sError: 
            error = LEX_ERR;
            return error;
            break;
        case sSemicolon :
        	printf("dostal som ;\n");
            error = OK_ERR;
            return error;
        case sCout :
        	printf("dostal som <<\n");
            error = term();

            if (error != OK_ERR) {
                return error;
            }
            break;
        default :
        	printf("dostal som chybu\n");
            error = SYN_ERR;
            return error;
            break;
    }

    if (error != OK_ERR) {
        return error;
    }
    printf("idem do multi_cout\n");
    error = multi_cout();

    if (error != OK_ERR) {
        return error;
    }

    return OK_ERR;
}

/**
 * @info: <assign> 
 *  <auto>;
 *  <inicializacia>;
**/

ERROR_CODE assign()
{
	printf("som v assign\n");
    ERROR_CODE error;

    if(stackEmpty(&(symbol_table.actual_function->symbol_table_of_block)))
        	printf("PRAZDNY zasobnik v assign \n");

    error = typ();
    printf("%s\n", token.attribute );
    if (error == OK_ERR) {

    	printf("idem do inicialization\n");
        error = inicialization();

        if (error != OK_ERR) {
            return error;
        } 
    }
    else {
    	printf("idem do fun_auto\n");
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
*/

ERROR_CODE declaration()
{
	printf("som v declaration\n");
    ERROR_CODE error;

    if(stackEmpty(&(symbol_table.actual_function->symbol_table_of_block)))
        		printf("PRAZDNY zasobnik v stat na DECLARATION\n");

    if(token.id != sAssign) {
    	printf("nedostal som =\n");
        error = SYN_ERR;
    }
    printf("dostal som =\n");
    printf("idem do hodnota_priradenia\n");
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
*/

ERROR_CODE hodnota_priradenia()
{	
	printf("hodnota_priradenia\n");
	if(stackEmpty(&(symbol_table.actual_function->symbol_table_of_block)))
        		printf("PRAZDNY zasobnik v stat na ;\n");
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
        	//token = get_Token();
        	Function_GTS *function_for_searching = NULL;
        	function_for_searching = searchFunction(&symbol_table,token.attribute);

			if (function_for_searching == NULL) {
        		printf("volam expression\n");
        		error = expression(TAKEN_FIRST_TOKEN,type_for_expression); // expression opravit este
                token = token_expression;

                if (error != OK_ERR) {
                    return error;
                }
        	}
        	else {
        		token = get_Token();

        		if (token.id == sError) {
			        error = LEX_ERR;
			        return error;
			    }

			    if (token.id != sLParenth) {
			        error = SYN_ERR;
			        return error;
			    }

			    error = arguments();

			    if (error != OK_ERR) {
			        return error;
			    }
			    token = get_Token();
			    printf("hodnota priradenia token atribute %s\n",token.attribute );
			break;
        	}
        default :
        	error = expression(TAKEN_FIRST_TOKEN,type_for_expression); // expression opravit este
            token = token_expression;
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
 *id / cislo / retazec 
*/

ERROR_CODE term()
{
	printf("som v term\n");
    ERROR_CODE error;
    token = get_Token ();
    // ak dostanem lexiklanu chybu LEX_ERR
    if (token.id == sError) {
        error = LEX_ERR;
        return error;
    // ak dostanem ocakavany vstup OK_ERR
    } else if((token.id == sIdent) || (token.id == sInteger) || 
        (token.id == sDouble) || (token.id == sString) ) 
    {
    	printf("dostal som ID alebo cislo alebo string\n");
        error = OK_ERR;
        return error;
    }
    // inak ak dostanem nieco ine SYN_ERR
    else {
    	printf("chyba v term\n");
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
*/

ERROR_CODE fun_auto()
{
	printf("som vo fun_auto\n");
    ERROR_CODE error;
    
    if(stackEmpty(&(symbol_table.actual_function->symbol_table_of_block)))
        	printf("PRAZDNY zasobnik v fun auto \n");

    if(!(strcmp(token.attribute, "auto"))) {
        token = get_Token();

        if (token.id == sError) {
            error = LEX_ERR;
            return error;
        }

        if(token.id != sIdent) {
        	printf("dostal som ID\n");
            error = SYN_ERR;
            return error;
        }
        error = insertFunctionVariableToStack(symbol_table.actual_function,token.attribute,AUTO);
        printf("stale som v aute\n");
        /* premenna ukazuje na aktualne vlozenu polozku */
        Variable *premenna_auto = searchFunctionVariableInStack(symbol_table.actual_function,token.attribute);
        if (error != OK_ERR) {
            return error;
        }

        token = get_Token();

        if (token.id == sError) {
            error = LEX_ERR;
            return error;
        }

        if(token.id != sAssign) {
        	printf("nedostal som =\n");
            error = SYN_ERR;
            return error;
        }
        printf("dostal som =\n");
        error = expression(NOT_TAKEN_FIRST_TOKEN,AUTO); // expression opravit este
        
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
        	printf("dostal som ;\n");
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
*/

ERROR_CODE inicialization()
{
	printf("som v inicialization\n");
    ERROR_CODE error;

    error = typ();
    type_for_expression = type_of_element_to_table_of_symbols;

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
    	printf("dostal som ID\n");
        error = SYN_ERR;
        return error;
    }

    // vkladam do tabulky symbolov typ a id
    error = insertFunctionVariableToStack(symbol_table.actual_function,token.attribute,type_of_element_to_table_of_symbols);

    if (error != OK_ERR) {
        return error;
    }
    printf("idem do declaration_orc\n");
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
*/

ERROR_CODE declaration_or()
{
	printf("som v declaration_or\n");
    ERROR_CODE error;
    token = get_Token();

    if (token.id == sError) {
        error = LEX_ERR;
        return error;
    }

    if (token.id == sSemicolon) {
    	printf("dostal som ;\n");
        error = OK_ERR;
        return error;
    }
    else {
    	printf("idem do declaration\n");
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
*/

ERROR_CODE for_definition()
{
	printf("som v for_definition\n");
    ERROR_CODE error;

    if(!(strcmp(token.attribute, "auto"))){
    	printf("idem do fun_auto\n");
        error = fun_auto();

        if (error != OK_ERR) {
            return error;
        }
    }
    else {
    	printf("idem do for_deklaration\n");
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
*/

ERROR_CODE for_deklaration() 
{
	printf("som v for_deklaration\n");
    ERROR_CODE error;
    error = typ();

    if (error == OK_ERR) {
        token = get_Token();

        if (token.id == sError) {
            error = LEX_ERR;
            return error;
        }

        if(token.id != sIdent) {
        	printf("nedostal som ID\n");
            error = SYN_ERR;
            return error;
        }
        printf("dostal som ID\n");
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
        printf("idem do foo\n");
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
*/

ERROR_CODE foo() 
{
	printf("som v foo\n");
    ERROR_CODE error;

    switch (token.id) {
        /*pokial lexer vrati lex error*/
        case sError: 
            error = LEX_ERR;
            return error;
            break;
        case sSemicolon :
        	printf("dostal som ;\n");
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