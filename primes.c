#include <stdio.h>
#include <stdlib.h>

int* sieve(int size) {
	int *arr = malloc(sizeof(int) * size);
	arr[0] = 0;
	arr[1] = 0;	
	for (int i = 2; i < size; i++)
		arr[i] = 1;

	for (int i = 2; i < size; i++) {
		if (arr[i] == 1) {
			for (int j = 2; i * j < size; j++) {
				arr[i*j] = 0;
			}
		}
	}

	return arr;
}

int main(int argc, char **argv) {
    	int n = atoi(argv[1]);
	int size = n * 100;
	int *arr = sieve(size);
	int count = 0;

	while (count < n) {
		for (int i = 0; i < size; i++) {
			if (arr[i] == 1) {
				count++;
			}
		}
		if (count < n) {
			size *= 100;
			free(arr);
			arr = sieve(size);
			count = 0;
		}	
	}

	count = 0;
	for (int i = 0; i < size && count < n; i++) {
		if (arr[i] == 1) {
			printf(count < n - 1 ? "%d " : "%d\n", i);
			count++; 
		}
	}		

    	printf("Printed first %d primes\n", n);
	return 0;
}
