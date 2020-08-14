int a=3;

int *foo()
{
	return &a;
}

int main() 
{
	return *foo();
}

