#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

//COMPILAR: gcc potencia.c -lgmp -L.

int modPowerGMP(int base_i, int exponente_i, int modulo_i) {
    /* Variable declaration*/
	mpz_t base;
    mpz_t exponente;
    mpz_t modulo;
    mpz_t output;

    /* Initializating and Setting first values*/
	mpz_init_set_si(base, base_i);
	mpz_init_set_si(exponente, exponente_i);
	mpz_init_set_si(modulo, modulo_i);
	mpz_init(output);

    /* Implemented power using libgmp*/
	mpz_powm(output, base, exponente, modulo);
	gmp_printf("LibGMP exponentiation: %Zd^%Zd (mod %Zd) = %Zd\n", base, exponente, modulo, output);


    /* Memory frees */
	mpz_clear(base);
	mpz_clear(exponente);
	mpz_clear(modulo);
	mpz_clear(output);

    /* Return status*/
	return 0;
}

// TODO: devolver resultado en return. 
int modPower(int base_i, int exponente_i, int modulo_i) {

    /* Main variables */
	mpz_t power;
    mpz_t base;
    mpz_t exponent;
    mpz_t modulus;
    mpz_t logical_mask;
	mpz_t temp;
	int i, longbits;

	/* Initialization and Setting of main variables */
	mpz_init_set_si(base, base_i);
	mpz_init_set_si(exponent, exponente_i);
	mpz_init_set_si(modulus, modulo_i);

    /* Initialization and Setting of mask used in decomposition*/
	mpz_init_set_si(logical_mask, 1);
	mpz_init(temp);


    /* Initialize power to 1 */
	mpz_init_set_si(power, 1);


    /* Get the size in base 2 of the exponent */
	longbits = mpz_sizeinbase(exponent, 2);

    /* Main loop for computations (Binary exponentiation, logical) */
	for(i = longbits - 1; i >= 0; i--) {
        
        /* Product and Modulus operations for calculation*/
		mpz_mul(power, power, power);
		mpz_mod(power, power, modulus);

        /* Logical operations equal to RightShift */
		mpz_fdiv_q_2exp(temp, exponent, i);
		mpz_and(temp, temp, logical_mask);			

        /* Check if bit in position i is 1 */
		if(mpz_cmp(temp, logical_mask) == 0) {
            /* Multiply by 1: Binary exponentiation 'sx' case */
			mpz_mul(power, power, base);
			mpz_mod(power, power, modulus);
		}
	}

    /* Output printing */
	gmp_printf("POWER OF 2 DECOMPOSITION: %Zd^%Zd (mod %Zd) = %Zd\n", base, exponent, modulus, power);

    /* Memory freeing*/
	mpz_clear(base);
	mpz_clear(exponent);
	mpz_clear(modulus);
	mpz_clear(logical_mask);
	mpz_clear(temp);
	mpz_clear(power);

    /* Status returnal */
	return 0;
}


int main(int argc, char **argv) {
    /* Problem computation storage */
    int base, exponente, modulo;

    /* Error control */
	if(argc != 4) {
		printf("Ejecucion incorrecta. Uso: ./potencia base exponente modulo\n");
		return -1;
	}
	
	base = atoi(argv[1]);
	exponente = atoi(argv[2]);
	modulo = atoi(argv[3]);
	modPower(base, exponente, modulo);
	modPowerGMP(base, exponente, modulo);
    return 0;
}