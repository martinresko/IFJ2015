/**
 * Predmet: IFJ / IAL
 * Subor:     expression.c
 *            Implementacia precedencnej syntaktickej analyzy
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
 */

#include "expression.h"

#define TAKEN_FIRST_TOKEN 1
#define NOT_TAKEN_FIRST_TOKEN 0

#define AUTO 9

tToken token_expression;/* ukazatel na token bude sluzit ako docasne ulozisko tokenu */
extern tToken token; /* token s rekurzivneho zostupu */
extern Table_symbols symbol_table;

int typ_pre_auto;

const char PRECEDENCE_TABLE[SIZE][SIZE] =
{
	/*			   *   /   +   -   <   >  <=  >=  ==  !=   (   )  OP   $  */
	[MUL]		={'R','R','R','R','R','R','R','R','R','R','S','R','S','R'},
	[DIV] 		={'R','R','R','R','R','R','R','R','R','R','S','R','S','R'},
	[PLUS]  	={'S','S','R','R','R','R','R','R','R','R','S','R','S','R'},
	[MINUS] 	={'S','S','R','R','R','R','R','R','R','R','S','R','S','R'},
	[LT]    	={'S','S','S','S','R','R','R','R','R','R','S','R','S','R'},
	[GT]    	={'S','S','S','S','R','R','R','R','R','R','S','R','S','R'},
	[LE]    	={'S','S','S','S','R','R','R','R','R','R','S','R','S','R'},
	[GE]    	={'S','S','S','S','R','R','R','R','R','R','S','R','S','R'},
	[EQ]    	={'S','S','S','S','R','R','R','R','R','R','S','R','S','R'},
	[NE]    	={'S','S','S','S','R','R','R','R','R','R','S','R','S','R'},
	[LEFT]  	={'S','S','S','S','S','S','S','S','S','S','S','T','S','X'},
	[RIGHT] 	={'R','R','R','R','R','R','R','R','R','R','X','R','X','R'},
	[OPERAND]  	={'R','R','R','R','R','R','R','R','R','R','X','R','X','R'},
	[DOLLAR] 	={'S','S','S','S','S','S','S','S','S','S','S','P','S','X'}
};



/* funkcia volana z rekuzivneho zostupu 
 * return - navratova hodnota z error.h */
ERROR_CODE expression(int first_token,int control_type)
{
	ListPointer list;
	ERROR_CODE return_analysis = Analysis(&list,first_token,control_type);
	DestroyList(&list);
	return return_analysis;
}

