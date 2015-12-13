/**
 * Predmet: IFJ / IAL
 * Subor:     ial.c
 *            Implementacia vstavanych funkcii a tabulky symbolov 
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
return pom;
}

////////////////////// TABLE OF SYMBOLS ///////////////////////////////
// tabulka symbolov zavisi nad stack.h list.h garbage.h
// stack.h - implementacia zasobnika
// list.h - implementacia obojsmerne viazaneho zoznamu

#define TRUE 1
#define FALSE 0
#define CONTINUE 2
#define DELETE 2
#define DEFINE 3

/* inicializuje globalnu tabulku symbolov
 * gts_table - ukazatel na strukturu GTS */
void globalTableOfSymbolsInit(Table_symbols *gts_table)
{
	if(gts_table!=NULL)
		treeInit(&(gts_table->functions));
		gts_table->actual_function=NULL;
}

/* vlozi do GTS funkciu 
 * return - intern ak nedostanem pamat alebo dobry ukazatel inak OK
 * gts_table - ukazatal na GTS strom
 * function_name - meno vkladanej funkcie
 * retrun_type - typ navratovej hodnoty vkladanej funkcie */
ERROR_CODE insertFunction(Table_symbols *gts_table,char *function_name,int return_type)
{
	if(gts_table!=NULL)
	{
		Function_GTS *func=memmalloc(sizeof(struct function_GTS));
		if(func!=NULL)
		{
			InitList(&(func->params));
			stackInit(&(func->symbol_table_of_block));
			InitList(&(func->instructions));
			func->defined=FALSE;
			func->return_occured=FALSE;
			func->return_type=return_type; 
			if( (treeInsert(&(gts_table->functions),function_name,func))==INTERN_ERR) /* do GTS stromu vlozim strukturu Function_GTS pod klucom meno danej funkcie */
				return INTERN_ERR;
			gts_table->actual_function=func; /* ulozim si vlozenu funkciu do aktualnej */
			return OK_ERR;
		}
		else
		{
			return INTERN_ERR;
		}
	}
	return INTERN_ERR;
}

/* vyhlada funkciu v GTS 
 * return - ukazatel na strukturu najdenej funkcie
 * gts_table - ukazatel na GTS strom
 * function_name - meno funkcie */
Function_GTS *searchFunction(Table_symbols *gts_table, char *function_name)
{
	if(gts_table!=NULL)
	{
		Tree helpful_pointer=NULL;
		helpful_pointer=treeSearch(&(gts_table->functions),function_name);
		return (helpful_pointer!=NULL)?(Function_GTS*)(helpful_pointer->data):NULL; /* ak som funkciu nasiel tak vratim ukazatel na Function_GTS strukturu ak som nenasiel tak vratim NULL */
	}
	return NULL;
}

/* vlozi do funkcie prameter danej funkcie 
 * return - Ok alebo interrn pri nedostani pamati alebo nedostani ukazatel na funkciu
 * function - ukazatel na funkciu
 * variable_name - id alebo meno premennej
 * variable_typ - typ vkladanej premennej */
ERROR_CODE insertFunctionParam(Function_GTS *function,char *variable_name,int variable_typ)
{
	if(function!=NULL)
	{
		Variable *param=memmalloc(sizeof(struct variable));
		if(param!=NULL)
		{
			param->name=variable_name;
			param->typ=variable_typ;
			InsertLast(&(function->params),param);
			return OK_ERR;
		}
		else
		{
			return INTERN_ERR;
		}
	}
	return INTERN_ERR;
}

/* najde v funkcii parameter danej funkcie
 * return - ukazatel na strukturu parametru
 * function - ukazatel na funkciu
 * variable_name - meno premennej ktoru hladame */
Variable *searchFunctionParam(Function_GTS *function,char *variable_name)
{
	if(function!=NULL)
	{
		Variable *helpful_pointer=NULL;
		helpful_pointer=findInList(&(function->params),variable_name);
		return (helpful_pointer!=NULL)?(Variable *)(helpful_pointer):NULL;
	}
	return NULL;
}

