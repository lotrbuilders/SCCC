/*
	Top level control for SCCC
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *i_strdup(char *str)
{
	int len;
	char *ptr;
	char *tmp;
	len=strlen(str); 
	ptr=malloc((len+4));
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

int strip_suffix(char *s)
{
	char *str=i_strdup(s);
	char *tmp=str;
	for(1;(*tmp!=0)&&(*tmp!='.');tmp=tmp+1) 1;
	*str=0;
	return str;
}

int main(int argc, char **argv)
{
	char *sccc_dir="/usr/local/lib/sccc/";
	char *input_file;
	char *stripped;
	char *output_file=0;
	int include_index=0;
	char **include_dirs=malloc(8*sizeof(char*));
	int max_process=-1;
	char *str;
	
	argv=argv+1;
	for(str=*argv;str!=0;str=*argv)
	{
		if(*str!='-')
		{
			input_file=str;
		}
		else
		{
			str=str+1;
			int c=*str;
			if(c=='o')
			{
				argv=argv+1;
				output_file=*argv;
			}
			else if((c=='c')||(c=='S'))
			{
				max_process=c;
			}
			else if(c=='I')
			{
				str=str+1;
				if(include_index>=8)
				{
					fprintf(stderr,"To many -I options\n");
					exit(-1);
				}
				*(include_dirs+include_index)=str;
				include_index=include_index+1;
			}
		}
		argv=argv+1;
	}
	if(input_file==0)
	{
		fprintf(stderr,"Expected an input file\n");
		return -1;
	}
	stripped=strip_suffix(input_file);
	if(output_file==0)
	{
		if((max_process=='c'))
			output_file=strcat(strip_suffix(input_file),".o");
		else if((max_process=='S'))
			output_file=strcat(strip_suffix(input_file),".S");
		else 
			output_file="a.out";
	}
	
	int return_code=0;
	char *cmd=malloc(1024);
	char *tmp=malloc(1024);
	int count=1024;
	count=count-snprintf(cmd,1024,"cpp %s -o /tmp/%s.ppc -I%sinclude/ -nostdinc -E",input_file,stripped,sccc_dir);
	int i;
	for(i=0;(i<include_index)&&(count>0);i=i+1)
	{
		count=count-snprintf(tmp,count,"%s -I%s",cmd,*(include_dirs+i));
		strcpy(cmd,tmp);
	}
	return_code=system(cmd);
	if(return_code)
		exit(return_code);
	
	if(max_process=='S')
		snprintf(cmd,1024,"%scompiler /tmp/%s.ppc -o %s",sccc_dir,stripped,output_file);
	else 
		snprintf(cmd,1024,"%scompiler /tmp/%s.ppc -o /tmp/%s.s",sccc_dir,stripped,stripped);
	return_code=system(cmd);
	if(return_code)
		exit(return_code);
	if(max_process=='S')
		return 0;
	
	if(max_process=='c')
		snprintf(cmd,1024,"nasm -felf -g -F dwarf -o %s /tmp/%s.s",output_file,stripped);
	else 
		snprintf(cmd,1024,"nasm -felf -g -F dwarf -o /tmp/%s.o /tmp/%s.s",stripped,stripped);
	return_code=system(cmd);
	if(return_code)
		exit(return_code);
	if(max_process=='c')
		return 0;
	
	snprintf(cmd,1024,"gcc -m32 /tmp/%s.o -o %s",stripped,output_file);
	return system(cmd);
	
}