#ifndef EXPRESION_INCLUDED
#define EXPRESION_INCLUDED

#include <string.h>

#include "list.h"
#include "scaner.h"
#include "error.h"

#define SIZE 14

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

extern const char PRECEDENCE_TABLE[SIZE][SIZE];

/* struktura prvku ktory sa bude vkladat do zoznamu ako teminal/neterminal */
typedef struct precedence_table_element {
	bool terminal;
	int expresion_id;
	int id;
	char *attribute;
} Precedence_table_element;

/* funkcie precedencnej syntaktickej analyzi */
ERROR_CODE expression(int,int);
ERROR_CODE InitExpressionList(ListPointer *);
ERROR_CODE  Reduce(ListPointer *);
void ReduceT(ListPointer *);
ERROR_CODE Shift(ListPointer *,int,char *);
int ConvertCharToAccessPrecedenceTable(char *);
char DecideShiftOrReduce(ListPointer *,int);
void FindLastTerminal(ListPointer *);
ERROR_CODE Analysis(ListPointer *,int,int);
List insertElement(ListPointer *,int,char *);
int expressionIdChose(int);
int convertIdToTableAccess(int);
int changeTypeOf(int,int,int);

#endif
