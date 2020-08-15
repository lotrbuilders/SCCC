#include "lexer.h"
#include "parser.h"
#include "symbols.h"
#include "eval.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int error(char *str);

int gen_prolog();

char *i_strdup(char *str)
{
	int len;
	char *ptr;
	char *tmp;
	len=strlen(str); 
	ptr=malloc((len+1));
	tmp=ptr;
	while(*str!=0)
	{
		*tmp=*str;
		tmp=tmp+1;
		str=str+1;
	}
	*tmp=0;
	//fprintf(stderr,"i-strdup %s\n",ptr); 
	return ptr;
}  

int main()
{
	//error("hello world");
	lexed_string=malloc(128);
	int **ast;
	/*while((tk=lex())!=SYM_EOF)
		printf("Token %d\n",tk);*/
	ast=parse();
	gen_prolog();
	eval(ast);
	
	return 0; 
} 