Variable *getFunctionParam(Function_GTS *function,int state)
{
	if(function!=NULL)
	{
		Variable *helpful_pointer=NULL;
		helpful_pointer=getFromList(&(function->params),state);
		return (helpful_pointer!=NULL)?(Variable *)(helpful_pointer):NULL;
	}
	return NULL;
}

/* vlozi na zasobnik novy blok 
 * return - intern ak nedostanem pamat alebo zly ukazatel inak OK 
 * function - ukazatel na funkciu */
ERROR_CODE pushBlock(Function_GTS *function)
{
	 if(function!=NULL)
	{
		TreePointer *zasobnik=memmalloc(sizeof(TreePointer*));
		if(zasobnik!=NULL)
		{
			treeInit(zasobnik);
			if((stackPush(&(function->symbol_table_of_block),zasobnik)==INTERN_ERR))
					return INTERN_ERR;
			return OK_ERR;
		}
		else
			return INTERN_ERR;
	}
	return INTERN_ERR;
}

/* odstrani blok z vrcholu zasobnika
 * function - ukazatel na funkciu */
void popBlock(Function_GTS *function)
{
	if(function!=NULL)
	{
		if(stackEmpty(&(function->symbol_table_of_block)))
		{
			;
		}
		else /* zasobnik prazdny nieje mozem popnut blok */
		{
			treeDestroy(stackTop(&(function->symbol_table_of_block)));
			stackPop(&(function->symbol_table_of_block));
		}
	}
}

/* funkcia vlozi na blok na vrchole zasobnika premennu
 * return - intern ak nedostanem pamat alebo dobry ukazatel inak OK
 * function - ukazatel na funkciu
 * variable_name - meno vkladanej premennej
 * variable_typ - typ vkladanej premennej */
ERROR_CODE insertFunctionVariableToStack(Function_GTS *function, char *variable_name,int variable_typ)
{
	
	if(function!=NULL)
	{
		Variable *var=memmalloc(sizeof(struct variable));
		if(var!=NULL)
		{
			var->typ=variable_typ;
			var->name=variable_name;
			if((treeInsert(stackTop(&(function->symbol_table_of_block)),variable_name,var)==INTERN_ERR))
					return INTERN_ERR;
			return OK_ERR;
		}
		else
			return INTERN_ERR;
	}
	return INTERN_ERR;
}

/* funkcia hlada v danej urovni bloku lokalnu premennu danej funkcie plus aj nahliada aj na parametre
 * return - NULL ak nic nenasiel alebo pointer na Parameter alebo Pointer na Variable
 * function - ukazatel na funkciu
 * variable_name - meno hladanej premennej */
Variable *searchFunctionVariableInActualLevel(Function_GTS *function, char *variable_name)
{
	if(function!=NULL)
	{
		Variable *found_param = findInList(&(function->params),variable_name);
		if(found_param!=NULL)
			return found_param;/* ak som nasiel parameter */
		
		/* ak som to nenasiel ako parameter tak hladam v aktualnom bloku */
		Tree helpful_pointer=NULL;
		helpful_pointer = treeSearch(stackTop(&(function->symbol_table_of_block)),variable_name);
		if(helpful_pointer!=NULL)
		{
			return (Variable *)(helpful_pointer->data);/* nasiel som */
		}
		else
		{
			return NULL; /* nasiel som */
		}
	}
	return NULL;
}


/* funkcia postupne od vrcholu zasabnika hlada ziadanu premennu az po lokalne premenne danej funkcie plus aj parametre 
 * return - NULL ak nic nenasiel alebo pointer na Parameter alebo Pointer na Variable
 * function - ukazatel na funkciu
 * variable_name - meno hladanej premennej */
