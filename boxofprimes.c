#include <openssl/bn.h>
#include <string.h>

#define SMALL_PRIME_BITS (16)
// TODO: Replace globals with config object
int GENERATE_SAFE_PRIME = 0;
#define BUFSIZE (128)
char RANDOM_BUFFER[BUFSIZE];

void output_prime(unsigned int bits) {
  BIGNUM *p = BN_new();

  fprintf(stderr, "Generating prime of %d bits\n", bits);

  BN_generate_prime(p, bits, GENERATE_SAFE_PRIME, 0, 0, 0, 0);
  char *pdec = BN_bn2dec(p);
  fprintf(stderr, "Generated prime: ");
  fprintf(stdout, "%s\n", pdec);

  free(pdec);
  BN_free(p);
}

void error_and_exit(char *s) {
  fprintf(stderr, "ERROR: %s\n", s);
  exit(1);
}

void small_prime_warning() {
  fprintf(stderr, "WARNING: primes smaller than %d will not be random as the OpenSSL prime generator is meant for large primes.\n", SMALL_PRIME_BITS);
}

void usage() {
  fprintf(stderr,
"boxofprimes\n"
"\n"
"Usage: boxofprimes [bit size for generated prime number]\n"
"  --help           Output this help and exit\n"
"  --safe           Generate a safe prime (i.e. a prime p so that (p-1)/2 is also prime)\n"
"                   NOTE: Generation is substantially slower for safe primes\n"
"\n"
"Examples:\n"
"  boxofprimes --help\n"
"  boxofprimes 512\n"
"\n"
"NOTE: The prime number generation has a negligible error probability (false positive rate of ~2^-80). See BN_generate_prime in the OpenSSL documentation.\n");
  small_prime_warning();
  exit(1);
}

int is_numeric_string(const char *s) {
  char *endp;
  strtol(s, &endp, 0);
  if (s != endp && *endp == '\0') {
    return 1;
  }
  return 0;
}

int main(int argc, const char *argv[])
{
  unsigned int i;
  unsigned int bits = 0;

  // WARNING: I make no claims as to the cryptographic security of this seeding
  SSL_load_error_strings();
  SSL_library_init();
  // Generate BUFSIZE cryptographically strong pseudo-random bytes or raise an error
  RAND_bytes(RANDOM_BUFFER, BUFSIZE);
  // On systems that provide "/dev/urandom", the randomness device is used to seed the PRNG transparently
  RAND_seed(RANDOM_BUFFER, BUFSIZE);

  if (argc <= 1) usage();
  for(i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
      usage();
    } else if (strcmp(argv[i], "--safe") == 0) {
      GENERATE_SAFE_PRIME = 1;
    } else if (is_numeric_string(argv[i])) {
      bits = atoi(argv[i]);
    } else {
      error_and_exit("Could not parse command line arguments.");
    }
  }

  if (bits < SMALL_PRIME_BITS) {
    small_prime_warning();
  }
  output_prime(bits);

  return 0;
}
