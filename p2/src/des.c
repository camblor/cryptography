#include "../include/des.h"

/*
 * Function:  permutation 
 * --------------------
 * 	Permutation generic method for applying a given table permutations
 *
 * 	inpt: Input bits to be operated by substitution boxes.
 * 	table: Table containint pertinent permutations.
 * 	inputsize: Size of Input.
 * 	tablesize: Size of table.
 *
 *  returns: Output bits after each S-Box operation.
 */
uint64_t permutation(uint64_t inpt, const unsigned short *table, int inputsize, int tablesize){
	
	/* Variable iteradora */
	int i;

	/* Variable almacén del valor a partir de las tablas */
	int shift;

	/* Variable del primer bit */
	uint64_t firstb;

	/* Variable de máscara para facilidad de operaciones */
	uint64_t mask;

	/* Variable de salida */
	uint64_t output = 0x0;

	/* Preparación para la expansión */
	if(inputsize < tablesize){
		/* Desplazamiento en función del requisito DES */
		inpt <<= (tablesize - inputsize);

		/* Cambio consiguiente en el tamaño de entrada */
		inputsize += (tablesize - inputsize);
	}

	/* Management of first bit */
	firstb = (uint64_t)1 << (inputsize -1);


	/* Bucle de permutación a partir de una tabla dada */
	for(i = 0; i < tablesize; i++){

		/* Obtención del desplazamiento desde índice de tabla */
		shift = (int) table[i];

		/* Cálculo de la máscara */
		mask = firstb >> (shift - 1);

		/* Cálculo del desplazamiento */
		shift = shift - i - 1;

		/* Evaluación de operación en función del desplazamiento */
		if(shift > 0)
			output = output | ((inpt & mask) << (shift));
		else 
			output = output | ((inpt & mask) >> (-shift));
		
	}

	/* Ajuste previo a la salida final por reducción */
	if(inputsize > tablesize){
		output >>= (inputsize - tablesize);
	}

	/* Retorno del resultado de la permutación */
	return output;
}


/*
 * Function:  sbox 
 * --------------------
 * 	S-Box method containing the 8 S-Boxes operations
 *
 * 	inpt: (48b) Input bits to be operated by substitution boxes.
 *
 *  returns: (32b) Output bits after each S-Box operation.
 */
uint64_t sbox(uint64_t inpt){
	
	/* Variable iteradora*/
	int i;

	/* Variable temporal para el trabajo con bits */
	uint64_t temporal;

	/* Variable de máscara*/
	uint64_t mask = 0xFC0000000000;

	/* Máscaras para bits primero y último (fila) */
	uint64_t maskFirst = 0x800000000000;
	uint64_t maskLast = 0x040000000000;

	/* Máscara para bits intermedios (columna) */
	uint64_t maskBetween = 0x7C0000000000;

	/* Variable del resultado*/
	uint64_t output = 0x0;

	/* Variables utilizadas para los cálculos de fila índice en las S-Boxes */
	uint64_t fila1, fila2;

	/* Variables para el almacenamiento de fila-columna en las S-Boxes */
	uint64_t row, col;


	/* Introducción de la entrada en las 8 S-Boxes*/
	for(i = 0; i < NUM_S_BOXES; i++){
		
		/* Preparación de entrada */
		temporal = inpt & mask;
		
		/* Obtener bits primero y último */
		fila1 = temporal & maskFirst;
		fila2 = temporal & maskLast;

		/* Obtención del valor a partir de los bits primero y último */
		fila1 >>= 4;

		/* Obtención del índice de la fila a partir de los bits anteriores */
		row = (fila1 | fila2) >> SBOX_BLOCK_SIZE * (7 - i);

		/* Obtener 4 bits intermedios */
		col = temporal & maskBetween;

		/* Obtención del índice a partir de los bits intermedios */
		col >>= SBOX_BLOCK_SIZE * (7 - i) + 1;

		/* Obtención de la salida de la S-Box*/
		output |= S_BOXES[i][row][col];

		/* Desplazamiento para hacer sitio a la salida de la siguiente S-Box */
		output <<= 4;

		/* Ajuste de mascaras para la siguiente iteracion */
		mask >>= SBOX_BLOCK_SIZE;

		/* Primero-Ultimo (restablecimiento) */
		maskFirst >>= SBOX_BLOCK_SIZE;
		maskLast >>= SBOX_BLOCK_SIZE;
		/* Intermedios (restablecimiento) */
		maskBetween >>= SBOX_BLOCK_SIZE;
			
	}

	/* Restablecemos el último desplazamiento */
	output >>= 4;

	/* Retorno de la salida de las S-Box */
	return output;
}