Variable *searchFunctionVariableInStack(Function_GTS *function, char *variable_name)
{
	if(function!=NULL)
	{
		Variable *found_param = findInList(&(function->params),variable_name);
		if(found_param!=NULL)
			return found_param;/* ak som nasiel parameter */
		
		/* ak som to nenasiel ako parameter tak hladam v zasobniku blokov */
		Stack store_top_stack=function->symbol_table_of_block.top_of_stack; /* ulozim si vrchol originalny vrchol zasobnika */
		while(stackEmpty(&(function->symbol_table_of_block))==0) /* kym neprejdem cely zasobnik */
		{
			Tree helpful_pointer=NULL;
			helpful_pointer = treeSearch(stackTop(&(function->symbol_table_of_block)),variable_name);
			if(helpful_pointer!=NULL)
			{
				function->symbol_table_of_block.top_of_stack=store_top_stack; /* nastavim originaly vrchol */
				return (Variable *)(helpful_pointer->data); /* nasiel som */
			}
			else
			{
				function->symbol_table_of_block.top_of_stack=function->symbol_table_of_block.top_of_stack->left_stack_element;/* nenasiel som posuniem idem hladat hlbsie v zasobniku */
			}
		}
		function->symbol_table_of_block.top_of_stack=store_top_stack; /* prenastavim vrchol zasobnika ak nenajdem */
		return NULL;
	}
	return NULL;
}

/* odstrani strom z zasobniku a nasledne aj odstrani polozku zasobniku
 * function - ukazatel na funkciu */
void SymbolTableStackDestroy(Function_GTS *function)
{
	while(stackEmpty(&(function->symbol_table_of_block))==0) /* kym zasobnik nieje prazdny */
	{
		popBlock(function);
	}
}

/* nastavy funkciu v GTS ako pred inicializaciou vsetky struktury su zmazane a hodnoty su nastavene na inicializacne 
 * function - ukazatel na funkciu v GTS */
void functionInGlobalTableDestroy(Function_GTS *function)
{
	if(function!=NULL)
	{
		DestroyList(&(function->params));
		SymbolTableStackDestroy(function);	
		stackDestroy(&(function->symbol_table_of_block));
		DestroyList(&(function->instructions));
		function->defined=0;
		function->return_type=0;
	}
}

/* prejst cez cely GTS strom a pre kazdy uzol vestko uvolnit
 * tabulka - ukazatel na GTS */
ERROR_CODE traversalGTS(Table_symbols *tabulka,int what_to_do)
{
	if(tabulka!=NULL)
		return traversalTree(&(tabulka->functions),what_to_do);
	return INTERN_ERR;
}

ERROR_CODE traversalTree(TreePointer *Tre, int what_to_do)
{
	if(Tre->root!=NULL)
		return nodesTree(Tre->root,what_to_do);
	return INTERN_ERR;
}

ERROR_CODE nodesTree(Tree node,int what_to_do)
{
	if(node->left!=NULL)
	{
		if(nodesTree(node->left,what_to_do)==SEM_UNDEF_ERR)
		{
			return SEM_UNDEF_ERR;
		}
	}
	if(node->right!=NULL)
	{
		if(nodesTree(node->right,what_to_do)==SEM_UNDEF_ERR)
		{
			return SEM_UNDEF_ERR;
		}
	}
	if(what_to_do==DELETE)
	{
		functionInGlobalTableDestroy(node->data);
		return OK_ERR;
	}
	else /* DEFINE */
	{
		if( ((Function_GTS*)(node->data))->defined==FALSE )	 
			return SEM_UNDEF_ERR;
		else
			return OK_ERR; /* ak je definovana */
	}
	return INTERN_ERR;
}

/* funkcia prejde cely strom uvolni vseko v kazdom uzle a nasledne sa uvolni aj strom sam
 * tabulka - ukazatel na GTS(strom) */
void destroyGTS(Table_symbols *tabulka)
{
	if(tabulka!=NULL)
	{
		traversalGTS(tabulka,DELETE);
		treeDestroy(&(tabulka->functions));
		tabulka->actual_function=NULL;
	}
}

/* funkcie prejde skvecne cely zozname a hlada parameter podla mena 
 * return - NULL ak nenajde inak ukazatel na najdeny prvok
 * Lis 	  - ukazatel na zoznam
 * param_name - meno hladaneho parametru */
