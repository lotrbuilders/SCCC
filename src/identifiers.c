#include "node.h"
#include "symbols.h"
#include <stdlib.h>
#include <string.h>

int **local_block=0;
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
	
	int **new_block;
	new_block=newnode(2);
	*new_block=0;
	*(new_block+1)=local_block;
	local_block=new_block;
	return 0;
	
}

int leave_block()
{
	
	int count;
	count=free_locals(*local_block);
	stack_loc=stack_loc-count;
	local_block=*(local_block+1);
	return count;
	
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

int free_locals(int **local_list)
{
	if(local_list!=0)
	{
		int **next=*(local_list+1);
		
		free(*local_list);
		free(local_list);
		return free_locals(next)+4;
	}
	
	return 0;
	
}

int isvalid(char *name)
{
	int **list;
	int **block;
	for(block=local_block;block!=0;block=*(block+1))
		for(list=*block;list!=0;list=*(list+1))
		{
			if(strcmp(name,*list)==0)
				return 1;
		}
	return 0;
}

int isglobal(char *name)
{
	return 0;
}

int islocal(char *name)
{
	int **list;
	int **block;
	for(block=local_block;block!=0;block=*(block+1))
		for(list=*block;list!=0;list=*(list+1))
		{
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
