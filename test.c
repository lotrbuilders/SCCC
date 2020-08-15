int a()
{
	if(1)
		return 2;
	else return *"Hello";
}

int main()
{
	if(2)
		return a();
	else return *"bye";
}