/* hlavna funkcia ktora vsetko koordinuje */
/* return - z error.h */
/* Lis - ukazatel na list */
ERROR_CODE Analysis(ListPointer *Lis,int first_token,int type_control)
{
	if(InitExpressionList(Lis)!=OK_ERR)
		return INTERN_ERR;
	if(first_token==NOT_TAKEN_FIRST_TOKEN)
	{
		token_expression = get_Token(); /* ukazatel na token bude sluzit ako docasne ulozisko tokenu */
		if(token_expression.id==sError)
			return LEX_ERR;
	}
	else /* ak mi rekurzivny zostup zobral prvy token */
	{
		token_expression=token;
		if(token_expression.id==sError)
			return LEX_ERR;
	}

	bool vykonavanie_cyklu=true;
	while(vykonavanie_cyklu) /* cyklus vykonavam pokial spracujem vsetko a na vstupe je DOLLAR(;) alebo mi pride pravidlo P (toto bolo uspesne vykonanie) a neuspesne ak mi ako dalsie pravidlo prislo X alebo na vstupe je UNKNOWN id */
	{
		char next_step=DecideShiftOrReduce(Lis,token_expression.id); /* zistime co mame vykonat */
		if(next_step=='R')
		{
			ERROR_CODE return_reduce = Reduce(Lis);
			if(return_reduce!=OK_ERR)
			{
				vykonavanie_cyklu=false;
				return return_reduce;
			}
		}
		else if(next_step=='S')
		{
			ERROR_CODE return_shift=Shift(Lis,token_expression.id,token_expression.attribute);
			if(return_shift==OK_ERR)
			{
				token_expression = get_Token(); /* aktualizujem token (nacitam dalsi) */
				if(token_expression.id==sError)
					return LEX_ERR;
			}
			else if(return_shift==SEM_UNDEF_ERR)
				return SEM_UNDEF_ERR;
			else if(return_shift==SYN_ERR)
				return SYN_ERR;
			else
				return INTERN_ERR;
		}
		else if(next_step=='T')
		{
			if(ReduceT(Lis)!=OK_ERR)
				return SYN_ERR;
			token_expression = get_Token(); /* aktualizujem token za ')' */
			if(token_expression.id==sError)
				return LEX_ERR;
		}
		else if(next_step=='P') /* pripad ked na posledny terminal je DOLLAR a na vstupe je ) */
		{
			if(	 (Lis->first_list_element->next)!=NULL)
			/* ak je zasobnik vyzera $ D ), a nie $ ) lebo toto by bola chyba */
			{
				vykonavanie_cyklu = false; /* dalej cyklus nevykonavam */
				/* na zasobniku je posledny terminal DOLLAR a na vrchole ) */
				if(type_control!=sEnd) /* sEnd oznacuje ze nechcem kontrolovat typ */
				{
					if(type_control == AUTO )
					{
						typ_pre_auto = ((Precedence_table_element *)(Lis->first_list_element->next->data))->id;
						return OK_ERR;
					}
					if( typeControl(((Precedence_table_element *)(Lis->first_list_element->next->data))->id,type_control)!=OK_ERR) /* ak nesedia typy na lavej a pravej strane */
					{
						return SEM_TYPE_ERR;
					}
				}
				return OK_ERR; /* koncim uspechom */
			}
			else
			{
				return SYN_ERR;
			}
		}
		else /* next_step==X */
		{
			vykonavanie_cyklu=false;
			return SYN_ERR; /* vratim chybu */
		}

		if( (expressionIdChose(token_expression.id)==DOLLAR) && (((struct precedence_table_element*)(Lis->last_terminal->data))->expresion_id==DOLLAR) )
		{
			vykonavanie_cyklu = false;
			if(type_control!=sEnd) /* sEnd oznacuje ze nechcem kontrolovat typ */
			{
				if(type_control == AUTO )
				{
					typ_pre_auto = ((Precedence_table_element *)(Lis->first_list_element->next->data))->id;
					return OK_ERR;
				}
				if( typeControl(((Precedence_table_element *)(Lis->first_list_element->next->data))->id,type_control)!=OK_ERR) /* ak nesedia typy na lavej a pravej strane */
				{
					return SEM_TYPE_ERR;
				}
			}
			return OK_ERR; /* koncim uspechom */
		}
	}
	return SYN_ERR;
}

/* inicializacia zoznamu pre precednecnu syntakcitku analyzu vlozenim $ na zoznam */
/* Lis - ukazatel na list */
ERROR_CODE InitExpressionList(ListPointer *Lis)
{
	if(Lis!=NULL)
	{
		InitList(Lis);
		Lis->last_terminal = insertElement(Lis,DOLLAR,"$"); /* do zoznamu vlozim symbol zaciatku zoznamu a nastavim ho ako posledny terminal */
		if(Lis->last_terminal == NULL)
			return INTERN_ERR;
		((struct precedence_table_element*)(Lis->last_terminal->data))->expresion_id=DOLLAR; /* musim UNKNOWN -> DOLLAR */
	}
	return OK_ERR;
}

