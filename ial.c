/**
 * Predmet: IFJ / IAL
 * Subor:     ial.c
 *            Implementacia vstavaných funkcií
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
 */
 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**Prototypy funkcii**/
int KMP_Find(char *string, char *pattern);
int *KMP_FAIL_INDEX(char *pattern, int pattern_length);
int length_of_string(char * string);
char* concatenation(char *string1, char *string2);


//////////////////////////**Vektor chyby**//////////////////////////////////////

int *KMP_FAIL_INDEX(char *pattern, int pattern_length) 
{
	int k,r;
	int *fail = malloc(sizeof(int) * pattern_length);

	if (fail == NULL) {
		return NULL;
	}

	fail[0] = 0;

	for (k = 1; k < pattern_length; k++) {
		r = fail[k - 1];

		while ((r > 0) && (pattern[r ] != pattern[k - 1])) {
			r = fail[r];
		}

		fail[k] = r + 1;
	}
	
	return fail;
}

///////////////**Implementacia Knutth-Morris-Pratt algoritmu**//////////////////

int KMP_Find(char *string, char *pattern)
{

	int tindex = 1;
	int pindex = 1;
	int string_length = strlen(string); // dlzka retazca
    int pattern_length = strlen(pattern); // dlzka podretazca
    int *fail = KMP_FAIL_INDEX(string, pattern_length);

    while ((tindex <= string_length) && (pindex <= pattern_length)) {
    	if ((pindex == 0) || (string[tindex - 1] == pattern[pindex - 1])) { // vracia prvy vyskyt
    		pindex++;
    		tindex++;
    	}
    	else {
    		pindex = fail[pindex - 1];
    	}
    }

    free(fail);
    fail = NULL;
	
    if (pindex > pattern_length) {
    	return ((tindex - pattern_length) - 1);
    }
    else {
    	return -1;
    }
}

/////////////**Implementacia algoritmu na zistovanie dlzky retazca**////////////

int length_of_string(char * string)
{
	int string_length = strlen(string); // dlzka retazca	
	return string_length;
}

//////////////**Implementacia konkatenancie dvoch retazcov **///////////////////

char* concatenation(char *string1, char *string2)
{
	char *result = malloc(strlen(string1) + strlen(string2) + 1);

	if (result == NULL) {
		return NULL;
	}

	strcpy(result, string1);
    strcat(result, string2);

    return result;
} 

////////////////////**Implementacie funkcie substring**/////////////////////////
/*
char* substring(char *string, int i, int n)
{	
	int string_length = strlen(string); // dlzka retazca
	//char *result = malloc(strlen(string));
	char pole[string_length];
	char pole2[string_length];
	int j;

	if ((i > string_length) || (n + i >string_length)) {
		return NULL;
	}

	for (j = i ; j < (i + n); j++) {
		result[j] = string[j];
	}
printf("%s\n",result );
return result;
// opravit
}
*/
//////////////////////////////**ShellSort**/////////////////////////////////////

char* ShellSort(char *string)
{
int i,j,step,gap;
int string_length = strlen(string); // dlzka retazca

step = string_length / 2;

while (step > 0) {

   for (i = 0; i < string_length - step  ; i++) {
       j =i+step;
       char tmp = string[j];
       while ((j >= step) && (string[j] > string[j - step])){
           string[j] = string[j - step];
           j -=  step;
       }
        string[j] = tmp;
    }
    if (gap == 2) { //zmena velikosti mezery
        gap = 1;
    } 
    else {
        gap = gap / 2;
    }
}
return string;
}// opravit



int main() 
{
	//printf("%s\n",substring("123456",2,2));
	printf("%s\n",ShellSort("987654321"));

	char * str = concatenation("ahoj", "jano");
	printf("%s\n",str );
	free (str);

	printf("%d\n", length_of_string("x\nz"));
	printf("%d\n", KMP_Find("cabcabcab", "ab"));
	printf("%d\n", KMP_Find("cccabcdefab", "de"));
	printf("%d\n", KMP_Find("cccabcdefab", "c"));
	printf("%d\n", KMP_Find("cccabcdefab", "aasdasdasda"));
	printf("%d\n", KMP_Find("cccabcdefab", "fab")); 
	return 0;
}