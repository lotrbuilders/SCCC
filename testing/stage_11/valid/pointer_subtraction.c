int *malloc(int size);

int main() 
{
	int *p=malloc(32);
	int i=0;
	for(i=0;i>-5;i=i-1)
		*(p-i)=i;
	int a=0;
	for(i=0;i>-5;i=i-1)
		a=a+*(p-i);
	return a;
}

