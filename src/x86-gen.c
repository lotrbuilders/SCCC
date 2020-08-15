#include <stdio.h>
#include <string.h>
#include "symbols.h"

int in_main;

int gen_prolog()
{
	puts("section .data");
	return 0;
}

int gen_function_prolog(char *name)
{
	puts("\nsection .text");
	printf("global %s:function (%s.end-%s)\n%s:\n",name,name,name,name);
	
	if(strcmp("main",name)==0)
	{
		puts("\tlea ecx, [esp+4]");
        puts("\tand esp, -16");
        puts("\tpush DWORD[ecx-4]");
        puts("\tpush ebp");
        puts("\tmov ebp, esp");
        puts("\tpush ecx");
        puts("\tsub esp, 4");
		in_main=1;
	}
	else
	{
		puts("\tpush ebp");
		puts("\tmov ebp,esp");
		in_main=0;
	}
	puts("\tpush ebx");
	return 0;
	
}

int gen_function_epilog(char *name)
{
	puts(".end:\n");
	puts("section .data");
	return 0;
	
}

int gen_extern(char *name)
{
	printf("extern %s\n",name);
	return 0;
}

int gen_common(char *name)
{
	printf("common %s 4:4\n",name);
	return 0;
}
int gen_decleration(char *name,int value)
{
	printf("global %s\n",name);
	printf("%s: dd %d\n",name,value);
	return 0;
}

int gen_label(int label_count)
{
	
	printf(".L%d:\n",label_count);
	return 0;
	
}

int gen_subtract_sp(int count)
{
	
	printf("\tadd esp,%d\n",count);
	return 0;
	
}

int gen_pop()
{
	
	puts("\tpop eax");
	return 0;
	
}

int gen_push_constant(int constant)
{
	printf("\tpush %d\n",constant);
	return 0;
}

int gen_dup()
{
	puts("\tpush eax");
	return 0;
}

int gen_normalize()
{
	
	puts("\tor eax,eax");
	puts("\tsetnz al");
	puts("\tmovzx eax,al");
	return 0;
	
}

int gen_return()
{
	
	if(in_main==0)
	{
		puts("\tmov ebx,[ebp-4]");
		puts("\tmov esp,ebp");
		puts("\tpop ebp");
		puts("\tret");
	}
	else
	{
		puts("\tmov ebx,[ebp-12]");
		puts("\tmov ecx, DWORD[ebp-4]");
        puts("\tleave");
        puts("\tlea esp, [ecx-4]");
        puts("\tret");
	}
	return 0;
	
}

int gen_jz(int label_count)
{
	
	puts("\tor eax,eax");
	printf("\tjz .L%d\n",label_count);
	return 0;
	
}

int gen_jnz(int label_count)
{
	
	puts("\tor eax,eax");
	printf("\tjnz .L%d\n",label_count);
	return 0;
	
}

int gen_jmp(int label_count)
{
	
	printf("\tjmp .L%d\n",label_count);
	return 0;
	
}

int gen_assign(int size)
{
	puts("\tmov ebx,eax");
	puts("\tpop eax");
	if(size==4)
		puts("\tmov [ebx],eax");
	else if(size==1)
		puts("\tmov [ebx],al");
	return 0;
}

int gen_comparison(int type)
{
	
	puts("\tpop ecx");
	puts("\tcmp eax,ecx");
	
	if(type==SYM_EQ)
		puts("\tsete al");
	else if(type==SYM_NE)
		puts("\tsetne al");
	else if(type=='>')
		puts("\tsetg al");
	else if(type=='<')
		puts("\tsetl al");
	else if(type==SYM_GE)
		puts("\tsetge al");
	else
		puts("\tsetle al");
	
	puts("\tmovzx eax,al");
	
	return 0;
	
}

int gen_add()
{
	
	puts("\tpop ecx");
	puts("\tadd eax,ecx");
	return 0;
	
}

int gen_subtract()
{
	
	puts("\tpop ecx");
	puts("\tsub eax,ecx");
	return 0;
	
}

int gen_multiply()
{
	
	puts("\tpop ecx");
	puts("\timul ecx");
	return 0;
	
}

int gen_negate()
{
	
	puts("\tneg eax");
	return 0;
	
}

int gen_pointer(int size)
{
	if(size==4)
		puts("\tmov eax,[eax]");
	else if(size==1)
		puts("\tmovsx eax,BYTE [eax]");
	return 0;
	
}

int gen_constant(int constant)
{
	
	puts("\tpush eax");
	printf("\tmov eax,%d\n",constant);
	return 0;
	
}

int gen_local_var(int location,int size)
{
	puts("\tpush eax");
	if(size==4)
		printf("\tmov eax,[ebp-%d]\n",location+4);
	else if(size==1)
		printf("\tmovsx eax,BYTE [ebp-%d]\n",location+4);
	return 0;
}


int gen_global_var(char *name,int size)
{
	puts("\tpush eax");
	if(size==4)
		printf("\tmov eax,[%s]\n",name);
	else if(size==1)
		printf("\tmovsx eax,BYTE [%s]\n",name);
	
	return 0;
}

int gen_local_address(int location)
{
	puts("\tpush eax");
	printf("\tlea eax,[ebp-%d]\n",location+4);
	return 0;
}

int gen_global_address(char *name)
{
	puts("\tpush eax");
	printf("\tlea eax,[%s]\n",name);
	return 0;
}

int gen_function_call(char *name)
{
	printf("\tcall %s\n",name);
	return 0;
}

int gen_string(char *str,int count)
{
	printf(".string%d:\n",count);
	while(*str!=0)
	{
		printf("\tdb %d\n",*str);
		str=str+1;
	}
	printf("\tdb 0\n");
	return 0;
}

int gen_load_string(int count)
{
	puts("\tpush eax");
	printf("\tmov eax,.string%d\n",count);
	return 0;
}