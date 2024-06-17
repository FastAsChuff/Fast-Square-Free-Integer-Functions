uint32_t *primes200k = NULL;
uint64_t *magicmult200k = NULL;
uint64_t *magicb200k = NULL;

_Bool fastprimedivs200k(void) {
  // d|n iff. (an mod m) <= b where d odd, m = 2^w > n, b = floor(m-1/d), ad = 1 mod m.
  // See https://math.stackexchange.com/questions/1251327/is-there-a-fast-divisibility-check-for-a-fixed-divisor
  // Primes sufficient for searching for prime factors of a uint64_t up to and including its cube root.
  primes200k = build_prime_buffer_slow(200000);
  if (primes200k == NULL) return false;
  magicmult200k = malloc(200000*sizeof(uint64_t));
  if (magicmult200k == NULL) {
    free(primes200k);
    primes200k = NULL;
    return false;
  }
  magicb200k = malloc(200000*sizeof(uint64_t));
  if (magicb200k == NULL) {
    free(primes200k);
    primes200k = NULL;
    free(magicmult200k);
    magicmult200k = NULL;
    return false;
  }
  for (uint32_t i=1; i<200000; i++) {
    magicmult200k[i] = modinv64x(primes200k[i]);
    if (magicmult200k[i]*primes200k[i] != 1) {
      fprintf(stderr, "ERROR: Incorrect modular inverse in fastprimedivs200k() for %u.\n", primes200k[i]);
      free(primes200k);
      primes200k = NULL;
      free(magicmult200k);
      magicmult200k = NULL;
      free(magicb200k);
      magicb200k = NULL;
      return false;
    }
    magicb200k[i] = 0xffffffffffffffffULL / primes200k[i];
  }
  return true;
}
