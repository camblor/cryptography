#include "../include/des.h"
#include <time.h>

#define N_TESTS 1000

/*
 * Function:  dHamming 
 * --------------------
 * 	Calculates Hamming distance between two given inputs.
 *
 * 	in1: First input for distance calculation.
 * 	in2: Second input for distance calculation.
 *
 *  returns: Hamming distance for given input.
 */
int hammingDistance(uint64_t in1, uint64_t in2){

	/* Iteration variable */
	int i=0;

	/* Hamming distance variable */
	int count=0;

	/* Looping while calculating Hamming Distance */
	for(; i<64; i++, in1>>=1, in2>>=1){

		/* Hammign distance counter condition */
		if( (in1&1) ^ (in2&1) )
			count++;
	}

	/* Returning of result */
	return count;
}

/* Estudio de la no linealidad de las S-boxes del DES
	Comprobar que f(a xor b) != f(a) xor f(b)
	Medimos la diferencia entre esos valores como la distancia de Hamming.
	> length(distanciasH)
	[1] 100000
	> hist(distanciasH)
	> summary(distanciasH)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max. 
   5.00   14.00   16.00   16.19   18.00   27.00 
*/
int main() {

	/* Main input variables */
	uint64_t inA, inB, inC;

	/* Main output variables */
	uint64_t outA, outB, outC;

	/* Storage variable for distances */
	int hDist[N_TESTS];

	/* Iteration variable */
	int i;

	/* Random seed */
	srand(time(NULL));

	/* Test looping iteration */
	for(i = 0; i < N_TESTS; i++){

		/* A variable random generation */
		inA = rand();
		/* A variable shift + generation*/
		inA = (inA << 32) | rand();
		/* A variable final generation step */
		inA &= 0x0000FFFFFFFFFFFF;
		
		/* B variable random generation */
		inB = rand();
		/* B variable shift + generation*/
		inB = (inB << 32) | rand();
		/* B variable final generation step */
		inB &= 0x0000FFFFFFFFFFFF;
		
		/* C variable generation as OR between A and B */
		inC = inA ^ inB;

		/* S-Boxes execution for each variable */
		outA = sbox(inA);
		outB = sbox(inB);
		outC = sbox(inC);

		/* Hamming distance calculation and storage*/
		hDist[i] = hammingDistance(outC, outA ^ outB);

		/* Verification of correctness */
		if (hDist[i] == 0) {
			printf("-----------------------------------------------------------------------------------\n");
			printf("Distancia de hamming para f(a xor b) y f(a) xor f(b) es 0. No se cumple el criterio\n");
			printf("-----------------------------------------------------------------------------------\n");
		}
	}

	/*Mostrar resultados*/
	printf("distanciasH = c(");
	for(i = 0; i < N_TESTS - 1; i++) {
		printf("%d, ", hDist[i]);
	}
	printf("%d)\n", hDist[i]);

	/* Correct exit */
	return 0;
}