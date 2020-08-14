#include "lexer.h"
#include "symbols.h"
#include "errorhandling.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int lex_id(int c);
int lex_num(int c);
int lex_punct(int c);
int lex_char();
int lex_string();

int LEXER_DEBUG=0;

int nextc()
{
	return fgetc(stdin);
}

int peekc()
{
	int c;
	c=fgetc(stdin);
	ungetc(c,stdin);
	return c;
}


int lex()
{
	
	int c;
	
	for(c=nextc();1;c=nextc())
	{
		if(isspace(c))
			;
		else if(isalpha(c)||(c=='_'))
			return lex_id(c);
		else if(isdigit(c))
			return lex_num(c);
		else if(c=='\'')
			return lex_char();
		else if(c=='"')
			return lex_string();
		else if(ispunct(c))
			return lex_punct(c);
		else if(c==EOF)
			return SYM_EOF;
		else
			error("lexer found unknown character");
	}
	
}

int lex_id(int c)
{
	
	int i;
	
	for(i=0;(isalnum(c)||(c=='_'))&&(i<31);i=i+1)
	{
		*(lexed_string+i)=c;
		c=nextc();
	}
	ungetc(c,stdin);
	*(lexed_string+(i))=0;
	if(LEXER_DEBUG)
		fprintf(stderr,"\tid=%s\n",lexed_string);
	if(strcmp(lexed_string,"int")==0)
		return SYM_INT;
	else if(strcmp(lexed_string,"char")==0)
		return SYM_CHAR;
	else if(strcmp(lexed_string,"if")==0)
		return SYM_IF;
	else if(strcmp(lexed_string,"else")==0)
		return SYM_ELSE;
	else if(strcmp(lexed_string,"for")==0)
		return SYM_FOR;
	else if(strcmp(lexed_string,"while")==0)
		return SYM_WHILE;
	else if(strcmp(lexed_string,"return")==0)
		return SYM_RETURN;
	else if(strcmp(lexed_string,"sizeof")==0)
		return SYM_SIZEOF;
	else 
	{
		return SYM_ID;
	}
	
}

int lex_num(int c)
{
	
	lexed_number=0;
	
	while(isdigit(c))
	{
		lexed_number=lexed_number*10+c-'0';
		c=nextc();
	}
	ungetc(c,stdin);
	//printf("lexed number %d\n",lexed_number);
	return SYM_CONSTANT;
	
}

int lex_punct(int c)
{
	if((c==';')||(c=='(')||(c==')')||(c=='{')||(c=='}')||(c=='-')||(c=='+')||(c=='*')||(c==',')||(c=='*'))
		return c;
	else if(c=='<')
	{
		if(peekc()=='=') {nextc();return SYM_LE;}
		else return c;
	}
	else if(c=='>')
	{
		if(peekc()=='=') {nextc();return SYM_GE;}
		else return c;
	}
	else if(c=='!')
	{
		if(peekc()=='=') {nextc();return SYM_NE;}
		else return c;
	}
	else if(c=='=')
	{
		if(peekc()=='=') {nextc();return SYM_EQ;}
		else return c;
	}
	else if(c=='|')
	{
		if(peekc()=='|') {nextc();return SYM_LOR;}
		else return c;
	}
	else if(c=='&')
	{
		if(peekc()=='&') {nextc();return SYM_LAND;}
		else return c;
	}	
	else
		error("unknown char whilst parsing punctuation");
	
}

int lex_char()
{
	int c;
	c=nextc();
	if(c=='\\')
	{
		c=nextc();
		if(c=='n')
			lexed_number='\n';
		else if(c=='\\')
			lexed_number=c;
		else if(c=='\'')
			lexed_number=c;
		else
			error("unknown char whilst parsing character constant after \\ mark");
	}
	else
		lexed_number=c;
	
	c=nextc();
	if(c!='\'')
		error("no \"'\" after character constant");
	
	return SYM_CONSTANT;
	
}

int lex_string()
{
	int c;
	int i=0;
	for(c=nextc();(c!='"')&&(i<31);c=nextc())
	{
		if(c=='\\')
		{
			c=nextc();
			if(c=='n')
				*(lexed_string+i)='\n';
			else if(c=='t')
				*(lexed_string+i)='\t';
			else if(c=='\\')
				*(lexed_string+i)=c;
			else if(c=='"')
				*(lexed_string+i)=c;
			else
				error("unknown char whilst parsing character constant after \\ mark");
		}
		else 
			*(lexed_string+i)=c;
		i=i+1;
	}
	return SYM_STRING;
}

