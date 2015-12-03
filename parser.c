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
 
 #include "parser.h"


tToken token; // aktualny token
extern tToken token_expression;
//Table_symbols symbol_table;


/**
 * @info:<prog> ->  <body> ; $
 */

ERROR_CODE prog()
{
    //globalTableOfSymbolsInit(&symbol_table);

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
    /*kontrola ci je to ID*/
    if (token.id != sIdent) {
        printf("Nieje id\n");
        error = SYN_ERR;
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
    ERROR_CODE error = OK_ERR;

    if (token.id == sKeyWord) {

        if(!(strcmp(token.attribute, "int"))) { 
            printf("je to int\n");
            error = OK_ERR;
            return error;
        }
        if(!(strcmp(token.attribute, "double"))) {
            printf("je to double\n");
            error = OK_ERR;
            return error;
        }
        if(!(strcmp(token.attribute, "string"))) {
            printf("je to string\n");
            error = OK_ERR;
            return error;
        }
    }
    else {
        printf("je to picovina\n");
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
            printf("je to ID\n");
            error = multi_params();

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
            error = OK_ERR;
            return error;
            break;
        case sLSetPar :
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
    
    switch (token.id) {
        /*pokila lexer vrati lex error*/
        case sError : 
            error = LEX_ERR;
            return error;
            break;
        /*ak som dostal "}"*/
        case sRSetPar :
            printf("dostal som }\n");
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
         * id<funkcia_priradenie>;
        */
        case sIdent :
            printf("idem do funkcia_priradenie\n");
            error = funkcia_priradenie();

            if (error != OK_ERR) {
                return error;
            }

            //token = get_Token();

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
                    printf("chyba\n");               
                    error = SYN_ERR;
                    return error;
                    break;
            }
        default : 
            /*
             *return expresion ;
             */
            if(!(strcmp(token.attribute, "return"))){
                 printf("volam expression\n");
                /*error = exppres(); // expression
                
                if (error != OK_ERR) { zmazat
                    return error;
                } */
                // zo prediktivnej som uz dostal token
                /*kontrola ci je Lex_ERR*/
                token = get_Token();
                if (token.id == sError) {
                    error = LEX_ERR;
                    return error;
                // ak bol zadany ;
                } else if (token.id == sSemicolon) {
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
                /*error = exppres(); // expression
                
                if (error != OK_ERR) { zmazat
                    return error;
                } */
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
             * for (<for_definice> ; expresion ; id = expression) { <stat_list>}
             */ 
            else if(!(strcmp(token.attribute, "for"))){
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
                    printf("nieje ;\n");
                    error = SYN_ERR;
                    return error;
                }

                // volam expression

                if(token.id != sSemicolon) {
                    printf("nieje ;\n");
                    error = SYN_ERR;
                    return error;
                }

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
                //error =expression();
                // ak dostanem (
                if(token.id != sLParenth) {
                    printf("nieje (\n");
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
                printf("e {\n");

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
                token = get_Token ();

                switch (token.id) {
                    /*pokial lexer vrati lex error*/
                    case sError: 
                        error = LEX_ERR;
                        return error;
                        break;
                    case sCout :
                        error = term();

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
    ERROR_CODE error;
    token = get_Token();

    switch (token.id) {
        /*pokial lexer vrati lex error*/
        case sError: 
            error = LEX_ERR;
            return error;
            break;
        /*ak dostanem zatvorku idem do arguments*/
        case sLParenth :
            error = arguments();

            if (error != OK_ERR) {
                return error;
            }

            // asi nebude token = get_Token();

            switch (token.id) {
                /*pokial lexer vrati lex error*/
                case sError: 
                    error = LEX_ERR;
                    return error;
                    break;
                case sRParenth :
                    error = OK_ERR;
                    return error;
                    break;
                default :
                    error = SYN_ERR;
                    return error;
                    break;
            }
            break;
        /*ak som dostal = tak idem do deklaracia*/
        case sAssign :
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

/**
 * @info:  <arguments> 
 * id <multi_arguments>
 */

ERROR_CODE arguments()
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
            error = multi_arguments();

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

/**
 * @info: <multi_arguments>
 *  , id <multi_arguments>
 * empty
**/

ERROR_CODE multi_arguments()
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
                    error = OK_ERR;
                    break;
                 default :
                    error = SYN_ERR;
                    return error;
                    break;
            }

            break;
        // ak som dostal )
        case sRParenth :
            error = OK_ERR;
            return error;
            break;
        default :
            error = SYN_ERR;
            return error;
            break;
    }

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
        case sCin :
            token = get_Token ();

            switch (token.id) {
                /*pokial lexer vrati lex error*/
                case sError: 
                    error = LEX_ERR;
                    return error;
                    break;
                case sIdent :
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


/**
 * @info: <multi_cout> 
 *  << <term> <multi_count>
 *  empty
**/

ERROR_CODE multi_cout()
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
        case sCout :
            error = term();

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

/**
 * @info: <assign> 
 *  <auto>;
 *  <inicializacia>;
**/

ERROR_CODE assign()
{
    ERROR_CODE error;

    error = typ();

    if (error != OK_ERR) {
        error = fun_auto();

        if (error != OK_ERR) {
            return error;
        } 
    }
    else {

        error = inicialization();

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
    ERROR_CODE error;

    if(token.id != sAssign) {
        error = SYN_ERR;
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
*/

ERROR_CODE hodnota_priradenia()
{
    ERROR_CODE error;
    //token = get_Token();
    // ak dostanem lexiklanu chybu LEX_ERR
    switch (token.id) {
        /*pokial lexer vrati lex error*/
        case sError: 
            error = LEX_ERR;
            return error;
            break;
        default :
            error = expression();
            printf("skoncil som s navratovym kodom %d, token s atributom %s\n",error,token_expression.attribute);
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
    ERROR_CODE error;
    token = get_Token ();
    // ak dostanem lexiklanu chybu LEX_ERR
    if (token.id == sError) {
        error = LEX_ERR;
        return error;
    // ak dostanem ocakavany vstup OK_ERR
    } else if((token.id == sIdent) || (token.id == sInteger) || 
        (token.id == sDouble) || (token.id == sExpo) || (token.id == sString) ) 
    {
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

        token = get_Token();

        if (token.id == sError) {
            error = LEX_ERR;
            return error;
        }

        if(token.id != sAssign) {
            error = SYN_ERR;
            return error;
        }

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
*/

ERROR_CODE inicialization()
{
    ERROR_CODE error;

    error = typ();

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
        //case sAssign :
            // volam expression
           // break;
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