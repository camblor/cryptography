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
	int inv = 0;
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
	*length = 0;
    do {
        while (*d == ' ' || *d == '\n') {
            ++d;
        }
        if (!isspace(*d)) *d = toupper(*d);
		*length++;
    } while (*s++ = *d++);
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
        }
    } else {
        buffer = (char*) malloc(sizeof(char) * (BUFSIZE + 1));
        fscanf(stdin,"%s",buffer);
    }

	buffer[length] = 0;
    
	
    

    
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
        }
    } else {
        buffer = (char*) malloc(sizeof(char) * (BUFSIZE + 1));
        fscanf(stdin,"%s",buffer);
    }

	buffer[inputlen] = 0;

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

int main (int argc, char **argv){

	int long_index=0, end=0, size=0, seed = 1;
	char opt;
	static int flagD=0,flagC=0;
	char* text=NULL,*c=NULL,*d=NULL,* buff=NULL,fltr[100]="";
	FILE  * fout =stdout, *fin=stdin;
	int fseed=0;
	static struct option options[] = {
	    {"C",no_argument,&flagC,1},
	    {"D",no_argument,&flagD, 1},
	    {"s",required_argument,0,'3'},
	    {"i",required_argument, 0, '6'},
	    {"o",required_argument, 0, '7'},
	    {0,0,0,0}
	};

	optarg =NULL;
	while ((opt = getopt_long_only(argc, argv,"3:4:5:6:7:", options, &long_index )) != -1){
		switch(opt){
			case '3':
				fseed=1;
				seed = atoi(optarg);
				printf("Seed = %d\n", seed);
			break;
			case '6':
				if(flagC){
					strcpy(fltr,"./filtro ");
					strcat(fltr, optarg);
					strcat(fltr, " F");
					strcat(fltr, optarg);
					printf("orden=%s\n",fltr );
					if(!system(fltr)){
						return 0;
					}
					strcpy(fltr,"F");	
				}
				strcat(fltr, optarg);
				printf("archivo=%s\n",fltr );
				fin=fopen (fltr, "r");
				if(fin==NULL){
					printf("\nError: el archivo %s no existe\n", optarg);
					return 0;
				}
			break;
			case '7':
				fout=fopen (optarg, "w");
			break;
			case'?':
				printf("%s {-C|-D} {-s seed} [-i f ile in ] [-o f ile out ]\n", argv[0]);
			break;

		}
	}
	if (fseed==0 || flagD==0 && flagC==0 || flagD==1 && flagC==1){
		printf("%s {-C|-D} {-s seed} [-i f ile in ] [-o f ile out ]\n", argv[0] );
		return 0;
	}
    parameters_afin optionsXD;
    optionsXD.output = malloc((strlen("hehe") + 1) * sizeof(char));
    optionsXD.input = malloc((strlen("j.dea") + 1) * sizeof(char));
	optionsXD.seed = malloc((sizeof(int)) * 1);
    strcpy(optionsXD.input, "j.dea");
    strcpy(optionsXD.output, "hehe");
	strcpy(optionsXD.seed, "4");

    CifrarTexto(optionsXD);

	optionsXD.output = realloc(optionsXD.output, (strlen("out") + 1) * sizeof(char));
    optionsXD.input = realloc(optionsXD.input, (strlen("hehe") + 1) * sizeof(char));
    strcpy(optionsXD.input, "hehe");
	strcpy(optionsXD.output, "out");

    DescifrarTexto(optionsXD);


	free(optionsXD.output);
	free(optionsXD.input);
	free(optionsXD.seed);

	return 0;
}