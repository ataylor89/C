#include <stdio.h>
#include <stdlib.h>
#include "primes2.h"

int main(int argc, char **argv) {
	int n = atoi(argv[1]);
	int *arr = primes(n);
	char *str = itos(arr, n, " ");
	puts(str);
	return 0;
}