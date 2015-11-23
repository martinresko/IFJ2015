#include <string.h>

#include "list.h"
#include "list.c"

#define SIZE 14

enum LEX_SYMBOLS
{
	MUL,DIV,PLUS,MINUS,LT,GT,LE,GE,EQ,NE,LEFT,RIGHT,OP,EOL
};
const char PRECEDENCE_TABLE[SIZE][SIZE] =
{
	/*				 *   /   +   -   <   >  <=  >=  ==  !=   (   )   OP        $  */
	[MUL]	={[MUL]='R','R','R','R','R','R','R','R','R','R','S','R','S',[EOL]='R'},
	[DIV]   ={[MUL]='R','R','R','R','R','R','R','R','R','R','S','R','S',[EOL]='R'},
	[PLUS]  ={[MUL]='S','S','R','R','R','R','R','R','R','R','S','R','S',[EOL]='R'},
	[MINUS] ={[MUL]='S','S','R','R','R','R','R','R','R','R','S','R','S',[EOL]='R'},
	[LT]    ={[MUL]='S','S','S','S','R','R','R','R','R','R','S','R','S',[EOL]='R'},
	[GT]    ={[MUL]='S','S','S','S','R','R','R','R','R','R','S','R','S',[EOL]='R'},
	[LE]    ={[MUL]='S','S','S','S','R','R','R','R','R','R','S','R','S',[EOL]='R'},
	[GE]    ={[MUL]='S','S','S','S','R','R','R','R','R','R','S','R','S',[EOL]='R'},
	[EQ]    ={[MUL]='S','S','S','S','R','R','R','R','R','R','S','R','S',[EOL]='R'},
	[NE]    ={[MUL]='S','S','S','S','R','R','R','R','R','R','S','R','S',[EOL]='R'},
	[LEFT]  ={[MUL]='S','S','S','S','S','S','S','S','S','S','S','T','S',[EOL]='X'},
	[RIGHT] ={[MUL]='R','R','R','R','R','R','R','R','R','R','X','R','X',[EOL]='R'},
	[OP]    ={[MUL]='R','R','R','R','R','R','R','R','R','R','X','R','X',[EOL]='R'},
	[EOL]   ={[MUL]='S','S','S','S','S','S','S','S','S','S','S','X','S',[EOL]='X'}
};

/* funkcie precedencnej syntaktickej analyzi */
void InitExpressionList(ListPointer *);
int Reduce(ListPointer *);
void ReduceT(ListPointer *);
void Shift(ListPointer *,char *);
int ConvertCharToAccessPrecedenceTable(char *);
char DecideShiftOrReduce(ListPointer *,char *);
void FindLastTerminal(ListPointer *);
void Analysis(ListPointer *);
