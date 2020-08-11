int main() {
    int i;
    int j;
	j = 0;

    for (i = 100; i > 0; i = i - 1) {
        int j;
		j=i;
        j = j * 2 + i;
    }

    int k;
	k = 3;

    return j + k;
}