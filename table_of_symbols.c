/**
 * Predmet: IFJ / IAL
 * Subor:     table_of_symbols.c
 *            Implementacia tabulky symbolov
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
**/

#include "table_of_symbols.h"

#define TRUE 1
#define FALSE 0
#define CONTINUE 2

#define DELETE 2
#define DEFINE 3

//int main()
//{
//	meminitialization();
//	Table_symbols tabulka;
//	globalTableOfSymbolsInit(&tabulka);
//	// funkcia
//	insertFunction(&tabulka,"main",1);
//	Function_GTS *a=NULL;
//	a=searchFunction(&tabulka,"main");
//	a->defined=TRUE;
//	if(a!=NULL)
//		printf("%d\n",a->return_type);
//	else
//		printf("nenasiel som \n");
//	insertFunctionInstruction(tabulka.actual_function,1,NULL,NULL,NULL);
//	insertFunctionParam(tabulka.actual_function,"param1",1);
//	insertFunctionParam(tabulka.actual_function,"param2",2);
//	getFunctionParam(tabulka.actual_function,TRUE);
//	getFunctionParam(tabulka.actual_function,CONTINUE);
//	Variable *par3=getFunctionParam(tabulka.actual_function,CONTINUE);
//	if(par3!=NULL)
//		printf("prvy parameter name: %s typ: %d \n",par3->name,par3->typ);
//	
//	insertFunction(&tabulka,"funkcia",1);
//	Function_GTS *b=NULL;
//	b=searchFunction(&tabulka,"funkcia");
//	b->defined=TRUE;
//	if(b!=NULL)
//		printf("%d\n",b->return_type);
//	else
//		printf("nenasiel som \n");
//
//	// parameter do funkcie
//	insertFunctionParam(tabulka.actual_function,"param1",2);
//	Variable *b=NULL;
//	b=searchFunctionParam(a,"param1");
//	if(b!=NULL)
//		printf("%d\n",b->typ);
//	else
//		printf("!!!!!!nenasiel som parameter \n");
//	//premenna prvy blok
//	pushBlock(a);
//	insertFunctionVariableToStack(tabulka.actual_function,"premenna1",9);
//	Variable *c=NULL;
//	c=searchFunctionVariableInStack(a,"param1");
//	if(c!=NULL)
//		printf("%d\n",c->typ);
//	else
//		printf("!!!!!!!!!!nenasiel som \n");
//	// druhy blok
//	pushBlock(a);
//	insertFunctionVariableToStack(a,"premenna2",98);
//	Variable *d=NULL;
//	d=searchFunctionVariableInStack(a,"premenna1");
//	if(d!=NULL)
//		printf("%d\n",d->typ);
//	else
//		printf("!!!!!!!!!!nenasiel som \n");
//	// druha funkcia 
//	insertFunction(&tabulka,"funkcia2",1);
//	Function_GTS *e=NULL;
//	e=searchFunction(&tabulka,"funkcia2");
//	if(e!=NULL)
//		//printf("%d\n",a->return_type);
//		printf("%d\n",e->return_type);
//	else
//		printf("nenasiel som \n");
//	
//	// parameter do funkcie
//	insertFunctionParam(tabulka.actual_function,"param1",2);
//	Variable *f=NULL;
//	f=searchFunctionParam(e,"param1");
//	if(f!=NULL)
//		printf("%d\n",f->typ);
//	else
//		printf("!!!!!!nenasiel som parameter \n");
//	//premenna prvy blok
//	pushBlock(e);
//	insertFunctionVariableToStack(tabulka.actual_function,"premenna1",9);
//	Variable *g=NULL;
//	g=searchFunctionVariableInStack(e,"param1");
//	if(g!=NULL)
//		printf("%d\n",g->typ);
//	else
//		printf("!!!!!!!!!!nenasiel som \n");
//	// druhy blok
//	pushBlock(e);
//	insertFunctionVariableToStack(e,"premenna2",98);
//	Variable *h=NULL;
//	h=searchFunctionVariableInStack(e,"premenna1");
//	if(h!=NULL)
//		printf("%d\n",h->typ);
//	else
//		printf("!!!!!!!!!!nenasiel som \n");
//
//	if(finalFunctionCheckout(&tabulka)!=OK_ERR)
//		printf(" final function ERROR \n");
//	destroyGTS(&tabulka); 
//	memallfree();
//	return 0;
//}

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
			printf("PRIDAL SOM FUNKCIU\n"); 
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
		printf("HLADAM FUNKCIU\n"); 
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
			printf("PRIDAVAM PARAMETER\n"); 
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
		printf("HLADAM PARAMETER\n"); 
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
		printf("VRACIAM PARAMETER\n"); 
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
			printf("PUSHOL SOM OK\n");
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
			//printf("chces popnut prazdny zasobnik \n");
		}
		else /* zasobnik prazdny nieje mozem popnut blok */
		{
			treeDestroy(stackTop(&(function->symbol_table_of_block)));
			stackPop(&(function->symbol_table_of_block));
			printf("POPOL SOM OK\n");
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
			printf("VKLADAM PREMENNU MENOM %s \n",variable_name); 
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
	printf("CHECEM NAJT PREMENNU V AKTUALNOM BLOKU MENOM %s\n",variable_name );
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
	printf("CHECEM NAJT PREMENNU V STACK MENOM %s\n",variable_name );
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
				printf("NASIEL SOM PREMENNU\n"); 
				return (Variable *)(helpful_pointer->data); /* nasiel som */
			}
			else
			{
				function->symbol_table_of_block.top_of_stack=function->symbol_table_of_block.top_of_stack->left_stack_element;/* nenasiel som posuniem idem hladat hlbsie v zasobniku */
			}
		}
		function->symbol_table_of_block.top_of_stack=store_top_stack; /* prenastavim vrchol zasobnika ak nenajdem */
		printf("NENASIEL SOM PREMENNU\n"); 
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
		printf("zoznam parametrov je prazdny \n");
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
				printf("ziaden dalsi parameter nieje vraciam NULL \n");
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
ERROR_CODE insertFunctionInstruction(Function_GTS *function, int type, void *destination, void *source1, void *source2)
{
	if(function!=NULL)
	{
		Instruction *instruct=memmalloc(sizeof(struct instruction));
		if(instruct!=NULL)
		{
			instruct->type=type;
			instruct->destination=destination;
			instruct->source1=source1;
			instruct->source2=source2;
			InsertLast(&(function->instructions),instruct);
			printf("PRIDAVAM INSTRUKCIU\n"); 
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

