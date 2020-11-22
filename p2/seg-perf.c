#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <tgmath.h>
#include <limits.h>
#include <gmp.h>


int euclidesExtendidoRec(int a, int b, int mcd, int x, int y)
{
    int x1=0, x2=0, y1=0, y2=0;

    if(b == 0)
    { 
        mcd = a;
        x2 = 1;
        y2 = 0;
    }
    else
    {
        mcd = euclidesExtendidoRec(b, a%b, mcd, x, y);
        x1 = x2;
        y1 = y2;
        x2 = y1;
        y2 = x1 - (a/b) * y1;
    }

    return mcd;
}

int* inversos_mult(int m, int *size)
{
	int *buffer = (int*)malloc(m * sizeof(int));
	int *inversos;	
	int inv=0;

	int i, j=0;

	for (i=0 ; i<m ; i++)
    {
		inv = euclidesExtendidoRec(i, m, 0, 0, 0);
		if(inv == 1)
        {
			buffer[j] = i;
			j++;
		}
	}

	inversos = (int*)malloc(j * sizeof(int));

	for(i=0 ; i<j ; i++)
		inversos[i] = buffer[i];
	
	free(buffer);
	*size = j;

	return inversos;
}

char* equiprobable(char *plain_text, int size)
{
    int nInversos, seed;
    int *inv;
    char *aux=NULL;

    int i=0;

    mpz_t x, a, b, zm;

    aux = (char*)malloc(size);
    srand(seed);

    inv = inversos_mult(26, &nInversos);
    mpz_init(x);
	mpz_init(a);
	mpz_init(b);
	mpz_init(zm);
	mpz_set_si(zm, 26);

    for(i=0 ; i<size ; i++)
    {
        mpz_set_si(a, inv[rand() % nInversos]);
        mpz_set_si(b, rand() % 26);

        mpz_set_si(x, (long)(plain_text[i] - 97));
		mpz_mul(x, x, a); 					
		mpz_add(x, x, b); 					
		mpz_mod(x, x, zm);

        aux[i] = (char)(mpz_get_si(x));
        aux[i] += 97;
    }

    mpz_clear(x);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(zm);
	free(inv);

	return aux;
}

int metodo_no_equiprobable(int width)
{
    double rate=width * 1.0;

	int num=width;

	rate /= 10.0;

	while(num >= width){
		num = rand();
		num =- rate * log(num * 1.0 / INT_MAX);
	}

	return num;
}

char* no_equiprobable(char *plain_text, int size){
	int nInversos, seed;
	int *inv;

    int i=0;

	char *aux=NULL;

	mpz_t x, a, b, zm;

	aux = (char*)malloc(size);
	srand(seed);
	
	
	inv = inversos_mult(26, &nInversos);

	mpz_init(x);
	mpz_init(a);
	mpz_init(b);
	mpz_init(zm);
	mpz_set_si(zm, 26);

	for(i=0 ; i<size ; i++)
    {
		mpz_set_si(a, inv[metodo_no_equiprobable(12) % nInversos]);
		mpz_set_si(b, metodo_no_equiprobable(26) % 26);

		mpz_set_si(x, (long)(plain_text[i] - 97));
		mpz_mul(x, x, a); 					
		mpz_add(x, x, b); 					
		mpz_mod(x, x, zm);					

		aux[i] = (char)(mpz_get_si(x));
		aux[i] += 97;
	}

	mpz_clear(x);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(zm);
	free(inv);

	return aux;
}

int* calculo_incidencia(char *plain_text, int size)
{
    int *incidencia=NULL;

    int i=0;

    incidencia = (int*)malloc(sizeof(int) * 26);
    memset(incidencia, 0, sizeof(int) * 26);

    for(i=0; i<size; i++)
        incidencia[plain_text[i]-97]++;
    
    return incidencia;
}

int** probabilidades(char *plain_text, int size, char *aux, int *incidencias)
{
    int **probs=NULL;
    int i=0;

    char *str=plain_text;

    probs = (int**)malloc(sizeof(int*)*26);

    for(i=0 ; i<26 ; i++)
    {
        probs[i] = (int*)malloc(sizeof(int)*26);
        memset(probs[i], 0, sizeof(int)*26);
    }

    for(i=0 ; i<26 ; i++, str=plain_text)
    {
		str = strchr(str, (char)i + 97);

		while(str != NULL)
        {
			probs[i][aux[str - plain_text] - 97]++;
			str++;		
			str = strchr(str, (char)i + 97);
		}
	}

	return probs;
}

int main(int argc, char* argv[])
{

    char opt;
    int flagP=0;
    int flagI=0;

    FILE *input=stdin, *output=NULL;

    char *plain_text=NULL, *buffer=NULL, *metodo=NULL;
    int final=0, size=0;
    int *incidencia=NULL;
    int **probs=NULL;

    int i=0, j=0, k=0;
    double norm=0.0;
 

    while((opt = getopt(argc, argv, "PIi:o:")) != -1)
    {
        switch (opt)
        {
        case 'P':
            flagP = 1;
            break;

        case 'I':
            flagI = 1;
            break;
        
        case 'i':
            input = fopen(optarg, "r");
            if(!input)
            {
                printf("Error: el fichero introducido por argumento no existe.\n");
                return -1;
            }
            break;
        
        case 'o':
            output = fopen(optarg, "w");
            break;
        
        case '?':
            fprintf(stderr, "Para ejecutar el programa: %s {-P | -I} [-i input file] [-o output file].\n", argv[0]);
            return -1;
            break;
        }
    }

    if((flagP==1 && flagI==1) || (flagP==0 && flagI==0))
    {
        fprintf(stderr, "Para ejecutar el programa: %s {-P | -I} [-i input file] [-o output file].\n", argv[0]);
        return -1;
    }

    if(!input)
        input = stdin;
    
    plain_text = (char*)malloc(512);
    buffer = (char*)malloc(512);

    do
    {
        if(input==stdin)
        {
            fscanf(input, "%s", plain_text);
        }
        else
        {
            final = fread(buffer, 1, 512, input);
            size += final;

            buffer = (char*)realloc(buffer, final);
            plain_text = (char*)realloc(plain_text, size);

            strcat(plain_text, buffer);
        }
    }while(input!=stdin && final==512);

    plain_text = (char*)realloc(plain_text, size+1);
    size = input!=stdin ? size : strlen(plain_text);

    incidencia = calculo_incidencia(plain_text, size);
    for(i=0 ; i<26 ; i++)
    {
        if(incidencia[i]==0)
            continue;
        fprintf(output, "Pp(%c) = %lf\n", i+97, incidencia[i]*1.0/size);
    }

    if(flagP)
    {
        metodo = equiprobable(plain_text, size);
    }
    else
    {
        metodo = no_equiprobable(plain_text, size);
    }

    probs = probabilidades(plain_text, size, metodo, incidencia);

    fprintf(output, "\n");

    for(i=0 ; i<26 ; i++)
    {
        for(j=0 ; j<26 ; j++, norm=0.0)
        {
            for(k=0 ; k<26 ; k++)
                norm += probs[k][j] * 1.0 / size;
            
            fprintf(output, "Pp(%c|%c) = %lf\t", i+97, j+97,
                                                ( (probs[i][j] * 1.0 / incidencia[i]) * 
                                                    (incidencia[i] * 1.0 / size)
                                                ) / norm);
        }

        fprintf(output, "\n\n");
    }
    
    return 0;
}
