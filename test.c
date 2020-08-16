int exit(int status);

int foo()
{
	exit(-1);
}

int main() {
    foo()
}