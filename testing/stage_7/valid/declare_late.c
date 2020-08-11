int main() {
    int a;
	a = 2;
    {
        a = 3;
        int a;
		a = 0;
    }
    return a;
}