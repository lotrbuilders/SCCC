#include "lexer.h"
#include "parser.h"
#include "symbols.h"
#include "eval.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *i_strdup(char *str)
{
	int len;
	char *ptr;
	char *tmp;
	len=strlen(str);
	ptr=malloc((len+1)*sizeof(char));
	tmp=ptr;
	while(*str!=0)
	{
		*tmp=*str;
		tmp=tmp+1;
		str=str+1;
	}
	*tmp=0;
	return ptr;
}

int main()
{
	lexed_string=malloc(32*sizeof(char));
	int **ast;
	/*while((tk=lex())!=SYM_EOF)
		printf("Token %d\n",tk);*/
	ast=parse();
	eval(ast);
	
	return 0;
}