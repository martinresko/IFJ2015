#include"table_of_symbols.h"

int main()
{
	Table_symbols tabulka;
	globalTableOfSymbolsInit(&tabulka);
	// funkcia
	insertFunction(&tabulka,"funkcia1",1);
	Function_GTS *a=NULL;
	a=searchFunction(&tabulka,"funkcia1");
	if(a!=NULL)
		//printf("%d\n",a->return_type);
		printf("%d\n",a->return_type);
	else
		printf("nenasiel som \n");
	
	// paramater do funkcie
	insertFunctionParam(a,"param1",2);
	Variable *b=NULL;
	b=searchFunctionParam(a,"param1");
	if(b!=NULL)
		printf("%d\n",b->typ);
	else
		printf("!!!!!!nenasiel som\n");
	//premenna prvy blok
	pushBlock(a);
	insertFunctionVariableToStack(a,"premenna1",9);
	Variable *c=NULL;
	c=searchFunctionVariableInStack(a,"premenna1");
	if(c!=NULL)
		printf("%d\n",c->typ);
	else
		printf("!!!!!!!!!!nenasiel som \n");

	pushBlock(a);
	insertFunctionVariableToStack(a,"premenna2",98);
	Variable *d=NULL;
	d=searchFunctionVariableInStack(a,"premenna1");
	if(d!=NULL)
		printf("%d\n",d->typ);
	else
		printf("!!!!!!!!!!nenasiel som \n");

	//popBlock(a);
	destroyGTS(&tabulka); 
	return 0;
}
/* inicializuje globalnu tabulku symbolov
 * gts_table - ukazatel na strukturu GTS */
void globalTableOfSymbolsInit(Table_symbols *gts_table)
{
	if(gts_table!=NULL)
		treeInit(&(gts_table->functions));
}

/* vlozi do GTS funkciu 
 * gts_table - ukazatal na GTS strom
 * function_name - meno vkladanej funkcie
 * retrun_type - typ navratovej hodnoty vkladanej funkcie */
void insertFunction(Table_symbols *gts_table,char *function_name,int return_type)
{
	Function_GTS *func=malloc(sizeof(struct function_GTS));
	if(func!=NULL)
		{
			treeInit(&(func->params));
			stackInit(&(func->symbol_table_of_block));
			func->pointer_to_instructions=NULL;
			func->defined=0; /* ZMENIT Z MAGIC NUMBER */ 
			func->return_type=return_type; 

			treeInsert(&(gts_table->functions),function_name,func); /* do GTS stromu vlozim strukturu Function_GTS pod klucom meno danej funkcie */
		}
	else
		printf("nedostal som pamat pre funkciu do GTS\n");
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
		return (helpful_pointer!=NULL)?(Function_GTS*)(helpful_pointer->data):NULL; /* ak som funkciu nasiel tak vravim ukazatel na Function_GTS strukturu ak som nenasiel tak vravim NULL */
	}
	return NULL;
}

/* vlozi do funkcie prameter danej funkcie 
 * function - ukazatel na funkciu
 * variable_name - id alebo meno premennej
 * variable_typ - typ vkladanej premennej */
void insertFunctionParam(Function_GTS *function,char *variable_name,int variable_typ)
{
	if(function!=NULL)
	{
		Variable *var=malloc(sizeof(struct variable));
		var->typ=variable_typ;
		treeInsert(&(function->params),variable_name,var);
	}
}

/* najde v funkcii parameter danej funkcie
 * return - ukazatel na strukturu parametru
 * function - ukazatel na funkciu
 * variable_name - meno premennej ktoru hladame */
Variable *searchFunctionParam(Function_GTS *function,char *variable_name)
{
	if(function!=NULL)
	{
		Tree helpful_pointer=NULL;
		helpful_pointer = treeSearch(&(function->params),variable_name);
		return (helpful_pointer!=NULL)?(Variable *)(helpful_pointer->data):NULL;
	}
	return NULL;
}

/* vlozi na zasobnik novy blok 
 * function - ukazatel na funkciu */
