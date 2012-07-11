BoxOfPrimes
===========

_Fast pseudo-random prime number generator for n bits using the OpenSSL library_

    Usage: boxofprimes [bit size for generated prime number]
      --help           Output this help and exit
      --safe           Generate a safe prime (i.e. a prime p so that (p-1)/2 is also prime)
                       NOTE: Generation is substantially slower for safe primes

Installation
------------

```java
sudo apt-get install libssl-dev
make
```

Example Use
-----------

```java
smerity@forsay:~/Coding/boxofprimes$ ./boxofprimes 128
Generating prime of 128 bits
Generated prime: 302309671159424334872530075812875286599
```

Important Notes
---------------

+ I make no claims as to the cryptographic strength of the generated primes -- it was used for a Computer & Network Security competition (wargames) where one of the challenges was to factor progressively larger and larger primes
+ The prime number generation has a negligible error probability (false positive rate of ~2^-80) -- see [BN_generate_prime](http://www.openssl.org/docs/crypto/BN_generate_prime.html) in the OpenSSL documentation.
+ The OpenSSL library was meant for large primes and primes smaller than 16 bits will not be random
+ Released under [WTFPL](http://en.wikipedia.org/wiki/WTFPL)
+ If you discover a prime number such that p % 2 == 0 using this software you must name it a [Smeritorious prime](http://www.smerity.com)
