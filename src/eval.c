#include "gen.h"
#include "errorhandling.h"
#include "symbols.h"
#include "identifiers.h"
#include "type.h"
#include <stdio.h>
#include <string.h>

int **newnode(int size);
char *i_strdup(char *str);
int in_main;

int eval_global(int **ast);
int eval_function_arguments(int **ast,int loc);
int eval_statements(int **ast);
int eval_statement(int **ast);
int eval_decleration(int **ast);
int eval_expression(int **ast);
int eval_lvalue(int **ast);
int eval_func_call(int **ast);
int eval_const_expression(int **ast);

int add_string(char *name,int count);
int eval_string(int **list);

int global_label_count=0;
int string_count=0;
int **string_list=0;
int stack_loc;

int EVAL_GLOBAL_DEBUG;
int EVAL_STATEMENT_DEBUG;
int EVAL_EXPRESSION_DEBUG;

int eval(int **ast)
{
	if(ast==0)
		return debug_warning("ast==NULL in eval");
	if(EVAL_GLOBAL_DEBUG)
		fprintf(stderr,"Evaluating at top\n");
	int id;
	id=*ast;
	if(id==0)
		return 0;
	else if(id==SYM_TRANSLATION_UNIT)
	{
		eval_global(*(ast+1));
		return eval(*(ast+2));
	}
	else
		error("unexpected symbol in eval");
	
}

int eval_global(int **ast)
{
	if(ast==0)
		return debug_warning("ast==NULL in eval-global");
	if(EVAL_GLOBAL_DEBUG)
		fprintf(stderr,"Evaluating global ");
	int id;
	id=*ast;
	if(id==SYM_FUNC_DEF)
	{
		if(EVAL_GLOBAL_DEBUG)
			fprintf(stderr,"function defenition %s\n",*(ast+1));
		enter_block();
		eval_function_arguments(*(ast+3),-12);
		int pop_count;
		gen_function_prolog(*(ast+1));
		enter_block();
		if(strcmp("main",*(ast+1))==0)
			stack_loc=12;
		eval_statements(*(ast+2));
		pop_count=leave_block(0);
		gen_function_epilog();
		leave_block(1);
		eval_string(string_list);
		string_count=0;
		//free_string(string_list);
		string_list=0;
	}
	else if(id==SYM_FUNC_DECL)
	{
		if(EVAL_GLOBAL_DEBUG)
			fprintf(stderr,"function decleration %s\n",*(ast+1));
		gen_extern(*(ast+1));
	}
	else if(id==SYM_GLOBAL_DECL)
	{
		if(EVAL_GLOBAL_DEBUG)
			fprintf(stderr,"variable decleration %s\n",*(ast+1));
		add_global(*(ast+1),(*(ast+4)));
		gen_common(*(ast+1));
	}
	else if(id==SYM_GLOBAL_DEF)
	{
		if(EVAL_GLOBAL_DEBUG)
			fprintf(stderr,"variable defenition %s\n",*(ast+1));
		int val;
		val=eval_const_expression(*(ast+2));
		gen_decleration(*(ast+1),val);
		add_global(*(ast+1),(*(ast+4)));
	}
	else
		error("unexpected symbol in eval-global");
	return 0;
}

int eval_function_arg(int **ast,int loc)
{
	if(ast==0)
		return debug_warning("ast==NULL in eval-function-arg");
	if(EVAL_GLOBAL_DEBUG)
		fprintf(stderr,"evaluating function argument %s\n",*(ast+2));
	int id;
	id=*ast;
	
	if(id==SYM_DECLARATION)
	{
		add_argument_identifier(*(ast+2),*(ast+1),loc);
	}
	return 0;
}


int eval_function_arguments(int **ast,int loc)
{
	if(ast==0)
		return -12;
	if(EVAL_GLOBAL_DEBUG)
		fprintf(stderr,"evaluating function argment list\n");
	int id;
	id=*ast;
	if(id!=SYM_DECL_LIST)
		error("Expected decleration list");
	eval_function_arg(*(ast+1),loc);
	loc=eval_function_arguments(*(ast+2),loc-4);
	loc=loc-4;
	return loc;
}

