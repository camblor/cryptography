#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "euclid.c"
#include <gmp.h>

#define ERR -1
#define BUFSIZE 512


/* Boolean data type*/
typedef enum { false, true } bool;

/* Parameters data type*/
typedef struct {
    bool mode;
    char* ciphertext_size;
    char* afin_mult;
    char* afin_const;
    char* input;
    char* output;

} parameters_afin;

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
void prepare_string(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
        if (!isspace(*d)) *d = toupper(*d);
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
    int inputlen, inputflag, outputflag;
    char * buffer = 0;
    long length;
    FILE * f;

    inputflag = strcmp(options.input, "stdin");
    outputflag = strcmp(options.output, "stdout");

    if(inputflag){
        f = fopen (options.input, "rb");
        if (f)
        {
            fseek (f, 0, SEEK_END);
            length = ftell (f);
            fseek (f, 0, SEEK_SET);
            buffer = (char*) malloc (sizeof(char) *length);
            if (buffer)
            {
                fread (buffer, 1, length, f);
            }
            fclose (f);
        }
    } else {
        buffer = (char*) malloc(sizeof(char) * BUFSIZE);
        fscanf(stdin,"%s",buffer);
    }

    

    
    /* Variable Initialization for libGMP*/
    mpz_init(modulus);
    mpz_init(multiplier);
    mpz_init(constant);
    mpz_init(a_inverse);
    mpz_init(y);
    mpz_init(gcd);

    /* Store as gmp variables*/
    mpz_set_str(multiplier, options.afin_mult, 10);
    mpz_set_str(constant, options.afin_const, 10);
    mpz_set_str(modulus, options.ciphertext_size, 10);

    /* Checks A inverse existance in Z_m */
    mpz_gcdext(gcd, a_inverse, y, multiplier, modulus);
    if (mpz_get_si(gcd) != 1){
        fprintf(stderr, "<a> and <m> not primes => ABORT\n");
        return;
    }
    mpz_clear(gcd);
    mpz_clear(y);
    mpz_clear(a_inverse);

    /* Prepare input to cipher */
    prepare_string(buffer);
    inputlen = strlen(buffer);
    inputnum = (mpz_t*) malloc(sizeof(mpz_t) * inputlen);

    if (outputflag) f = fopen(options.output, "w+");
    

    /* Cipher text */
    char temp;
    for(i=0; i< inputlen; i++){
        mpz_init(inputnum[i]);
        mpz_set_d(inputnum[i], buffer[i] % 32 - 1);
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

    for(i=0;i<inputlen;i++){
        mpz_clear(inputnum[i]);
    }
    free(inputnum);
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

    inputflag = strcmp(options.input, "stdin");
    outputflag = strcmp(options.output, "stdout");
    
    if(inputflag){
        f = fopen (options.input, "rb");
        if (f){
            fseek (f, 0, SEEK_END);
            inputlen = ftell (f);
            fseek (f, 0, SEEK_SET);
            buffer = (char*) malloc(sizeof(char)*inputlen);
            if (buffer)
            {
                fread (buffer, 1, inputlen, f);
                
            }
            fclose (f);
        }
    } else {
        buffer = (char*) malloc(sizeof(char) * BUFSIZE);
        fscanf(stdin,"%s",buffer);
    }


    /* Variable Initialization for libGMP*/
    mpz_init(modulus);
    mpz_init(multiplier);
    mpz_init(constant);
    mpz_init(a_inverse);
    mpz_init(y);
    mpz_init(gcd);

    /* Store as gmp variables*/
    mpz_set_str(multiplier, options.afin_mult, 10);
    mpz_set_str(constant, options.afin_const, 10);
    mpz_set_str(modulus, options.ciphertext_size, 10);

    /* Checks A inverse existance in Z_m */
    mpz_gcdext(gcd, a_inverse, y, multiplier, modulus);
    if (mpz_get_si(gcd) != 1){
        fprintf(stderr, "<a> and <m> not primes => ABORT\n");
        return;
    }
    mpz_clear(gcd);
    mpz_clear(y);
    
    /* Prepare input to cipher */
    inputnum = (mpz_t*) malloc(sizeof(mpz_t) * inputlen);

    if (outputflag) f = fopen(options.output, "w+");
    /* Cipher text */
    char temp;
    for(i=0; i< inputlen; i++){
        mpz_init(inputnum[i]);
        mpz_set_d(inputnum[i], buffer[i]);
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
    free(buffer);
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
    bool ciphertext_size_flag = false;
    bool afin_mult = false;
    bool afin_const = false;

    /* Error controller*/
    bool doDefault = false;

    /* ------------------------- */
    /* PROGRAM ARGUMENTS PARSING */
    /* ------------------------- */
    
    /* Variable declaration */
    int c;
    opterr = 0;

    /* Argument parsing with common arguments and list of possible options */
    while ((c = getopt (argc, argv, "CDm:a:b:i:o:")) != -1)
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

            /* CIPHERTEXT SPACE SITE */
            case 'm':
                if (isdigit(*optarg) == 0){
                    doDefault = true;
                    break;
                }
            
                options.ciphertext_size = (char *) malloc(strlen(optarg) * sizeof(char) + 1);
                strcpy(options.ciphertext_size, optarg);
                ciphertext_size_flag = true;
                break;
            
            /* MULTIPLICATIVE COEFFICIENT (AFIN) */
            case 'a':
                if (!isdigit(*optarg)){
                        doDefault = true;
                        break;
                }
                options.afin_mult = (char *) malloc(strlen(optarg) * sizeof(char) + 1);
                strcpy(options.afin_mult, optarg);
                afin_mult = true;
                break;

            /* CONSTANT TERM (AFIN) */
            case 'b':
                if (!isdigit(*optarg)){
                        doDefault = true;
                        break;
                }
                options.afin_const = (char *) malloc(strlen(optarg) * sizeof(char) + 1);
                strcpy(options.afin_const, optarg);
                afin_const = true;
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
            if (optopt == 'm')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 'a')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 'b')
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
    /* ERROR CONTROL, REQUIRED ARGUMENTS: {-E, -D}, -m, -a, -b */
    if( !ciphertext_size_flag || !afin_mult || !afin_const ){
        fprintf (stderr, "REQUIRED ARGUMENTS:\n");
        fprintf (stderr, "\t{-C (Encrypt), -D (Decrypt)} (Default: C)\n");
        fprintf (stderr, "\t-m <tamaño del espacio de texto cifrado (value)>\n");
        fprintf (stderr, "\t-a <coeficiente multiplicativo de la función afín (value)>\n");
        fprintf (stderr, "\t-b <término constante de la función afín (value)>\n");
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
    
    free(options.ciphertext_size);
    free(options.afin_mult);
    free(options.afin_const);
    free(options.input);
    free(options.output);

    return 0;
}