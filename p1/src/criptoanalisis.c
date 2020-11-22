#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <limits.h>
#include <float.h>

int eucExtend_v2(int a, int b) {

    /* Variables*/
    int resp =0;
    int x = 0, y = 0;
    int x2, x1, y2, y1, q, r;

    /* Base Case */
    if (b==0) {
        resp= a;
    }
    else {
        /* Variable Set*/
        x2 = 1;
        x1 = 0;
        y2 = 0;
        y1 = 1;
        q = 0;
        r = 0;

        /* Sequential algorithm*/
        while(b>0) {
            q = (a/b);
            r = a - q*b;
            x = x2-q*x1;
            y = y2 - q*y1;
            a = b;
            b = r;
            x2 = x1;
            x1 = x;
            y2 = y1;
            y1 = y; 
        }

        resp= a;
    }
    return resp;
}
double absolute(double value){
    /* Absolute value of value */
	if(value<0){
		return value*(-1.0);
	}
	return value;
}

char* ObtenerClave(char* text, int textsize, int coincidencia){

    /* Main variables*/
	char* key = (char*) malloc(sizeof(char) * (coincidencia+1));
	char* str= text;

    /* Auxiliary Variables*/
	int i=0, j=0, max=0;

    /* Helper variables*/
    int** frequency = NULL;
	long int denominador=0, numerador=0;
	float aux=0.0, best=100.0,ref=0.0665384615;

    /* Initialization */
	memset(key, 0, sizeof(char) * coincidencia);
	frequency=(int**) malloc(sizeof(int*) * coincidencia);
	for(i=0;i<coincidencia;i++){
		frequency[i]= (int*) malloc(sizeof(int) * 26);
		memset(frequency[i], 0, sizeof(int) * 26);
	}

    /* Frequency counting */
	for(i=0;i<textsize/coincidencia;i++, str+=coincidencia){
		for(j=0;j<coincidencia;j++){
			frequency[j][str[j]-97]+=1;
		}
	}


    /* Obtain accurate values for key */
	for(i=0;i<coincidencia;i++, best=100.0, max=0){
		for(j=0;j<26;j++){
            /* Previous computation */
			denominador=(long int)textsize/coincidencia*(textsize/coincidencia-1);
			numerador=(long int)frequency[i][j]*(frequency[i][j]-1);
			aux=1.0*numerador/denominador;

            /* Comparison between accurate values*/
			if(best>absolute(ref-aux)){
				best=absolute(ref-aux);
				max=j+93;
				if (max<97)
					max+=26;
			}			
		}

        /* Free memory */
		key[i]=max;
		free(frequency[i]);
		frequency[i]=NULL;
	}

	free(frequency);
	key[i]='\0';
	return key;

}

int IC(char* text, int size, int fLimit){

    /* Iteration and Miscellaneous variables */
	int ret=0, i=1, j=0, k=0;

    /* Frequency table */
    int** frequency= NULL;

    /* Input text storage */
	char* str=NULL;

    /* Auxiliary Variables*/
	float ref=0.06653846153846153L;
    float best=DBL_MAX;
    float ic=0.0,* ics=NULL, aux=0.0;
	long int denominador=0, numerador=0;

    /* Main Functionality */
	for(str=text;i< (fLimit ? size: 32);i++,str=text){
        /* Vector Initialization */
		frequency=(int**)malloc(sizeof(int*)*i);

        /* Create frequency vectors */
		for (j=0; j < i; j++){
			frequency[j]=(int*)malloc(sizeof(int)*26);
            /* Fill with 0 */
			memset(frequency[j], 0, sizeof(int)*26);
		}
		
        /* Acquire memory and fill with 0*/
		ics=(float*)realloc(ics, i*sizeof(float));
		memset(ics, 0, sizeof(float)*i);


        /* Frequency study */
		for(j=0;j<size/i;j++,str+=i){
			for(k=0;k<i;k++){
                /* Frequency counter */
				frequency[k][str[k]-97]+=1;
			}
		}

        /* Storage and Computation of Frequency */
		for(k=0;k<i;k++){
			for(j=0;j<26;j++){
				denominador=(long int)size/i*(size/i-1);
				numerador=(long int)frequency[k][j]*(frequency[k][j]-1);
				aux= 1.0 * numerador / denominador;
				ics[k]= ics[k]+aux;
			}
			free(frequency[k]);
			frequency[k]=NULL;
		}

        /* Adjust */
		for(j=0, ic=0.0; j<i; j++){
			ic+=ics[j];			
		}


        /* Check IC */
		ic/=i;		
		if(best>absolute(ref-ic)){
			best=absolute(ref-ic);
			ret= j;
		}

        /* Free memory */
		free(frequency);
		frequency= NULL;
	}
	free(ics);
	return ret;

}

