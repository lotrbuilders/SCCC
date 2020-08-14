#include "errorhandling.h"
#include "lexer.h"
#include "symbols.h"
#include "node.h"
#include "type.h"
#include <stdio.h>

char *i_strdup(char *str);

int tk;

int **parse_translation_unit();
int **parse_global();
int **parse_decleration_list();
int **parse_statement_list();

int **parse_statement();
int **parse_jump_statement();
int **parse_selection_statement();
int **parse_iteration_statement();
int **parse_decleration();
int **parse_declerator();
int **parse_local_decleration();

int **parse_expression();
int **parse_assignment_expression();
int **parse_logical_or_expression();
int **parse_logical_and_expression();
int **parse_equality_expression();
int **parse_comparison_expression();
int **parse_additive_expression();
int **parse_multiplicative_expression();
int **parse_postfix_expression();
int **parse_expression_list();
int **parse_unary_expression();
int **parse_primary_expression();

int PARSER_DEBUG=1;
int PEEK_DEBUG=1;

int peek()
{
	
	if(tk==0)
		tk=lex();
	
	if(PEEK_DEBUG)
		fprintf(stderr,"\tpeek into tk: %d\n",tk);
	
	return tk;
	
}

int pop()
{
	
	if(tk==0)
	{
		int tmp;
		tmp=lex();
		
		if(PEEK_DEBUG)
			fprintf(stderr,"\tpop tk: %d\n",tmp);
		
		return tmp;
	}
	else
	{
		int tmp;
		tmp=tk;
		tk=0;
		
		if(PEEK_DEBUG)
			fprintf(stderr,"\tpop tk: %d\n",tmp);
		
		return tmp;
	}
	
}

int expect(int t)
{
	
	if(t!=pop())
		error("unexpected token");
	
	return t;
	
}

int **parse()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parsing\n");
	
	return parse_translation_unit();
	
}

/* 
	translation-unit :=
		decleration
		decleration +
*/

int **parse_translation_unit()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parse_translation_unit\n");
	
	int **start;
	int **current;
	start=newnode(3);
	current=start;
	while(peek()!=SYM_EOF)
	{
		*current=SYM_TRANSLATION_UNIT;
		*(current+1)=parse_global();
		*(current+2)=newnode(3);
		current=*(current+2);
	}
	
	return start;
	
}

/*
	global-decleration :=
		decleration ;
		decleration = expression ;
		decleration ( ) ; 
		decleration ( ) { statement-list }
*/

int **parse_global()
{
	if(PARSER_DEBUG)
		fprintf(stderr,"parse global\n");
	
	char *name;
	int **decleration;
	int type;
	int **node;
	int **arg;
	int **statements;
	statements=0;
	node=newnode(5);
	decleration=parse_decleration();
	name=*(decleration+2);
	type=*(decleration+1);
	if(peek()==';')
	{
		pop();
		*node=SYM_GLOBAL_DECL;
		*(node+1)=name;
		*(node+4)=type;
	}
	else if(peek()=='=')
	{
		pop();
		*node=SYM_GLOBAL_DEF;
		*(node+1)=name;
		*(node+2)=parse_expression();
		*(node+4)=type;
		expect(';');
		
	}
	else
	{
		expect('(');
		arg=parse_decleration_list();
		expect(')');
		
		if(peek()=='{')
		{
			*node=SYM_FUNC_DEF;
			expect('{');
			statements=parse_statement_list();
			expect('}');
			if(PARSER_DEBUG)
				fprintf(stderr,"function %s\n",name);
		}
		else 
		{
			expect(';');
			*node=SYM_FUNC_DECL;
		}
		
		*(node+1)=name;
		*(node+2)=statements;
		*(node+3)=arg;
		*(node+4)=type;
	}
	
	return node;
	
}

/*
	decleration-list :=
		decleration { , decleration }
*/
int **parse_decleration_list()
{
	int **list;
	if(PARSER_DEBUG)
		fprintf(stderr,"parse decleration-list\n");
	if(peek()==')')
		return 0;
	else 
	{
		list=newnode(3);
		*list=SYM_DECL_LIST;
		*(list+1)=parse_decleration();
		if(peek()==',')
		{
			pop();
			*(list+2)=parse_decleration_list();
		}
		else
			*(list+2)=0;
		return list;
	}
}