/*
 * Function:  rotateDES 
 * --------------------
 * 	Rotation method for a given input, size and rotation parameters
 *
 * 	inpt: Input bits to be rotated.
 * 	rotation: Number of bits to rotate.
 * 	size: Size of the performed rotation.
 * 	mask: Mask used in the rotation.
 *
 *  returns: Array of subkeys used in the round method.
 */
uint64_t rotateDES(uint64_t inpt, int rotation, int size, uint64_t mask) {

	return mask & ((inpt << rotation) | (inpt >> (size - rotation)));
}


/*
 * Function:  keyGeneration 
 * --------------------
 * 	Generates all subkeys from the full key in DES algorithm
 *
 * 	fullkey: Key used in this execution.
 *
 *  returns: Array of subkeys used in the round method.
 */
uint64_t* keyGeneration(uint64_t fullkey){
	
	/* Variable Iteradora*/
	int i;

	/* Variables de clave principal y división de esta clave*/
	uint64_t k;
	uint64_t c;
	uint64_t d;

	/* Array de subclaves */
	uint64_t *subkeys;

	/* Reserva de memoria para el almacenamiento de subclaves*/
	subkeys = (uint64_t *) malloc(ROUNDS * sizeof(uint64_t));
	
	/* Permutación Permuted Choice 1 previo a los LCS*/
	k = permutation(fullkey, PC1, 64, 56);

	/* División de la clave de 56 bits en dos variables de 28bits */
	c = (k & sup56) >> 28;
	d = k & inf56;
	
	/* Generación de subclaves para cada una de las rondas con la clave */
	for(i = 0; i < ROUNDS; i++) {

		/* Rotaciones */
		c = rotateDES(c, ROUND_SHIFTS[i], 28, inf56);
		d = rotateDES(d, ROUND_SHIFTS[i], 28, inf56);
		
		/* Asignación del nuevo valor de la clave a partir de la rotación */
		k = c << 28 | d;

		/* Permutación a partir del resultado de la rotación de la clave*/
		subkeys[i] = permutation(k, PC2, 56, 48);
	}
	
	/* Retorno del array completo con las subclaves necesarias para DES */
	return subkeys;
}



/*
 * Function:  des 
 * --------------------
 * 	Applies DES algorithm in order to generate a ciphertext
 *
 * 	input: Data input to DES algorithm.
 * 	fullkey: Key used in this execution.
 *
 *  returns: ciphertext encrypted by DES.
 */
uint64_t des(uint64_t input,uint64_t fullKey, int ciphermode){

	/* Variables necesarias para la correcta ejecución de DES */
	uint64_t L , R, C;
	int i=0;

	/* Generación de las subclaves a partir de la clave K*/
	uint64_t* subclave=  keyGeneration(fullKey);

	/* Control del modo de ejecución de DES */
	if (!ciphermode){

		/* Reserva de memorias para las subclaves preparadas */
		uint64_t* Ks2 = (uint64_t *)malloc(16 * sizeof(uint64_t));

		/* Si estamos en modo desencriptar invertimos el orden*/
		for (i = 0; i < ROUNDS; i++)
			Ks2[i]  = subclave[ROUNDS -1 -i];

		/* Liberamos la memoria temporal */
		free(subclave);

		/* Y asignamos la variable de subclaves*/
		subclave = Ks2;
	}

	/* Permutación inicial con IP del texto plano */
	input = permutation(input, IP, 64, 64);

	/* Asignación de los bits de mayor y menor peso a variables */
	L = (input & sup64) >> 32;
	R = input & inf64;	

	/* Bucle iterativo para las 16 rondas del DES*/
	for(i=0; i < ROUNDS; i++){

		/* Permutación de R(i-1)*/
		C=permutation(R, E,32,48);

		/* XOR previo a la entrada en las S-Boxes*/
		C^=subclave[i];

		/* Sustitución mediante S-Boxes*/
		C=sbox(C);

		/* Permutación posterior a la sustitución mediante S-Boxes */
		C= permutation(C,P,32,32);

		/* XOR final entre L(i-1) y la salida del proceso con R(i-1)*/
		C=C^L;

		/* R(i-1) pasa a ser la entrada L(i)*/
		L=R;

		/* La salida del proceso con R(i-1) pasa a ser la entrada R(i)*/
		R=C;
	}

	/* 32 bit Swap */
	input = R << 32;
	input |= L;

	/* Permutación final mediante la inversa de la permutación inicial*/
	input=permutation(input,IP_INV,64,64);

	/* Liberación de memoria para las subclaves*/
	free(subclave);

	/* Retorno del resultado del algoritmo DES (input <= paso por valor) */
	return input;
}