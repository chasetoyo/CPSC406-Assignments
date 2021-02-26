#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <limits.h>

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
	printf("%016x\n", as[0]);

	return c;
}

void partialprod32(uint32_t as[], int sz_a, uint32_t bs[], int sz_b, uint32_t d) {
	// Compute as += bs * d
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
	printf("%016x\n", as[0]);

	return c;
}

void bigmul64(uint64_t a[], int sz_a, uint64_t b[], sz_b, uint64_t c[], int sz_c) {
	//a = b*c
	//sz_a ≥ sz_b + sz_c
}

int main(int argc, char const *argv[])
{
	//transform 64 bit array to 32 bit array
	uint64_t a_64[4] = {0xffffffffaaaaaaaa, 0x40, 0x3465, 0xeeeeeeeeffffffff}; // an array
	const uint32_t *a_32 = (uint32_t *) a_64;
	
	uint64_t b_64[4] = {0xffffffffffffffff, 0x40, 0x3465, 0x1}; // an array
	const uint32_t *b_32 = (uint32_t *) b_64;

	uint64_t c[10];
	// uint64_t x = 0xCABACABA98989898;
	// uint64_t y = 0xffffffffffffffff;

	uint32_t x[1] = {0x0000ffff};
	// printf("%016x\n", x[0]);
	// uint32_t y[1] = {0xffff0000};
	uint32_t y[1] = {0xffffffff};
	printf("%016x\n", x[0]);
	addto32(x,1,y,1);
	return 0;
}