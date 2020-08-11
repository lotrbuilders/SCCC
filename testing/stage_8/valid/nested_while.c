int main() {
    int a; 
	a = 1;

    while (a - 3 < 20) {
        int b;
		b = 1;
        while (b < 10)
            b = b*2;
        a = a + b;
    }

    return a;
}