#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main (int argc, char **argv){

	int long_index=0;
	char opt;
	char* text=NULL,* buff=NULL;
	FILE* fout =stdout,* fin=NULL;
	int size=0;
	int fk = 0, end=0, ic=0, i=0,j=0;
	static int flagC=0,flagD=0;
	uint64_t k=0, k2=0, c, m=0;
	char strout[9],* strout2;
	static struct option options[] = {
		{"C",no_argument,&flagC,1},
	    {"D",no_argument,&flagD, 1},
	    {"k",required_argument,0,'3'},
	    {"i",required_argument, 0, '6'},
	    {"o",required_argument, 0, '7'},
	    {0,0,0,0}
	};

	
	while ((opt = getopt_long_only(argc, argv,"3:6:7:", options, &long_index )) != -1){
		switch(opt){
			case '3':
				fk=1;
				sscanf(optarg,"%08lX", (uint64_t*)&k);
				k<<=32;
				sscanf(optarg,"%016lX", (uint64_t*)&k2);
				k|=k2;
				sscanf(optarg,"%08lX", (uint64_t*)&k2);
				break;
			case '6':
				fin=fopen (optarg, "rb");
				if(fin==NULL){
					printf("\nError: el archivo %s no existe\n", optarg);
					return 0;
				}
				break;
			case '7':
				fout=fopen (optarg, "wb");
				break;
			case'?':
				printf("%s {-C |-D -k} [-i file in ] [-o file out ] \n", argv[0]);
				break;

		}
	}
	if (fk==0 || !(flagD || flagC)){
		printf("%s {-C |-D -k} [-i file in ] [-o file out ]\n", argv[0] );
		return 0;
	}
	if(fin==NULL)
		fin=stdin;

	/* Llamada a DES*/
	do{
		if(fin ==stdin){
			fscanf(fin, "%s", text);
			size=strlen(text);

		}
		else{
			/* Lectura en bloques de 64 bits */
			end=fread(&m,sizeof(uint64_t),1, fin);

			/* Adaptación de formato */
			strout2=(char*)&m;

			/* Variable de lectura*/
			size+=end;

			/* Obtención del ciphertext */
			c=des(m,k,flagC);
            

			/* Escritura del ciphertext */
			fwrite(&c,sizeof(uint64_t),1, fout);
		}
		
	}while(fin!=stdin&&end==1);

	/* Liberación de memoria */
	fclose(fout);
	fclose(fin);
	free(text);

	/* Fin de función */
	return 0;
}