#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#define ERR -1
#define OK 0
#define BUFSIZE 512

typedef enum { false, true } bool;


typedef struct {
    char* text;
    int* key1;
    int* key2;
    int ptsize;
    int k1size;
    int k2size;

} parameters_perm;


/* Function that stores all values of a key*/
int* parse_key(char* key, int* keysize) {
	char* ptr;
	int* intbuffer = (int*) malloc((strlen(key) + 1) * sizeof(int));

    /* Initialize key size */
    *keysize = 0;

    /* Remove spaces from key and read 1 by 1 key values. */
	ptr = strtok(key, " ");

    /* Store key values */        
	while(ptr != NULL) {		
		intbuffer[(*keysize)] = atoi(ptr);
		ptr = strtok(NULL, " ");
        *keysize += 1;
	}
    
    /* Return stored key */
	return intbuffer;
}

char* permutation_cipher(char* text, parameters_perm params){
    /* Matrix size*/
    int matsize = params.k1size * params.k2size;

    /* Output size having in mind blocks */
    int finalsize = (params.ptsize + matsize) - (params.ptsize % matsize);
    
    /* Auxiliary variables*/
    int index, textindex;
    int total_i, key1_i, key2_i;
    char temp;
    char* output = (char*) malloc(sizeof(char)*(finalsize+1));
    
	/* Block Iteration */
	for(total_i=0; total_i < finalsize ; total_i+=matsize){
        /* Iterate first key */

		for(key1_i=0;key1_i<params.k1size;key1_i++){
            /* Iterate second key for every first key*/

			for(key2_i=0; key2_i<params.k2size; key2_i++){
                /* Compute index */
                index = total_i + params.key2[key2_i] * params.k1size + params.key1[key1_i];
                textindex = total_i + key1_i + key2_i * params.k1size;

                /* Two main scenarios */
				if(textindex >= params.ptsize){
                    /* Filling empty block with random values*/
                    temp = 'f';                    
					output[index]= temp;
					
				}
				else{
                    /* Casual permutation */
					output[index]=text[textindex];	
				}
				
			}
		}
	}
	output[total_i]=0;
    
	return output;

}

char* permutation_decipher(char * text, parameters_perm params){
    /* Key Matrix size */
    int matsize = params.k1size * params.k2size;

    /* Auxiliary variables*/
	int pt_i, key1_i, key2_i;    
    int index1, index2;
	char* output = (char*) malloc( sizeof(char) * (params.ptsize + params.ptsize%(matsize)+1));


    /* Iterates encrypted text by blocks */
	for(pt_i=0; pt_i<params.ptsize ; pt_i += matsize){

        /* Iteration of key 1 */
		for(key1_i=0; key1_i<params.k1size; key1_i++){

            /* Iteration of key 2 */
			for(key2_i=0; key2_i<params.k2size; key2_i++){

                /* Index computation and deciphering */
                index1 = (key2_i * params.k1size) + pt_i + key1_i;
                index2 = (params.key2[key2_i] * params.k1size) + pt_i + params.key1[key1_i];
				output[index1] = text[index2];
			}
		}
	}
	output[pt_i]=0;
	return output; 
}



int main (int argc, char **argv){

    /* Parameters Variables */
    parameters_perm params;    
	int lg_idx=0;
	char opt;
	static int flagD=0,flagC=0;

    /* Cipher / Decipher variables */
	char *plaintext=NULL;
    char *output=NULL; 

    /* File variables */
    char *buff=NULL;
	FILE *fout = stdout, *fin = stdin;
	char fltr[100]="";

    /* Reading variables */
    int end=0;
	int size = 0;

    /* Control flags*/
    bool key1_flag = false, key2_flag = false;
	
	static struct option options[] = {
	    {"C",no_argument,&flagC,1},
	    {"D",no_argument,&flagD, 1},
	    {"k1",required_argument,0,'1'},
	    {"k2",required_argument,0,'2'},
	    {"i",required_argument, 0, '3'},
	    {"o",required_argument, 0, '4'},
	    {0,0,0,0}
	};

    /* Pseudo-Random number generation */
	srand(time(NULL));

    /* Parameter Handling */
	while ((opt = getopt_long_only(argc, argv,"1:2:3:4:", options, &lg_idx )) != -1){
		switch(opt){
			case '1':
				params.key1 = parse_key(optarg, &(params.k1size));
				key1_flag = true;
			    break;
			case '2':
				params.key2 = parse_key(optarg, &(params.k2size));
				key2_flag = true;
			    break;
			case '3':
				strcat(fltr, optarg);
				fin=fopen (fltr, "r");
				if(fin==NULL){
					printf("\nError: el archivo %s no existe\n", optarg);
					return 0;
				}
			    break;
			case '4':
				fout=fopen (optarg, "w+");
			    break;
			/* ERROR CONTROL */
            case '?':
            default:
                break;

		}
	}
	if (!key1_flag || !key2_flag){
        fprintf (stderr, "REQUIRED ARGUMENTS:\n");
        fprintf (stderr, "\t{-C (Encrypt), -D (Decrypt)} (Default: C)\n");
        fprintf (stderr, "\t-k1 <Primera clave, en la forma \"1 2 3 4\">\n");
        fprintf (stderr, "\t-k2 <Segunda clave, en la forma \"1 2 3 4\">\n");
		return ERR;
	}

    /* FILE READING */	
	buff = (char*)malloc(BUFSIZE + 1);	
	do{
		if(fin==stdin){
			plaintext=buff;
			size += fscanf(fin,"%s",plaintext);
		}else{
			end = fread(buff,1,BUFSIZE, fin);
			size += end;
			buff = (char*) realloc(buff, end + 1);
			plaintext = (char*) realloc(plaintext, size + 1);
			strncat(plaintext, buff,end);
		}
	}while(fin!=stdin && end==BUFSIZE);

    params.ptsize = size;


    /* FUNCTIONALITY */
	if(flagC) {
		output = permutation_cipher(plaintext, params);
        fprintf(fout, "%s", output);
	}

	else if(flagD){
		output = permutation_decipher(plaintext, params);
        fprintf(fout, "%s", output);		
	}

    /* Memory free */
    free(output);
	free(plaintext);
    free(buff);
	fclose(fin);
	fclose(fout);
	free(params.key1);
	free(params.key2);
	return 0;
}
