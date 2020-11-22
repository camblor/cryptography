#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 256   // 2^8

/*
 * Function:  swap 
 * --------------------
 * swaps pointer values (Permutation)
 *
 * *a: first char pointer
 * *b: second char pointer
 * 
 */
void swap(unsigned char *a, unsigned char *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}


/*
 * Function:  KSA 
 * --------------------
 * Key-Scheduling Algorithm.
 *
 * *key: Input key to the algorithm.
 * *vector: N-byte state vector.
 * 
 */
int KSA(char *key, unsigned char *vector) {

    /* Lenght and iterators */
    int len = strlen(key);
    int index = 0;
    int iter;

    /* Vector initialization as equal to index */
    for(iter = 0; iter < N; iter++)
        vector[iter] = iter;

    /* Permutation */
    for(iter = 0; iter < N; iter++) {

        /* Swap target calculation */
        index = (index + vector[iter] + key[iter]) % N;

        /* Swap execution */
        swap(&vector[iter], &vector[index]);
    }
}


/*
 * Function:  PRGA 
 * --------------------
 * Pseudo random generation algorithm
 *
 * *vector: N-byte state vector after KSA algorithm.
 * *plaintext: Data to be ciphered.
 * *ciphertext: N-byte state vector to store ciphered data.
 * 
 */
int PRGA(unsigned char *vector, char *plaintext, unsigned char *ciphertext) {
    /* Size variables */
    size_t len = strlen(plaintext);

    /* Iteration variable */
    size_t n;

    /* Indexing variables */
    int i = 0;
    int j = 0;

    for(n = 0; n < len; n++) {

        /* Swap index calculations according to KSA algorithm initialization with given key*/
        i = (i + 1) % N;
        j = (j + vector[i]) % N;

        /* Swap execution */
        swap(&vector[i], &vector[j]);

        /* Swap */
        int or_target = vector[(vector[i] + vector[j]) % N];

        /* XOR gate encryption with Key Streams and Original text */
        ciphertext[n] = or_target ^ plaintext[n];

    }
}


/*
 * Function:  RC4 
 * --------------------
 * RC4 Encryption Algorithm: Stream cipher algorithm with variable key length.
 * This algorithms encrypts one byte at a time.
 * It has two main computations: Key Generation Algorithm (KSA)
 *                               Pseudo random generation algorithm (PRGA)
 * PRGA makes possible the stream generation.
 *
 * *key: Input key to the algorithm.
 * *plaintext: Data to be ciphered.
 * *ciphertext: N-byte state vector to store ciphered data.
 * 
 */
int RC4(char *key, char *plaintext, unsigned char *ciphertext) {

    /* Vector Initialization with Key using KSA*/
    unsigned char S[N];
    KSA(key, S);

    /* Stream cipher */
    PRGA(S, plaintext, ciphertext);

}

int main(int argc, char *argv[]) {

    if(argc < 3) {
        printf("Usage: %s <key> <plaintext>\n", argv[0]);
        return -1;
    }

    unsigned char *ciphertext = malloc(sizeof(int) * strlen(argv[2]));
    unsigned char *ciphertext2 = malloc(sizeof(int) * strlen(argv[2]));

    printf("Plaintext: %s\n", argv[2]);

    RC4(argv[1], argv[2], ciphertext);
    printf("Ciphertext: ");
    for(size_t i = 0, len = strlen(argv[2]); i < len; i++)
        printf("%02hhX", ciphertext[i]);
    printf("\n");
    RC4(argv[1], ciphertext, ciphertext2);
    printf("Ciphertext: ");
    for(size_t i = 0, len = strlen(argv[2]); i < len; i++)
        printf("%c", ciphertext2[i]);
    printf("\n");

    return 0;
}