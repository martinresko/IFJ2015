#include "expression.h"

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
/* Lis - ukazatel na list */
void Analysis(ListPointer *Lis)
{
	InitExpressionList(Lis);
	char *input[]={"i","+","(","i",")","#"};
	int i=0;
	PrintList(Lis);
	while((strcmp(input[i],"#")!=0) || (strcmp(Lis->last_terminal->data,"$")!=0))/* kym este je nieco na input */
	{
		char next_step=DecideShiftOrReduce(Lis,input[i]);
		if(next_step=='R')
		{
			printf("Redukujeme:\n");
			if(Reduce(Lis)==-1)
			{
				printf("Redukcia neprabehla\n");
				return;
			}
				
		}
		else if(next_step=='S')
		{
			printf("Shiftujeme:\n");
			Shift(Lis,input[i]);
			i++; /* posuniem sa dalej v inpute len ked shitfujem */
		}
		else if(next_step=='T')
		{
			printf("pravidlo T:\n");
			ReduceT(Lis);
			i++; /* chem sa zbavit uzatvarajucej zatvorky na vstupe */
		}
		else
		{
			printf("Syntakticka chyba\n");
			return;
		}
		PrintList(Lis);
	}
}
/* inicializacia listu podla precednecnej syntaktickej analyzi tj. vlozeenie zaciatku lista alias znak $ */
/* Lis - ukazatel na list */
void InitExpressionList(ListPointer *Lis)
{
	InitList(Lis);
	Lis->last_terminal=InsertLast(Lis,"$"); /* zaciatok lista a prvy terminal */
	Lis->last_terminal->terminal=true; /* set element as terminal */
}


/* funkcia podla posledneho terminalu na listu vykona redukciu */
/* return - -1 error redukcie, 1 redukcia prebehla ok */
/* Lis - ukazatel na list */
int Reduce(ListPointer *Lis)
{
	int rule = ConvertCharToAccessPrecedenceTable(Lis->last_terminal->data);
	if(rule==OP) /* terminal je i*/
	{
			printf(" i -> D \n");
			Lis->last_terminal->data="D";
			Lis->last_terminal->terminal=false;
			FindLastTerminal(Lis); /* najdenie noveho terminalu */
			return 1;
	}
	else if((rule >=MUL) && (rule<=NE) ) /* terminal je operand */
	{
		List left_operand = Lis->last_terminal->prev;
		//List right_operand = Lis->last_terminal->next;
		switch(rule)
		{
			case(MUL):
				printf("MUL\n");
				left_operand->data="D";/* Doplnit semanticku akciu */
				break;
			case(DIV):
				printf("DIV\n");
				left_operand->data="D";/* Doplnit semanticku akciu */
				break;
			case(PLUS):
				printf("PLUS\n");
				left_operand->data="D";/* Doplnit semanticku akciu */
				break;
			case(MINUS):
				printf("MINUS\n");
				left_operand->data="D";/* Doplnit semanticku akciu */
				break;
			case(LT):
				printf("LT\n");
				left_operand->data="D";/* Doplnit semanticku akciu */
				break;
			case(GT):
				printf("GT\n");
				left_operand->data="D";/* Doplnit semanticku akciu */
				break;
			case(LE):
				printf("LE\n");
				left_operand->data="D";/* Doplnit semanticku akciu */
				break;
			case(GE):
				printf("GE\n");
				left_operand->data="D";/* Doplnit semanticku akciu */
				break;
			case(EQ):
				printf("EQ\n");
				left_operand->data="D";/* Doplnit semanticku akciu */
				break;
			case(NE):
				printf("NE\n");
				left_operand->data="D";/* Doplnit semanticku akciu */
				break;
		}
		Lis->last_terminal->terminal=false; /* pre istotu aj ked by ten operator mal za chvilu zmiznut */
		FindLastTerminal(Lis); /* najdenie noveho terminalu */
		DeleteLast(Lis); /* odstanime operator a pravy operand */
		DeleteLast(Lis);
		return 1;
	}
	else
		return -1;
}
void ReduceT(ListPointer *Lis)
{
	printf("(D) -> D\n");
	Lis->last_terminal->data=Lis->last_terminal->next->data;
	DeleteLast(Lis); /* TREBA VYRIESIT uzatvaraciu zatvorku !!!! */
	Lis->last_terminal->terminal=false;
	FindLastTerminal(Lis);
}
/* shift vlastne iba pushne znak z input na list a nastavy novy last terminal */
/* Lis - ukazatel na list */
/* char_input - prvy znak z input */
void Shift(ListPointer *Lis,char * char_input)
{
	Lis->last_terminal=InsertLast(Lis,char_input);
	Lis->last_terminal->terminal=true;
}

/* funkcia ma za ulohu rozhodnut ci sa ma redukovat alebo shiftovat a to podla posledneho terminalu na listu a prveho znaku(tokenu)*/
/* Lis - ukazatel na list */
/* char_input - prvy retazec */
/* return - hodnota R(reduce), S(shift), T(uzatvorenie zatvoriek), X(chyba) */
char DecideShiftOrReduce(ListPointer *Lis,char* char_input)
{
	int list_terminal = ConvertCharToAccessPrecedenceTable(Lis->last_terminal->data);
	int token_symbol = ConvertCharToAccessPrecedenceTable(char_input);
	return PRECEDENCE_TABLE[list_terminal][token_symbol];
}

/* fukncia sluzi na konverziu retazca na jedno z globalnych cisel (MUL,DIV,EOL,...) */
/* symbol - konvertovany symbol musi byt retazec lebo napr. '<=' sa neda ulozit ako char */
/* return - globalne cislo alebo -1 ako error */
int ConvertCharToAccessPrecedenceTable(char *symbol)
{
	if(strcmp(symbol,"*")==0)
		return MUL;
	else if(strcmp(symbol,"/")==0)
		return DIV;
	else if(strcmp(symbol,"+")==0)
		return PLUS;
	else if(strcmp(symbol,"-")==0)
		return MINUS;
	else if(strcmp(symbol,"<")==0)
		return LT;
	else if(strcmp(symbol,">")==0)
		return GT;
	else if(strcmp(symbol,"<=")==0)
		return LE;
	else if(strcmp(symbol,">=")==0)
		return GE;
	else if(strcmp(symbol,"==")==0)
		return EQ;
	else if(strcmp(symbol,"!=")==0)
		return NE;
	else if(strcmp(symbol,"(")==0)
		return LEFT;
	else if(strcmp(symbol,")")==0)
		return RIGHT;
	else if(strcmp(symbol,"i")==0)
		return OP;
	else if(strcmp(symbol,"$")==0)
		return EOL;
	else
		return -1;
	return -1;
}

/* najde sa nasledujuci terminal na listu a prenastavy sa */
/* Lis - ukazatel na list */
void FindLastTerminal(ListPointer *Lis)
{
 	List helpful_pointer = Lis->last_terminal; /* set pointer to last terminal */
	if(helpful_pointer->prev==NULL) /* posledny terminal je $ */
		return; /* nic nerob */
	else
	{
		helpful_pointer = Lis->last_terminal->prev;
		while((helpful_pointer!=NULL))
		{
			if(helpful_pointer->terminal==true)
			{
				Lis->last_terminal=helpful_pointer;
				return;/* nasli sme dalsi terminal */
			}
			else
				helpful_pointer=helpful_pointer->prev;

		}
	}
}
