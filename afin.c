#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "euclid.c"

#define ERR -1


/* Boolean data type*/
typedef enum { false, true } bool;

/* Parameters data type*/
typedef struct {
    bool mode;
    int ciphertext_size;
    int afin_mult;
    int afin_const;
    FILE *input;
    FILE *output;

} parameters_afin;

bool check_a_inverse(int a, int m){
    if (euclid(a, m) == 1){
        return true;
    }
    return false;
}

int cipher_afin(int plaintext, int quotient, int constant, int m){
    int cipherletter = (quotient*plaintext % m);
    cipherletter += (constant % m);
    cipherletter = cipherletter % m;
    return cipherletter;
}

int decipher_afin(int cipherletter, int b, int a_inv, int m){
    cipherletter = (cipherletter - b) % m;
    printf("!!%d ", cipherletter);
    cipherletter = (cipherletter * a_inv) % m;
    return cipherletter;
}

/* MAIN FUNCTION */
int main(int argc, char *argv[]) {

    if (argc < 2){
        fprintf(stderr, "Introduzca al menos un argumento.\n");
        return ERR;
    }

    /* Parameter options*/
    parameters_afin options;

    /* Default mode: Encryption */
    options.mode = false;

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
    while ((c = getopt (argc, argv, "CDm:a:b:i::o::")) != -1)
    {
        /* Checks if the selected option is in our list*/
        switch (c)
        {
            /* ENCRYPTION */
            case 'C':
                break;

            /* DECRYPTION */
            case 'D':
                options.mode = true;
                break;

            /* CIPHERTEXT SPACE SITE */
            case 'm':
                if (isdigit(*optarg) == 0){
                    doDefault = true;
                    break;
                }
                options.ciphertext_size = atoi(optarg);
                ciphertext_size_flag = true;
                break;
            
            /* MULTIPLICATIVE COEFFICIENT (AFIN) */
            case 'a':
                if (!isdigit(*optarg)){
                        doDefault = true;
                        break;
                }
                options.afin_mult = atoi(optarg);
                afin_mult = true;
                break;

            /* CONSTANT TERM (AFIN) */
            case 'b':
                if (!isdigit(*optarg)){
                        doDefault = true;
                        break;
                }
                options.afin_const = atoi(optarg);
                afin_const = true;
                break;
            
            /* INPUT FILE */
            case 'i':
                options.input = stdin;
                break;
            
            /* OUTPUT FILE */
            case 'o':
                options.output = stdout;
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
    int m = options.ciphertext_size;
    int a = options.afin_mult;
    int b = options.afin_const;



    int a_inv_exists = check_a_inverse(options.afin_mult, options.ciphertext_size);
    if(!a_inv_exists){
        fprintf(stderr, "<a> and <m> not primes => ABORTING!");
        return ERR;
    }
    int a_inverse;
    int y;
    gcdExtended(a, m, &a_inverse, &y);

    int conversion_table[m];
    int i;
    for (i=0; i<m; i++){
        conversion_table[i] = cipher_afin(i, options.afin_mult, options.afin_const, m);        
    }

    char inpt[] = "toy prozBANDO";
    int n_in = strlen(inpt);
    int inptnum[n_in];    
    bool caps[n_in];

    for(i=0; i< n_in; i++){
        caps[i] = false;

        if (inpt[i] >= 'A' && inpt[i] <= 'Z'){
            caps[i] = true;
        }

        inptnum[i] = (int)inpt[i] % 32;
    }


    int ciphertext[n_in];
    char decoded[n_in];

    for(i=0; i < n_in; i++){
        printf("%d => ", inptnum[i]);
        ciphertext[i] = cipher_afin(inptnum[i], a, b, m);
        printf("%d => ", ciphertext[i]);
        ciphertext[i] = decipher_afin(ciphertext[i], b, a_inverse, m);
        if(ciphertext[i] < 0){
            ciphertext[i] = m - ciphertext[i];
        }
        printf("%d\n", ciphertext[i]);
    }

    printf("\n");

    for (i=0; i < n_in; i++){

        if (caps[i]){
            decoded[i] = ciphertext[i] + 'A';
        } else {
            decoded[i] = ciphertext[i] + 'a';
        }
        
    }

    printf("%s", decoded);
    
    printf("\n");
    return 0;

    /* COMPROBAR ENTRE ENCRYPT Y DECRYPT MEDIANTE LA BANDERA options.mode */
    /*
    
    */

    /* UTILIZAR LOS VALORES DE 'm', 'a' y 'b' para hacer el cifrado afin. */
}

/*
        cipherletter = cipher_afin(i, options.afin_mult, options.afin_const, m);
        conversion_table[i] = cipherletter;
        cipherletter = decipher_afin(cipherletter, options.afin_const, a_inverse, m)
        */
       