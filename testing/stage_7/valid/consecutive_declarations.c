int main() {
    int a;
	a = 0;
    {
        int b;
		b = 1;
        a = b;
    }
    {
        int b;
		b= 2;
        a = a + b;
    }
    return a;
}