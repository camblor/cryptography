#include <stdint.h>
#include <stdio.h>
#include <getopt.h>

#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))

void initialize_aes_sbox(unsigned char sbox[256]) {
	uint8_t p = 1, q = 1;
    int count=0;
	
	/* loop invariant: p * q == 1 in the Galois field */
	do {
		/* MULTIPLICAMOS P POR 3 */
		p = p ^ (p << 1) ^ (p & 0x80 ? 0x11B : 0);

		/* DIVIDIMOS Q ENTRE 3 (LO QUE ES LO MISMO QUE MULTIPLICARLO POR 0xF6) */
		q ^= q << 1;
		q ^= q << 2;
		q ^= q << 4;
		q ^= q & 0x80 ? 0x09 : 0;

		/* REALIZAMOS LA TRANSFORMACION AFIN */
		uint8_t xtime = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);

		sbox[p] = xtime ^ 0x63;
	} while (p != 1);

	/* ANIADIMOS EL VALOR 0x63 YA QUE EL 0 ES UN CASO ESPECIAL PUESTO QUE NO TIENE INVERSO */
	sbox[0] = 0x63;

    /*
	for(int i=0 ; i<256 ; i++, count++)
    {
        if(count==16)
        {
            printf("\n");
            count = 0;
        }
        printf("%x\t", sbox[i]);
    }

    printf("\n\n");
	*/
}

void get_inv_sbox(uint8_t inv_sbox[256], uint8_t sbox[256])
{
	int i, j;

	for(i=0 ; i<256 ; i++)
		for(j=0 ; j<256 ; j++)
			if(i==sbox[j])
				inv_sbox[i] = j;
	
}

void print_sbox(FILE *output, uint8_t sbox[256])
{
	int count=0;

	for(int i=0 ; i<256 ; i++, count++)
    {
        if(count==16)
        {
            fprintf(output, "\n");
            count = 0;
        }
			fprintf(output, "0x%02x\t", sbox[i]);   
    }
    fprintf(output, "\n\n");
}

int main(int argc, char* argv[])
{
	char opt;
	int flagC=0;
	int flagD=0;
	FILE *output=NULL;

    uint8_t sbox[256]="";
	uint8_t inv_sbox[256]="";

	while((opt = getopt (argc, argv, "CDo:")) != -1)
	{
		switch (opt)
		{
		case 'C':
			flagC = 1;
			break;
		
		case 'D':
			flagD = 1;
			break;
		
		case 'o':
			output = fopen(optarg, "w");
			if(!output)
			{
				fprintf(stderr, "Error al crear el archivo.\n");
				return -1;
			}
			break;
		
		case '?':
			fprintf(stderr, "Para ejecutar el programa: %s {-C | -D} [-o outpur file].\n", argv[0]);
			return -1;
			break;
		}
	}

	if( (flagC==1 && flagD==1) || (flagC==0 && flagD==0))
	{
		fprintf(stderr, "Para ejecutar el programa: %s {-C | -D} [-o outpur file].\n", argv[0]);
		return -1;
	}

	if(!output)
		output = stdout;

	if(flagC)
	{
		initialize_aes_sbox(sbox);
		print_sbox(output, sbox);
	}
	else if(flagD)
	{
		initialize_aes_sbox(sbox);
		print_sbox(output, sbox);

		get_inv_sbox(inv_sbox, sbox);
		print_sbox(output, inv_sbox);
	}

	fclose(output);

    return 0;
}