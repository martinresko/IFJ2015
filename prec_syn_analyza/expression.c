#include"expression.h"
#include"scaner.h"

tToken TOKEN;/* ukazatel na token bude sluzit ako docasne ulozisko tokenu */

int main()
{
	ListPointer list;
	//InitList(&list);
	//InitExpressionList(&list);
	//InsertLast(&list,"i");
	//InsertLast(&list,"i");
	//PrintList(&list);
	//DeleteLast(&list);
	//PrintList(&list);
	//DestroyList(&list);

	//char *input[]={"i","+",")"};
	
	//printf("returned %c\n" , DecideShiftOrReduce(&list,input[2]));
	//Shift(&list,input[0]);
	//Shift(&list,"(");
	//InsertLast(&list,"D");
	//Reduce(&list);
	//PrintList(&list);
	//printf("posledny terminal %s\n",list.last_terminal->data);
	Analysis(&list);
	return EXIT_SUCCESS;
}

/* hlavna funkcia ktora vsetko koordinuje */
/* return - 1 OK, -1 NOK */
/* Lis - ukazatel na list */
int Analysis(ListPointer *Lis)
{
	InitExpressionList(Lis);
	TOKEN = get_Token(); /* ukazatel na token bude sluzit ako docasne ulozisko tokenu */
	bool vykonavanie_cyklu=true;
	while(vykonavanie_cyklu) /* cyklus vykonavam pokial spracujem vsetko a na vstupe je END(;) alebo mi pride pravidlo P (toto bolo uspesne vykonanie) a neuspesne ak mi ako dalsie pravidlo prislo X alebo na vstupe je UNKNOWN id */
	{
		char next_step=DecideShiftOrReduce(Lis,TOKEN.id); /* zistime co mame vykonat */
		if(next_step=='R')
		{
			printf("Redukujeme:\n");
			if(Reduce(Lis)==-1)
			{
				printf("Redukcia neprabehla\n");
				return -1;
			}
		}
		else if(next_step=='S')
		{
			printf("Shiftujeme:\n");
			Shift(Lis,TOKEN.id,TOKEN.attribute);
			TOKEN = get_Token(); /* aktualizujem token (nacitam dalsi) */
		}
		else if(next_step=='T')
		{
			printf("pravidlo T:\n");
			ReduceT(Lis);
			get_Token(); /* spracujem ')' a rovno aj zahodim lebo som ju uz pouzil */
			TOKEN = get_Token(); /* aktualizujem token za ')' */
		}
		else if(next_step=='P') /* pripad ked na posledny terminal je START a na vstupe je ) */
		{
			TOKEN=get_Token(); /* ulozim si ) */
			vykonavanie_cyklu = false; /* dalej cyklus nevykonavam */
			/* na zasobniku je posledny terminal START a na vrchole ) */
			return 0; /* koncim uspechom */
		}
		else /* next_step==X */
		{
			TOKEN=get_Token(); /* ulozim si token pre rekurzivny */
			printf("Syntakticka chyba pravidlo X \n");
			return -1; /* vratim chybu */
		}
		if( (expressionIdChose(TOKEN.id)==END) && (((struct precedence_table_element*)(Lis->last_terminal->data))->expresion_id==START) )
		{
			vykonavanie_cyklu = false;
			TOKEN=get_Token(); /* precitam strednik */
			return 1; /* koncim uspechom */
		}
	}
}

/* inicializacia zoznamu pre precednecnu syntakcitku analyzu vlozenim $ na zoznam */
/* Lis - ukazatel na list */
void InitExpressionList(ListPointer *Lis)
{
	if(Lis!=NULL)
	{
		InitList(Lis);
		Lis->last_terminal = insertElement(Lis,START,"$"); /* do zoznamu vlozim symbol zaciatku zoznamu a nastavim ho ako posledny terminal */
		((struct precedence_table_element*)(Lis->last_terminal->data))->expresion_id=START; /* musim UNKNOWN -> START */
	}
}

