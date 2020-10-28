#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <gmp.h>
#include <ctype.h>

#define ERR -1
#define BUFSIZE 512
#define MOD_LENGUAJE 26

/* Boolean data type*/
typedef enum { false, true } bool;

/* Parameters data type*/
typedef struct {
    bool mode;
    char* seed;
    char* input;
    char* output;

} parameters_afin;

mpz_t* invMult(int base, int* n_inversos) {
    
    /* Preparación de variables */
	mpz_t * inverses = (mpz_t*) malloc (base * sizeof(mpz_t));	
	mpz_t modulus, multiplier, y, a_inverse, gcd;
	int num;
    *n_inversos = 0;
	mpz_init(modulus);
    mpz_init(multiplier);
    mpz_init(a_inverse);
    mpz_init(y);
    mpz_init(gcd);

	mpz_set_si(modulus, base);
	
	
	for (num = 0; num < base; num++) {
		mpz_set_si(multiplier, num);
		mpz_gcdext(gcd, a_inverse, y, multiplier, modulus);
		if (mpz_get_si(gcd) == 1){
			mpz_init(inverses[*n_inversos]);
			mpz_set(inverses[*n_inversos], a_inverse);
			(*n_inversos)++;
		}
	}
	mpz_clear(modulus);
	mpz_clear(multiplier);
	mpz_clear(a_inverse);
	mpz_clear(y);
	mpz_clear(gcd);
	inverses = (mpz_t *) realloc(inverses, (*n_inversos) * sizeof(mpz_t));
	return inverses;
}


void cipher_afin(mpz_t plaintext, mpz_t quotient, mpz_t constant, mpz_t modulus){
    mpz_mul(plaintext, quotient, plaintext);
    mpz_mod(plaintext, plaintext, modulus);
    mpz_add(plaintext, plaintext, constant);
    mpz_mod(plaintext, plaintext, modulus);

    if (mpz_get_si(plaintext) < 0){
        mpz_add(plaintext, plaintext, modulus);
    }
    mpz_mod(plaintext, plaintext, modulus);
}

void decipher_afin(mpz_t cipherletter, mpz_t a_inv, mpz_t constant, mpz_t modulus){
    mpz_sub(cipherletter, cipherletter, constant);
    mpz_mod(cipherletter, cipherletter, modulus);

    if (mpz_get_si(cipherletter) < 0){
        mpz_add(cipherletter, cipherletter, modulus);
    }

    mpz_mul(cipherletter, cipherletter, a_inv);
    mpz_mod(cipherletter, cipherletter, modulus);

    if (mpz_get_si(cipherletter) < 0){
        mpz_add(cipherletter, cipherletter, modulus);
    }
}

/*
    Function: prepare_string
    Description: Prepares input for encryption.
*/
void prepare_string(char* s, long* length) {
    char* d = s;
	(*length) = 0;
    do {
        while (*d == ' ' || *d == '\n') {
            ++d;
        }
        if (!isspace(*d)) *d = toupper(*d);
		(*length)++;
    } while ((*s++ = *d++));
}

