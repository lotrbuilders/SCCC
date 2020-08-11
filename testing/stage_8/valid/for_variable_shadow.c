int main() {
    int i;
    int j;
	j = 0;
    for (i = 0; i < 10; i = i + 1) {
        int k;
		k = i;
		int i;
        for (i = k; i < 10; i = i + 1)
            j = j + 1;
    }
    return j + i;
}