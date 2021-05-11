#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include<inttypes.h>
#include<limits.h>
#include<string.h>
void printFunc(uint32_t as[], int sz)
{ 
  for(int j=sz-1; j>=0; j--)
  {
    printf("%08x ",as[j]);
  }
  printf("\n");
}
void printFunc64(uint64_t as[], int sz)
{ 
  for(int j=sz-1; j>=0; j--)
  {
    printf("%016lx ",as[j]);
  }
  printf("\n");
}
//helper functions
uint64_t mult(uint32_t bsNum, uint32_t digit)	
{						
  return ((uint64_t)bsNum * (uint64_t) digit);				
}			
uint64_t add(uint32_t resNum, uint32_t s)		
{				
  return ((uint64_t)resNum + (uint64_t)s);				        
}
uint32_t lo(uint64_t s)
{
    return (s & 0xffffffff);
}
uint32_t hi(uint64_t s)
{
    return (s >> 32);
}
// include an offset or index for as[] so we know what column to start each time 
//precond: sz_b + offset < sz_a
//postcond: 
//sum_{k<sz_a} as[k] * b^k = sum_{k<sz_a} as'[k] * b^k + sum_{i<sz_b} bs[i] * b^i * digit
void partialProd32(uint32_t as[], const int sz_a, uint32_t bs[], const int sz_b, uint32_t digit, int offset)
{
  uint32_t low = 0;
  uint32_t high = 0;
  uint64_t pps = 0;
  uint64_t addTemp = 0;
  for (int j=0; j < sz_b; j++)
  // inv: sum_{k<sz_a} as[k] * b^k = sum_{k<sz_a} as'[k] * b^k + sum_{i<j} bs[i] * b^(i+offset') * digit
  //offset' = original value of offset
  //original value of offset+j is the same as the current value of offset
  // offset = offset'+ j  *******
  // basis: the initial time through the loop is j = 0, current offset = original offset + 0 is true because we haven't changed offset
  // ind step: prove this is true for the next time through the loop 
  // offset = offset' + j   //needs to be true at the top of the loop
  // basis: when j = 0, (sum_{i<j} bs[i] * b^i * digit) = 0 and as = as'
  // ind hyp: Suppose sum_{k<sz_a} as[k] * b^k = sum_{k<sz_a} as'[k] * b^k + sum_{i<j} bs[i] * b^(i+offset') * digit is true, 
  // ind step: my goal is to prove, the same equation for the next j + 1
	{ 
	  pps = mult(bs[j], digit);
    printf("MULT/S: %016lx\n", pps);
    //true:
    //sum_{k<sz_a} as[k] * b^k + pps*b^(j+offset') = sum_{k<sz_a} as'[k] * b^k + sum_{i<j} bs[i] * b^(i+offset') * digit +bs[j]*b^(j+offset') *digit
    //= sum_{k<sz_a} as'[k] * b^k + sum_{i<j+1} bs[i] * b^(i+offset') * digit
    addTemp = add (as[offset], lo(pps)); 
	  //sum_{k<sz_a} as[k] * b^k + (hi(pps)*b+lo(pps)) *b^(offset) = sum_{k<sz_a} as'[k] * b^k + sum_{i<j} bs[i] * b^(i+offset') * digit +bs[j]*b^(j+offset') *digit
    //sum_{k<sz_a} as[k] * b^k + (hi(pps)*b* b^(offset)) + (b^(offset)*lo(pps)) = ...
    //sum_{k<sz_a} as[k] * b^k + (hi(pps)*b^(offset+1)) + (b^(offset)*lo(pps)) = ...
    //sum_{k<offset}  as[k] * b^k + a[offset]*b^(offset) + a[offset+1]*b^(offset+1) + sum_{offset+2<=k<sz_a} as[k]*b^k +(hi(pps)*b^(offset+1)+(b^(offset)*lo(pps))
    //sum_{k<offset}  as[k] * b^k + addTemp*b^(offset) + a[offset+1]*b^(offset+1) + sum_{offset+2<=k<sz_a} as[k]*b^k +(hi(pps)*b^(offset+1)
    //sum_{k<offset}  as[k] * b^k +  (hi(addTemp)*b + low(addTemp)) *b^(offset)   + a[offset+1]*b^(offset+1) + sum_{offset+2<=k<sz_a} as[k]*b^k +(hi(pps)*b^(offset+1)
    //sum_{k<offset}  as[k] * b^k +  (hi(addTemp)*b ) *b^(offset) +  (low(addTemp)) *b^(offset)) + a[offset+1]*b^(offset+1) + sum_{offset+2<=k<sz_a} as[k]*b^k +(hi(pps)*b^(offset+1)
    //sum_{k<offset}  as[k] * b^k +  (hi(addTemp)*b^(offset+1) ) +  (low(addTemp)) *b^(offset)) + a[offset+1]*b^(offset+1) + sum_{offset+2<=k<sz_a} as[k]*b^k +(hi(pps)*b^(offset+1)
    //= sum_{k<sz_a} as'[k] * b^k + sum_{i<j+1} bs[i] * b^(i+offset') * digit 
    as[offset] = lo(addTemp); 
    //sum_{k<offset}  as[k] * b^k +  (hi(addTemp)*b^(offset+1) ) +  (as[offset]) *b^(offset)) + a[offset+1]*b^(offset+1) + sum_{offset+2<=k<sz_a} as[k]*b^k +(hi(pps)*b^(offset+1)
    //sum_{k<offset+1}  as[k] * b^k +  (hi(addTemp)*b^(offset+1) ) + a[offset+1]*b^(offset+1) + sum_{offset+2<=k<sz_a} as[k]*b^k +(hi(pps)*b^(offset+1)
    //sum_{k<offset+1}  as[k] * b^k +  (hi(addTemp)*b^(offset+1) ) + a[offset+1]*b^(offset+1) + sum_{offset+2<=k<sz_a} as[k]*b^k +(hi(pps)*b^(offset+1)
//
    //sum_{k<sz_a} as[k] * b^k  = sum_{k<sz_a} as'[k] * b^k + sum_{i<j} bs[i] * b^(i+offset') * digit
    printf("LOW: AS[%d]: %08x\n", offset , as[offset]); 
    //sum_{k<offset+1}  as[k] * b^k +  (hi(addTemp)+ hi(pps) +  a[offset+1]) *b^(offset+1)  + sum_{offset+2<=k<sz_a} as[k]*b^k
    // 
    as[offset+1] += hi(addTemp) + hi(pps); 
    //sum_{k<offset+1}  as[k] * b^k +  (as[offset+1]) *b^(offset+1)  + sum_{offset+2<=k<sz_a} as[k]*b^k
    //sum_{k<sz_a}  as[k] * b^k
    printf("HIGH: AS[%d]: %08x\n", offset+1, as[offset+1]); 
    offset++;
    printf("=========================================\n");
    //carry at the last time in the loop (?)
	}
  printf("END OF PARTIAL PROD 32 \n");
  //sum_{k<sz_a} as[k] * b^k = sum_{k<sz_a} as'[k] * b^k + sum_{i<j} bs[i] * b^(i+offset) * digit
  //j == sz_b
  printFunc(as, sz_a);
  printf("\n");
} 
 //end of partialProd32
