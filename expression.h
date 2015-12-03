#ifndef EXPRESION_INCLUDED
#define EXPRESION_INCLUDED

#include <string.h>

#include "list.h"
#include "scaner.h"
#include "error.h"

#define SIZE 14

extern int AKO_SA_MAS;
enum 
{
	MUL,
	DIV,
	PLUS,
	MINUS,
	LT,
	GT,
	LE,
	GE,
	EQ,
	NE,
	LEFT,
	RIGHT,
	OPERAND,
	DOLLAR,
	UNKNOWN
};
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

/* struktura prvku ktory sa bude vkladat do zoznamu ako teminal/neterminal */
typedef struct precedence_table_element {
	bool terminal;
	int expresion_id;
	int id;
	char *attribute;
} Precedence_table_element;

/* funkcie precedencnej syntaktickej analyzi */
ERROR_CODE expression();
ERROR_CODE InitExpressionList(ListPointer *);
ERROR_CODE  Reduce(ListPointer *);
void ReduceT(ListPointer *);
ERROR_CODE Shift(ListPointer *,int,char *);
int ConvertCharToAccessPrecedenceTable(char *);
char DecideShiftOrReduce(ListPointer *,int);
void FindLastTerminal(ListPointer *);
ERROR_CODE Analysis(ListPointer *);
List insertElement(ListPointer *,int,char *);
int expressionIdChose(int);
int convertIdToTableAccess(int);
#endif
