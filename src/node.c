#include <stdio.h>
#include <stdlib.h>

int **newnode(int size)
{
	
	int **ptr;
	ptr=calloc(size,sizeof(int *));
	if(ptr==0)
	{
		fputs("out of memory",stderr);
		exit(-1);
	}
	
	return ptr;
	
}