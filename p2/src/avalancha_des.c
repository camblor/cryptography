#include "../include/des.h"

/*Cuenta los bits de diferencia entre n1 y n2*/
int hammingDistance(uint64_t n1, uint64_t n2){

    /* Iteration variables */
	int i=0;
	int count=0;

    /* Looping while calculating Hamming Distance */
	for(i=0; i<64; i++, n1>>=1, n2>>=1){
		if( (n1&1) ^ (n2&1) )
			count++;
	}

    /* Returning of result */
	return count;
}

uint64_t desRondas(int rondas, uint64_t M,uint64_t K, int c){
	uint64_t L , R, C;
	int i=0;
	uint64_t* Ks=  keyGeneration(K);
	uint64_t* Ks2 = (uint64_t *)malloc(16 * sizeof(uint64_t));

	if ( !c){
		for (i = 0; i < ROUNDS; i++)
			Ks2[i]  = Ks[ROUNDS -1 -i];
		free(Ks);
		Ks = Ks2;

	}
	M = permutation(M, IP, 64, 64);
	L = (M & sup64) >> 32;
	R = M & inf64;	
	//printf("inicial R=%lx  L=%lx\n",L,R);
	for(i=0; i < rondas; i++){

		C=permutation(R, E,32,48);
	//	printf("expansion C = %lx\n",C );
		C^=Ks[i];
	//	printf("ks[i] xor C = %lx\n",C );
		C=sbox(C);
	//	printf("sbox C = %lx\n",C );
		C= permutation(C,P,32,32);
	//	printf("permutation P = %lx\n",C );
		C=C^L;
	//	printf("L xor C = %lx\n",C );
		L=R;
		R=C;
		//printf("R=%lx  L=%lx\n\n",R,L);
	}

	M = R << 32;
	M |= L;
	M=permutation(M,IP_INV,64,64);

	//printf("M=%lx\n",M );
	free(Ks);
	return M;
}

int* desRondasF(int rondas, uint64_t M,uint64_t K, int c){
	uint64_t L , R, C;
	int i=0;
	int *distanciasH;
	distanciasH = (int*)malloc(16 * sizeof(int));
	uint64_t* Ks=  keyGeneration(K);
	uint64_t* Ks2 = (uint64_t *)malloc(16 * sizeof(uint64_t));

	if ( !c){
		for (i = 0; i < ROUNDS; i++)
			Ks2[i]  = Ks[ROUNDS -1 -i];
		free(Ks);
		Ks = Ks2;

	}
	M = permutation(M, IP, 64, 64);
	L = (M & sup64) >> 32;
	R = M & inf64;	
	//printf("inicial R=%lx  L=%lx\n",L,R);
	for(i=0; i < rondas; i++){

		C=permutation(R, E,32,48);
		C^=Ks[i];
		C=sbox(C);
		C= permutation(C,P,32,32);
		// CALCULA LA DISTANCIAH ENTRE R Y SALIDA SBOX PASADA POR PERM P
		distanciasH[i] = hammingDistance(R, C);
		C=C^L;
		L=R;
		R=C;
		//printf("R=%lx  L=%lx\n\n",R,L);
	}

	M = R << 32;
	M |= L;
	M=permutation(M,IP_INV,64,64);

	//printf("M=%lx\n",M );
	free(Ks);
	return distanciasH;
}


int main (){
    /* Bits input y bits clave*/
	uint64_t m=0x0123456789abcdef, k=0x133457799bbcdff1;

    /* Array de resultados */
    uint64_t* salidas;

    /* Variable iteradora*/
	int i=0;

    /* Array de distancias de Hamming */
	int * distanciasH;

    /* File pointer */
	FILE * fout= stdout;

    /* Reserva de memoria */
	salidas=(uint64_t*) calloc(32, sizeof(uint64_t));

    /* Bits cambio a partir del número de rondas */
	for (i=1; i<=32 ; i++){
		salidas[i-1]=desRondas(i, m, k, 1);
	}

    /* Impresión por pantalla de los valores de distancia de Hamming */
	for (i=0; i<32;i++){
		fprintf(fout, "%d,", hammingDistance(m, salidas[i]));
	}

    /* Bits de cambio entre entrada a s-box y salida de función f por ronda */
	distanciasH = desRondasF(16, m, k, 1);
	printf("\nBits de cambio entre la entrada a la sbox previa permutation y la salida de la función f previa xor para cada ronda\n");

    /* Impresión por pantalla de los valores */
	for(i = 0; i < 16; i++) {
		fprintf(fout, "%d,", distanciasH[i]);
	}
	printf("\n");

    /* Liberación de variables */
	free(distanciasH);
	
    /* Salida correcta */
    return 0;
}