/*
	statement-list :=
		statement {statement +}
*/
int **parse_statement_list()
{
	if(PARSER_DEBUG)
		fprintf(stderr,"parse statement-list\n");
	
	int **node;
	int **statement;
	if(peek()!='}')
	{
		statement=parse_statement();
		node=newnode(3);
		*node=SYM_STATEMENT_LIST;
		*(node+1)=statement;
		*(node+2)=parse_statement_list();
		return node;
	}
	else 
		return 0;
	
}

/*
	statement :=
		jump-statement
		selection-statement
		local-decleration
*/

int **parse_statement()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parse statement\n");
	
	int **node;
	int pk=peek();
	if(pk==SYM_RETURN)
		node=parse_jump_statement();
else if((pk==SYM_INT)||(pk==SYM_CHAR))
		node=parse_local_decleration();
	else if(pk==SYM_IF)
		node=parse_selection_statement();
	else if((pk==SYM_FOR)||(pk==SYM_WHILE))
		node=parse_iteration_statement();
	else if(pk=='{')
	{
		node=newnode(2);
		*node=SYM_COMPOUND_STATEMENT;
		pop();
		*(node+1)=parse_statement_list();
		expect('}');
	}
	else 
	{
		node=parse_expression();
		expect(';');
	}
	
	return node;
	
}

/*
	local-decleration :=
		decleration ;
		decleration = expression ';'
*/
int **parse_local_decleration()
{
	if(PARSER_DEBUG)
		fprintf(stderr,"parse local decleration\n");
	
	int **node=newnode(3);
	*node=SYM_LOCAL_DECLARATION;
	*(node+1)=parse_decleration();
	if(peek()=='=')
	{
		pop();
		*(node+2)=parse_expression();
	}
	expect(';');
	
	return node;
	
}

/*
	decleration :=
		INT declerator 
*/
int **parse_decleration()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parse decleration\n");
	
	int **node;
	node=0;
	
	if((peek()==SYM_INT)||(peek()==SYM_CHAR))
	{
		int **declerator;
		int type;
		char *name;
		type=pop();
		declerator=parse_declerator();
		
		type=declarator_type(declerator,type);
		
		name=declarator_name(declerator);
		node=newnode(4);
		*node=SYM_DECLARATION;
		*(node+1)=type;
		*(node+2)=name;
		*(node+3)=declerator;
	}
	
	return node;
	
}

/*
	declarator :=
		{ * } + ID
*/

int **parse_declerator()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parse declerator\n");
	
	int **node;
	node=0;
	int ptr_count;
	ptr_count=0;
	while(peek()=='*')
	{
		pop();
		ptr_count=ptr_count+1;
	}
	node=newnode(3);
	*node=SYM_DECLARATOR;
	expect(SYM_ID);
	*(node+1)=i_strdup(lexed_string);
	*(node+2)=ptr_count;
	
	return node;
	
}




/*
	jump-statement :=
		RETURN expression ;
*/

int **parse_jump_statement()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parse jump-statement\n");
	
	int **node;
	node=0;
	
	if(peek()==SYM_RETURN)
	{
		pop();
		int **exp;
		exp=parse_expression();
		if(exp!=0)
		{
			if(PARSER_DEBUG)
				fprintf(stderr,"parsed return statement\n");
			node=newnode(2);
			*node=SYM_RETURN;
			*(node+1)=exp;
		}
		else 
		{
			error("expected expression after return");
		}
		expect(';');
	}
	
	return node;
	
}

/*
	selection-statement :=
		IF ( expression ) statement
		IF ( expression ) statement else statement
	
*/
int **parse_selection_statement()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parse selection-statement\n");
	
	int **node;
	node=0;
	
	if(peek()==SYM_IF)
	{
		pop();
		int **exp;
		int **node_if;
		int **node_else;
		expect('(');
		exp=parse_expression();
		expect(')');
		node_if=parse_statement();
		if(peek()==SYM_ELSE)
		{
			pop();
			node_else=parse_statement();
		}
		else
			node_else=0;
		
		if(PARSER_DEBUG)
			fprintf(stderr,"parsed if statement\n");
		
		node=newnode(4);
		*node=SYM_IF;
		*(node+1)=exp;
		*(node+2)=node_if;
		*(node+3)=node_else;
	}
	
	return node;
	
}

