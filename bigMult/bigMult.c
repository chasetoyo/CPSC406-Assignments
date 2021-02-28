#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <stdint.h>
#include <limits.h>

void print_result(uint64_t as[], int sz_a) {
	for (int i = sz_a - 1; i >= 0; --i) {
		printf("%016lx ", as[i]);
	}
	printf("\n");
}

uint16_t addto32(uint32_t as[], int sz_a, uint32_t bs[], int sz_b) {
	// Assume that sz_b <= sz_a
	// Compute as += bs
	int i;
	uint32_t c = 0;
	uint64_t s;

	for (i=0; i< sz_b; i++) {
		s = (uint32_t) as[i] + (uint32_t) bs[i] + (uint32_t) c; // s is a 17 bit value
		c = s >> 32;
		as[i] = (uint8_t) s;
	}
	for ( ; i< sz_a; i++) {
		s = (uint16_t) as[i] + (uint16_t) c; // s is a 33 bit value
		c = s >> 32;
		as[i] = (uint8_t) s;
	}
	return c;
}

/*need to pass in "iter"
this allows us to keep track of the "place value", so that we can add into correct column*/
void partialprod32(uint32_t as[], int sz_a, uint32_t bs[], int sz_b, uint32_t d, int iter) {
	// Compute as += bs * d
	int i;
	uint32_t c = 0;
	uint64_t p;

	for (i=0; i< sz_b; i++) {
		// printf("as: %08x + ds %08x * bs: %08x + %016x c\n", as[i+iter],d,bs[i],c);
		p = (uint64_t) as[i+iter] + (uint64_t) d * (uint64_t) bs[i] + (uint64_t) c; // p is a 9 bit value
		c = p >> 32;
		as[i+iter] = (uint32_t) p;
	}
	// responsible for propogating carry
	for ( ; i< sz_a; i++) {
		p = (uint64_t) as[i+iter] + (uint64_t) c; // p is a 33 bit value
		c = p >> 32;
		as[i+iter] = (uint32_t) p;
	}

	// printf("----result-----\n");
	// print_result2(as, sz_a);
	// printf("--------------\n");
}

void bigmul64(uint64_t a[], int sz_a, uint64_t b[], int sz_b, uint64_t c[], int sz_c) {
	//a = b*c
	//sz_a ≥ sz_b + sz_c
	uint32_t *a_32 = (uint32_t *) a;
	uint32_t *b_32 = (uint32_t *) b;
	uint32_t *c_32 = (uint32_t *) c;

	// iterate over all "columns" of c
	for (int i = 0; i < sz_c*2; ++i) {
		partialprod32(a_32, sz_a*2, b_32, sz_b*2, c_32[i], i);
	}
}

int main(int argc, char const *argv[])
{
	uint64_t as[4] = {0x0,0x0,0x0,0x0};
	uint64_t bs[2] = {0xFFDDCCAA,0x12345678};
	uint64_t cs[2] = {0x12345678,0xFFDDCCAA};
	bigmul64(as,4,bs,2,cs,2);

	print_result(as, 4);
	// confirm the results
	// printf("correct: %016lx\n", (uint64_t) 0xFFDDCCAA * (uint64_t) 0x12345678);
	return 0;
}