int puts(char *str);
char *malloc(int size);

char *foo()
{
	return "Hello World";
}

int main()
{
	return puts(foo());
}