int eval_statements(int **ast)
{
	int **node;
	if(EVAL_GLOBAL_DEBUG)
		fprintf(stderr,"evaluating statement list\n");
	for(node=ast;node!=0;node=*(node+2))
	{
		int id=*node;
		if(id==SYM_STATEMENT_LIST)
			eval_statement(*(node+1));
		else
			error("expected statement-list entry while evaluating statements");
	}
	return 0;
	
}

int eval_statement(int **ast)
{
	if(ast==0)
		return debug_warning("ast==NULL in eval-statement");
	
	int id;
	id=*ast;
	if(id==SYM_RETURN)
	{
		if(EVAL_STATEMENT_DEBUG)
			fprintf(stderr,"evaluating return statement\n");
		gen_pop();
		eval_expression(*(ast+1));
		gen_return();
	}
	else if(id==SYM_LOCAL_DECLARATION)
	{
		if(EVAL_STATEMENT_DEBUG)
			fprintf(stderr,"evaluating local decleration\n");
		eval_decleration(ast);
	}
	else if(id==SYM_COMPOUND_STATEMENT)
	{
		if(EVAL_STATEMENT_DEBUG)
			fprintf(stderr,"evaluating compound statement\n");
		int count;
		enter_block();
		eval_statements(*(ast+1));
		count=leave_block(0);
		gen_subtract_sp(count);
	}
	else if(id==SYM_IF)
	{
		if(EVAL_STATEMENT_DEBUG)
			fprintf(stderr,"evaluating if statement\n");
		int if_label;
		int else_label;
		
		gen_pop();
		eval_expression(*(ast+1));
		
		if_label=global_label_count;
		else_label=if_label+(*(ast+3)!=0);
		global_label_count=else_label+1;
		
		gen_jz(if_label);
		
		eval_statement(*(ast+2));
		
		if(*(ast+3)!=0)
		{
			if(EVAL_STATEMENT_DEBUG)
				fprintf(stderr,"evaluating else statement\n");
			gen_jmp(else_label);
			gen_label(if_label);
			eval_statement(*(ast+3));
			gen_label(else_label);
		}
		else 
			gen_label(if_label);
			
	}
	else if(id==SYM_WHILE)
	{
		if(EVAL_STATEMENT_DEBUG)
				fprintf(stderr,"evaluating while statement\n");
		int label_begin;
		int label_end;
		
		label_begin=global_label_count;
		label_end=label_begin+1;
		global_label_count=label_end+1;
		
		gen_label(label_begin);
		gen_pop();
		eval_expression(*(ast+1));
		gen_jz(label_end);
		eval_statement(*(ast+2));
		gen_jmp(label_begin);
		gen_label(label_end);
	}
	else if(id==SYM_FOR)
	{
		if(EVAL_STATEMENT_DEBUG)
				fprintf(stderr,"evaluating for statement\n");
		int label_begin;
		int label_end;
		
		label_begin=global_label_count;
		label_end=label_begin+1;
		global_label_count=label_end+1;
		
		gen_pop();
		eval_expression(*(ast+1));
		gen_label(label_begin);
		gen_pop();
		eval_expression(*(ast+2));
		gen_jz(label_end);
		eval_statement(*(ast+4));
		gen_pop();
		eval_expression(*(ast+3));
		gen_jmp(label_begin);
		gen_label(label_end);
	}
	else
	{
		if(EVAL_STATEMENT_DEBUG)
			fprintf(stderr,"evaluating expression statement\n");
		gen_pop();
		eval_expression(ast);
		//gen_pop();
	}
	return 0;
}


int eval_decleration(int **ast)
{
	if(EVAL_STATEMENT_DEBUG)
		fprintf(stderr,"evaluating decleration\n");
	int **child=*(ast+1);
	add_identifier(*(child+2),*(child+1));
	if(*(ast+2)==0)
		gen_push_constant(0);
	else
	{
		gen_pop();
		eval_expression(*(ast+2));
		gen_dup();
	}
	return 0;
}