/* funkcia podla posledneho terminalu na zozname vykona redukciu */
/* return - SYN_ERR error redukcie napriklad pri operatore neboli 2 operandy, OK_ERR redukcia prebehla ok */
/* Lis - ukazatel na list */
ERROR_CODE Reduce(ListPointer *Lis)
{
	int rule = ((struct precedence_table_element*)(Lis->last_terminal->data))->expresion_id;

	//char *c = ((Precedence_table_element *)(Lis->last_terminal->data))->attribute;
	InsVal val;
	char * err = NULL;
	int i = (int)strtoll(((Precedence_table_element *)(Lis->last_terminal->data))->attribute, &err, 0);
	if (*err) {
		double d = strtod(((Precedence_table_element *)(Lis->last_terminal->data))->attribute, &err);
		if (*err) {
			val.VarType = sString;
			val.Str = ((Precedence_table_element *)(Lis->last_terminal->data))->attribute;
		} else {
			val.VarType = sDouble;
			val.Dnum = d;
		}
	} else {
		val.VarType = sInteger;
		val.Inum = i;
	}

	
	if(rule==OPERAND) /* terminal je i*/
	{

		//val.VarType = ((Precedence_table_element *)(Lis->last_terminal->data))->id;
	   	insertFunctionInstruction(symbol_table.actual_function, iPUSH, NULL, &val, NULL);
		
		((Precedence_table_element *)(Lis->last_terminal->data))->terminal=false;
		FindLastTerminal(Lis); /* najdenie noveho terminalu */
		return OK_ERR;
	}
	else if(( rule >= MUL ) && (rule <= NE ) ) /* terminal je operand */
	{
		if( (Lis->last_terminal->prev==NULL) || (Lis->last_terminal->next==NULL) )
		{
			return SYN_ERR;
		}

		List left_operand = Lis->last_terminal->prev;
		List right_operand = Lis->last_terminal->next;

		if( ( (left_operand!=NULL) && (((Precedence_table_element *)(left_operand->data))->expresion_id==OPERAND) ) && ( (right_operand!=NULL) && (((Precedence_table_element *)(right_operand->data))->expresion_id==OPERAND) ) )/* ci okolo operatoru nieco je a ci su to operandy */
		{
			int left_id=((Precedence_table_element *)(left_operand->data))->id;
			int right_id=((Precedence_table_element *)(right_operand->data))->id;
			switch(rule)
			{
				case(MUL):
					if( left_id==sString || right_id==sString)
						return SEM_TYPE_ERR;
					((Precedence_table_element *)(left_operand->data))->id=changeTypeOf(left_id,right_id,MUL); /* pretypovanie */
					insertFunctionInstruction(symbol_table.actual_function, iMUL, NULL, &val, NULL);
					/* Doplnit instrukciu */
					break;
				case(DIV):
					if( left_id==sString || right_id==sString )
						return SEM_TYPE_ERR;
					((Precedence_table_element *)(left_operand->data))->id=changeTypeOf(left_id,right_id,DIV); /* pretypovanie */
					insertFunctionInstruction(symbol_table.actual_function, iDIV, NULL, &val, NULL);
					/* Doplnit instrukciu */
					break;
				case(PLUS):
					if( left_id==sString || right_id==sString )
						return SEM_TYPE_ERR;
					((Precedence_table_element *)(left_operand->data))->id=changeTypeOf(left_id,right_id,PLUS); /* pretypovanie */
					insertFunctionInstruction(symbol_table.actual_function, iADD, NULL, &val, NULL);
					//insertFunctionInstruction(symbol_table.actual_function, iTOPPOP, NULL, NULL, NULL);
					//insertFunctionInstruction(symbol_table.actual_function, iSETVALUE, NULL, &val, NULL);
					/* Doplnit instrukciu */
					break;
				case(MINUS):
					if( left_id==sString || right_id==sString )
						return SEM_TYPE_ERR; /* chyba v pripade ze jeden alebo oba operatory su string */
					((Precedence_table_element *)(left_operand->data))->id=changeTypeOf(left_id,right_id,MINUS); /* pretypovanie */
					insertFunctionInstruction(symbol_table.actual_function, iSUB, NULL, &val, NULL);
					/* Doplnit instrukciu */
					break;
				case(LT):
					if( (left_id==sString && right_id!=sString) || ( (left_id!=sString && right_id==sString) ) )
						return SEM_TYPE_ERR;
					((Precedence_table_element *)(left_operand->data))->id=changeTypeOf(left_id,right_id,LT); /* pretypovanie */
					insertFunctionInstruction(symbol_table.actual_function, iLESS, NULL, &val, NULL);
					/* Doplnit instrukciu */
					break;
				case(GT):
					if( (left_id==sString && right_id!=sString) || ( (left_id!=sString && right_id==sString) ) )
						return SEM_TYPE_ERR;
					((Precedence_table_element *)(left_operand->data))->id=changeTypeOf(left_id,right_id,GT); /* pretypovanie */
					insertFunctionInstruction(symbol_table.actual_function, iGREATER, NULL, &val, NULL);
					/* Doplnit instrukciu */
					break;
				case(LE):
					if( (left_id==sString && right_id!=sString) || ( (left_id!=sString && right_id==sString) ) )
						return SEM_TYPE_ERR;
					((Precedence_table_element *)(left_operand->data))->id=changeTypeOf(left_id,right_id,LE); /* pretypovanie */
					insertFunctionInstruction(symbol_table.actual_function, iELESS, NULL, &val, NULL);
					/* Doplnit instrukciu */
					break;
				case(GE):
					if( (left_id==sString && right_id!=sString) || ( (left_id!=sString && right_id==sString) ) )
						return SEM_TYPE_ERR;
					((Precedence_table_element *)(left_operand->data))->id=changeTypeOf(left_id,right_id,GE); /* pretypovanie */
					insertFunctionInstruction(symbol_table.actual_function, iEGREATER, NULL, &val, NULL);
					/* Doplnit instrukciu */
					break;
				case(EQ):
					if( (left_id==sString && right_id!=sString) || ( (left_id!=sString && right_id==sString) ) )
						return SEM_TYPE_ERR;
					((Precedence_table_element *)(left_operand->data))->id=changeTypeOf(left_id,right_id,EQ); /* pretypovanie */
					insertFunctionInstruction(symbol_table.actual_function, iEQUAL, NULL, &val, NULL);
					/* Doplnit instrukciu */
					break;
				case(NE):
					if( (left_id==sString && right_id!=sString) || ( (left_id!=sString && right_id==sString) ) )
						return SEM_TYPE_ERR;
					((Precedence_table_element *)(left_operand->data))->id=changeTypeOf(left_id,right_id,NE); /* pretypovanie */
					insertFunctionInstruction(symbol_table.actual_function, iNEQUAL, NULL, &val, NULL);
					/* Doplnit semanticku akciu */
					break;
			}
			FindLastTerminal(Lis); /* najdenie noveho terminalu */
			DeleteLast(Lis);/* odstranime pravy operand */
			DeleteLast(Lis); /* odstanime operator */
			return OK_ERR;
		}
		else
		{
			return SYN_ERR;
		}
	}
	else
	{
		return SYN_ERR;
	}
}

