
uint32_t isqrt(uint64_t n) {
  if (n < 2) return n;
  uint64_t ai = sqrt(n);
  while (!((ai <= n/ai) && ((ai+1) > n/(ai+1)))) {    
    ai = (ai + n/ai)/2;
  }
  return ai;
}

uint32_t icbrt(uint64_t n) {
  if (n < 2) return n;
  uint64_t ai = cbrt(n);
  while (!((ai <= n/(ai*ai)) && ((ai+1) > n/((ai+1)*(ai+1))))) {    
    ai = (2*ai + n/(ai*ai))/3;
  }
  return ai;
}

_Bool issquarefree(uint64_t n) {
  if (n < 4) return true;
  if ((n % 2) == 0) {
    if ((n % 4) == 0) return false;
    n /= 2;
  }
  if ((n % 3) == 0) {
    if ((n % 9) == 0) return false;
    n /= 3;
  }
  if (n == 1) return true;
  uint64_t rootn = isqrt(n);
  if (n == (rootn*rootn)) return false;
  // n is prime | n is semiprime | n has factor <= icbrt(n)
  uint32_t cbrtn = icbrt(n);
  for (uint32_t a=5; a<=cbrtn; a+=4) {
    if ((n % a) == 0) {
      n /= a;
      if ((n % a) == 0) return false;
      rootn = isqrt(n);
      if (n == (rootn*rootn)) return false;
      cbrtn = icbrt(n);
    }
    a+=2;
    if ((n % a) == 0) {
      n /= a;
      if ((n % a) == 0) return false;
      rootn = isqrt(n);
      if (n == (rootn*rootn)) return false;
      cbrtn = icbrt(n);
    }
  }
  return true;
}

_Bool issquarefree_manycall2(uint64_t n) {
  // Use instead of issquarefree() when ~10k+ calls required in a tight loop.
  // Uses 4MB of buffers.
  // Initialise with call to fastprimedivs200k();
  // Thread-safe after initialisation.
  // Uses d|n iff. (an mod m) <= b where d odd, m = 2^w > n, b = floor(m-1/d), ad = 1 mod m.
  // See https://math.stackexchange.com/questions/1251327/is-there-a-fast-divisibility-check-for-a-fixed-divisor
  if (n < 4) return true;
  if ((n % 2) == 0) {
    if ((n % 4) == 0) return false;
    n /= 2;
  }
  if ((n % 3) == 0) {
    if ((n % 9) == 0) return false;
    n /= 3;
  }
  if (n == 1) return true;
  uint64_t magica, magicb, rootn = isqrt(n);
  if (n == (rootn*rootn)) return false;
  // n is prime | n is semiprime | n has factor <= icbrt(n)
  uint32_t cbrtn = icbrt(n);
  uint32_t bufix = 2;
  uint32_t a = primes200k[bufix];
  magica = magicmult200k[bufix];
  magicb = magicb200k[bufix];
  while (a<=cbrtn) {
    if (magica*n <= magicb) {
      n /= a;
      rootn = isqrt(n);
      if (n == (rootn*rootn)) return false;
      if ((n % a) == 0) return false;
      cbrtn = icbrt(n);
    }
    bufix++;
    a = primes200k[bufix];
    magica = magicmult200k[bufix];
    magicb = magicb200k[bufix];
  }
  return true;
}
/*
_Bool issquarefree_manycall(uint64_t n, _Bool init) {
  // Uses 800KB primes buffer.
  // Thread-safe after initialisation.
  // Initialise with assert(issquarefree_manycall(0, true));
  // Then call thereafter with issquarefree_manycall(n, false);
  // Approx. 4x faster worst case performance than issquarefree() after initialisation.
  // Approx. 1.5x-3.0x faster average case performance than issquarefree() after initialisation depending on argument size.
  static uint32_t *primes200k;
  if (init) {
    primes200k = build_prime_buffer_slow(200000);
    return (primes200k != NULL);
  }
  if (n < 4) return true;
  if ((n % 2) == 0) {
    if ((n % 4) == 0) return false;
    n /= 2;
  }
  if ((n % 3) == 0) {
    if ((n % 9) == 0) return false;
    n /= 3;
  }
  uint64_t rootn = isqrt(n);
  if (n == (rootn*rootn)) return false;
  // n is prime | n is semiprime | n has factor <= icbrt(n)
  uint32_t cbrtn = icbrt(n);
  uint32_t primeix = 2;
  uint32_t a;
  a = primes200k[primeix];
  while (a<=cbrtn) {
    if ((n % a) == 0) {
      n /= a;
      rootn = isqrt(n);
      if (n == (rootn*rootn)) return false;
      if ((n % a) == 0) return false;
      cbrtn = icbrt(n);
    }
    primeix++;
    a = primes200k[primeix];
  }
  return true;
}
*/

