#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <limits.h>
#include <float.h>


#include "euclid.h"
#include "jwHash.h"

#define REF 0.06653846153846153L

double valor_absoluto(double x)
{
    if(x<0) return x*(-1.0);

    return x;
}


long int kasiski(char *pT, int size, int n)
{
    int kas=0, i=0, pos=0, value=0;
    char *s=NULL;

    char *auxS=NULL, *token=NULL, *s1=NULL, *s2=NULL, *s3=NULL;

    jwHashTable *ht = create_hash(1000000);

    long int gdc=0, v=0, max=0, num;

    jwHashEntry *node=NULL;

    FILE *file=NULL;

    int *aux_x=NULL, *aux_y=NULL;

    s=pT;

    while(pos++ +n<size)
    {
        strncpy(token, s, n);
        token[n]='\0';

        s1=s;
        s1=strstr(s1,token);

        if(!s1) break;

        s2=strstr(s1, token);

        do
        {
            if(!s2) break;

            s3 = s2+n;
            s3=strstr(s3, token);

            do
            {
                if(!s3) break;

                gdc=euclidExtended(s2-s1, s3-s1, aux_x, aux_y);
                if(get_int_by_int(ht, gdc, &value) == HASHNOTFOUND)
                { 
                    add_int_by_int(ht, gdc, 1);
                }
                else
                {
                    value++;
                    v++;
                    add_int_by_int(ht, gdc, value);
                }

                num++;
                s3=strstr(++s3, token);
                
            } while (s3 != NULL);
            
            s2=strstr(++s2, token);

        }while(s2 != NULL);

        s+=1;
    }

    file=fopen("kasiski.dat", "w");

    for(node=ht->bucket[i] ; i<ht->buckets ; i++, node=ht->bucket[i])
    {
        if(!node) continue;

        fprintf(file, "%d\t%d\n", node->key.intValue, node->value.intValue);
        if(node->value.intValue * node->key.intValue > v)
        {
			if(node->value.intValue * node->key.intValue > max)
            {
				max=node->value.intValue * node->key.intValue;
				kas=node->key.intValue;
			}
        }
    }

    fclose(file);
    free(token);

    return kas;
}


int IC(char *pT, int size, int fLimit)
{
    int result=0;

    int **frecuencias=0;

    char *auxS=NULL;

    float best=DBL_MAX, indice=0.0, aux=0.0;

    float *ic=NULL;

    long int den=0, num=0;

    int i, j, k; // ITERATORS

    for(auxS=pT, i=0 ; i<(fLimit ? size : 32) ; i++, auxS=pT)
    {
        frecuencias=(int**)malloc(sizeof(int*)*i);

        for(j=0; j<i ; j++)
        {
            frecuencias[j]=(int*)malloc(sizeof(int)*26);
            memset(frecuencias[j], 0, sizeof(int)*26);
        }

        ic=(float*)realloc(ic, i*sizeof(float));
        memset(ic, 0, sizeof(float)*i);

        for(j=0 ; j<size/i ; j++, auxS+=i)
            for(k=0 ; k<i ; k++)
                frecuencias[k][auxS[k] - 97] += 1;
        
        for(j=0 ; j<i ; j++)
        {
            for(k=0 ; k<26 ; k++)
            {
                den = (long int) size/i * (size/i - 1);
                num = (long int) frecuencias[j][k] * (frecuencias[j][k] - 1);
                aux = 1.0 * num/den;
                ic[j] = ic[j] + aux;
            }

            free(frecuencias[j]);
            frecuencias[j]=NULL;
        }

        for(j=0, indice=0.0 ; j<i ; j++)
            indice += ic[j];

        indice/=i;

        if(best>valor_absoluto(REF-indice))
        {
            best=valor_absoluto(REF-indice);
            result = j;
        }

        free(frecuencias);
        frecuencias=NULL;
    }
    free(ic);

    return result;
}

int main (int argc, char **argv){
	int long_index=0;

	char opt;

	char *text=NULL, *buff=NULL;

	FILE  *fout=stdout, *input=NULL;

	int ngramas=0, size=0;

	int fk = 0, end=0, ic=0;

	static int fLimit=0;

	static struct option options[] = {
	    {"l",required_argument,0,'3'},
	    {"i",required_argument, 0, '6'},
	    {"o",required_argument, 0, '7'},
	    {"nl",no_argument,&fLimit, 1},
	    {0,0,0,0}
	};

	
	while ((opt = getopt_long_only(argc, argv,"3:6:7:", options, &long_index )) != -1){
		switch(opt){
			case '3':
				printf ("ngramas=  %s\n", optarg);
				fk=1;
				ngramas=atoi(optarg);
			    break;

			case '6':
				printf ("i\n");
				input=fopen (optarg, "r");
				if(input==NULL){
					printf("\nError: el archivo %s no existe\n", optarg);
					return -1;
				}
			    break;

			case '7':
				printf ("o\n");
				fout=fopen (optarg, "w");
			    break;

			case'?':
				printf("%s {-l Ngramas} [-i file in ] [-o file out ] [-nl ]\n", argv[0]);
			    break;
		}
	}

	if (fk==0){
		printf("%s {-l Ngramas} [-i file in ] [-o file out ] [-nl ]\n", argv[0] );
		return 0;
	}

	if(input==NULL)
		input=stdin;

	text=(char*)malloc(512);
	buff=(char*)malloc(512);

	do{
		if(input ==stdin)
			fscanf(input, "%s", text);
		else{
			end=fread(buff, 1, 512, input);
			size+=end;
			buff = (char*) realloc(buff,end);
			text = (char*) realloc(text,size);
			strcat(text, buff);
		}
	} while(input!=stdin && end==512);

	text = (char*) realloc(text,size+1);
	text[size]='\n';


	printf("kasiski empiza, puede tardar\nvarios minutos dependiendo del ordenador\n");
	printf("kasiski=%ld\n",kasiski(text,size > 0 ? size : strlen(text),ngramas));
	ic=IC(text, size, fLimit);
	printf("ic=%d\n",ic );

	return 0;

}

