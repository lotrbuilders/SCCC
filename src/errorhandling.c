#include <stdio.h>
#include <stdlib.h>

int error(char *error_msg)
{
	fprintf(stderr,"Error: %s\n",error_msg);
	exit(1);
}

int debug_warning(char *error_msg)
{
	fprintf(stderr,"Debug warning: %s\n",error_msg);
	return 0;
}