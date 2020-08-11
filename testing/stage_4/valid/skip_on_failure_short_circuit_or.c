int main() {
    int a;
	a = 1;
    int b;
	b = 0;
    a || (b = 5);
    return b;
}