int **parse_iteration_statement()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parse iteration-statement\n");
	
	int **node;
	node=0;
	
	if(peek()==SYM_WHILE)
	{
		int **exp;
		int **statement;
		pop();
		expect('(');
		exp=parse_expression();
		expect(')');
		statement=parse_statement();
		node=newnode(3);
		*node=SYM_WHILE;
		*(node+1)=exp;
		*(node+2)=statement;
	}
	else if(peek()==SYM_FOR)
	{
		node=newnode(5);
		*node=pop();
		expect('(');
		*(node+1)=parse_expression();
		expect(';');
		*(node+2)=parse_expression();
		expect(';');
		*(node+3)=parse_expression();
		expect(')');
		*(node+4)=parse_statement();
	}
	
	return node;
	
}

/*
	expression :=
		equality-expression
*/
int **parse_expression()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parse expression\n");
	
	return parse_assignment_expression();
	
}

/*
	assignment_expression :=
		logical-or-expression
		logical-or-expression = assignment_expression
*/
int **parse_assignment_expression()
{
	if(PARSER_DEBUG)
		fprintf(stderr,"parse assignment-expression\n");
	
	int **node;
	node=parse_logical_or_expression();
	if(peek()=='=')
	{
		int **ast;
		ast=newnode(3);
		*ast=pop();
		*(ast+1)=node;
		*(ast+2)=parse_assignment_expression();
		return ast;
	}
	else
		return node;
	
	
}

int **parse_logical_or_expression()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parse logical-or-expression\n");
	
	int **top;
	top=parse_logical_and_expression();
	while(peek()==SYM_LOR)
	{
		
		if(PARSER_DEBUG)
			fprintf(stderr,"parsed %d\n",peek());
		
		int **new;
		new=newnode(3);
		*new=pop();
		*(new+1)=top;
		*(new+2)=parse_logical_and_expression();
		top=new;
		
	}
	
	return top;
	
}

int **parse_logical_and_expression()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parse logical-and-expression\n");
	
	int **top;
	top=parse_equality_expression();
	while(peek()==SYM_LAND)
	{
		
		if(PARSER_DEBUG)
			fprintf(stderr,"parsed %d\n",peek());
		
		int **new;
		new=newnode(3);
		*new=pop();
		*(new+1)=top;
		*(new+2)=parse_equality_expression();
		top=new;
		
	}
	
	return top;
	
}


int **parse_equality_expression()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parse equality-expression\n");
	
	int **top;
	top=parse_comparison_expression();
	while((peek()==SYM_NE)||(peek()==SYM_EQ))
	{
		
		if(PARSER_DEBUG)
			fprintf(stderr,"parsed %d\n",peek());
		
		int **new;
		new=newnode(3);
		*new=pop();
		*(new+1)=top;
		*(new+2)=parse_comparison_expression();
		top=new;
		
	}
	
	return top;
	
}

int **parse_comparison_expression()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parse comparison-expression\n");
	
	int **top;
	top=parse_additive_expression();
	while((peek()=='>')||(peek()=='<')||(peek()==SYM_LE)||(peek()==SYM_GE))
	{
		
		if(PARSER_DEBUG)
			fprintf(stderr,"parsed %d\n",peek());
		
		int **new;
		new=newnode(3);
		*new=pop();
		*(new+1)=top;
		*(new+2)=parse_additive_expression();
		top=new;
		
	}
	
	return top;
	
}


/*
	additive-expression :=
		multiplicative-expression { (+|-) multiplicative-expression } + 
*/
int **parse_additive_expression()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parse additive-expression\n");
	
	int **top;
	top=parse_multiplicative_expression();
	while((peek()=='+')||(peek()=='-'))
	{
		
		if(PARSER_DEBUG)
			fprintf(stderr,"parsed %c\n",peek());
		
		int **new;
		new=newnode(3);
		*new=pop();
		*(new+1)=top;
		*(new+2)=parse_multiplicative_expression();
		top=new;
		
	}
	
	return top;
	
}

