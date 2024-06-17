uint32_t *build_prime_buffer_slow(uint32_t prime_buffer_size) {
  // Requires ~1GB RAM to find all 203,280,221 unsigned 32 bit primes.
  if (prime_buffer_size > 203280221) return NULL; // Max uint32_t prime is 203280221st prime 4294967291
  // NOTE: Free returned pointer after use.
  uint32_t pixhead=1;
  uint32_t i, j=0, p=3;
  uint32_t *primes = malloc(prime_buffer_size*sizeof(uint32_t));
  if (primes == NULL) return NULL;
  primes[0] = 2;
  while (pixhead < prime_buffer_size) {
    if ((uint64_t)primes[j]*primes[j] <= p) j++;
    for (i=1; i<j; i++) {
      if ((p % primes[i]) == 0) break;
    }
    if (i >= j) {
      primes[pixhead] = p;
      pixhead++;
    //} else {
    //  if (!is_definite_composite(p)) printf("%u, ", p);
    }
    p+=2;
  }  
  return primes;
}

uint32_t *build_prime_buffer_fast(uint32_t prime_buffer_size) {
  // Requires ~10GB RAM to find all 203,280,221 unsigned 32 bit primes. (~50s on Xeon E3-1225v2 32GB)
  if (prime_buffer_size > 203280221) return NULL; // Max uint32_t prime is 203280221st prime 4294967291
  if (prime_buffer_size < 180000) return build_prime_buffer_slow(prime_buffer_size);
  uint64_t max, maxbuffersize;
  if (prime_buffer_size >= 6) {
    long double lnn = logl(prime_buffer_size);
    max = ceil(prime_buffer_size*(lnn + logl(lnn)));
  } else {
    max = 11;
  }
  max |= 1;
  if (max > 0xffffffff) max = 0xffffffff;
  maxbuffersize = max/2;
  //printf("max = %lu\n", max);
  //printf("Fast algo. requires approx. = %lu MB RAM\n", (maxbuffersize+prime_buffer_size)*sizeof(uint32_t)/1000000);
  uint32_t num = 3;
  uint32_t *nums = malloc(maxbuffersize*sizeof(uint32_t));
  if (nums == NULL) return build_prime_buffer_slow(prime_buffer_size);
  for (uint64_t i=0; i<maxbuffersize; i++) {
    nums[i] = num;
    num += 2;
  }
  // Estimate of maximum number of sieve primes needed such that sieve_primes[sieve_primes_size-1]^2 > max
  uint32_t sieve_primes_size = 250; 
  uint32_t sieve_primeix = 0;
  uint32_t sieve_prime;
  uint32_t *sieve_primes = build_prime_buffer_slow(sieve_primes_size);
  if (sieve_primes == NULL) return NULL;
  while (sieve_primes[sieve_primes_size-1] <= max/sieve_primes[sieve_primes_size-1]) {
    free(sieve_primes);
    sieve_primes_size *= 2;
    sieve_primes = build_prime_buffer_slow(sieve_primes_size);
    //printf("Extended sieve primes.\n");
  }
  while (sieve_primes[sieve_primeix] <= max/sieve_primes[sieve_primeix]) sieve_primeix++;
  sieve_primes_size = sieve_primeix;
  // Remove sieve primes and all muliples thereof from nums array
  for (uint64_t j=1; j<sieve_primes_size; j++) {
    sieve_prime = sieve_primes[j];
    for (uint64_t i=(sieve_prime - 3)/2; i<maxbuffersize; i+=sieve_prime) nums[i] = 0;
  }  
  uint32_t *primes = malloc(prime_buffer_size*sizeof(uint32_t));
  uint32_t primesix = sieve_primes_size;
  // Copy sieve primes
  memcpy(primes, sieve_primes, sieve_primes_size*sizeof(uint32_t));
  // Copy rest of primes from non-zero elements of nums array
  for (uint64_t i=0; primesix<prime_buffer_size; i++) {
    if (nums[i] != 0) {
      primes[primesix] = nums[i];
      primesix++;
    }
  }
  free(nums);
  free(sieve_primes);
  return primes;
}
