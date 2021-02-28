#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <stdint.h>
#include <limits.h>

void print_result(uint16_t as[], int sz_a) {
	// printf("res: ");
	for (int i = sz_a - 1; i >= 0; --i) {
		printf("%04x", as[i]);
	}
	printf("\n");
}

void print_result2(uint8_t as[], int sz_a) {
	// printf("res: ");
	for (int i = sz_a - 2; i >= 0; --i) {
		printf("%02x ", as[i]);
	}
	printf("\n");
}

uint16_t addto8(uint8_t as[], int sz_a, uint8_t bs[], int sz_b) {
	// Assume that sz_b <= sz_a
	// Compute as += bs
	int i;
	uint8_t c = 0;
	uint16_t s;

	for (i=0; i< sz_b; i++) {
		s = (uint16_t) as[i] + (uint16_t) bs[i] + (uint16_t) c; // s is a 17 bit value
		// printf("ok: %08x\n", s);
		c = s >> 8;
		as[i] = (uint8_t) s;
	}
	for ( ; i< sz_a; i++) {
		s = (uint16_t) as[i] + (uint16_t) c; // s is a 33 bit value
		c = s >> 8;
		as[i] = (uint8_t) s;
	}
	// printf("end: %08x ", as[1]);
	printf("end: %08x\n", as[0]);
	printf("carry: %08x\n", c);

	return c;
}

// this works for 8 bit number but not 256 bit number, basically its adding into the wrong columns
// void partialprod16(uint8_t as[], int sz_a, uint8_t bs[], int sz_b, uint8_t d) {
// 	// Compute as += bs * d
// 	int i;
// 	uint8_t c = 0;
// 	uint16_t p;

// 	for (i=0; i< sz_b; i++) {
// 		p = (uint16_t) as[i] + (uint16_t) d * (uint16_t) bs[i] + (uint16_t) c; // p is a 9 bit value
// 		c = p >> 8;
// 		as[i] = (uint8_t) p;
// 		printf("current a: %d %02x\n",i, as[i]);
// 		printf("carry: %02x\n", c);
// 		print_result(as, sz_a);
// 	}
// 	for ( ; i< sz_a; i++) {
// 		p = (uint16_t) as[i] + (uint16_t) c; // p is a 33 bit value
// 		c = p >> 8;
// 		as[i] = (uint8_t) p;
// 	}

// 	printf("----result-----\n");
// 	print_result(as, sz_a);
// }

/*need to pass in "iter"
this allows us to keep track of the "place value", so that we can add into correct column*/
void partialprod16(uint8_t as[], int sz_a, uint8_t bs[], int sz_b, uint8_t d, int iter) {
	// Compute as += bs * d
	int i;
	uint8_t c = 0;
	uint16_t p;
	for (i=0; i< sz_b; i++) {
		printf("as: %02x + ds %02x * bs: %02x + %04x c\n", as[i+iter],d,bs[i],c);
		p = (uint16_t) as[i+iter] + (uint16_t) d * (uint16_t) bs[i] + (uint16_t) c; // p is a 9 bit value
		printf("p: %04x\n", (uint8_t) p);
		c = p >> 8;
		as[i+iter] = (uint8_t) p;
		print_result2(as, sz_a);
	}
	// responsible for propogating carry
	for ( ; i< sz_a; i++) {
		p = (uint16_t) as[i+iter] + (uint16_t) c; // p is a 33 bit value
		c = p >> 8;
		as[i+iter] = (uint8_t) p;
	}

	printf("----result-----\n");
	print_result2(as, sz_a);
	printf("--------------\n");
}

void bigmul32(uint16_t a[], int sz_a, uint16_t b[], int sz_b, uint16_t c[], int sz_c) {
	//a = b*c
	//sz_a ≥ sz_b + sz_c
	uint8_t *a_8 = (uint8_t *) a;
	uint8_t *b_8 = (uint8_t *) b;
	uint8_t *c_8 = (uint8_t *) c;

	// iterate over all "columns" of c
	for (int i = 0; i < sz_c*2; ++i) {
		partialprod16(a_8, sz_a*2, b_8, sz_b*2, c_8[i], i);
	}
}

int main(int argc, char const *argv[])
{
	uint16_t as[4] = {0x0,0x0,0x0,0x0};
	uint16_t bs[2] = {0x322F,0xC};
	uint16_t cs[2] = {0x1,0x2356};
	bigmul32(as,4,bs,2,cs,2);

	printf("correct: ");
	print_result(as, 4);
	// confirm the results
	printf("correct: %016lx\n", (uint64_t) 0x000C322F* (uint64_t) 0x23560001);
	return 0;
}