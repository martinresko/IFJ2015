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

#include "ial.h"

//////////////////////////**Vektor chyby**//////////////////////////////////////

int *KMP_FAIL_INDEX(char *pattern, int pattern_length) 
{
	int k,r;
	int *fail = memmalloc(sizeof(int) * pattern_length);

	
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

    memfree(fail);
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
	char *result = memmalloc(strlen(string1) + strlen(string2) + 1);


	strcpy(result, string1);
    strcat(result, string2);

    return result;
} 

////////////////////**Implementacie funkcie substring**/////////////////////////

char* substring(char *string, int i, int n)
{	
	
	int string_len = strlen(string); // dlzka retazca

	char *result = memmalloc(strlen(string));

	if ((i >= string_len) || ((i + n) >= string_len) || (n >= string_len) || (i < 0)){
		memfree(result); 
		return NULL;
		}

	strncpy(result,string+i,n);
return result;
// opravit
}

//////////////////////////////**ShellSort**/////////////////////////////////////
/*
char* ShellSort(char *string)
{
int i,j,step;
int string_length = strlen(string); // dlzka retazca
int *result = memmalloc(sizeof(char) * string_length);
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
*/

///////////////////////**MY SHELLSORT**///////////////////////////////////
/*
void shellsort(char *string){

int i,gap;
int j = 0;
int string_length = strlen(string);
char pom;
gap = string_length / 2;

printf("%d\n",string_length);
printf("%d\n",gap);


while ( gap > 0 ) {

	for( i = 0; i < string_length - gap ; i++) {
		j= i + gap;
		pom = string[j];
		printf("pred while\n");
		while (j >= gap && string[j] > string[j-gap]) {
			string[j] = string[j-gap];
			printf("po while");
			j = j-gap;
		}
		string[j] = pom;
	}
	if ( gap == 2) {
		gap = 1;
	}
	else {
		gap = gap / 2;
	}
 	}
//return string;
}*/

char* shell(char *items)  
  {  
  
  char* pom = memmalloc(sizeof(strlen(items)+1));
  strcpy(pom,items);

     int count = strlen(items);
    register int i, j, gap;  
    char x;

  
    for (gap = count/2; gap > 0; gap = gap/2) {
     
      for(i=gap; i < count; ++i) {  
        x = pom[i];  
        for(j=i-gap; (x < pom[j]) && (j >= 0); j=j-gap) 
          pom[j+gap] = pom[j];  
        pom[j+gap] = x;  
      }  
    }  
printf("string je %s\n",items);
return pom;
}  


/*void strsort (char *array)
{
      int string_length = strlen(array);	
      int gap, i, j;
      char *a, *b, tmp;
 
      for (gap = 0; ++gap < string_length ; )
            gap *= 2;
      while (gap /= 2)
      {
            for (i = gap; i < string_length ; i++)
            {
                  for (j = i - gap; ;j -= gap)
                  {
                        a = array + j;
                        b = a + gap;
                        if (strcmp(*a, *b) <= 0)
                              break;
                        tmp = *a;
                        *a = *b;
                        *b = tmp;
                        if (j < gap)
                              break;
                  }
            }
      }
printf("%s\n",array);
}*/
/*
int main() 
{
	meminitialization();
	char* a;
	char* tt = substring("hatatitla",2,5); //neriesi free...
	printf("%s\n",tt );
	if (tt != "")
		memfree(tt);

	char* s = "98761";
	printf("string je %s\n",s);	
	char* vysl;
	vysl = shell(s);
	//shellsort(s);
	printf("string je %s\n",vysl);

	
	//printf("%s\n",shellsort("987654321"));
	char * str = concatenation("ahoj", "jano");
	printf("%s\n",str );
	memfree (str);

	printf("%d\n", length_of_string("xhos"));
	printf("%d\n", KMP_Find("cabcabcab", "ab"));
	printf("%d\n", KMP_Find("cccabcdefab", "de"));
	printf("%d\n", KMP_Find("cccabcdefab", "c"));
	printf("%d\n", KMP_Find("cccabcdefab", "aasdasdasda"));
	printf("%d\n", KMP_Find("cccabcdefab", "fab")); 
	memallfree();
	return 0;
}
*/
