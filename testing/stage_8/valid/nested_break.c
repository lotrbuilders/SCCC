int main() {
	int i;
	int j;
    int ans;
	ans = 0;
    for (i = 0; i < 10; i = i + 1)
        for (j = 0; j < 10; j = j + 1)
                ans = ans + i * j;
    return ans;
}