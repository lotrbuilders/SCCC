#include "symbols.h"
#include "errorhandling.h"
#include <stdio.h>
char *i_strdup(char *str);
int TYPE_INT=0;
int TYPE_INTPTR=1;
int TYPE_PTRPTR=2;
char *declarator_name(int **ast)
{
	
	char *name;
	int id=*ast;
	if(id==SYM_DECLARATION)
		return declarator_name(*(ast+3));
	if(id!=SYM_DECLARATOR)
		error("Expected declerator type");
	name=i_strdup(*(ast+1));
	return name;
}
char *declarator_type(int **ast,int type)
{
	
	int base;
	int ptr_count;
	if(type==SYM_INT)
		base=TYPE_INT;
	else
		error("Expected decleration type to be int");
	ptr_count=*(ast+2);
	fprintf(stderr,"test\n");
	if(ptr_count>=2)
		return TYPE_PTRPTR;
	else return base+TYPE_INTPTR;
	
}