int eval_expression(int **ast)
{
	if(EVAL_EXPRESSION_DEBUG)
		fprintf(stderr,"evaluating expression\n");
	int label_count;
	if(ast==0)
		return debug_warning("ast==NULL in eval-expression");
	
	int id;
	int left_type;
	int right_type;
	id=*ast;
	if(id==SYM_CONSTANT)
	{
		if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"evaluating constant %d\n",*(ast+1));
		gen_constant(*(ast+1));
	}
	else if(id==SYM_ID)
	{
		if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"evaluating id %s\n",*(ast+1));
		if(isvalid(*(ast+1)))
		{
			int size=4;
			int type=find_id_type(*(ast+1));
			if(EVAL_EXPRESSION_DEBUG)
				fprintf(stderr,"type of %s is %d\n",*(ast+1),type);
			if(type==TYPE_CHAR)
				size=1;
			
			if(islocal(*(ast+1))==0)
			{
				if(EVAL_EXPRESSION_DEBUG)
					fprintf(stderr,"%s is local\n",*(ast+1));
				gen_global_var(*(ast+1),size);
			}
			else
			{
				if(EVAL_EXPRESSION_DEBUG)
					fprintf(stderr,"%s is global\n",*(ast+1));
				int loc;
				loc=locate_identifier(*(ast+1));
				gen_local_var(loc,size);
			}
		}
		else
		{
			fprintf(stderr,"name:%s\n",*(ast+1));
			error("Unknown identifier");
		}
		
	}
	else if(id==SYM_STRING)
	{
		if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"evaluating string \"%s\"\n",*(ast+1));
		gen_load_string(string_count);
		add_string(*(ast+1),string_count);
		string_count=string_count+1;
	}
	else if(id==SYM_NEGATE)
	{
		if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"evaluating negate\n");
		eval_expression(*(ast+1));
		gen_negate();
	}
	else if(id==SYM_POINTER)
	{
		if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"evaluating pointer\n");
		int size=4;
		int type=find_type(*(ast+1));
		if(type==TYPE_CHARPTR)
			size=1;
		eval_expression(*(ast+1));
		gen_pointer(size);
	}
	else if(id==SYM_ADDRESS)
	{
		if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"evaluating address\n");
		eval_lvalue(*(ast+1));
	}
	else if(id==SYM_FUNC_CALL)
	{
		if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"evaluating function call\n");
		gen_dup();
		eval_func_call(ast);
	}
	else if(id=='*')
	{
		if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"evaluating multiply\n");
		eval_expression(*(ast+2));
		eval_expression(*(ast+1));
		gen_multiply();
		
	}
	else if(id=='+')
	{
		if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"evaluating addition\n");
		left_type=find_type(*(ast+1));
		right_type=find_type(*(ast+2));
		eval_expression(*(ast+2));
		if((left_type>=TYPE_INTPTR)&&(right_type==TYPE_INT))
		{
			if((left_type-TYPE_INTPTR)!=TYPE_CHAR)
			{
				gen_constant(4);
				gen_multiply();
			}
		}
		eval_expression(*(ast+1));
		if((right_type>=TYPE_INTPTR)&&(left_type==TYPE_INT))
		{
			if((right_type-TYPE_INTPTR)!=TYPE_CHAR)
			{
				gen_constant(4);
				gen_multiply();
			}
		}
		gen_add();
	}
	else if(id=='-')
	{
		if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"evaluating subtraction\n");
		left_type=find_type(*(ast+1));
		right_type=find_type(*(ast+2));
		eval_expression(*(ast+2));
		if((left_type>TYPE_INT)&&(right_type==TYPE_INT))
		{
			gen_constant(4);
			gen_multiply();
		}
		eval_expression(*(ast+1));
		if((right_type>TYPE_INT)&&(left_type==TYPE_INT))
		{
			gen_constant(4);
			gen_multiply();
		}
		gen_subtract();
	}
	else if((id=='>')||(id=='<')||(id==SYM_LE)||(id==SYM_GE)||(id==SYM_EQ)||(id==SYM_NE))
	{
		if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"evaluating comparison\n");
		eval_expression(*(ast+2));
		eval_expression(*(ast+1));
		gen_comparison(id);
	}
	else if(id==SYM_LOR)
	{
		if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"evaluating logical or\n");
		eval_expression(*(ast+1));
		label_count=global_label_count;
		gen_jnz(label_count);
		gen_pop();
		global_label_count=global_label_count+1;
		eval_expression(*(ast+2));
		gen_label(label_count);
		gen_normalize();
	}
	else if(id==SYM_LAND)
	{
		if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"evaluating logical and\n");
		eval_expression(*(ast+1));
		label_count=global_label_count;
		gen_jz(label_count);
		gen_pop();
		global_label_count=global_label_count+1;
		eval_expression(*(ast+2));
		gen_label(label_count);
		gen_normalize();
	}
	else if(id=='=')
	{
		if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"evaluating assignment\n");
		int size=4;
		int type=find_lvalue_type(*(ast+1));
		if(type==TYPE_CHAR)
			size=1;
		eval_expression(*(ast+2));
		eval_lvalue(*(ast+1));
		gen_assign(size);
	}
	else 
		error("unexpected symbol in eval-expression");
	
	return 0;
	
}