/*
    Function: CifrarTexto
    Description: 
*/
void CifrarTexto(parameters_afin options){
    /* Main variables*/
    mpz_t modulus, multiplier, constant;
    mpz_t a_inverse, y, gcd;
    mpz_t* inputnum;
    int i;
    int inputflag, outputflag;
    char * buffer = 0;
    long length;
    FILE * f;
    int n_inv;
    mpz_t* invMults = invMult(MOD_LENGUAJE, &n_inv);
	srand(atoi(options.seed));
    
    inputflag = strcmp(options.input, "stdin");
    outputflag = strcmp(options.output, "stdout");
    
    

    if(inputflag){
        f = fopen (options.input, "rb");
        if (f)
        {
            fseek (f, 0, SEEK_END);
            length = ftell (f);
            fseek (f, 0, SEEK_SET);
            buffer = (char*) malloc (sizeof(char) * (length + 1));
            if (buffer)
            {
                fread (buffer, 1, length, f);
            }
            fclose (f);
			buffer[length] = 0;
        }
    } else {
        buffer = (char*) malloc(sizeof(char) * (BUFSIZE + 1));
        fscanf(stdin,"%s",buffer);
    }

	
    
	
    

    
    /* Variable Initialization for libGMP*/
    mpz_init(modulus);
    mpz_init(multiplier);
    mpz_init(constant);
    mpz_init(a_inverse);
    mpz_init(y);
    mpz_init(gcd);

    /* Prepare input to cipher */
	mpz_set_si(modulus, MOD_LENGUAJE);
    prepare_string(buffer, &length);
    length = strlen(buffer);
    inputnum = (mpz_t*) malloc(sizeof(mpz_t) * length);

    if (outputflag) f = fopen(options.output, "w+");
    
	
    /* Cipher text */
    
    char temp;
    for(i=0; i< length; i++){
        mpz_init(inputnum[i]);
        mpz_set_d(inputnum[i], buffer[i] % 32 - 1);
		
		mpz_set(multiplier, invMults[rand() % n_inv]);
		mpz_set_si(constant, rand() % MOD_LENGUAJE);

		/* Checks A inverse existance in Z_m */
		mpz_gcdext(gcd, a_inverse, y, multiplier, modulus);
        cipher_afin(inputnum[i], multiplier, constant, modulus);
        temp = (char) mpz_get_si(inputnum[i]);
        if (outputflag){
            fprintf(f, "%c", temp);
        } else {
            fprintf(stdout, "%c", temp);
        }
    }
    
    if (outputflag) fclose(f);
    else printf("\n");

	mpz_clear(gcd);
    mpz_clear(y);
    mpz_clear(a_inverse);

    for(i=0;i<length;i++){
        mpz_clear(inputnum[i]);
    }
    free(inputnum);
	for(i=0;i<n_inv;i++){
		mpz_clear(invMults[i]);
	}
	free(invMults);
    free(buffer);
    mpz_clear(modulus);
    mpz_clear(multiplier);
    mpz_clear(constant);
}

void DescifrarTexto(parameters_afin options){
    /* Main variables*/
    mpz_t modulus, multiplier, constant;
    mpz_t a_inverse, y, gcd;
    mpz_t* inputnum;
    int inputflag, outputflag;
    int i;
    long inputlen;
    char * buffer = 0;
    FILE * f;
    int n_inv;
    mpz_t* invMults = invMult(MOD_LENGUAJE, &n_inv);
	srand(atoi(options.seed));

    inputflag = strcmp(options.input, "stdin");
    outputflag = strcmp(options.output, "stdout");
    
    if(inputflag){
        f = fopen (options.input, "rb");
        if (f){
            fseek (f, 0, SEEK_END);
            inputlen = ftell (f);
            fseek (f, 0, SEEK_SET);
            buffer = (char*) malloc(sizeof(char) * (inputlen + 1));
            if (buffer)
            {
                fread (buffer, 1, inputlen, f);
                
            }
            fclose (f);
			buffer[inputlen] = 0;
        }
    } else {
        buffer = (char*) malloc(sizeof(char) * (BUFSIZE + 1));
        fscanf(stdin,"%s",buffer);
		inputlen = strlen(buffer);
    }

	

    /* Variable Initialization for libGMP*/
    mpz_init(modulus);
    mpz_init(multiplier);
    mpz_init(constant);
    mpz_init(a_inverse);
    mpz_init(y);
    mpz_init(gcd);

    /* Store as gmp variables*/
    mpz_set_si(modulus, MOD_LENGUAJE);
    
    
    
    /* Prepare input to cipher */
    inputnum = (mpz_t*) malloc(sizeof(mpz_t) * inputlen);

    if (outputflag) f = fopen(options.output, "w+");
    /* Cipher text */
    char temp;
    for(i=0; i< inputlen ; i++){
        mpz_init(inputnum[i]);
        mpz_set_d(inputnum[i], buffer[i]);
		mpz_set(multiplier, invMults[rand() % n_inv]);
		mpz_set_si(constant, rand() % 26);

		/* Checks A inverse existance in Z_m */
		mpz_gcdext(gcd, a_inverse, y, multiplier, modulus);

        decipher_afin(inputnum[i], a_inverse, constant, modulus);
        temp = mpz_get_si(inputnum[i]) + 'A';
        if (outputflag){
            fprintf(f, "%c", temp);
        } else {
            fprintf(stdout, "%c", temp);
        }
    }
    
    if (outputflag) fclose(f);
    else printf("\n");

    for(i=0;i<inputlen;i++){
        mpz_clear(inputnum[i]);
    }
    free(inputnum);
	for(i=0;i<n_inv;i++){
		mpz_clear(invMults[i]);
	}
	free(invMults);
    free(buffer);
	mpz_clear(gcd);
    mpz_clear(y);
    mpz_clear(a_inverse);
    mpz_clear(modulus);
    mpz_clear(multiplier);
    mpz_clear(constant); 
}

