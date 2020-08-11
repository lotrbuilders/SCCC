int main() {
    int a;
	a = 2;
    int b;
	b = 3;
    {
        int a;
		a = 1;
        b = b + a;
    }
    return b;
}