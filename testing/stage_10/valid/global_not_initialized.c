int foo;

int main() {
	int i;
    for (i = 0; i < 3; i = i + 1)
        foo = foo + 1;
    return foo;
}