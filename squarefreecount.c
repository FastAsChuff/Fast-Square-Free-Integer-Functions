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

// gcc squarefreecount.c -o squarefreecount.bin -O3 -march=native -Wall -lm

uint64_t atolu(char* arg) {
  uint64_t res = 0;
  while (*arg) {
    res = (res * 10) + (*arg - '0');
    arg++;
  }
  return res;
}

int main(int argc, char **argv) {  
  if (argc < 3) {
    printf("This program calculates the number of square free integers in a range.\nUsage: %s rangestart rangeend [maxmemory]\nmaxmemory is an optional argument and is a soft limit of the amount of RAM the program may use in bytes.\nrangeend must be <= 10^18\nE.g %s 1 100\nOutput:- Number of squarefree integers between 1 and 100 inclusive is 61.\n", argv[0], argv[0]);
    exit(0);
  }  
  uint64_t input1 = atolu(argv[1]);
  uint64_t input2 = atolu(argv[2]);  
  uint64_t maxmem = 0;
  if (argc > 3) maxmem = atolu(argv[3]);  
  if (input1 > input2) {
    fprintf(stderr, "ERROR! - Invalid range!!\n");
    exit(1);    
  }
  uint32_t isqrtend = isqrt(input2);
  if (input2 > 1000000000000000000ULL) {
    fprintf(stderr, "ERROR! - rangend %lu too big!!\n", input2);
    exit(1);
  }
  if ((argc > 3) && (isqrtend > maxmem/5)) {
    fprintf(stderr, "ERROR! - Insufficient RAM allocated!!\n");
    exit(1);
  }
  //if (!fastprimedivs200k()) {
  //  fprintf(stderr, "ERROR! - Could not build 200k prime buffers!!\n");
  //  exit(1);
  //}
  uint64_t output;
  output = squarefreecount2(input1, input2);
  //output = squarefreecount3(input1, input2);
  printf("Number of squarefree integers between %lu and %lu inclusive is %lu.\n", input1, input2, output);
}

// Numbers of square free integers less than or equal to 10^n are 
// 7, 61, 608, 6083, 60794, 607926, 6079291, 60792694, 607927124, 6079270942, 
// 60792710280, 607927102274, 6079271018294, 60792710185947, 607927101854103, 6079271018540405, 
// 60792710185403794, 607927101854022750... more at https://arxiv.org/pdf/1107.4890