/* MAIN FUNCTION */
int main(int argc, char *argv[]) {


    /* Parameter options*/
    parameters_afin options;

    /* Default mode: Encryption */
    options.mode = true;
    
    options.input = malloc(sizeof(char) * strlen("stdin") + 1);
    options.output = malloc(sizeof(char) * strlen("stdout") + 1);

    strcpy(options.input, "stdin");
    strcpy(options.output, "stdout");
    /* Required parameters */
    bool seed_flag = false;

    /* Error controller*/
    bool doDefault = false;

    /* ------------------------- */
    /* PROGRAM ARGUMENTS PARSING */
    /* ------------------------- */
    
    /* Variable declaration */
    int c;
    opterr = 0;

    /* Argument parsing with common arguments and list of possible options */
    while ((c = getopt (argc, argv, "CDs:i:o:")) != -1)
    {
        /* Checks if the selected option is in our list*/
        switch (c)
        {
            /* ENCRYPTION */
            case 'C':
                break;

            /* DECRYPTION */
            case 'D':
                options.mode = false;
                break;

            /* SEED */
            case 's':
                if (!isdigit(*optarg)){
                        doDefault = true;
                        break;
                }
                options.seed = (char *) malloc(strlen(optarg) * sizeof(char) + 1);
                strcpy(options.seed, optarg);
				seed_flag = true;
                break;
            
            /* INPUT FILE */
            case 'i':
                options.input = (char *) realloc(options.input, strlen(optarg) * sizeof(char) + 1);
                strcpy(options.input, optarg);                
                break;
            
            /* OUTPUT FILE */
            case 'o':
                options.output = (char *) realloc(options.output, strlen(optarg) * sizeof(char) + 1);
                strcpy(options.output, optarg);
                break;
            
            /* ERROR CONTROL */
            case '?':
            default:
                doDefault = true;
                break;
        }

        /* ERROR CONTROL FUNCTION FOR ARGUMENT PARSING */
        if (doDefault){
            if (optopt == 's')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 'i')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 'o')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option '-%c'.\n", optopt);
            else
                fprintf (stderr, "Wrong value for -%c.\n", c);
            
            free(options.input);
            free(options.output);
            return 1;
        }
    }
    /* ERROR CONTROL, REQUIRED ARGUMENTS: {-E, -D}, -s */
    if( !seed_flag ){
        fprintf (stderr, "REQUIRED ARGUMENTS:\n");
        fprintf (stderr, "\t{-C (Encrypt), -D (Decrypt)} (Default: C)\n");
        fprintf (stderr, "\t-s <seed (number)>\n");
		free(options.input);
        free(options.output);
        return ERR;
    }

    /* ------------- */
    /* FUNCTIONALITY */
    /* ------------- */
    
    if (options.mode){
        CifrarTexto(options);
    } else {
        DescifrarTexto(options);
    }
    free(options.seed);
    free(options.input);
    free(options.output);

    return 0;
}