/**
 * Predmet: IFJ / IAL
 * Subor:     frames.c
 *            Implementacia ramcov premennych 
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
**/
#include "frames.h"

#define TRUE 1
#define FALSE 0

//int main()
//{
//	meminitialization();
//
//	char *menoInstrukcie = "GOTO";
//	List dalsiaInstrukcia = memmalloc(sizeof(struct list));
//	dalsiaInstrukcia->data = menoInstrukcie;
//	dalsiaInstrukcia->next = NULL;
//	dalsiaInstrukcia->prev = NULL;
//
//	StackPointer zasobnik;
//	stackInit(&zasobnik);
//	pushFrame(&zasobnik);
//	fromPreparationDoBase(&zasobnik,sString,dalsiaInstrukcia);
//	Frame_variable *F_var = insertVariableToFrame(&zasobnik,"premenna",sInteger);
//	insertVariableToFrame(&zasobnik,"return_test",sString);
//	setValueVariable(&zasobnik,"premenna","4");
//	F_var=F_var;
//
//	pushFrame(&zasobnik);
//	fromPreparationDoBase(&zasobnik,sString,NULL);
//	insertVariableToFrame(&zasobnik,"premenna2",sString);
//	setValueVariable(&zasobnik,"$return","navratova hodnota STRING");
//	Frame_variable *ret  = searchVariableInFrames(&zasobnik,"$return");
//	fromBaseDoEnded(&zasobnik);
//	Frame_variable *S_var = searchVariableInFramesAfterEnded(&zasobnik,"return_test");
//	copyValue(&zasobnik,ret,S_var);
//	popFrame(&zasobnik);
//	if(S_var!=NULL)
//		printf("hodnota premennej: %s\n",S_var->frame_var_value.S);
//	else
//		printf("Nenasiel som \n");
//
//	Frame_variable *instrukc = searchVariableInFrames(&zasobnik,"$next_instruction");
//	if(instrukc!=NULL)
//		printf("nasledujuca instrukcia je %s",(char *)(instrukc->next_instruction->data));
//	else
//		printf("nenasiel som instrukciu\n");
//
//	//setValueVariable(&zasobnik,"premenna2","AHOJ");
//	//copyValue(&zasobnik,"premenna2","$return");
//	//popFrame(&zasobnik);			
//	//Frame_variable *S_var = searchVariableInFrames(&zasobnik,"$return");
//	//if(S_var!=NULL)
//	//	printf("hodnota premennej: %s\n",S_var->frame_var_value.S);
//	//else
//	//	printf("Nenasiel som \n");
//	//
//
//	destroyAllFrames(&zasobnik);
//	memallfree();
//	return 0;
//}

/* funkcia prida na zasobnik ramcov novy blokovy ramec
 * return - INTERN_ERR,OK_ERR
 * Stac - ukazatel na zasobnik ramcov */
ERROR_CODE pushFrame(StackPointer *Stac)
{
	if(Stac!=NULL)
	{
		 Frame *new_frame = memmalloc(sizeof(struct frame));
		 if(new_frame!=NULL)
		{
			new_frame->type_of_frame = BLOCK;
			treeInit(&(new_frame->frame_tree));
			if(stackPush(Stac,new_frame)==INTERN_ERR) /* vlozim ramec na zasobnik */
			{
				return INTERN_ERR;
			}
			return OK_ERR;
		}
	}
	return INTERN_ERR;
}

/* funkcia vlozi novu premennu do ramca 
 * return - vrati ukazatal na vlozenu polozku alebo NULL ak sa vlozenie nepodarilo
 * Stac  - ukazatel na zasobnik
 * name  - meno vkladanej premennej
 * type  - typ vkladanej premennej
 * hodnota - hodnota vkladanej premennej */
Frame_variable *insertVariableToFrame(StackPointer *Stac, char *name,int type)
{
	if(Stac!=NULL)
	{ 
		Frame_variable *new_variable = memmalloc(sizeof(struct frame_variable));
		if(new_variable!=NULL)
		{
			new_variable->frame_var_name=name;
			new_variable->frame_var_type=type;
			switch(type)
			{
				case sInteger:
					new_variable->frame_var_value.I=99;
					break;
				case sDouble:
					new_variable->frame_var_value.D=99;
					break;
				case sString:
					new_variable->frame_var_value.S="NULL";
					break;
			}
			new_variable->inicialized=FALSE;
			new_variable->next_instruction = NULL;
			if(treeInsert( &(((Frame *)(stackTop(Stac)))->frame_tree),name,new_variable)==INTERN_ERR)
			{
				memfree(new_variable);
				return NULL;
			}
			return new_variable;
		}
	}
	return NULL;
}

