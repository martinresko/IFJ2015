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
int find(char *string, char *pattern);
int *KMPGraf(char *pattern, int pattern_length);
int length_of_string(char * string);
char* concatenation(char *string1, char *string2);


/**Vektor chyby**/
int *KMPGraf(char *pattern, int pattern_length) 
{
	int k,r;
	int *pi = malloc(sizeof(int) * pattern_length);

	if (pi == NULL) {
		return NULL;
	}

	pi[0] = 0;

	for (k = 1; k < pattern_length; k++) {
		r = pi[k - 1];

		while ((r > 0) && (pattern[r ] != pattern[k - 1])) {
			r = pi[r];
		}

		pi[k] = r + 1;
	}
	
	return pi;
}

/**Implementacia Knutth-Morris-Pratt algoritmu**/
int KMP(char *string, char *pattern)
{

	int tindex = 1;
	int pindex = 1;
	int string_length = strlen(string); // dlzka retazca
    int pattern_length = strlen(pattern); // dlzka podretazca
    int *pi = KMPGraf(string, pattern_length);

    while ((tindex <= string_length) && (pindex <= pattern_length)) {
    	if ((pindex == 0) || (string[tindex - 1] == pattern[pindex - 1])) { // vracia prvy vyskyt
    		pindex++;
    		tindex++;
    	}
    	else {
    		pindex = pi[pindex - 1];
    	}
    }

    free(pi);
    pi = NULL;
	
    if (pindex > pattern_length) {
    	return ((tindex - pattern_length) - 1);
    }
    else {
    	return -1;
    }
}

/**Implementacia algoritmu na zistovanie dlzky retazca**/
int length_of_string(char * string)
{
	int string_length = strlen(string); // dlzka retazca	
	return string_length;
}

/**Implementacia konkatenancie dvoch retazcov **/
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



int main() 
{
	char * str = concatenation("ahoj", "jano");
	printf("%s\n",str );
	free (str);
	printf("%d\n", length_of_string("x\nz"));
	printf("%d\n", KMP("cabcabcab", "ab"));
	printf("%d\n", KMP("cccabcdefab", "de"));
	printf("%d\n", KMP("cccabcdefab", "c"));
	printf("%d\n", KMP("cccabcdefab", "aasdasdasda"));
	printf("%d\n", KMP("cccabcdefab", "fab"));
	return 0;
}