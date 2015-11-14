#include "expression.h"

int main()
{
	StackPointer zasobnik;
	//InitStack(&zasobnik);
	//InitExpressionStack(&zasobnik);
	//Push(&zasobnik,"i");
	//Push(&zasobnik,"i");
	//PrintStack(&zasobnik);
	//Pop(&zasobnik);
	//PrintStack(&zasobnik);
	//DestroyStack(&zasobnik);

	//char *input[]={"i","+",")"};
	
	//printf("returned %c\n" , DecideShiftOrReduce(&zasobnik,input[2]));
	//Shift(&zasobnik,input[0]);
	//Shift(&zasobnik,"(");
	//Push(&zasobnik,"D");
	//Reduce(&zasobnik);
	//PrintStack(&zasobnik);
	//printf("posledny terminal %s\n",zasobnik.last_terminal->data);
	Analysis(&zasobnik);
	return EXIT_SUCCESS;
}

/* hlavna funkcia ktora vsetko koordinuje */
/* Stac - ukazatel na zasobnik */
void Analysis(StackPointer *Stac)
{
	InitExpressionStack(Stac);
	char *input[]={"i","+","(","i",")","#"};
	int i=0;
	PrintStack(Stac);
	while((strcmp(input[i],"#")!=0) || (strcmp(Stac->last_terminal->data,"$")!=0))/* kym este je nieco na input */
	{
		char next_step=DecideShiftOrReduce(Stac,input[i]);
		if(next_step=='R')
		{
			printf("Redukujeme:\n");
			if(Reduce(Stac)==-1)
			{
				printf("Redukcia neprabehla\n");
				return;
			}
				
		}
		else if(next_step=='S')
		{
			printf("Shiftujeme:\n");
			Shift(Stac,input[i]);
			i++; /* posuniem sa dalej v inpute len ked shitfujem */
		}
		else if(next_step=='T')
		{
			printf("pravidlo T:\n");
			ReduceT(Stac);
			i++; /* chem sa zbavit uzatvarajucej zatvorky na vstupe */
		}
		else
		{
			printf("Syntakticka chyba\n");
			return;
		}
		PrintStack(Stac);
	}
}
/* inicializacia zasobniku podla precednecnej syntaktickej analyzi tj. vlozeenie zaciatku zasobnika alias znak $ */
/* Stac - ukazatel na zasobnik */
void InitExpressionStack(StackPointer *Stac)
{
	InitStack(Stac);
	Stac->last_terminal=Push(Stac,"$"); /* zaciatok zasobnika a prvy terminal */
	Stac->last_terminal->terminal=true; /* set element as terminal */
}


/* funkcia podla posledneho terminalu na zasobniku vykona redukciu */
/* return - -1 error redukcie, 1 redukcia prebehla ok */
/* Stac - ukazatel na zasobnik */
int Reduce(StackPointer *Stac)
{
	int rule = ConvertCharToAccessPrecedenceTable(Stac->last_terminal->data);
	if(rule==OP) /* terminal je i*/
	{
			printf(" i -> D \n");
			Stac->last_terminal->data="D";
			Stac->last_terminal->terminal=false;
			FindLastTerminal(Stac); /* najdenie noveho terminalu */
			return 1;
	}
	else if((rule >=MUL) && (rule<=NE) ) /* terminal je operand */
	{
		Stack left_operand = Stac->last_terminal->prev;
		//Stack right_operand = Stac->last_terminal->next;
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
		Stac->last_terminal->terminal=false; /* pre istotu aj ked by ten operator mal za chvilu zmiznut */
		FindLastTerminal(Stac); /* najdenie noveho terminalu */
		Pop(Stac); /* odstanime operator a pravy operand */
		Pop(Stac);
		return 1;
	}
	else
		return -1;
}
void ReduceT(StackPointer *Stac)
{
	printf("(D) -> D\n");
	Stac->last_terminal->data=Stac->last_terminal->next->data;
	Pop(Stac); /* TREBA VYRIESIT uzatvaraciu zatvorku !!!! */
	Stac->last_terminal->terminal=false;
	FindLastTerminal(Stac);
}
/* shift vlastne iba pushne znak z input na zasobnik a nastavy novy last terminal */
/* Stac - ukazatel na zasobnik */
/* char_input - prvy znak z input */
void Shift(StackPointer *Stac,char * char_input)
{
	Stac->last_terminal=Push(Stac,char_input);
	Stac->last_terminal->terminal=true;
}

/* funkcia ma za ulohu rozhodnut ci sa ma redukovat alebo shiftovat a to podla posledneho terminalu na zasobniku a prveho znaku(tokenu)*/
/* Stac - ukazatel na zasobnik */
/* char_input - prvy retazec */
/* return - hodnota R(reduce), S(shift), T(uzatvorenie zatvoriek), X(chyba) */
char DecideShiftOrReduce(StackPointer *Stac,char* char_input)
{
	int stack_terminal = ConvertCharToAccessPrecedenceTable(Stac->last_terminal->data);
	int token_symbol = ConvertCharToAccessPrecedenceTable(char_input);
	return PRECEDENCE_TABLE[stack_terminal][token_symbol];
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

/* najde sa nasledujuci terminal na zasobniku a prenastavy sa */
/* Stac - ukazatel na zasobnik */
void FindLastTerminal(StackPointer *Stac)
{
 	Stack helpful_pointer = Stac->last_terminal; /* set pointer to last terminal */
	if(helpful_pointer->prev==NULL) /* posledny terminal je $ */
		return; /* nic nerob */
	else
	{
		helpful_pointer = Stac->last_terminal->prev;
		while((helpful_pointer!=NULL))
		{
			if(helpful_pointer->terminal==true)
			{
				Stac->last_terminal=helpful_pointer;
				return;/* nasli sme dalsi terminal */
			}
			else
				helpful_pointer=helpful_pointer->prev;

		}
	}
}
