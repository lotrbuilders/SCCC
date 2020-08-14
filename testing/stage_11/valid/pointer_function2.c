int foo(int *p)
{
	*p=*p**p;
	return 0;
}

int main() 
{
	int a=3;
	foo(&a);
	return a;
}

