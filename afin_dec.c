#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //toupper()

/*
Descifrador de cifrado afín por Arget.
Uso:
affine-decipher a b ciphertext
Usa el siguiente alfabeto
ABCDEFGHIJKLMNOPQRSTUVWXYZ
Con A=1, B=2...
*/

int cim(int a, int m);

int main(int argc, char **argv)
{
    if (argc == 1)
        exit(-1);

    int a = atoi(argv[1]),
        b = atoi(argv[2]),
        m = 26,
        i,
        len = strlen(argv[3]),
        x,
        ai = cim(a, m);
    char *ciphertext;
    ciphertext = (char *)malloc(len);
    strcpy(ciphertext, argv[3]);

    if (!ai)
    {
        printf("Error!");
        exit(-2);
    }

    for (i = 0; i < len; i++)
    {
        x = ciphertext[i] - 64;
        x = ((ai * abs((x - b))) % m) + 64;
        printf("%c", x);
    }

    return 0;
}

int cim(int a, int m)
{
    int b, //Almacena el valor de b en (a * b)(mod m)
        x; //Almacena el resultado de la op.

    for (b = 0; b < m; b++)
    {
        x = (a * b) % m;
        if (x == 1)
            return b;
    }
    return 0;
}