#ifndef EXPRESION_INCLUDED
#define EXPRESION_INCLUDED

#include <string.h>

#include "list.h"
#include "scaner.h"

#define SIZE 14

enum LEX_SYMBOLS
{
	START,OPERAND,END,UNKNOWN
};
const char PRECEDENCE_TABLE[SIZE][SIZE] =
{
	/*				  		   *   /   +   -   <   >  <=  >=  ==  !=   (   )   OPERAND   $  */
	[sMult]			={[sMult]='R','R','R','R','R','R','R','R','R','R','S','R','S',[END]='R'},
	[sDivide] 		={[sMult]='R','R','R','R','R','R','R','R','R','R','S','R','S',[END]='R'},
	[sPlus]  		={[sMult]='S','S','R','R','R','R','R','R','R','R','S','R','S',[END]='R'},
	[sMinus] 		={[sMult]='S','S','R','R','R','R','R','R','R','R','S','R','S',[END]='R'},
	[sLess]    		={[sMult]='S','S','S','S','R','R','R','R','R','R','S','R','S',[END]='R'},
	[sGreater]    	={[sMult]='S','S','S','S','R','R','R','R','R','R','S','R','S',[END]='R'},
	[sLeorEq]    	={[sMult]='S','S','S','S','R','R','R','R','R','R','S','R','S',[END]='R'},
	[sGrorEq]    	={[sMult]='S','S','S','S','R','R','R','R','R','R','S','R','S',[END]='R'},
	[sEqual]    	={[sMult]='S','S','S','S','R','R','R','R','R','R','S','R','S',[END]='R'},
	[sNotEq]    	={[sMult]='S','S','S','S','R','R','R','R','R','R','S','R','S',[END]='R'},
	[sLParenth]  	={[sMult]='S','S','S','S','S','S','S','S','S','S','S','T','S',[END]='X'},
	[sRParenth] 	={[sMult]='R','R','R','R','R','R','R','R','R','R','X','R','X',[END]='R'},
	[OPERAND]  		={[sMult]='R','R','R','R','R','R','R','R','R','R','X','R','X',[END]='R'},
	[START] 	 	={[sMult]='S','S','S','S','S','S','S','S','S','S','S','P','S',[END]='X'}
};

/* struktura prvku ktory sa bude vkladat do zoznamu ako teminal/neterminal */
typedef struct precedence_table_element {
	bool terminal;
	int expresion_id;
	int id;
	char *attribute;
} Precedence_table_element;

/* funkcie precedencnej syntaktickej analyzi */
void InitExpressionList(ListPointer *);
int Reduce(ListPointer *);
void ReduceT(ListPointer *);
void Shift(ListPointer *,int,char *);
int ConvertCharToAccessPrecedenceTable(char *);
char DecideShiftOrReduce(ListPointer *,int);
void FindLastTerminal(ListPointer *);
int Analysis(ListPointer *);
List insertElement(ListPointer *,int,char *);
int expressionIdChose(int);
#endif
