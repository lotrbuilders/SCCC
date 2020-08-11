int main() {
    int a;
	a = 0;
    if (a) {
        int b;
		b = 2;
        return b;
    } else {
        int c;
		c = 3;
        if (a < c) {
            return 4;
        } else {
            return 5;
        }
    }
    return a;
}