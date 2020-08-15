int **a=0;

char *malloc(int size);

int main()
{
	a=malloc(sizeof(int *));
	*a=1;
	return *a;
}