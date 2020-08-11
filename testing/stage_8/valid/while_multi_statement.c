int main() {
    int a;
	a = 0;
    int b;
	b = 1;

    while (a < 5) {
        a = a + 2;
        b = b * a;
    }

    return a;
}