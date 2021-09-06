#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int n = 10;
	if (argc == 2)
		n = atoi(argv[1]);
	if (n == 0)
		n = 10;

	int m = 100;
	if (n > 20)
		m = 500;
	if (n > 50)
		m = 1000;
    	if (n > 100)
        		m = 10000;
	if (n > 500)
		m = 100000;
    	if (n > 750)
        		m = 1000000;

	int sieve[m];
	sieve[0] = 0;
	sieve[1] = 0;	
	for (int i = 2; i < m; i++)
		sieve[i] = 1;

	for (int i = 2; i < m; i++) {
		if (sieve[i] == 1) {
			for (int j = 2; i * j < m; j++) {
				sieve[i*j] = 0;
			}
		}
	}

	int count = 0;
	for (int i = 0; i < m && count < n; i++) {
		if (sieve[i] == 1) {
			printf(count == n - 1 ? "%d\n" : "%d ", i);
			count++;
		}
	}
    	printf("Printed first %d primes\n", count);
	return 0;
}