/*
	multiplicative-expression :=
		unary-expression { * unary-expression } + 
*/
int **parse_multiplicative_expression()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parse multiplicative-expression\n");
	
	int **top;
	top=parse_unary_expression();
	while(peek()=='*')
	{
		
		if(PARSER_DEBUG)
			fprintf(stderr,"parsed %c\n",peek());
		
		int **new;
		new=newnode(3);
		*new=pop();
		*(new+1)=top;
		*(new+2)=parse_unary_expression();
		top=new;
		
	}
	
	return top;
	
}



/*
	unary-expression :=
		- unary-expression
		* unary-expression
		& unary-expression
		postfix-expression
*/
int **parse_unary_expression()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parse unary-expression\n");
	
	int **ast;
	if(peek()=='-')
	{
		if(PARSER_DEBUG)
			fprintf(stderr,"parsed negation\n");
		
		pop();
		ast=newnode(2);
		*ast=SYM_NEGATE;
		*(ast+1)=parse_unary_expression();
	}
	else if(peek()=='*')
	{
		if(PARSER_DEBUG)
			fprintf(stderr,"parsed negation\n");
		
		pop();
		ast=newnode(2);
		*ast=SYM_POINTER;
		*(ast+1)=parse_unary_expression();
	}
	else if(peek()=='&')
	{
		if(PARSER_DEBUG)
			fprintf(stderr,"parsed negation\n");
		
		pop();
		ast=newnode(2);
		*ast=SYM_ADDRESS;
		*(ast+1)=parse_unary_expression();
	}
	else if(peek()==SYM_SIZEOF)
	{
		if(PARSER_DEBUG)
			fprintf(stderr,"parsed sizeof\n");
		
		int size=4;
		pop();
		expect('(');
		if(pop()==SYM_CHAR)
			size=1;
		while(peek()=='*')
		{
			size=4;
			pop();
		}
		expect(')');
		ast=newnode(2);
		*ast=SYM_CONSTANT;
		*(ast+1)=size;
	}
	else
		ast=parse_postfix_expression();
	
	return ast;
	
}

/*
	postfix-expression :=
		primary-expression { ( expression-list ) } +
*/		
int **parse_postfix_expression()
{
	if(PARSER_DEBUG)
		fprintf(stderr,"parse postfix-expression\n");
	
	int **top;
	top=parse_primary_expression();
	if(peek()=='(')
	{
		pop();
		if(PARSER_DEBUG)
			fprintf(stderr,"parsed %c\n",peek());
		
		int **new;
		new=newnode(3);
		*new=SYM_FUNC_CALL;
		*(new+1)=top;
		*(new+2)=parse_expression_list();
		top=new;
		expect(')');
		
	}
	
	return top;
	
}

int **parse_expression_list()
{
	int **list;
	if(PARSER_DEBUG)
		fprintf(stderr,"parse expression-list\n");
	if(peek()==')')
		return 0;
	else 
	{
		list=newnode(3);
		*list=SYM_EXPR_LIST;
		*(list+1)=parse_expression();
		if(peek()==',')
		{
			pop();
			*(list+2)=parse_expression_list();
		}
		else
			*(list+2)=0;
		return list;
	}
}

/*
	primary-expression :=
		( expression )
		CONSTANT
		ID
		STRING
*/
int **parse_primary_expression()
{
	
	if(PARSER_DEBUG)
		fprintf(stderr,"parsed primary-expression\n");
	
	int **node;
	if(peek()=='(')
	{
		pop();
		node=parse_expression();
		expect(')');
	}
	else if(peek()==SYM_ID)
	{
		node=newnode(2);
		*node=pop();
		*(node+1)=i_strdup(lexed_string);
	}
	else if(peek()==SYM_STRING)
	{
		node=newnode(2);
		*node=pop();
		*(node+1)=i_strdup(lexed_string);
	}
	else 
	{
		expect(SYM_CONSTANT);
		node=newnode(2);
		*node=SYM_CONSTANT;
		*(node+1)=lexed_number;
	}
	
	return node;
	
}