int8_t *mobiusbuffer2(uint32_t n) {
// Memory requirement > 9*n bytes.
  uint32_t isqrtn = isqrt(n);
  uint64_t np1 = (uint64_t)n + 1;
  uint32_t prime_buffer_size = isqrtn; // Can do better!
  uint32_t *primes = build_prime_buffer_slow(prime_buffer_size);  
  if (primes == NULL) return NULL;
  int8_t *mu = calloc(1, np1);
  if (mu == NULL) {
    free(primes);
    return NULL;
  }
  uint64_t *munums = malloc((np1)*sizeof(uint64_t));
  if (munums == NULL) {
    free(mu);
    free(primes);
    return NULL;
  }
  uint64_t i, j, k, temp, ix;
  for (j=1; j<=n; j++) munums[j] = 1;
  for (i=0; i<prime_buffer_size; i++) {
    ix = primes[i];
    if (ix >= isqrtn) break;
    temp = ix;
    while (temp <= n) {
      mu[temp]++;
      k = temp;
      do {
        munums[temp] *= ix;
        k /= ix;        
      } while ((k % ix) == 0);
      temp += ix;
    }
  }
  for (j=1; j<=n; j++) {
    if (munums[j] != j) mu[j]++; 
    if (mu[j] & 1) {
      mu[j] = -1; 
    } else {
      mu[j] = 1; 
    }
  }
  free(munums);
  for (i=0; i<prime_buffer_size; i++) {
    ix = (uint64_t)primes[i]*primes[i];
    if (ix > n) break;
    j = 1;
    while (ix*j <= n) {
      mu[ix*j] = 0;
      j++;
    }
  }
  if (i >= prime_buffer_size) {
    fprintf(stderr, "ERROR! - Not enoungh primes in buffer!!\n");
    exit(1);
  }
  free(primes);
  return mu;
}

int8_t *mobiusbuffer(uint32_t max)
{
// Modified for c from https://mathoverflow.net/questions/99473/calculating-m%C3%B6bius-function
// Memory requirement > 5*max bytes.
    if (max > 1000000000) return mobiusbuffer2(max);
    uint32_t sqrt = isqrt(max);
    int32_t *mu = malloc((max + 1)*sizeof(int32_t));
    for (int32_t i = 1; i <= max; i++)
        mu[i] = 1;
    for (int32_t i = 2; i <= sqrt; i++)
    {
        if (mu[i] == 1)
        {
            for (int32_t j = i; j <= max; j += i)
                mu[j] *= -i;
            for (int32_t j = i * i; j <= max; j += i * i)
                mu[j] = 0;
        }
    }
    for (int32_t i = 2; i <= max; i++)
    {
        if (mu[i] == i)
            mu[i] = 1;
        else if (mu[i] == -i)
            mu[i] = -1;
        else if (mu[i] < 0)
            mu[i] = 1;
        else if (mu[i] > 0)
            mu[i] = -1;
    }
    int8_t *mu8 = malloc((max + 1)*sizeof(int8_t));
    for (int32_t i = 1; i <= max; i++) mu8[i] = mu[i];
    free(mu);
    return mu8;
}

uint64_t squarefreecount3(uint64_t start, uint64_t end) {
  uint64_t count = 0;
  for (uint64_t n = start; ; n++) {
    count += issquarefree_manycall2(n);
    //count += issquarefree(n);
    if (n == end) break;
  }
  return count;
}

uint64_t squarefreecount2(uint64_t start, uint64_t end) {
  // Returns the number of squarefree integers i s.t. start <= i <= end.
  // Theorem from https://smsxgz.github.io/post/pe/counting_square_free_numbers/
  // S(n) = sum_{d=1 to isqrt(n)} mu(d) [n/(d*d)] = No. of squarefree integers <= n
  // Returns S(end) - S(start-1)
  if (end < start) return 0;
  if (start == 0) start = 1;
  uint64_t sum = 0;
  uint64_t startm1 = start - 1;
  uint32_t isqrtstartm1 = isqrt(startm1);
  uint32_t isqrtend = isqrt(end);
  uint64_t i;
  int8_t *mu = mobiusbuffer(isqrtend);
  if (mu == NULL) return 0;
  i=1;
  for (; i<=isqrtstartm1; i++) sum += mu[i]*((end/(i*i)) - (startm1/(i*i)));
  for (; i<=isqrtend; i++) sum += mu[i]*(end/(i*i));
  free(mu);
  return sum;
}

uint64_t squarefreecount(uint64_t n, uint32_t *primes, uint32_t prime_buffer_size) {
  // Returns the number of squarefree integers i s.t. 1 <= i < n.
  uint64_t i, sum = 0;
  uint64_t psqrd;
  uint32_t primeix = 0;
  uint8_t *sievebuffer = calloc(1, n);
  while (n/primes[primeix] > primes[primeix]) {
    psqrd = primes[primeix]*primes[primeix];
    i=1;
    while (i*psqrd < n) {
      if (sievebuffer[i*psqrd] == 0) {
        sievebuffer[i*psqrd] = 1;
        sum++;
      }
      i++;
    }
    primeix++;
    if (primeix >= prime_buffer_size) {
      fprintf(stderr, "ERROR! - Not enoungh primes in buffer!!\n");
      exit(1);
    }
  }
  free(sievebuffer);
  return n - 1 - sum;
}
