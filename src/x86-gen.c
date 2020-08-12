#include <stdio.h>
#include "symbols.h"

int gen_function_prolog(char *name)
{
	
	printf("global %s:function (%s.end-%s)\n%s:\n",name,name,name,name);
	puts("\tpush ebp");
	puts("\tmov ebp,esp");
	return 0;
	
}

int gen_function_epilog()
{
	
	puts(".end:");
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
	
	puts("\tmov esp,ebp");
	puts("\tpop ebp");
	puts("\tret");
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

int gen_assign()
{
	puts("\tmov ebx,eax");
	puts("\tmov eax,[esp]");
	puts("\tmov [ebx],eax");
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

int gen_pointer()
{
	
	puts("\tmov eax,[eax]");
	return 0;
	
}

int gen_constant(int constant)
{
	
	puts("\tpush eax");
	printf("\tmov eax,%d\n",constant);
	return 0;
	
}

int gen_local_var(int location)
{
	puts("\tpush eax");
	printf("\tmov eax,[ebp-%d]\n",location+4);
	return 0;
}

int gen_global_var(char *name)
{
	puts("\tpush eax");
	printf("\tmov eax,[%s]\n",name);
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