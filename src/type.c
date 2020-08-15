#include "symbols.h"
#include "errorhandling.h"
#include "identifiers.h"
#include <stdio.h>
char *i_strdup(char *str);
int TYPE_INT=0;
int TYPE_CHAR=1;
int TYPE_INTPTR=256;
int TYPE_CHARPTR=257;
int TYPE_PTRPTR=512;

int DEBUG_TYPE;

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
	else if(type==SYM_CHAR)
		base=TYPE_CHAR;
	else
		error("Expected decleration type to be int");
	ptr_count=*(ast+2);
	if(ptr_count>=2)
		return TYPE_PTRPTR;
	else
		return base+TYPE_INTPTR*ptr_count;
	
}

int find_type(int **ast)
{
	int type;
	int left_type;
	int right_type;
	int id;
	id=*ast;
	if(DEBUG_TYPE)
		fprintf(stderr,"find type; id=%d\n",id);
	if(id==SYM_CONSTANT)
	{
		return TYPE_INT;
	}
	else if(id==SYM_ID)
	{
		type=find_id_type(*(ast+1));
		if(type==TYPE_CHAR)
			type=TYPE_INT;
		return type;
	}
	else if(id==SYM_STRING)
	{
		return TYPE_CHARPTR;
	}
	else if(id==SYM_NEGATE)
	{
		return find_type(*(ast+1));
	}
	else if(id==SYM_POINTER)
	{
		type=find_type(*(ast+1));
		if(type>TYPE_CHAR)
			type=type-TYPE_INTPTR;
		else if(type==TYPE_CHAR)
			type=TYPE_INT;
		return type;
	}
	else if(id==SYM_ADDRESS)
	{
		type=find_id_type(*(ast+1));
		if(type<TYPE_PTRPTR)
			type=type+TYPE_INTPTR;
		return type;
	}
	else if(id==SYM_FUNC_CALL)
	{
		return TYPE_INT;
		// Not really implemented as function types are not stored. We assume int and as long as no pointer arithmetic is performed on functions
		// which return pointers it should be passable
	}
	else if(id=='*')
	{
		//Multiplication is not permissible on pointers so we assume it doesn't happen
		return TYPE_INT;
	}
	else if((id=='+')||(id=='-'))
	{
		left_type=find_type(*(ast+1));
		right_type=find_type(*(ast+2));
		if((left_type>=TYPE_INTPTR)||(right_type>=TYPE_INTPTR))
		{
			if(left_type>right_type)
				return left_type;
			else
				return right_type;
		}
		else
			return TYPE_INT;	
	}
	else if((id=='>')||(id=='<')||(id==SYM_LE)||(id==SYM_GE)||(id==SYM_EQ)||(id==SYM_NE))
	{
		return TYPE_INT;
	}
	else if((id==SYM_LOR)||(id==SYM_LAND))
	{
		return TYPE_INT;
	}
	else
		error("unable to check type");
	
}

int find_lvalue_type(int **ast)
{
	int type;
	int left_type;
	int right_type;
	int id;
	id=*ast;
	if(DEBUG_TYPE)
		fprintf(stderr,"find lvalue type; id=%d\n",id);
	
	if(id==SYM_ID)
	{
		type=find_id_type(*(ast+1));
		return type;
	}
	else if(id==SYM_POINTER)
	{
		type=find_type(*(ast+1));
		if(type>=TYPE_CHAR)
			type=type-TYPE_INTPTR;
		return type;
	}
	else
		error("unable to check lvalue type");
	
}