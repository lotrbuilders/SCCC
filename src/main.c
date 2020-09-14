/*
	Starting point for SCCC
    Copyright (C) 2020  Daan Oosterveld

    SCCC is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
	any later version.

    SCCC is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "lexer.h"
#include "parser.h"
#include "symbols.h"
#include "eval.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int error(char *str);

int gen_prolog();

int *output_file;
int *input_file;

char *i_strdup(char *str)
{
	int len;
	char *ptr;
	char *tmp;
	len=strlen(str); 
	ptr=malloc(len+2);
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

int arg_parser(int argc, char **argv)
{
	char *str;
	char **arg=argv+1;
	output_file=stdout;
	input_file=stdin;
	for(str=*arg;str!=0;str=*arg)
	{
		if(strcmp(str,"-o")==0)
		{
			arg=arg+1;
			if(*arg==0)
				error("Expected output file after \"-o\"");
			output_file=fopen(*arg,"w+");
			if(output_file==0)
				error("Output file does not exist");
			arg=arg+1;
		}
		else
		{
			input_file=fopen(str,"r");
			if(input_file==0)
				error("Input file does not exist");
			arg=arg+1;
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	lexed_string=malloc(128);
	arg_parser(argc,argv);
	
	int **ast;
	ast=parse();
	gen_prolog();
	eval(ast);
	
	return 0; 
}