int eval_lvalue(int **ast)
{
	if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"evaluating lvalue ");
	if(ast==0)
		return debug_warning("ast==NULL in eval-expression");
	int id=*ast;
	if(id==SYM_ID)
	{
		
		if(isvalid(*(ast+1)))
		{
			if(islocal(*(ast+1))==0)
			{
				if(EVAL_EXPRESSION_DEBUG)
					fprintf(stderr,"global id %s\n",*(ast+1));
				gen_global_address(*(ast+1));
			}
			else 
			{
				if(EVAL_EXPRESSION_DEBUG)
					fprintf(stderr,"local id %s\n",*(ast+1));
				int loc;
				loc=locate_identifier(*(ast+1));
				gen_local_address(loc);
			}
		}
		else
			error("Unknown identifier");
	}
	else if(id==SYM_POINTER)
	{
		if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"pointer\n");
		eval_expression(*(ast+1));
	}
	else 
	{
		fprintf(stderr,"%d\n",id);
		error("unexpected symbol in eval-lvalue");	
	}
	return 0;
}

int eval_func_arg(int **ast)
{
	if(ast==0)
	{
		return 0;
	}
	int id;
	int loc;
	id=*ast;
	if(id!=SYM_EXPR_LIST)
		error("Expected expression list");
	loc=eval_func_arg(*(ast+2))+4;
	gen_pop();
	eval_expression(*(ast+1));
	gen_dup();
	return loc;
	
}
int count_arguments(int **ast)
{
	if(ast==0)
		return 0;
	else 
		return count_arguments(*(ast+2))+1;
}

int eval_func_call(int **ast)
{
	int child_id;
	char *name;
	int offset;
	int sp;
	int padding;
	child_id=**(ast+1);
	if(child_id!=SYM_ID)
		error("Expected call to identifier");
	int **child;
	child=*(ast+1);
	name=*(child+1);
	
	sp=4*count_arguments(*(ast+2))+stack_loc+8-8*in_main;
	for(padding=sp;padding>16;padding=padding-16) 1;
	
	gen_subtract_sp(-padding);
	offset=eval_func_arg(*(ast+2));
	gen_function_call(name);
	gen_subtract_sp(offset+padding);
	return 0;
}

int eval_const_expression(int **ast)
{
	if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"evaluating constant expression\n");
	int id;
	id=*(ast);
	if(id==SYM_NEGATE)
		return -eval_const_expression(*(ast+1));
	else if(id==SYM_CONSTANT)
		return *(ast+1);
	else 
		error("expected const expression to be calculatable");
}

int add_string(char *name,int count)
{
	if(EVAL_EXPRESSION_DEBUG)
		fprintf(stderr,"adding string %s\n",name);
	int **entry;
	entry=newnode(3);
	*entry=i_strdup(name);
	*(entry+1)=count;
	*(entry+2)=string_list;
	string_list=entry;
	return 0;
}

int eval_string(int **list)
{
	if(EVAL_EXPRESSION_DEBUG)
		fprintf(stderr,"inserting constant strings:\n");
	int **entry;
	for(entry=list;entry!=0;entry=*(entry+2))
	{
		if(EVAL_EXPRESSION_DEBUG)
			fprintf(stderr,"\tstring: \"%s\"\n",*entry);
		gen_string(*entry,*(entry+1));
	}
	return 0;
}




