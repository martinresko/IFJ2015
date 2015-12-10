#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.c"
#include "garbage.h"


/**Prototypy funkcii**/
int KMP_Find(char *string, char *pattern);
int length_of_string(char * string);
char* concatenation(char *string1, char *string2);
void shell(char *items, int count);
char* substring(char *string, int i, int n);

