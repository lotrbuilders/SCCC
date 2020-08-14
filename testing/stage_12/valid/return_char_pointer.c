int puts(char *str);
char *malloc(int size);

char *foo()
{
	return malloc(8);
}

int main()
{
	char *str=foo();
	*(str+0)='H';
	*(str+1)='e';
	*(str+2)='l';
	*(str+3)='l';
	*(str+4)='o';
	*(str+5)=0;
	puts(str);
	return *(str+2);
}