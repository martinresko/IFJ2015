#ifndef IAL_INCLUDED
#define IAL_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "garbage.h"
#include "error.h"


/**Prototypy funkcii**/
int KMP_Find(char *string, char *pattern);
int length_of_string(char * string);
char* concatenation(char *string1, char *string2);
char* shell(char *items);
char* substring(char *string, int i, int n);

#endif