/* funkcia pre pravidlo (D) -> D
 * Lis - ukazatel na zoznam */
ERROR_CODE ReduceT(ListPointer *Lis)
{
	if(Lis->last_terminal->next != NULL)
	{
		int typ = ((Precedence_table_element *)(Lis->last_terminal->next->data))->id;
		DeleteLast(Lis); /* odstranim operand */
		((Precedence_table_element *)(Lis->last_terminal->data))->id=typ;
		((Precedence_table_element *)(Lis->last_terminal->data))->expresion_id=OPERAND;
		((Precedence_table_element *)(Lis->last_terminal->data))->terminal=false; 
		FindLastTerminal(Lis); /* nastavim novy terminal */
		return OK_ERR;
	}
	else
	{
		return SYN_ERR;
	}
}

/* shift vlastne iba pushne znak z input na list */
/* return - INTERN alebo OK */
/* Lis - ukazatel na list */
/* id  - id z tokenu */
/* attribute - attribute z tokenu */
ERROR_CODE Shift(ListPointer *Lis,int id,char *attribute)
{
	Lis->last_terminal=insertElement(Lis,id,attribute);
	if(id==sIdent) /* skontrolujem semantiku vkladaneho prvku */
	{
		Variable *polozka_identifikatoru = searchFunctionVariableInStack(symbol_table.actual_function,attribute);
		if( polozka_identifikatoru == NULL) /* ci sa premenna nachacha v tabulke symbolov */
			return SEM_UNDEF_ERR;/* nenasiel som v tabulke err pouzivanie nedefinovanej premennej */
		switch (polozka_identifikatoru->typ) /* prenastavim sIdent na typ */
		{
			case sInteger :
				((Precedence_table_element *)(Lis->last_terminal->data))->id=sInteger;
			break;
			case  sString :
				((Precedence_table_element *)(Lis->last_terminal->data))->id=sString;
			break;
			case  sDouble : 
				((Precedence_table_element *)(Lis->last_terminal->data))->id=sDouble;
			break;
		}
	}

	if(Lis->last_terminal->prev->data!=NULL)
	{
		/* nemozem shiftovat ak na vrchole je OPERAND a na vstupre je OPERAND */
		if( (((Precedence_table_element *)(Lis->last_terminal->data))->expresion_id == OPERAND) && ((((Precedence_table_element *)(Lis->last_terminal->prev->data))->expresion_id == OPERAND)) )
			return SYN_ERR;
	}
	return (Lis->last_terminal==NULL)?INTERN_ERR:OK_ERR;
}

/* funkcia ma za ulohu rozhodnut ci sa ma redukovat alebo shiftovat a to podla posledneho terminalu na zozname a prveho znaku(tokenu)*/
/* Lis - ukazatel na list */
/* id  - id z tokenu */
/* return - hodnota R(reduce), S(shift), T(uzatvorenie zatvoriek), X(chyba) P(specialny pripad ked mam vratit token pravej zatvorky parseru )*/
char DecideShiftOrReduce(ListPointer *Lis,int id)
{
	int list_terminal = ((Precedence_table_element *)(Lis->last_terminal->data))->expresion_id;
	int token_symbol = expressionIdChose(id);
	if(token_symbol!=UNKNOWN)
		return PRECEDENCE_TABLE[list_terminal][token_symbol];
	else
		return 'X';
}

