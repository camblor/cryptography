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

mpz_t* inverso(mpz_t a ,mpz_t base){
	mpz_t x , y, d, a1;
	mpz_t *resp;
	mpz_t x2, x1, y2, y1, q, r,aux,auxbase;
  	resp = (mpz_t*) malloc(3*sizeof(mpz_t));
	mpz_init_set(auxbase,base);
	mpz_init_set(a1,a);
	mpz_init(y);
	mpz_init(d);
	mpz_init(x);
	mpz_init(x1);
	mpz_init(x2);
	mpz_init(y2);
	mpz_init(y1);
	mpz_init(q);
	mpz_init(aux);
	mpz_init(r);
	if (mpz_get_si(auxbase)==0) {
    	mpz_init_set(resp[0], a1);
    	mpz_init_set_si(resp[1], 1);
    	mpz_init(resp[2]);
  	}
	else {
		mpz_set_si(x2, 1);
		mpz_set_si(x1, 0);
		mpz_set_si(y2, 0);
		mpz_set_si(y1, 1);
		mpz_set_si(q, 0);
		mpz_set_si(r, 0);
		while(mpz_get_ui(auxbase)>0) {
			mpz_cdiv_q(q,a1,auxbase); //q = (a/b);
			
			mpz_mul(aux,q,auxbase);
			mpz_sub(r,a1,aux);			//r = a - q*b;

			mpz_mul(aux, q,x1);			
			mpz_sub(x,x2,aux);			//x = x2-q*x1;
			
			mpz_mul(aux, q,y1);
			mpz_sub(y,y2,aux);			//y = y2 - q*y1;
			
			mpz_set(a1,auxbase);		//a = b;

			mpz_set(auxbase,r);			//b = r

			mpz_set(x2,x1);				//x2 = x1

			mpz_set(x1,x);				//x1 = x

			mpz_set(y2,y1);				//y2 = y1
			
			mpz_set(y1,y);				//y1 = y
		}
		mpz_init_set(resp[0], a1);
		mpz_init_set(resp[1], x2);
	    mpz_init_set(resp[2], y2);
	}
	mpz_clear(auxbase);
	mpz_clear(a1);
	mpz_clear(y);
	mpz_clear(d);
	mpz_clear(x);
	mpz_clear(x1);
	mpz_clear(x2);
	mpz_clear(y2);
	mpz_clear(y1);
	mpz_clear(q);
	mpz_clear(aux);
	mpz_clear(r);
	return resp;
}

int euclidesExtendidoRec(int a, int b, int mcd, int x, int y) {

  int x2=0,y2=0,x1=0,y1=0;
  if (b == 0)  { 
      mcd = a;
      x2 = 1;
      y2 = 0;
  }
  else {     
      mcd = euclidesExtendidoRec (b,a%b,mcd,x,y);
      x1= x2; y1=y2; x2=y1;
      y2=x1- (a/b)*y1;   
  }
  
  return mcd;
}
/*Devuelve array con inversos multiplicativos posibles para Zm
 * Se debe liberar memoria del retorno
 */

