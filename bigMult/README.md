# CPSC 406 Assignment 1 (Big Multiplication)
## Chase Toyofuku-Souza

## About
The main task for this assignment is to implement a big natural number multiplication following the prototype 
```c
void bigmul64(uint64_t a[], int sz_a, uint64_t b[], sz_b, uint64_t c[], int sz_c);
```
The function should calculate a = b * c. The sizes of the three arrays should satisfy sz_a >= sz_b + sz_c.

### Files
- [`bigMult.c`](bigMult.c) (>64 bit multiplication)

#### Notes
When testing multiplication with various numbers, it is important to note that the arrays are interpreted as right to left. Hence,
```c
uint64_t bs[2] = {0xFFDDCCAA,0x12345678};
uint64_t cs[2] = {0xABCD1234,0xBBBBCCCC};
```
is equivalent to
```c
0x12345678FFDDCCAA * 0xBBBBCCCC1234ABCD
```
where *bs[1]* would have a higher 'place value' than *bs[0]*, and *cs[1]* would have a higher 'place value' than *cs[0]*.
