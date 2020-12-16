#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <math.h>
#include <getopt.h>

int main(){

    /* Variables */
    int nbits = 4096;
    int i;

    /* Primes*/
    mpz_t p;
    mpz_t q;
    mpz_t n;
    mpz_t n_fi;
    mpz_t e;
    mpz_t d;
    mpz_t m;
    mpz_t tmp;

    /* Prime number variable */
    mpz_t randNumber;

    /* Loop variables */
    mpz_t iter;
    mpz_t x;
    mpz_t v;
    mpz_t v0;
    

    /* LibGMP init*/
    mpz_init(randNumber);
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(n_fi);
    mpz_init(e);
    mpz_init(d);
    mpz_init(m);
    mpz_init(tmp);
    mpz_init(iter);
    mpz_init(x);
    mpz_init(v);
    mpz_init(v0);

    
    

    /* Random State */
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));
    /* Random number with nbits */
    mpz_urandomb(randNumber, state, nbits);

    /* Generate p and q */
    mpz_nextprime(p, randNumber);
    mpz_nextprime(q, p);
    /* Generate n*/
    mpz_mul(n, p, q);

    /* Generate fi(n)*/
    mpz_sub_ui(n_fi, p, 1);
    mpz_sub_ui(tmp, q, 1);
    mpz_mul(n_fi, n_fi, tmp);

    /* Select integer e*/
    mpz_div_ui(e, n_fi, 4);
    mpz_nextprime(e, e);

    /* Find exponent d */
    mpz_invert(d, e, n_fi);

    fprintf(stdout, "RSA VALUES\n");
    gmp_fprintf(stdout, "p: %Zd\n", p);
    gmp_fprintf(stdout, "q: %Zd\n", q);
    gmp_fprintf(stdout, "n: %Zd\n", n);
    gmp_fprintf(stdout, "fi(n): %Zd\n", n_fi);
    gmp_fprintf(stdout, "e: %Zd\n", e);
    gmp_fprintf(stdout, "d: %Zd\n", d);

    /* Compute m = e * d*/
    mpz_mul(m, e, d);
    mpz_sub_ui(m, m, 1);

    /* Iteration spec */
    mpz_set_ui(iter, 0);
    gmp_fprintf(stdout, "iter: %Zd\n", iter);

    /* Floor with 2 */
    while (mpz_even_p(m)){
        mpz_fdiv_q_ui(m, m, 2);
    }
    gmp_fprintf(stdout, "m: %Zd\n", m);

    /* decryptionexp */
    mpz_set_ui(iter, 0);
    while(mpz_get_ui(iter) < 5){

        /* Iteration*/
        mpz_add_ui(iter, iter, 1);

        /* random generation */
        mpz_urandomm(x, state, n);

        /* Check gcd*/
        mpz_gcd(tmp, x, n);
        if (mpz_get_ui(tmp) != 1){
            gmp_fprintf(stdout, "x: %Zd\n", x);
        }
        /* Power */
        mpz_powm(v, x, m, n);

        /* Next */
        if (mpz_get_ui(v) == 1){
            continue;
        }

        /* Prepare v and v0*/
        while(mpz_get_ui(v) != 1){
            mpz_set(v0, v);
            mpz_set_ui(tmp, 2);
            mpz_powm(v, v, tmp, n);
        }

        /* Get n-1 */
        mpz_sub_ui(tmp, n, 1);
        
        /* Final comparison */
        if( mpz_cmp_ui(v0, -1) && mpz_cmp(v0, tmp)){
            mpz_add_ui(tmp, v0, 1);
            mpz_gcd(tmp, tmp, n);
            gmp_fprintf(stdout, "result1: %Zd\n", tmp);
        }

        mpz_div(tmp, n, tmp);
        gmp_fprintf(stdout, "result2: %Zd\n", tmp);
    }

    mpz_clear(randNumber);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(n_fi);
    mpz_clear(e);
    mpz_clear(d);
    mpz_clear(m);
    mpz_clear(tmp);
    mpz_clear(iter);
    mpz_clear(x);
    mpz_clear(v);
    mpz_clear(v0);

    return 0;
}
