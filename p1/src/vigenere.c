#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ERR -1


char *vigenereC(char *pT, char* k, int size)
{
	char *e=(char*)malloc(size); 

	for (int i=0, j=0; i < size; i++, j++, j = j%strlen(k))
		e[i] = (pT[i] - 97 + k[j] - 97) % 26 + 97;
	
	return e;
}

char* vigenereD(char *pT, char* k, int size)
{
	int mod;
	char *d=(char*)malloc(size); 

	for (int i=0, j=0; i < size; i++, j++, j = j%strlen(k)){
		mod = (pT[i] - k[j]) % 26;
		d[i] = (mod >= 0 ? mod : 26 + mod) + 97;
    }

	return d;

}


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Para ejecutar el programa: ./vigenere {-C|-D} {-k clave} [-i input file] [-o output file].\n");
        return ERR;
    }


    /* REQUIREMENT ARGUMENTS */

    char k[512]="";

    FILE *input = NULL;
    FILE *output = stdout;


    int c;
    int flagC=0, flagD=0, flagK=0;

    char *buffer=NULL;
    char *pT=NULL;
    int aux=0;
    int size=0;

    while ((c = getopt(argc, argv, "CDk:i:o:")) != -1)
    {
        switch(c)
        {
            case 'C':
                flagC = 1;
                break; 
            
            case 'D':
                flagD=1;
                break;
            
            case 'k':
                if(isalpha(*optarg) == 0)
                {
                    fprintf(stderr, "Error al introducir la clave. No puede contener caracteres numericos.\n");
                    return ERR;
                }
                flagK=1;
                strcpy(k, optarg);
                break;
            
            case 'i':
                input = fopen(optarg, "r");
                if(!input)
                {
                    printf("Error: el archivo introducido por argumento no existe.\n");
                    return ERR;
                }
                break;
            
            case 'o':
                output = fopen(optarg, "w");
                break;
            
            case '?':
                fprintf(stderr, "Para ejecutar el programa: %s {-C|-D} {-k clave} [-i input file] [-o output file].\n", argv[0]);
                return ERR;
                break;
        }
    }

    if(flagK == 0 || (flagC == 0 && flagD == 0) || (flagC == 1 && flagD == 1))
    {
        fprintf(stderr, "Para ejecutar el programa: %s {-C|-D} {-k clave} [-i input file] [-o output file].\n", argv[0]);
        return ERR;
    }

    if(input == NULL)
        input = stdin;
    
    buffer = (char*)malloc(512);
    
    do
    {
        if(input == stdin)
        {
            pT=buffer;
            fscanf(input, "%s", pT);
        }
        else
        {
            aux = fread(buffer, 1, 512, input);
            size += aux;
            buffer = (char*)realloc(buffer, aux+1);
            pT = (char*)realloc(pT, size+1);
            strncat(pT, buffer, aux);
        }
        
    }while(input != stdin && aux==512);

    char *e=NULL, *d=NULL;

    if(flagC)
    {
        e = vigenereC(pT, k, size != 0 ? size : strlen(pT));
        fwrite(e, 1, size>0 ? size : strlen(e), output);
        free(e);
    }
    else if(flagD)
    {
        d = vigenereD(pT, k, size != 0 ? size : strlen(pT));
        fwrite(d, 1, size>0 ? size : strlen(d), output);
        free(d);
    }

    printf("\n");
    free(pT);
    fclose(input);
    fclose(output);


    return 0;
}
