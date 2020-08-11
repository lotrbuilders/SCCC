#include "gen.h"
#include "errorhandling.h"
#include "symbols.h"
#include "identifiers.h"
#include <stdio.h>

int eval_global(int **ast);
int eval_statements(int **ast);
int eval_statement(int **ast);
int eval_decleration(int **ast);
int eval_expression(int **ast);
int eval_lvalue(int **ast);

int global_label_count=0;

int eval(int **ast)
{
	if(ast==0)
		return debug_warning("ast==NULL in eval");
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
	
	int id;
	id=*ast;
	if(id==SYM_FUNC_DEF)
	{
		int pop_count;
		gen_function_prolog(*(ast+1));
		enter_block();
		eval_statements(*(ast+2));
		pop_count=leave_block();
		gen_function_epilog();
		return 0;
	}
	else
		error("unexpected symbol in eval-global");
}

int eval_statements(int **ast)
{
	int **node;
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
		gen_pop();
		eval_expression(*(ast+1));
		gen_return();
	}
	else if(id==SYM_LOCAL_DECLERATION)
	{
		eval_decleration(ast);
	}
	else if(id==SYM_COMPOUND_STATEMENT)
	{
		int count;
		enter_block();
		eval_statements(*(ast+1));
		count=leave_block();
		gen_subtract_sp(count);
	}
	else if(id==SYM_IF)
	{
		int if_label;
		int else_label;
		
		eval_expression(*(ast+1));
		
		if_label=global_label_count;
		else_label=if_label+(*(ast+3)!=0);
		global_label_count=else_label+1;
		
		gen_jz(if_label);
		gen_pop();
		eval_statement(*(ast+2));
		
		if(*(ast+3)!=0)
		{
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
		int label_begin;
		int label_end;
		
		label_begin=global_label_count;
		label_end=label_begin+1;
		global_label_count=label_end+1;
		
		gen_label(label_begin);
		eval_expression(*(ast+1));
		gen_jz(label_end);
		gen_pop();
		eval_statement(*(ast+2));
		gen_jmp(label_begin);
		gen_label(label_end);
		gen_pop();
	}
	else if(id==SYM_FOR)
	{
		int label_begin;
		int label_end;
		
		label_begin=global_label_count;
		label_end=label_begin+1;
		global_label_count=label_end+1;
		
		eval_expression(*(ast+1));
		gen_pop();
		gen_label(label_begin);
		eval_expression(*(ast+2));
		gen_jz(label_end);
		gen_pop();
		eval_statement(*(ast+4));
		eval_expression(*(ast+3));
		gen_pop();
		gen_jmp(label_begin);
		gen_label(label_end);
		gen_pop();
	}
	else
	{
		gen_pop();
		eval_expression(ast);
		gen_pop();
	}
	return 0;
}


int eval_decleration(int **ast)
{
	add_identifier(*(ast+2));
	gen_push_constant(0);
	return 0;
}

int eval_expression(int **ast)
{
	int label_count;
	
	if(ast==0)
		return debug_warning("ast==NULL in eval-expression");
	
	int id;
	id=*ast;
	if(id==SYM_CONSTANT)
	{
		gen_constant(*(ast+1));
	}
	else if(id==SYM_ID)
	{
		if(isvalid(*(ast+1)))
		{
			int loc;
			loc=locate_identifier(*(ast+1));
			gen_local_var(loc);
		}
		else
			error("Unknown identifier");
		
	}
	else if(id==SYM_NEGATE)
	{
		eval_expression(*(ast+1));
		gen_negate();
	}
	else if(id=='*')
	{
		eval_expression(*(ast+2));
		eval_expression(*(ast+1));
		gen_multiply();
		
	}
	else if(id=='+')
	{
		eval_expression(*(ast+2));
		eval_expression(*(ast+1));
		gen_add();
	}
	else if(id=='-')
	{
		eval_expression(*(ast+2));
		eval_expression(*(ast+1));
		gen_subtract();
	}
	else if((id=='>')||(id=='<')||(id==SYM_LE)||(id==SYM_GE)||(id==SYM_EQ)||(id==SYM_NE))
	{
		eval_expression(*(ast+2));
		eval_expression(*(ast+1));
		gen_comparison(id);
	}
	else if(id==SYM_LOR)
	{
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
		eval_expression(*(ast+2));
		eval_lvalue(*(ast+1));
		gen_assign();
	}
	else 
		error("unexpected symbol in eval-expression");
	
	return 0;
	
}

int eval_lvalue(int **ast)
{
	if(ast==0)
		return debug_warning("ast==NULL in eval-expression");
	int id=*ast;
	if(id==SYM_ID)
	{
		if(isvalid(*(ast+1)))
		{
			int loc;
			loc=locate_identifier(*(ast+1));
			gen_local_address(loc);
		}
		else
			error("Unknown identifier");
	}
	else 
		error("unexpected symbol in eval-lvalue");	
	
}










