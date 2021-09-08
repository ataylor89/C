#include "primes2.h"

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

int *primes(int n) {
	int *primesarr = malloc(sizeof(int)*n);
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
			primesarr[count]=i;
			count++; 
		}
	}		

    	return primesarr;
}