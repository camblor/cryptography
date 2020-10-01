#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Este código (creado por Arget) compilado tal cual está se convierte en un
  cifrador afín usado de la siguiente manera:
  affine-cipher a b plaintext
  Siendo 'a' y 'b' las claves y 'plaintext' el texto a cifrar.
  De manera predeterminada considera este alfabeto
  ABCDEFGHIJKLMNOPQRSTUVWXYZ
  Con A=1; B=2... sin embargo se puede modificar siguiendo las intrsucciones,
  Para usar la 'Ñ' el valor de 'm' se debe modificar por 27 así como
  descomentar/modificar las líneas indicadas. También se puede configurar
  fácilmente para usar A=0;B=1...
  Si se obtiene una '@' es debido a que el valor cifrado es 0, y al realizar
  la conversión para el ASCII se debe sumar 64: 0 + 64 = 64, valor de '@'*/

int main(int argc, char **argv)
{
    int a = atoi(argv[1]),
        b = atoi(argv[2]),
        i = 0,
        m = 26, //m = 27 para usar Ñ
        len = strlen(argv[3]);
    char *plaintext;
    plaintext = (char *)malloc(len);
    strcpy(plaintext, argv[3]);

    char ciphertext, x;

    i = 0;
    while (i < len)
    {
        x = toupper(plaintext[i]) - 64; //A=1; Cambiar 64 por 65 para A=0.
        //if(x > 14) x++;//Para usar 'Ñ''
        ciphertext = ((a * x + b) % m) + 64; //Quitar "+ 64" si se usa 'Ñ' / Cambiar 64 por 65 para A=0
        printf("%c", ciphertext);            //Cambiar "%c" por "%i\n" cuando se usa 'Ñ'
        i++;
    }

    free(plaintext);
    return 0;
}