Variable *findInList(ListPointer *Lis,char *param_name)
{
	if(EmptyList(Lis))
	{
		return NULL; /* zoznam je prazdny == nenasiel som */
	}
	List helpful_pointer = Lis->first_list_element; /* nastavim pointer na prvy prvok */
	while(helpful_pointer!=NULL)
	{
		if((strcmp(((Variable *)(helpful_pointer->data))->name,param_name))==0)
		{
			return (Variable *)(helpful_pointer->data); /* nasiel som */
		}
		else
		{
			helpful_pointer=helpful_pointer->next; /* hladam dalej */
		}
	}
	return NULL; /* nenasiel som */
}

/* funkcia po kazdom zavolani vrati paramater 
 * return - vrati ukazatel na paramater
 * Lis 	  - ukazatel na zoznam parametrov  
 * state  - 0 reset, 1 read , 2 continue */
Variable *getFromList(ListPointer *Lis,int state)
{
	if(EmptyList(Lis))
	{
		return NULL; /* zoznam je prazdny nemam co vratit */
	}
	if(state==FALSE) /* reset */
	{
		Lis->last_terminal=Lis->first_list_element;
		return NULL;	
	}
	else if(state==TRUE) /* start */
	{
		Lis->last_terminal=Lis->first_list_element;
		return Lis->last_terminal->data;		
	}
	else /* continue */
	{
		if(Lis->last_terminal!=NULL)
		{
			if(Lis->last_terminal->next!=NULL)
			{
				Lis->last_terminal=Lis->last_terminal->next;
				return Lis->last_terminal->data;
			}
			else
			{
				return NULL;
			}
		}
	}
	return NULL;
}

/* funkcia prida instrukciu na instrukcnu pasku 
 * return - INTERN_ERR, OK_ERR
 * function - ukazatel na funkciu do ktorej sa bude instrukcia pridava
 * type 	- typ instrukcie
 * destination - prvy prarameter instrukcie
 * source1 - druhy parameter instrukcie
 * source2 - treti parameter instrukcie */
ERROR_CODE insertFunctionInstruction(Function_GTS *function, int type, InsVal *dest, InsVal *src1, InsVal *src2)
{
	if(function!=NULL)
	{
		Instruction *instruct=memmalloc(sizeof(struct instruction));
		if(instruct!=NULL)
		{
			instruct->type=type;
			if(dest!=NULL)
				instruct->des=*dest;
			if(src1!=NULL)
				instruct->src1=*src1;
			if(src2!=NULL)
				instruct->src2=*src2;

			InsertLast(&(function->instructions),instruct);
			return OK_ERR;
		}
		else
		{
			return INTERN_ERR;
		}
	}
	return INTERN_ERR;
}

/* funkcia prejde celu GTS a bude zistovat ci su vsetky funkcie v nej definovane ak nie je to chyba SEM_UNDEF_ERR, ak su vsekty funkcie definovane tak este skontoluje GTS ci sa v nej vyskytuje funkcia main, ktora je povinna inak error SEM_UNDEF_ERR
* return - OK_ERR, SEM_UNDEF_ERR
* tabulka - ukazatel na tabulku symbolov */
ERROR_CODE finalFunctionCheckout(Table_symbols * tabulka)
{
	if(traversalGTS(tabulka,DEFINE)==OK_ERR) /* ak su vsetky funkcie definovane */
	{
		Function_GTS *main_funkcia = searchFunction(tabulka,"main");
		if(main_funkcia!=NULL)
		{
			if((EmptyList(&main_funkcia->params)) && (main_funkcia->return_type==2)) /* skontrolujem ci main nema ziadne parametre a navtratovatova hodnota je int */
                 return OK_ERR;
             else
                 return SEM_UNDEF_ERR;
		}
	}
	return SEM_UNDEF_ERR; /* funkcie definovane niesu */
}

/////////////////// BINARNY STROM //////////////////////////////


/* vyhlada polozku pomocou kluca vo vsetkych podstromoch daneho uzlu
 * return - vrati polozku ktora bola hladana
 * node   - ukazatel na polozku v strome
 * key    - kluc podla ktoreho sa vyhladava */
