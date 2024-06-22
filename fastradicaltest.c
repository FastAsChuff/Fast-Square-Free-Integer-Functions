#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include </home/simon/build_prime_buffer.c>
#include </home/simon/fastmodinvpow2fns.c> // Find here https://github.com/FastAsChuff/Fast-Modular-Inverse-Modulo-Powers-Of-2/blob/main/fastmodinvpow2fns.c
#include </home/simon/fastprimedivs200k.c>
#include </home/simon/squarefreefns.c>
#include </home/simon/fastradical200k.c>

// gcc fastradicaltest.c -o fastradicaltest.bin -O3 -march=native -Wall -lm

uint64_t atolu(char* arg) {
  uint64_t res = 0;
  while (*arg) {
    res = (res * 10) + (*arg - '0');
    arg++;
  }
  return res;
}

uint64_t gcd(uint64_t a, uint64_t b) {
  uint64_t temp;
  while (1) {
    if (b > a) {
      temp = a;
      a = b;
      b = temp;
    }
    // a >= b
    a = (a % b);
    if (a == 0) return b;
    if (a == 1) return 1;
  }
}

int main(int argc, char **argv) {  
  if (argc < 3) {
    printf("This program calculates the sum of the radicals of integers in a range.\nUsage: %s rangestart rangeend\nE.g %s 1 100\nOutput:- Sum of radicals of integers between 1 and 100 inclusive is 3512 mod 2^64.\n", argv[0], argv[0]);
    exit(0);
  }  
  uint64_t input1 = atolu(argv[1]);
  uint64_t input2 = atolu(argv[2]);  
  if (input1 > input2) {
    fprintf(stderr, "ERROR! - Invalid range!!\n");
    exit(1);    
  }
  if (!fastprimedivs200k()) {
    fprintf(stderr, "ERROR! - Could not build 200k prime buffers!!\n");
    exit(1);
  }
  uint64_t g, ii, rad, sum = 0;
  for (uint64_t i=input1; ;i++) {
    rad = rad_200k(i);
    /*
    ii = i;
    g = gcd(ii, rad);
    while (g > 1) {
      ii = ii / g;
      g = gcd(ii, rad);
    }    
    // Check rad is squarefree and i does not have any prime factors which don't divide rad.
    if (!issquarefree_manycall2(rad) || (ii != 1)) {
      fprintf(stderr, "ERROR! - rad calculation incorrect at %lu!!\n", i);
      exit(1);
    }
    */
    sum += rad;
    if (i == input2) break;
  }
  printf("Sum of the radicals of integers between %lu and %lu inclusive is %lu mod 2^64.\n", input1, input2, sum);
}
// Sum of the radicals of integers between 1 and 100000 inclusive is 3522204030 mod 2^64.
// Sum of the radicals of integers between 1 and 1000000 inclusive is 352218412502 mod 2^64.
// Sum of the radicals of integers between 1 and 10000000 inclusive is 35222287961010 mod 2^64.
// Sum of the radicals of integers between 1 and 100000000 inclusive is 3522209924796624 mod 2^64.