/* funkcia najprvr uvolni vsetko co obsahuje prvok na vrchole a potom nasledne uvolni aj jeho
 * Stac - ukazatel na zasobnik ramcov */
void popFrame(StackPointer *Stac)
{
	if(Stac!=NULL)
	{
		if(!stackEmpty(Stac)) /* ak zasobnik nieje prazdny */
		{
			treeDestroy( &(((Frame *)(stackTop(Stac)))->frame_tree) );
			stackPop(Stac);
		}
	}
}
/* funkcie hlada premennu vsetkych ramcoch po zakladny ramec
 * return - vrati ukazatel na najdenu premennu, ak nenajde vrati NULL
 * Stac   - ukazatel na zasobnik
 * var_name - meno premennej ktoru hladame */
Frame_variable *searchVariableInFrames(StackPointer *Stac, char *var_name)
{
	if(Stac!=NULL)
	{
		Stack helpful_pointer = Stac->top_of_stack; /* pouzijem pomocnu premennu */
		Stack last_base = findLastBaseFrame(Stac);
		while(helpful_pointer!=last_base)
		{
			Tree searched_variable = treeSearch(&((Frame *)(helpful_pointer->data))->frame_tree,var_name);
			if(searched_variable!=NULL)
			{
				return searched_variable->data;
			}
			else
				helpful_pointer=helpful_pointer->left_stack_element; /* nenasiel som pozriem sa hlbsie */
		}
		/* v pripade ze helpful pointer je rovny zakladnemu ramcu este treba pozriet ci sa premenna nenachadza v nom */
		Tree searched_variable = treeSearch(&((Frame *)(helpful_pointer->data))->frame_tree,var_name);
		if(searched_variable!=NULL)
			return searched_variable->data;
	}
	return NULL;
}

/* funkcie vyhlada premennu za medzi BASE a ENDED(exclusive)
 * return - vrati najdenu premenne alebo NULL ak ju nenejde
 * Stac   - ukazatel na zasobnik
 * var_name - meno hladanej premennej */
Frame_variable *searchVariableInFramesAfterEnded(StackPointer *Stac, char *var_name)
{
	if(Stac!=NULL)
	{
		Stack helpful_pointer = Stac->top_of_stack->left_stack_element; /* pouzijem pomocnu premennu */
		Stack last_base = findLastBaseFrame(Stac);
		while(helpful_pointer!=last_base)
		{
			Tree searched_variable = treeSearch(&((Frame *)(helpful_pointer->data))->frame_tree,var_name);
			if(searched_variable!=NULL)
			{
				return searched_variable->data;
			}
			else
				helpful_pointer=helpful_pointer->left_stack_element; /* nenasiel som pozriem sa hlbsie */
		}
		/* v pripade ze helpful pointer je rovny zakladnemu ramcu este treba pozriet ci sa premenna nenachadza v nom */
		Tree searched_variable = treeSearch(&((Frame *)(helpful_pointer->data))->frame_tree,var_name);
		if(searched_variable!=NULL)
			return searched_variable->data;
	}
	return NULL;
}

/* funkcia hlada base frame od vrcholu zasobniku
 * return - ukazatel na polozku zasobniku ktory je base frame
 * Stac   - ukazatel na zasobnik */
Stack findLastBaseFrame(StackPointer *Stac)
{
	if(Stac!=NULL)
	{
		if(!stackEmpty(Stac))
		{
			Stack helpful_pointer = Stac->top_of_stack;
			if( ((Frame *)(helpful_pointer->data))->type_of_frame==BASE )
				return helpful_pointer;
			while(((Frame *)(helpful_pointer->data))->type_of_frame!=BASE )
			{
				if( ((Frame *)(helpful_pointer->data))->type_of_frame==BASE )
					return helpful_pointer;
				else
					helpful_pointer=helpful_pointer->left_stack_element;
			}
		}
	}
	return NULL;
}

/* zmeni typ ramca z pripravovaneho ktory je na vrchole na zakladny 
 * Stac - ukazatel na zasobnik */
void fromPreparationDoBase(StackPointer *Stac,int return_type,List next_instruction)
{
	if(Stac!=NULL)
	{
		Frame *converting_frame = stackTop(Stac);
		converting_frame->type_of_frame=BASE;
		switch (return_type)/* vlozim specialnu premennu $return */
		{
			case sInteger:
				insertVariableToFrame(Stac,"$return",return_type);
				break;
			case sDouble:
				insertVariableToFrame(Stac,"$return",return_type);
				break;
			case sString:
				insertVariableToFrame(Stac,"$return",return_type);
				break;
		}
		if(next_instruction!=NULL)
		{
			Frame_variable *Inst_next = insertVariableToFrame(Stac,"$next_instruction",return_type); /* specialne premennja ktora ako jedina bude mat nastavenu polozku next_instruction co je vlastne ukazatel na dalsiu instrukciu v volajucej funkcii */
			Inst_next->next_instruction=next_instruction;/* samotne priradenie ukazatela */
		}
	}
}

