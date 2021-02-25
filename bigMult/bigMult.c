#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <limits.h>

uint64_t add64(uint64_t dest[], uint64_t src1[], uint64_t src2[], int sz) {
	int i;
	uint64_t c = 0;
	for (i = 0; i < sz; i++) {
		dest[i] = src1[i] + src2[i] + c;
		c = (dest[i] < src1[i] || dest[i] < src2[i] || dest[i] < c);
	}
	return c;
}

uint32_t addto32(uint32_t as[], int sz_a, uint32_t bs[], int sz_b) {
	// Assume that sz_b <= sz_a
	// Compute as += bs
	int i;
	uint32_t c = 0;
	uint64_t s;

	for (i=0; i< sz_b; i++) {
		s = (uint64_t) as[i] + (uint64_t) bs[i] + (uint64_t) c; // s is a 33 bit value
		c = s >> 32;
		as[i] = (uint32_t) s;
	}
	for ( ; i< sz_a; i++) {
		s = (uint64_t) as[i] + (uint64_t) c; // s is a 33 bit value
		c = s >> 32;
		as[i] = (uint32_t) s;
	}
	return c;
}

void twocomp(uint32_t as[], int sz_a, uint32_t bs[]) {
	int i;
	uint32_t one[1] = {0x1};
	uint32_t c = 1;
	uint64_t s;
	for (i = 0; i < sz_a; i++) {
		as[i] = ~bs[i];
		c = s >> 32;
		as[i] = (uint32_t) s;
	}
}

uint64_t get_lsw(uint64_t x) {
	return x & 0xffffffff;
}

uint64_t get_hsw(uint64_t x) {
	return x >> 32; //shifts down by half a number
}

void bigprint(uint64_t val[], int sz) {
	for (int i = 0; i < sz; ++i) {
		printf("%016lx ", val[sz-1-i]);
	}
}

uint64_t pack64(uint32_t hi, uint32_t lo) {
	return ((uint64_t) hi << 32) + lo;
}
// void bigmul64(uint64_t a[], int sz_a, uint64_t b[], sz_b, uint64_t c[], int sz_c) {
// 	//a = b*c
// 	//sz_a ≥ sz_b + sz_c
// }

int main(int argc, char const *argv[])
{
	//transform 64 bit array to 32 bit array
	uint64_t a_64[4]; // an array
	const uint32_t *a_32 = (uint32_t *) a_64;
	
	uint64_t b_64[4]; // an array
	const uint32_t *b_32 = (uint32_t *) b_64;


	return 0;
}