///////////////***********HOMEWORK_PART3**************////////////////////
void bigMult64(uint64_t a[], int sz_a, uint64_t b[], int sz_b, uint64_t c[], int sz_c)
{
  int j;
  for (j=0; j < sz_a; j++)
  {
    a[j] = 0;
  }
  //convert to 32 bits  IMPORTANT!!!!  
  uint32_t *des = (uint32_t *) a;
  uint32_t *as = (uint32_t *) b;
  uint32_t *bs = (uint32_t*) c;
  printFunc(as,sz_b*2);
  printFunc(bs,sz_c*2);
  int k;
  // printf("SIZE AS: %d\n",sz_b); // 0x55 and 0xffff...
  // printf("SIZE BS: %d\n",sz_c); // 0x2
  printf("\n");
  for(k=0; k<sz_c*2; k++)
  { 
    partialProd32(des, sz_a*2, as, sz_b*2, bs[k], k);
  }
  //convert back to 64 bits
  uint64_t *final_a = (uint64_t *) des;
  uint64_t *final_b = (uint64_t *) as;
  uint64_t *final_c = (uint64_t*) bs;
  printf("FINAL ANSWER: \n");
  printFunc64(final_a,sz_a);
}//end of bigMult64
//==================main============================
int main(void) 
{
  //test1
  uint64_t a_64[2] = {0x55,0xffffffffffffffff}; //src 1
  uint64_t b_64[1] = {0x2};// src 2
  uint64_t dest[3];
  bigMult64(dest,3,a_64,2,b_64,1); 
  return 0;
}//end of main