void pushBlock(Function_GTS *function)
{
	if(function!=NULL)
	{
		TreePointer *zasobnik=malloc(sizeof(TreePointer*));
		treeInit(zasobnik);
		stackPush(&(function->symbol_table_of_block),zasobnik);
	}
}

/* odstrani blok z vrcholu zasobnika
 * function - ukazatel na funkciu */
void popBlock(Function_GTS *function)
{
	if(function!=NULL)
	{
		if(stackEmpty(&(function->symbol_table_of_block)))
		{
			printf("chces popnut prazdny zasobnik \n");
		}
		else /* zasobnik prazdny nieje mozem popnut blok */
		{
			treeDestroy(stackTop(&(function->symbol_table_of_block)));
			stackPop(&(function->symbol_table_of_block));
		}
	}
}

/* funkcia vlozi na blok na vrchole zasobnika premennu
 * function - ukazatel na funkciu
 * variable_name - meno vkladanej premennej
 * variable_typ - typ vkladanej premennej */
void insertFunctionVariableToStack(Function_GTS *function, char *variable_name,int variable_typ)
{
	if(function!=NULL)
	{
		Variable *var=malloc(sizeof(struct variable));
		if(var!=NULL)
		{
			var->typ=variable_typ;
			treeInsert(stackTop(&(function->symbol_table_of_block)),variable_name,var);
		}
		else
			printf("nedostal v zasobniku pre ukazatel na BVS \n");
	}

}

/* funkcia postupne od vrcholu zasabnika hlada ziadanu premennu az po lokalne premenne danej funkcie 
 * function - ukazatel na funkciu
 * variable_name - meno hladanej premennej */
Variable *searchFunctionVariableInStack(Function_GTS *function, char *variable_name)
{
	if(function!=NULL)
	{
		//Tree helpful_pointer=NULL;
		//helpful_pointer = treeSearch(stackTop(&(function->symbol_table_of_block)),variable_name);
		//return (helpful_pointer!=NULL)?(Variable *)(helpful_pointer->data):NULL;
		
		StackPointer *store_stack=&(function->symbol_table_of_block); /* ulozim si stav zasobnika */
		Stack store_top_stack=NULL; /* premenna pre vrchol zasobnika */
		while(stackEmpty(store_stack)==0) /* kym neprejdem cely zasobnik */
		{
			Tree helpful_pointer=NULL;
			store_top_stack=store_stack->top_of_stack->left_stack_element; /* uchovam si za vrcholom */

			helpful_pointer = treeSearch(stackTop(store_stack),variable_name);
			if(helpful_pointer!=NULL)
				return (Variable *)(helpful_pointer->data);
			else
				store_stack->top_of_stack=store_top_stack;/* kedze som nenasiel v aktualnom vrchole tak musim ist hladat nizsie do zasobnik takze nastavym novy vrchol */
		}
		return NULL;
	}
	return NULL;
}

//OPRAVIT!!!!!!!

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
		treeDestroy(&(function->params));
		SymbolTableStackDestroy(function);	
		stackDestroy(&(function->symbol_table_of_block));
		function->pointer_to_instructions=NULL;
		function->defined=0;
		function->return_type=0;
	}
}

/* prejst cez cely GTS strom a pre kazdy uzol vestko uvolnit
 * tabulka - ukazatel na GTS */
void traversalGTS(Table_symbols *tabulka)
{
	if(tabulka!=NULL)
		traversalTree(&(tabulka->functions));
}

void traversalTree(TreePointer *Tre)
{
	if(Tre->root!=NULL)
		nodesTree(Tre->root);
}

void nodesTree(Tree node)
{
	if(node->left!=NULL)
		nodesTree(node->left);
	if(node->right!=NULL)
		nodesTree(node->right);
	functionInGlobalTableDestroy(node->data);
}

/* funkcia prejde cely strom uvolni vseko v kazdom uzle a nasledne sa uvolni aj strom sam
 * tabulka - ukazatel na GTS(strom) */
void destroyGTS(Table_symbols *tabulka)
{
	if(tabulka!=NULL)
	{
		traversalGTS(tabulka);
		treeDestroy(&(tabulka->functions));
	}
}