Tree treeNodesSearch(Tree node,char *key)
{
	if(node!=NULL)	
	{
		int compare = strcmp(node->key,key);	/* porovnam hladany kluc s klucom v prvku */
		if(compare!=0)
		{
			if(compare>0)	
			{
				return treeNodesSearch(node->right,key);
			}
			else /* compare je mensie ako nula */
			{
				return treeNodesSearch(node->left,key);
			}
		}
		else /* compare == 0, cize su kluce rovnake */
		{
			return node; 
		}
	}
	else /* nic som nenasiel */
	{
		return NULL;
	}
	return NULL;
}
/* vymaze vsetky podstromy daneho uzlu 
 * node - ukazatel na polozku v strome */
void treeNodesDestroy(Tree node)
{		
	if(node->left!=NULL)
		treeNodesDestroy(node->left);
	if(node->right!=NULL)
		treeNodesDestroy(node->right);
	memfree(node->data); // POZOR znamena ze mozem do data ukladat len pointer na alokovanu pamat ak by som do dat dal pointer na int napriklad tak hodi chybu lebo nema byt aka pamat uvolnena */
	memfree(node);
}
/*funkcia inicializuje strom
 * Tre - ukazatel na strom */
void treeInit(TreePointer *Tre)
{
	Tre->root=NULL;
}

/* funkcia odstrani strom a uvolni vsetky jeho prvky 
 * Tre - ukazatel na strom */
void treeDestroy(TreePointer *Tre)
{
	if(Tre->root!=NULL)
	{
		treeNodesDestroy(Tre->root);
		Tre->root=NULL;
	}
}

/* funkcia najde polozku v strome podla zadaneho kluca v lavo su kluce mensie v pravo vacsie 
 * return - ukazatel na prislusnu polozku alebo NULL ak nic nenasiel
 * Tre 	  - ukazatel na strom
 * key 	  - kluc vyhladavania */
Tree treeSearch(TreePointer *Tre,char *key)
{
	if((Tre!=NULL) && (key!=NULL))
		return treeNodesSearch(Tre->root,key);
	return NULL;
}

/* funkcia vlozi do stromu novy polozku a ulozi ju na spravne miesto v strome 
 * Tre	- ukazatel na strom
 * key 	- kluc vkladaneho prvku
 * data - ukazatel na cokolvek co cheme uchovavat v prvku stromu */
ERROR_CODE treeInsert(TreePointer *Tre, char *key,void *data)
{
	if(Tre->root!=NULL) /* strom nieje prazdny */
	{
		int compare;
		Tree helpful_pointer=Tre->root;
		while(helpful_pointer!=NULL)
		{
			compare=strcmp(helpful_pointer->key,key);
			if(compare>0)
			{
				if(helpful_pointer->right==NULL) /* je to list */
				{
					helpful_pointer->right=memmalloc(sizeof(struct tree));
					if(helpful_pointer->right!=NULL)
					{
						helpful_pointer->right->left=NULL;
						helpful_pointer->right->right=NULL;
						helpful_pointer->right->data=data;
						helpful_pointer->right->key=key;
					}
					else
						return INTERN_ERR;
				}
				else /* list to nieje tak musim sa posunut dalej */
					helpful_pointer=helpful_pointer->right;
			}
			else if(compare<0)
			{
				if(helpful_pointer->left==NULL) /* je to list */
				{
					helpful_pointer->left=memmalloc(sizeof(struct tree));
					if(helpful_pointer->left!=NULL)
					{
						helpful_pointer->left->left=NULL;
						helpful_pointer->left->right=NULL;
						helpful_pointer->left->data=data;
						helpful_pointer->left->key=key;
					}
					else
						return INTERN_ERR;
				}
				else /* list to nieje tak musim sa posunut dalej */
					helpful_pointer=helpful_pointer->left;
			}
			else /*compare == 0 */
			{
				helpful_pointer->data=data; /* aktualizujem data */
				return OK_ERR;/*nasiel som co som hladal*/
			}
		}
	}
	else /* strom je prazdny tj. vkladam koren */
	{
		Tre->root=memmalloc(sizeof(struct tree));	
		if(Tre->root!=NULL)
		{
			Tre->root->left=NULL;
			Tre->root->right=NULL;
			Tre->root->data=data;
			Tre->root->key=key;
		}
		else
			return INTERN_ERR;
	}
	return OK_ERR;
}
