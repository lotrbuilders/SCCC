# 1 "test.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "test.c"
int **a=0;

char *malloc(int size);

int main()
{
 a=malloc(sizeof(int *));
 *a=1;
 return *a;
}
