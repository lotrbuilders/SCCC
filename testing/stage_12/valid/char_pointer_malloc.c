int puts(char *str);
char *malloc(int size);

int main()
{
	char *str=malloc(6);
	*str='H';
	*(str+1)='e';
	*(str+2)='l';
	*(str+3)='l';
	*(str+4)='o';
	*(str+5)=0;
	puts(str);
	return *(str+2);
}