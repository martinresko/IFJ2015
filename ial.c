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

int find (char *string, char *pattern);
int *KMPGraf(char *pattern, int pattern_length);

int *KMPGraf(char *pattern, int pattern_length)
{
	int k,r;
	int *pi = malloc(sizeof(int)*pattern_length);

	if(pi == NULL) {
		return NULL;	 
	}

	pi[1] = 0;

	for (k = 2; k < pattern_length; k++) {
		r = pi[k-1];
		while ((r > 0) && (pattern[r] != pattern[k-1])) {
			r = pi[r];
		}
		pi[k] = r+1;;
	}
	return pi;
}

int KMP(char *string, char *pattern)
{

	int tindex = 1;
	int pindex = 1;
	int string_length = strlen(string);
    int pattern_length = strlen(pattern);
    int *pi = KMPGraf(string, pattern_length);

    while ((tindex<=string_length) && (pindex <=pattern_length)) {
    	if ((pindex == 0) || (string[tindex] == pattern[pindex])) {
    		pindex++;
    		tindex++;
    	}
    	else {
    		pindex = pi[pindex];
    	}
    }

    if (pindex>pattern_length) {
    	return (tindex-pattern_length);
    }
    else {
    	return -1;
    }
}




int main() 
{
	printf("%d\n", KMP("kokotar", "ar"));

	return 0;
}