/* najde sa nasledujuci terminal na zozname a prenastavy sa */
/* Lis - ukazatel na list */
void FindLastTerminal(ListPointer *Lis)
{
 	List helpful_pointer = Lis->last_terminal; 
	if(helpful_pointer->prev==NULL) /* posledny terminal je DOLLAR */
		return; /* nic nerob */
	else
	{
		helpful_pointer = Lis->last_terminal->prev;
		while((helpful_pointer!=NULL))
		{
			if( ((Precedence_table_element *)(helpful_pointer->data))->terminal==true)
			{
				Lis->last_terminal=helpful_pointer;
				return;/* nasli sme dalsi terminal */
			}
			else
				helpful_pointer=helpful_pointer->prev;
		}
	}
}

/* funkcie vlozi prvok do zoznamu, kazdy vkladany prvok je terminal
 * return	 - ukazatel na zoznam pri chybe NULL
 * id		 - id ziskane z tokenu
 * attribute - attribute z tokenu */
List insertElement(ListPointer *Lis,int id,char *attribute)
{
	 	Precedence_table_element *elem = memmalloc(sizeof(struct precedence_table_element));
		if(elem!=NULL)
		{
			elem->terminal=true;
			elem->id=id;
			elem->expresion_id=expressionIdChose(id);
			elem->attribute=attribute;
			List ret_insertLast=InsertLast(Lis,elem);
			return (ret_insertLast==NULL)?NULL:ret_insertLast;
		}
		else
			return NULL; /* nepodarilo sa vlozit prvok */
}

/* funkcia doplni expresion_id do struktury podla id v tokene 
 * return - cislo ktore sa vlozi to expresion_id
 * id 	  - id ziskane z tokenu */
int expressionIdChose(int id)
{
	if( (id==sIdent) || (id==sInteger) || (id==sDouble) || (id==sExpo) || (id==sString) )
	{
		return OPERAND;
	}
	else if( (id==sMult) || (id==sDivide) || (id==sPlus) || (id==sMinus) || (id==sLess) || (id==sGreater) || (id==sLeorEq) || (id==sGrorEq) || (id==sNotEq) || (id==sLParenth) || (id==sRParenth) || (id==sEqual) )
	{
		return convertIdToTableAccess(id);			
	}
	else if( id == sSemicolon )
	{
		return DOLLAR;
	}
	else
	{
		return UNKNOWN; /* nezname id */
	}
}

/* funkcie zmeni zmeni id tokenu aby sa dal pouzit do tabulky 
 * return - zmenene id alebo UNKNOWN pri neznamom id
 * id 	  - id ziskane z tokenu */
int convertIdToTableAccess(int id)
{
	switch (id)
	{
		case(sMult):
			return MUL; 
			break;
		case(sDivide):
			return DIV;
			break;
		case(sPlus):
			return PLUS;
			break;
		case(sMinus):
			return MINUS;
			break;
		case(sLess):
			return LT;
			break;
		case(sGreater):
			return GT;
			break;
		case(sLeorEq):
			return LE;
			break;
		case(sGrorEq):
			return GE;
			break;
		case(sEqual):
			return EQ;
			break;
		case(sNotEq):
			return NE;
			break;
		case(sLParenth):
			return LEFT;
			break;
		case(sRParenth):
			return RIGHT;
			break;
	}
	return UNKNOWN;
}

/* funkcie podla operatoru vykona pretypovanie
 * return - vrati hodnotu na ktory typ ma byt pretypovany 
 * left_id - id laveho operandu
 * right_id - id praveho operandu
 * operator - typ operatoru ( delenie, nasobenie, scitanie,...) */
int changeTypeOf(int left_id,int right_id,int operator)
{
 	if(operator==PLUS || operator==MINUS || operator==MUL)
	{
		if(left_id==right_id)
		{
			return left_id;
		}
		else
		{
			return sDouble;
		}
	}
	else if(operator==DIV)
	{
		if(left_id==sInteger && right_id==sInteger)
			return sInteger;
		else
			return sDouble;
	}
	else /* operatory <, > , <= , => ,== , != */
	{
		if(left_id==right_id)
			return sInteger;
		else /* ak je porovnavane int<->double alebo vice versa */
			return sInteger;
	}
	return sError; /* tu by sa nemalo dostat nikdy */
}

/* zisti ci je mozne priradiit pravu stranu vyrazu do typu laveho operandu 
 * return - OK_Err alebo SEM_TYPE_ERR
 * right_side - typ vpravej strany
 * left_side  - typ lavej strany */
ERROR_CODE typeControl(int right_side,int left_side)
{	
	if(right_side==left_side) /* ak su strany rovnake nemam co riesit */
		return OK_ERR;
	else if( right_side==sString || left_side==sString ) /* rovnake niesu tak ak je jeden string druhy string nieje tak err */
		return SEM_TYPE_ERR;
	else /* ak su typy double alebo int a vice versa to nieje chyba */
		return OK_ERR;
}
