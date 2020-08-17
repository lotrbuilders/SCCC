/*
	x86-32 backend for SCCC
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
#include <stdio.h>
#include <string.h>
#include "symbols.h"

int in_main;

int TARGET_SIZEOF_INT=4;
int TARGET_ARGUMENT_START=-12;
int TARGET_VARIABLE_START=8;
int TARGET_STACK_RESET=4;

int **output_file;

int gen_prolog()
{
	fputs("section .data\n",output_file);
	return 0;
}

int gen_function_prolog(char *name)
{
	fputs("\nsection .text\n",output_file);
	fprintf(output_file,"global %s:function (%s.end-%s)\n%s:\n",name,name,name,name);
	
	fputs("\tpush ebp\n",output_file);
	fputs("\tmov ebp,esp\n",output_file);
	fputs("\tpush ebx\n",output_file);
	return 0;
	
}

int gen_function_epilog(char *name)
{
	fputs(".end:\n\n",output_file);
	fputs("section .data\n",output_file);
	return 0;
	
}

int gen_extern(char *name)
{
	fprintf(output_file,"extern %s\n",name);
	return 0;
}

int gen_common(char *name)
{
	fprintf(output_file,"common %s 4:4\n",name);
	return 0;
}
int gen_decleration(char *name,int value)
{
	fprintf(output_file,"global %s\n",name);
	fprintf(output_file,"%s: dd %d\n",name,value);
	return 0;
}

int gen_label(int label_count)
{
	
	fprintf(output_file,".L%d:\n",label_count);
	return 0;
	
}

int gen_subtract_sp(int count)
{
	
	fprintf(output_file,"\tadd esp,%d\n",count);
	return 0;
	
}

int gen_pop()
{
	
	fputs("\tpop eax\n",output_file);
	return 0;
	
}

int gen_push_constant(int constant)
{
	fprintf(output_file,"\tpush %d\n",constant);
	return 0;
}

int gen_dup()
{
	fputs("\tpush eax\n",output_file);
	return 0;
}

int gen_normalize()
{
	
	fputs("\tor eax,eax\n",output_file);
	fputs("\tsetnz al\n",output_file);
	fputs("\tmovzx eax,al\n",output_file);
	return 0;
	
}

int gen_return()
{
	
	/*if(in_main==0)
	{
		fputs("\tmov ebx,[ebp-4]\n",output_file);
		fputs("\tmov esp,ebp\n",output_file);
		fputs("\tpop ebp\n",output_file);
		fputs("\tret\n",output_file);
	}
	else
	{
		fputs("\tmov ebx,[ebp-12]\n",output_file);
		fputs("\tmov ecx, DWORD[ebp-4]\n",output_file);
        fputs("\tleave\n",output_file);
        fputs("\tlea esp, [ecx-4]\n",output_file);
        fputs("\tret\n",output_file);
	}*/
	fputs("\tmov ebx,[ebp-4]\n",output_file);
	fputs("\tmov esp,ebp\n",output_file);
	fputs("\tpop ebp\n",output_file);
	fputs("\tret\n",output_file);
	return 0;
	
}

int gen_jz(int label_count)
{
	
	fputs("\tor eax,eax\n",output_file);
	fprintf(output_file,"\tjz .L%d\n",label_count);
	return 0;
	
}

int gen_jnz(int label_count)
{
	
	fputs("\tor eax,eax\n",output_file);
	fprintf(output_file,"\tjnz .L%d\n",label_count);
	return 0;
	
}

int gen_jmp(int label_count)
{
	
	fprintf(output_file,"\tjmp .L%d\n",label_count);
	return 0;
	
}

int gen_assign(int size)
{
	fputs("\tmov ebx,eax\n",output_file);
	fputs("\tpop eax\n",output_file);
	if(size==4)
		fputs("\tmov [ebx],eax\n",output_file);
	else if(size==1)
		fputs("\tmov [ebx],al\n",output_file);
	return 0;
}

int gen_comparison(int type)
{
	
	fputs("\tpop ecx\n",output_file);
	fputs("\tcmp eax,ecx\n",output_file);
	
	if(type==SYM_EQ)
		fputs("\tsete al\n",output_file);
	else if(type==SYM_NE)
		fputs("\tsetne al\n",output_file);
	else if(type=='>')
		fputs("\tsetg al\n",output_file);
	else if(type=='<')
		fputs("\tsetl al\n",output_file);
	else if(type==SYM_GE)
		fputs("\tsetge al\n",output_file);
	else
		fputs("\tsetle al\n",output_file);
	
	fputs("\tmovzx eax,al\n",output_file);
	
	return 0;
	
}

int gen_add()
{
	
	fputs("\tpop ecx\n",output_file);
	fputs("\tadd eax,ecx\n",output_file);
	return 0;
	
}

int gen_subtract()
{
	
	fputs("\tpop ecx\n",output_file);
	fputs("\tsub eax,ecx\n",output_file);
	return 0;
	
}

int gen_multiply()
{
	
	fputs("\tpop ecx\n",output_file);
	fputs("\timul ecx\n",output_file);
	return 0;
	
}

int gen_negate()
{
	
	fputs("\tneg eax\n",output_file);
	return 0;
	
}

int gen_pointer(int size)
{
	if(size==4)
		fputs("\tmov eax,[eax]\n",output_file);
	else if(size==1)
		fputs("\tmovsx eax,BYTE [eax]\n",output_file);
	return 0;
	
}

int gen_constant(int constant)
{
	
	fputs("\tpush eax\n",output_file);
	fprintf(output_file,"\tmov eax,%d\n",constant);
	return 0;
	
}

int gen_local_var(int location,int size)
{
	fputs("\tpush eax\n",output_file);
	if(size==4)
		fprintf(output_file,"\tmov eax,[ebp-%d]\n",location+4);
	else if(size==1)
		fprintf(output_file,"\tmovsx eax,BYTE [ebp-%d]\n",location+4);
	return 0;
}


int gen_global_var(char *name,int size)
{
	fputs("\tpush eax\n",output_file);
	if(size==4)
		fprintf(output_file,"\tmov eax,[%s]\n",name);
	else if(size==1)
		fprintf(output_file,"\tmovsx eax,BYTE [%s]\n",name);
	
	return 0;
}

int gen_local_address(int location)
{
	fputs("\tpush eax\n",output_file);
	fprintf(output_file,"\tlea eax,[ebp-%d]\n",location+4);
	return 0;
}

int gen_global_address(char *name)
{
	fputs("\tpush eax\n",output_file);
	fprintf(output_file,"\tlea eax,[%s]\n",name);
	return 0;
}

int gen_func_arg()
{
	fputs("\tpush eax\n",output_file);
	return 0;
}

int gen_function_call(char *name)
{
	fprintf(output_file,"\tcall %s\n",name);
	return 0;
}

int gen_padding(int argc,int stack_loc)
{
	int padding;
	int sp;
	sp=4*argc+stack_loc;
	for(padding=sp;padding>=16;padding=padding-16) 1;
	fprintf(output_file,"\tsub esp,%d\n",padding);
	return padding;
}

int gen_string(char *str,int count)
{
	fprintf(output_file,".string%d:\n",count);
	while(*str!=0)
	{
		fprintf(output_file,"\tdb %d\n",*str);
		str=str+1; 
	}
	fprintf(output_file,"\tdb 0\n\n",output_file);
	return 0;
}

int gen_load_string(int count)
{
	fputs("\tpush eax\n",output_file);
	fprintf(output_file,"\tmov eax,.string%d\n",count);
	return 0;
}