/* funkcia podla posledneho terminalu na zozname vykona redukciu */
/* return - -1 error redukcie napriklad pri operatore neboli 2 operandy, 1 redukcia prebehla ok */
/* Lis - ukazatel na list */
int Reduce(ListPointer *Lis)
{
	int rule = ((struct precedence_table_element*)(Lis->last_terminal->data))->expresion_id;
	if(rule==OPERAND) /* terminal je i*/
	{
		printf(" i -> D \n");
		((Precedence_table_element *)(Lis->last_terminal->data))->terminal=false;
		FindLastTerminal(Lis); /* najdenie noveho terminalu */
		return 1;
	}
	else if((rule >= sMult ) && (rule <= sNotEq ) ) /* terminal je operand */
	{
		List left_operand = Lis->last_terminal->prev;
		List right_operand = Lis->last_terminal->next;
		if( ( (left_operand!=NULL) && (((Precedence_table_element *)(left_operand->data))->expresion_id==OPERAND) ) && ( (right_operand!=NULL) && (((Precedence_table_element *)(right_operand->data))->expresion_id==OPERAND) ) )/* ci okolo operatoru nieco je a ci su to operandy */
		{
			switch(rule)
			{
				case(sMult):
					printf("MUL\n");
					/* Doplnit semanticku akciu a generovanie kodu */
					break;
				case(sDivide):
					printf("DIV\n");
					/* Doplnit semanticku akciu */
					break;
				case(sPlus):
					printf("PLUS\n");
					/* Doplnit semanticku akciu */
					break;
				case(sMinus):
					printf("MINUS\n");
					/* Doplnit semanticku akciu */
					break;
				case(sLess):
					printf("LT\n");
					/* Doplnit semanticku akciu */
					break;
				case(sGreater):
					printf("GT\n");
					/* Doplnit semanticku akciu */
					break;
				case(sLeorEq):
					printf("LE\n");
					/* Doplnit semanticku akciu */
					break;
				case(sGrorEq):
					printf("GE\n");
					/* Doplnit semanticku akciu */
					break;
				case(sEqual):
					printf("EQ\n");
					/* Doplnit semanticku akciu */
					break;
				case(sNotEq):
					printf("NE\n");
					/* Doplnit semanticku akciu */
					break;
			}
		   //	Lis->last_terminal->data->terminal=false; /* pre istotu aj ked by ten operator mal za chvilu zmiznut */
			FindLastTerminal(Lis); /* najdenie noveho terminalu */
			DeleteLast(Lis);/* odstranime pravy operand */
			DeleteLast(Lis); /* odstanime operator */
			/* priradenie hodnoty niekam aby som mohol pouzit lavy operand */
			return 1;
		}
		else
		{
			printf("ERRROR chyba operator\n");
			return -1;
		}
	}
	else
		return -1;
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

/* shift vlastne iba pushne znak z input na list a nastavy novy last terminal */
/* Lis - ukazatel na list */
/* id  - id z tokenu */
/* attribute - attribute z tokenu */
void Shift(ListPointer *Lis,int id,char *attribute)
{
	Lis->last_terminal=insertElement(Lis,id,attribute);
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
	if(helpful_pointer->prev==NULL) /* posledny terminal je START */
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
 * List		 - ukazatel na zoznam
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
			return InsertLast(Lis,elem);
		}
		else
			printf("nedostal som pamat \n");
		return NULL; /* nepodarilo sa vlozit prvok */
}

/* funkcia doplni expresion_id do struktury podla id v tokene 
 * return - cislo ktore sa vlozi to expresion_id
 * id 	  - id ziskane z tokenu */
int expressionIdChose(int id)
{
	if( (id==sIdent) || (id==sInteger) || (id==sDouble) || (id==sIsExpo) || (id==sIsDouble) || (id==sIsExpo2) || (id==sExpo) )
	{
		return OPERAND;
	}
	else if( (id==sMult) || (id==sDivide) || (id==sPlus) || (id==sMinus) || (id==sLess) || (id==sGreater) || (id==sLeorEq) || (id==sGrorEq) || (id==sNotEq) || (id==sLParenth) || (id==sRParenth) || (id==sEqual) )
	{
		return id;			
	}
	else if( id == sSemicolon )
	{
		return END;
	}
	else
	{
		return UNKNOWN; /* nezname id */
	}
}
