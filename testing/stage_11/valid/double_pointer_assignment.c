int main() {
    int a=3;
	int *p=&a;
	int **pp=&p;
	**pp=8;
	return **pp;
}