int* invMult(int base, int* n_inversos) {
    
    /* Preparación de variables */
	int * inverses = (int*) malloc (base * sizeof(int));	
	int inv = 0;
	int num;
    *n_inversos = 0;
	

	for (num = 0; num < base; num++) {
		inv = euclidesExtendidoRec(num, base, 0, 0, 0);
		if(inv == 1) {
			inverses[*n_inversos] = num;
			(*n_inversos)++;
		}
	}

	inverses = (int *) realloc(inverses, (*n_inversos) * sizeof(int));
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
    char buffer[] = "holaquetalhhhh";
    long length;
    FILE * f;
    int n_inv;
    int* invMults = invMult(MOD_LENGUAJE, &n_inv);
    
    //inputflag = strcmp(options.input, "stdin");
    outputflag = strcmp(options.output, "stdout");
    /*
    

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
    */

    

    
    /* Variable Initialization for libGMP*/
    mpz_init(modulus);
    mpz_init(multiplier);
    mpz_init(constant);
    mpz_init(a_inverse);
    mpz_init(y);
    mpz_init(gcd);

    /* Store as gmp variables*/
    /*
    mpz_set_str(multiplier, options.afin_mult, 10);
    mpz_set_str(constant, options.afin_const, 10);
    mpz_set_str(modulus, options.ciphertext_size, 10);
    */
    mpz_set_si(modulus, MOD_LENGUAJE);
    mpz_set_si(multiplier, invMults[rand() % n_inv]);
	mpz_set_si(constant, rand() % MOD_LENGUAJE);

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
    //free(buffer);
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
    int* invMults = invMult(MOD_LENGUAJE, &n_inv);

    inputflag = strcmp(options.input, "stdin");
    //outputflag = strcmp(options.output, "stdout");
    
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

    srand(4);
    /* Variable Initialization for libGMP*/
    
    mpz_init(modulus);
    mpz_init(multiplier);
    mpz_init(constant);
    mpz_init(a_inverse);
    mpz_init(y);
    mpz_init(gcd);

    /* Store as gmp variables*/
    mpz_set_si(modulus, MOD_LENGUAJE);
    mpz_set_si(multiplier, invMults[rand() % n_inv]);
	mpz_set_si(constant, rand() % 26);

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

    outputflag = 0;
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

//La semilla funciona como clave del cifrado.
//Genera los parámetros a y b para cada letra a cifrar.
char* afinmejoradoC(char* text, int seed, int size){
	int i, ninversos;
	int *inversos;
	char* c = NULL;
	mpz_t x, a, b, zm;
	c = (char *) malloc(size);
	srand(seed);
	
	//Obtener un array con los numeros con inverso multiplicativo posibles para z26
	inversos = invMult(26, &ninversos);
	mpz_init(x);
	mpz_init(a);
	mpz_init(b);
	mpz_init(zm);
	mpz_set_si(zm, 26);
	for (i = 0; i < size; i++){
		//Obtener a, b
		mpz_set_si(a, inversos[rand() % ninversos]);
		mpz_set_si(b, rand() % 26);

		mpz_set_si(x, (long)(text[i] - 97));//x = text[i] - 97;
		mpz_mul(x, x, a); 					//x *= inversos[a];
		mpz_add(x, x, b); 					//x += b;
		mpz_mod(x, x, zm);					//x = x % 26;

		c[i] = (char)(mpz_get_si(x));
		c[i] += 97;
	}
	mpz_clear(x);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(zm);
	free(inversos);
	return c;
}
char* afinmejoradoD(char* text, int seed, int size){
	int i, ninversos;
	int *inversos;
	char* d = NULL;
	mpz_t x, a, b, zm, inv;
	mpz_t* mcd= NULL;
	d = (char*) malloc(size);
	srand(seed);
	inversos = invMult(26, &ninversos);


	mpz_init(x);
	mpz_init(a);
	mpz_init(b);
	mpz_init(zm);
	mpz_set_si(zm, 26);

	for(i = 0; i < size; i++) {

		mpz_init(inv);
		mpz_set_si(a, inversos[rand() % ninversos]);
		mpz_set_si(b, rand() % 26);

		
		//Calcular el inverso de a, para poder multiplicar
		mcd=inverso(a,zm);
		mpz_set(inv, mcd[1]);
		if(mpz_sgn(mcd[0]) == -1){
			mpz_sub(inv, inv, mcd[1]);
			mpz_sub(inv, inv, mcd[1]);
			mpz_add(inv, inv, zm);
		}

		mpz_set_si(x, (long)(text[i] - 97));
		mpz_sub(x, x, b);
		mpz_mod(x, x, zm);
		mpz_mul(x, x, inv);
		mpz_mod(x, x, zm);
		d[i] = mpz_get_si(x);
		d[i] +=97;
		//Limpiar mcd
		mpz_clear(mcd[0]);
		mpz_clear(mcd[1]);
		mpz_clear(mcd[2]);
		mpz_clear(inv);
		free(mcd);
	}
	mpz_clear(x);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(zm);
	mpz_clear(inv);
	free(inversos);
	return d;
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
    optionsXD.output = malloc((strlen("j.dea") + 1) * sizeof(char));
    optionsXD.input = malloc((strlen("j.dea") + 1) * sizeof(char));
    strcpy(optionsXD.input, "j.dea");
    strcpy(optionsXD.output, "hehe");

    CifrarTexto(optionsXD);
    strcpy(optionsXD.input, "hehe");
    DescifrarTexto(optionsXD);

	buff = (char*)malloc(512);
					
	do{
		if(fin==stdin){
			text=buff;
			fscanf(fin,"%s",text);
		}else{
			end=fread(buff,1,512, fin);
			size+=end;
			buff = (char*) realloc(buff,end+1);
			text = (char*) realloc(text,size+1);
			strncat(text, buff,end);
		}
	}while(fin!=stdin && end==512);

	if(flagC){

		c = afinmejoradoC(text, seed, size>0?size:strlen(text));
		fwrite(c,sizeof(char),size>0?size:strlen(text),fout);
		free(c);
	}
	else if(flagD){
		d = afinmejoradoD(text, seed, size>0?size:strlen(text));
		fwrite(d,1,size>0?size:strlen(text),fout);
		free(d);

	}
	if(fout==stdout) printf("\n");
	free(text);
	if(fin !=stdin) free(fin);
	if(fout !=stdout)free(fout);
	return 0;
}