#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include "sm4.h"

void generate_random_data(unsigned char *buffer, size_t size) {
	for (size_t i = 0; i < size; i++) {
		buffer[i] = rand() % 256;
	}
}

int main() {
	const uint32_t rk[32] = {
		0xf12186f9, 0x41662b61, 0x5a6ab19a, 0x7ba92077,
		0x367360f4, 0x776a0c61, 0xb6bb89b3, 0x24763151,
		0xa520307c, 0xb7584dbd, 0xc30753ed, 0x7ee55b57,
		0x6988608c, 0x30d895b7, 0x44ba14af, 0x104495a1,
		0xd120b428, 0x73b55fa3, 0xcc874966, 0x92244439,
		0xe89e641f, 0x98ca015a, 0xc7159060, 0x99e1fd2e,
		0xb79bd80c, 0x1d2115b0, 0x0e228aeb, 0xf1780c81,
		0x428d3654, 0x62293496, 0x01cf72e5, 0x9124a012,
	};
	size_t test_sizes[] = { 128, 1024, 1024 * 1024, 5 * 1024 * 1024 };
	int num_tests = 10;
	srand((unsigned int)time(NULL));
	for (int i = 0; i < sizeof(test_sizes) / sizeof(size_t); i++) {
		size_t size = test_sizes[i];
		double total_time1 = 0;
		double total_time2 = 0;

		for (int j = 0; j < num_tests; j++) {
			unsigned char *input = malloc(size);
			generate_random_data(input, size);
			uint8_t ctr[16];
			generate_random_data(ctr, 16);
			unsigned char *output = malloc(size);
			clock_t start1 = clock();
			sm4_ctr_encrypt(rk, ctr, input, size, output);
			clock_t end1 = clock();
			total_time1 += (double)(end1 - start1) / CLOCKS_PER_SEC;
			free(output);

			output = malloc(size);
			clock_t start2 = clock();
			sm4_ctr_encrypt2(rk, ctr, input, size, output);
			clock_t end2 = clock();
			total_time2 += (double)(end2 - start2) / CLOCKS_PER_SEC;
			free(input);
			free(output);
		}

		double avg_time1 = total_time1 / num_tests;
		double avg_time2 = total_time1 / num_tests;
		printf("Data size: %zu bytes, Average Time1: %f seconds, Average Time2: %f seconds\n", size, avg_time1, avg_time2);
	}
	return 0;
}