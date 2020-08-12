
int main() {
	int i=4;
	int *p=&i;
	int **pp=&p;
	**pp=8;
	return **pp;
}