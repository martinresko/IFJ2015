#include"expression.h"

tToken TOKEN;/* ukazatel na token bude sluzit ako docasne ulozisko tokenu */

FILE *file;

//extern int AKO_SA_MAS;

int main()
{
	file = fopen("test","r");
//	printf("GLOOBAL %d\n",AKO_SA_MAS);
	ERROR_CODE ret=expression();
	printf("skoncil som s navratovym kodom %d, token s atributom %s\n",ret,TOKEN.attribute);
	fclose(file);
	return EXIT_SUCCESS;
}

/* funkcia volana z rekuzivneho zostupu 
 * return - navratova hodnota z error.h */
ERROR_CODE expression()
{
	ListPointer list;
	ERROR_CODE return_analysis = Analysis(&list);
	return return_analysis;
}

/* hlavna funkcia ktora vsetko koordinuje */
/* return - z error.h */
/* Lis - ukazatel na list */
ERROR_CODE Analysis(ListPointer *Lis)
{
	if(InitExpressionList(Lis)!=OK_ERR)
		return INTERN_ERR;
	//if(get_first_token==false)
	//{
		TOKEN = get_Token(); /* ukazatel na token bude sluzit ako docasne ulozisko tokenu */
		if(TOKEN.id==sError)
			return LEX_ERR;
	//}
	//else
	//{
	//	TOKEN=RECURSE_TOKEN;
	//}
	
	bool vykonavanie_cyklu=true;
	while(vykonavanie_cyklu) /* cyklus vykonavam pokial spracujem vsetko a na vstupe je DOLLAR(;) alebo mi pride pravidlo P (toto bolo uspesne vykonanie) a neuspesne ak mi ako dalsie pravidlo prislo X alebo na vstupe je UNKNOWN id */
	{
		char next_step=DecideShiftOrReduce(Lis,TOKEN.id); /* zistime co mame vykonat */
		if(next_step=='R')
		{
			ERROR_CODE return_reduce = Reduce(Lis);
			if(return_reduce!=OK_ERR)
			{
				printf("Redukcia neprabehla\n");
				vykonavanie_cyklu=false;
				return return_reduce;
			}
		}
		else if(next_step=='S')
		{
			ERROR_CODE return_shift=Shift(Lis,TOKEN.id,TOKEN.attribute);
			if(return_shift==OK_ERR)
			{
				TOKEN = get_Token(); /* aktualizujem token (nacitam dalsi) */
				if(TOKEN.id==sError)
					return LEX_ERR;
			}
			else if(return_shift==SEM_UNDEF_ERR)
				return SEM_UNDEF_ERR;
			else
				return INTERN_ERR;
		}
		else if(next_step=='T')
		{
			printf("pravidlo T:\n");
			ReduceT(Lis);
			TOKEN = get_Token(); /* aktualizujem token za ')' */
			if(TOKEN.id==sError)
				return LEX_ERR;
		}
		else if(next_step=='P') /* pripad ked na posledny terminal je DOLLAR a na vstupe je ) */
		{
			if(	 (Lis->first_list_element->next)!=NULL)
			/* ak je zasobnik vyzera $ D ), a nie $ ) lebo toto by bola chyba */
			{
				vykonavanie_cyklu = false; /* dalej cyklus nevykonavam */
				/* na zasobniku je posledny terminal DOLLAR a na vrchole ) */
				printf("koncim uspesne\n");
				return OK_ERR; /* koncim uspechom */
			}
			else
			{
				printf("pravidlo P ale bez opedandu \n");
				return SYN_ERR;
			}
		}
		else /* next_step==X */
		{
			printf("Syntakticka chyba pravidlo X \n");
			vykonavanie_cyklu=false;
			return SYN_ERR; /* vratim chybu */
		}

		if( (expressionIdChose(TOKEN.id)==DOLLAR) && (((struct precedence_table_element*)(Lis->last_terminal->data))->expresion_id==DOLLAR) )
		{
			vykonavanie_cyklu = false;
			printf("koncim uspesne\n");
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
	if(rule==OPERAND) /* terminal je i*/
	{
		printf(" i -> D \n");
		((Precedence_table_element *)(Lis->last_terminal->data))->terminal=false;
		FindLastTerminal(Lis); /* najdenie noveho terminalu */
		return OK_ERR;
	}
	else if(( rule >= MUL ) && (rule <= NE ) ) /* terminal je operand */
	{
		List left_operand = Lis->last_terminal->prev;
		List right_operand = Lis->last_terminal->next;
		if( ( (left_operand!=NULL) && (((Precedence_table_element *)(left_operand->data))->expresion_id==OPERAND) ) && ( (right_operand!=NULL) && (((Precedence_table_element *)(right_operand->data))->expresion_id==OPERAND) ) )/* ci okolo operatoru nieco je a ci su to operandy */
		{
			int left_id=((Precedence_table_element *)(left_operand->data))->id;
			int right_id=((Precedence_table_element *)(right_operand->data))->id;
			switch(rule)
			{
				case(MUL):
					printf("MUL\n");
					if( left_id==sString || right_id==sString)
						return SEM_TYPE_ERR;
					/* Doplnit instrukciu */
					break;
				case(DIV):
					printf("DIV\n");
					if( left_id==sString || right_id==sString )
						return SEM_TYPE_ERR;
					/* Doplnit instrukciu */
					break;
				case(PLUS):
					printf("PLUS\n");
					if( left_id==sString || right_id==sString )
						return SEM_TYPE_ERR;
					/* Doplnit instrukciu */
					break;
				case(MINUS):
					printf("MINUS\n");
					if( left_id==sString || right_id==sString )
						return SEM_TYPE_ERR; /* chyba v pripade ze jeden alebo oba operatory su string */
					/* Doplnit instrukciu */
					break;
				case(LT):
					printf("LT\n");
					if( (left_id==sString && right_id!=sString) || ( (left_id!=sString && right_id==sString) ) )
						return SEM_TYPE_ERR;
					/* Doplnit instrukciu */
					break;
				case(GT):
					printf("GT\n");
					if( (left_id==sString && right_id!=sString) || ( (left_id!=sString && right_id==sString) ) )
						return SEM_TYPE_ERR;
					/* Doplnit instrukciu */
					break;
				case(LE):
					printf("LE\n");
					if( (left_id==sString && right_id!=sString) || ( (left_id!=sString && right_id==sString) ) )
						return SEM_TYPE_ERR;
					/* Doplnit instrukciu */
					break;
				case(GE):
					printf("GE\n");
					if( (left_id==sString && right_id!=sString) || ( (left_id!=sString && right_id==sString) ) )
						return SEM_TYPE_ERR;
					/* Doplnit instrukciu */
					break;
				case(EQ):
					printf("EQ\n");
					if( (left_id==sString && right_id!=sString) || ( (left_id!=sString && right_id==sString) ) )
						return SEM_TYPE_ERR;
					/* Doplnit instrukciu */
					break;
				case(NE):
					printf("NE\n");
					if( (left_id==sString && right_id!=sString) || ( (left_id!=sString && right_id==sString) ) )
						return SEM_TYPE_ERR;
					/* Doplnit semanticku akciu */
					break;
			}
			FindLastTerminal(Lis); /* najdenie noveho terminalu */
			DeleteLast(Lis);/* odstranime pravy operand */
			DeleteLast(Lis); /* odstanime operator */
			/* priradenie hodnoty niekam aby som mohol pouzit lavy operand */
			return OK_ERR;
		}
		else
		{
			printf("ERRROR chyba operator\n");
			return SYN_ERR;
		}
	}
	else
		return SYN_ERR;
}

/* funkcia pre pravidlo (D) -> D
 * Lis - ukazatel na zoznam */
void ReduceT(ListPointer *Lis)
{
	printf("(D) -> D\n");
	Lis->last_terminal->data=Lis->last_terminal->next->data; /* skopirujem data z operandu do ( */
	DeleteLast(Lis); /* odstranim operand */
	((Precedence_table_element *)(Lis->last_terminal->data))->terminal=false; /* z ( odnastavim terminal lebo teraz je to Operand */
	FindLastTerminal(Lis); /* nastavim novy terminal */
}

/* shift vlastne iba pushne znak z input na list */
/* return - INTERN alebo OK */
/* Lis - ukazatel na list */
/* id  - id z tokenu */
/* attribute - attribute z tokenu */
ERROR_CODE Shift(ListPointer *Lis,int id,char *attribute)
{
	Lis->last_terminal=insertElement(Lis,id,attribute);
	//if(id==sIdent) /* skontrolujem semantiku vkladaneho prvku */
	//{
	//	if(searchFunctionVariable(table.actual_function,attribute)==NULL) /* ci sa premenna nachacha v tabulke symbolov */
	//		return SEM_UNDEF_ERR;/* nenasiel som v tabulke err pouzivanie nedefinovanej premennej */
	//}
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
	printf("%s <-> %s\n",((Precedence_table_element *)(Lis->last_terminal->data))->attribute,TOKEN.attribute);
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
	 	Precedence_table_element *elem = malloc(sizeof(struct precedence_table_element));
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
