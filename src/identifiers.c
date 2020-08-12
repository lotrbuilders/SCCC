#include "node.h"
#include "symbols.h"
#include "identifiers.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int **local_block=0;
int **global_list=0;
int stack_loc=0;

int free_locals(int **local_list);
char *i_strdup(char *str);

/*
	Local block aka stackframe exist out of:
		pointer to list of locals
		pointer to next(upper) stackframe
*/

int enter_block()
{
	fprintf(stderr,"enter block\n");
	int **new_block;
	new_block=newnode(2);
	*new_block=0;
	*(new_block+1)=local_block;
	local_block=new_block;
	return 0;
	
}

int leave_block(int reset_stack)
{
	fprintf(stderr,"leave block:\n");
	int count;
	count=free_locals(*local_block);
	stack_loc=stack_loc-count;
	if(reset_stack)
		stack_loc=0;
	local_block=*(local_block+1);
	return count;
	
}

int add_global(char *name,int type)
{
	fprintf(stderr,"added global identifier %s\n",name);
	int **entry;
	entry=newnode(4);
	*entry=i_strdup(name);
	*(entry+1)=global_list;
	*(entry+2)=type;
	global_list=entry;
	return 0;
}
/* 
	Local list exists out of:
		Pointer to identifier name
		Next entry in list
		Type of identifier
		Location on stack
*/
int add_identifier(char *name,int type)
{
	fprintf(stderr,"added identifier %s at %d\n",name,stack_loc);
	int **entry;
	entry=newnode(4);
	*entry=i_strdup(name);
	*(entry+1)=*local_block;
	*(entry+2)=type;
	*(entry+3)=stack_loc;
	stack_loc+=4;
	*local_block=entry;
	return 0;
}

int add_argument_identifier(char *name,int type,int loc)
{
	fprintf(stderr,"added identifier %s as function argument at%d\n",name,loc);
	int **entry;
	entry=newnode(4);
	*entry=i_strdup(name);
	*(entry+1)=*local_block;
	*(entry+2)=type;
	*(entry+3)=loc;
	*local_block=entry;
	return 0;
}

int free_locals(int **local_list)
{
	if(local_list!=0)
	{
		int **next=*(local_list+1);
		fprintf(stderr,"\tremoved %s\n",*local_list);
		free(*local_list);
		free(local_list);
		return free_locals(next)+4;
	}
	
	return 0;
	
}

int isvalid(char *name)
{
	return islocal(name)||isglobal(name);
}

int isglobal(char *name)
{
	int **list;
	for(list=global_list;list!=0;list=*(list+1))
	{
		fprintf(stderr,"\tcompare globals %s & %s\n",*list,name);
		if(strcmp(name,*list)==0)
			return 1;
	}
	return 0;
}

int islocal(char *name)
{
	int **list;
	int **block;
	for(block=local_block;block!=0;block=*(block+1))
		for(list=*block;list!=0;list=*(list+1))
		{
			fprintf(stderr,"\tcompare locals %s & %s\n",*list,name);
			if(strcmp(name,*list)==0)
				return 1;
		}
	return 0;
}

int locate_identifier(char *name)
{
	
	int **list;
	int **block;
	for(block=local_block;block!=0;block=*(block+1))
		for(list=*block;list!=0;list=*(list+1))
		{
			if(strcmp(name,*list)==0)
				return *(list+3);
		}
	return 0;
}
