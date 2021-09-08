#include <stdio.h>
#include <stdlib.h>
#include "primes2.h"

int main(int argc, char **argv) {
	int n = atoi(argv[1]);
	int *arr = primes(n);
	for (int i = 0; i < n; i++) {
		printf(i < n-1 ? "%d " : "%d\n", arr[i]);
	}
	return 0;
}