long int TestKasiski(char * text, int size, int ngramas){

    /* Main Variables*/
    int pos=0;
	char* str=text;
	char* ngramhelper= (char*) malloc(ngramas+2);

    /* N-gram variables */
	char* ng1=NULL,* ng2=NULL,* ng3=NULL;

    /* Hash Table Variables*/
	jwHashTable* hashtable =create_hash(1000000);
    jwHashEntry* entry= NULL;

    /* Helper variables*/
	long int mcd =0;
	int value=0;
    int i=1;
	long int values=0, nums;

    /* Test variables */
    int kasiski=0;
    long int max=0;

	while(pos + ngramas< size){

        /* Working with a copy of original string*/
		strncpy(ngramhelper, str, ngramas);
		ngramhelper[ngramas]='\0';
        
        /* Helper computations with ngrams*/
		ng1=str;
		ng1=strstr(ng1,ngramhelper);
		if(!ng1) break;        
		ng2=strstr(ng1+ngramas,ngramhelper);

        /* Second step in ngrams*/
		do{
			if(!ng2) break;

            /* Last ngram step*/
			ng3 = ng2+ngramas;
			ng3 = strstr(ng3,ngramhelper);

			do{
				if(!ng3) break;
                /* Obtain mcd */
				mcd=eucExtend_v2(ng2-ng1, ng3-ng1);

                /* Hash table only has two cases for update */
				if(HASHNOTFOUND==get_int_by_int(hashtable, mcd, &value )){
                    /* Add to table*/
					add_int_by_int(hashtable, mcd, 1 );
				}
				else{
                    /* Update table values*/
					value++;
					values++;
					add_int_by_int(hashtable, mcd, value);
				}
                /* Update per 3rd n-gram iteration*/
				nums ++;
				ng3=strstr(++ng3,ngramhelper);
			}while(ng3);
        
            /* Update per 3rd n-gram iteration*/
			ng2=strstr(++ng2,ngramhelper);
		}while(ng2);

		str +=1;
        pos++;
	}

    /* Searchs for best coincidence. */
	for(entry=hashtable->bucket[i] ; i < hashtable->buckets ; i++ , entry=hashtable->bucket[i]){
        /* Check if exists*/
		if(entry==NULL)
			continue;

        /* If so, compare with actual values */
		if(entry->value.intValue * entry->key.intValue > values){
			if(entry->value.intValue * entry->key.intValue > max){
				max=entry->value.intValue * entry->key.intValue;
				kasiski=entry->key.intValue;
			}
		}
	}

    /* Free Memory */
	free(ngramhelper);
	return kasiski;
}

int main (int argc, char **argv){

    /* Main arguments*/
    char *text=NULL;
	int ngramas=0,size=0;
    static int fLimit=0;

    /* Helper variables*/
    char opt;
    char *buff=NULL;
    int long_index=0;
	int fk = 0, end=0;
    FILE  * fout =stdout, *fin=NULL;
    int reading = 0;
	

    /* Main variables*/
    int indicecoincidencia=0;
    long kasiski;
    char* clavefinal;


    /* ARGUMENT PARSING */
	static struct option options[] = {
	    {"l",required_argument,0,'2'},
	    {"i",required_argument, 0, '3'},
	    {"o",required_argument, 0, '4'},
	    {"nl",no_argument,&fLimit, 1},
	    {0,0,0,0}
	};	
	while ((opt = getopt_long_only(argc, argv,"2:3:4:", options, &long_index )) != -1){
		switch(opt){
			case '2':
				fk=1;
				ngramas=atoi(optarg);
			break;
			case '3':
				fin=fopen (optarg, "r");
				if(fin==NULL){
					printf("\nError: el archivo %s no existe\n", optarg);
					return -1;
				}
			break;
			case '4':
				fout=fopen (optarg, "w");
				

			break;
			case'?':
				printf("%s {-l} [-i file in ] [-o file out ] [-nl ]\n", argv[0]);
			break;

		}
	}
	if (fk==0){
		printf("%s {-l} [-i file in ] [-o file out ] [-nl ]\n", argv[0] );
		return 0;
	}


    /* Input reading*/
	if(fin==NULL)
		fin=stdin;

    /* Buffer and Text Memory allocation */
	text=(char*)malloc(512);
	buff=(char*)malloc(512);

    /* Sequential reading */
	do{
		if(fin ==stdin)
			fscanf(fin, "%s", text);
		else{
			end=fread(buff,1,512, fin);
			size+=end;
			buff = (char*) realloc(buff,end);
			text = (char*) realloc(text,(size+1) * sizeof(char));
            if (!reading){
                text[0] = 0;
                reading = 1;
            }
            /* Store buffer */       
			strcat(text, buff);
		}
	}while(fin!=stdin&&end==512);

    /* Prepare text variable */
	text = (char*) realloc(text,size+1);
	text[size]=0;
    
	

    /* Functionality*/
    if(size < 0) {
        size = strlen(text);
    }
    kasiski = TestKasiski(text,size,ngramas);
	printf("Tamaño de clave (Kasiski): %ld\n", kasiski);   

	indicecoincidencia=IC(text, size, fLimit);
	printf("Índice de coincidencia: %d\n",indicecoincidencia );

    clavefinal = ObtenerClave(text, size,indicecoincidencia);
    clavefinal = realloc(clavefinal, sizeof(char) * (kasiski + 1));
    clavefinal[kasiski] = 0;
	printf("Clave de cifrado Vignere: %s\n", clavefinal);
    fprintf(fout, "%s\n", clavefinal);

	
	free(text);
    fclose(fout);
    free(clavefinal);
	free(buff);
    return 0;
}
