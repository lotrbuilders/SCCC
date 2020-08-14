int main() {
    int a=3;
	int *p=&a;
	int **pp=&p;
	return **pp;
}