/* zrusim vsetyk ramce ak nejake su nad zakladnym a potom aj samotny zakladny ramec 
 * Stac - ukazatel na zasobnik */
void destroyFramesWithBase(StackPointer *Stac)
{
	if(Stac!=NULL)
	{
		Stack last_base = findLastBaseFrame(Stac);
		while(Stac->top_of_stack!=last_base)
		{
			popFrame(Stac);
		}
		popFrame(Stac);/* este treba odstranit base frame */
	}
}

/* odstarani a uvolni vsetky ramce zo zasobnika 
 * Stac - ukazatel na zasobnik */
void destroyAllFrames(StackPointer *Stac)
{
		while(!stackEmpty(Stac))
		{
			popFrame(Stac);
		}
}

/* funkcia nastavi premennu ako inicializovanu
 * return - ak premennu nenasla alebo bol zadany zly zasobnik vrati INTERN_ERR inak OK_ERR
 * Stac - ukazatel na zasobnik
 * name - meno menenej premennej */
ERROR_CODE setVariableAsInitialized(StackPointer *Stac,char *name)
{
	if(Stac!=NULL)
	{
		Frame_variable *variable = searchVariableInFrames(Stac,name);
		if(variable==NULL)
			return INTERN_ERR;
		variable->inicialized=TRUE;
		return OK_ERR;
	}
	return INTERN_ERR;
}

/* zmeni hodnotu premennej ktora v ramci uz je 
 * return - INTERE_ERR ak sa nenasla alebo bol zadany zly pointer
 * name   - meno premennej ktoru chceme zmenit
 * value  - hodnota ktoru chceme nastavit */
ERROR_CODE setValueVariable(StackPointer *Stac,char *name,char *value)
{
	if(Stac!=NULL)
	{
		Frame_variable *variable = searchVariableInFrames(Stac,name);
		if(variable!=NULL)
		{
			switch(variable->frame_var_type)
			{
				case sInteger:
					variable->frame_var_value.I=atoi(value);
					break;
				case sDouble:
					variable->frame_var_value.D=strtod(value,NULL);
					break;
				case sString:
					variable->frame_var_value.S=value;
					break;
			}
			return OK_ERR;
		}
	}
	return INTERN_ERR;
}

/* najde BASE frame a zmeni ho na ENEDED
 * Stac - ukazatel na zasobnik */
void fromBaseDoEnded(StackPointer *Stac)
{
	if(Stac!=NULL)
	{
		Stack last_base = findLastBaseFrame(Stac);
		((Frame *)(last_base->data))->type_of_frame=ENDED;
	}
}

/* najprv odstrani ramce co su nad base frame a potom base frame zmeni na ENDED
 * Stac - ukazatel na zasobnik */
void destroyFramesToEnded(StackPointer *Stac)
{
	if(Stac!=NULL)
	{
		Stack last_base = findLastBaseFrame(Stac);
		while(Stac->top_of_stack!=last_base)
		{
			popFrame(Stac);
		}
		/*base frame neodstranim len ho zmenim na ENDED */
		fromBaseDoEnded(Stac);
	}
}

ERROR_CODE copyValue(StackPointer *Stac,Frame_variable *from_variable, Frame_variable *to_variable)
{
	if(Stac!=NULL)
	{
		if(from_variable==NULL || to_variable==NULL)
			return INTERN_ERR;
		int from_type = from_variable->frame_var_type;		
		int to_type = to_variable->frame_var_type;		
		if(from_type!=to_type)
			return INTERN_ERR;
		switch (from_type)
		{
			case sInteger:
				to_variable->frame_var_value.I = from_variable->frame_var_value.I;
				return OK_ERR;
				break;
			case sString:
				to_variable->frame_var_value.S = memmalloc(sizeof(char) * strlen(from_variable->frame_var_value.S)+1);
				if(to_variable->frame_var_value.S !=NULL)
				{
					strcpy(to_variable->frame_var_value.S,from_variable->frame_var_value.S);
					return OK_ERR;
				}
				break;
			case sDouble:
				to_variable->frame_var_value.D = from_variable->frame_var_value.D;
				return OK_ERR;
				break;
		}
	}
	return INTERN_ERR;
}
