uint64_t divide_out(uint64_t n, uint64_t d) {
  while ((n % d) == 0) n /= d;
  return n;
}

uint64_t rad_200k(uint64_t n) {
  // Returns product of all distinct prime divisors of n. A.K.A. radical of n. (Largest squarefree divisor)
  uint64_t rad = 1;
  if ((n % 2) == 0) {
    rad = 2;
    n = divide_out(n, 2);
  }
  if ((n % 3) == 0) {
    rad *= 3;
    n = divide_out(n, 3);
  }
  if (n == 1) return rad;
  uint64_t magica, magicb;
  uint64_t rootn = isqrt(n);
  while (n == (rootn*rootn)) {
    n = rootn;
    rootn = isqrt(n);
  }
  // n is prime | n is product of two distinct primes | n has factor <= icbrt(n)
  uint32_t cbrtn = icbrt(n);
  uint32_t bufix = 2;
  uint32_t a = primes200k[bufix];
  magica = magicmult200k[bufix];
  magicb = magicb200k[bufix];
  while (a<=cbrtn) {
    if (magica*n <= magicb) {
      n /= a;
      rad *= a;
      while (magica*n <= magicb) n /= a;
      if (n == 1) return rad;
      rootn = isqrt(n);
      while (n == (rootn*rootn)) {
        n = rootn;
        rootn = isqrt(n);
      }
      cbrtn = icbrt(n);
    }
    bufix++;
    a = primes200k[bufix];
    magica = magicmult200k[bufix];
    magicb = magicb200k[bufix];
  }
  return rad*n;
}
