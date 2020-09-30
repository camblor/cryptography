/*
Cryptography
Author: Alfonso Camblor García
Content:
    - Basic Euclid Algorithm (Máximo Común Divisor)
    - Extended Euclid Algorithm (Greater Common Divisor and Lineal Combination of Remainders)
*/

#include <stdio.h>

/*
Funcion: gcd ( Greater Common Divisor )
Entrada: Integers to evaluate ( a, b )
Salida: Greater Common Divisor between the two integers (a,b)
*/
int gcd(int a, int b){

    if (a == 0){
        return b;
    }
    return gcd(b%a, a);
}

/*
Funcion: gcdExtended ( Greater Common Divisor and Lineal Combination of Remainders)
Entrada: Integers to evaluate ( a, b ) and Lineal Combination Integers pointers ( x, y )
Salida: Greater Common Divisor between the two integers (a,b)
        Lineal Combination required to satisfy the Greater Common Divisor
*/
int gcdExtended(int a, int b, int *x, int *y) 
{ 
    // Base Case 
    if (a == 0) 
    { 
        *x = 0; 
        *y = 1; 
        return b; 
    } 
  
    /* Storage for lineal combination integer variables */
    int x1, y1;
    int gcd = gcdExtended(b%a, a, &x1, &y1); 
  
    /* Update Remainders Lineal Combination*/ 
    *x = y1 - (b/a) * x1; 
    *y = x1;

    printf("(%d * %d) + (%d * %d) = %d\n", a,*x,b,*y, (a * (*x) + b * (*y)));
  
    return gcd; 
} 

/*
Funcion: euclid ( Basic Euclidean Algorithm )
Entrada: Integers to evaluate ( a, b ) and Lineal Combination Integers pointers ( x, y )
Salida: Greater Common Divisor between the two integers (a,b)
*/
int euclid(int a, int b){
    return gcd(a, b);
}

/*
Funcion: euclidExtended ( Extended Euclidean Algorithm )
Entrada: Integers to evaluate ( a, b )
Salida: Greater Common Divisor between the two integers (a,b)
        Lineal Combination required to satisfy the Greater Common Divisor
*/
int euclidExtended(int a, int b, int* x, int* y){
    return gcdExtended(a, b, x, y);
}

/* MAIN PROGRAM */
int main(){

    int a = 2366;
    int b = 273;
    int x, y;
    int result = gcd(a,b);

    printf("gcd(%d,%d) = %d\n", a, b, euclidExtended(a,b, &x, &y));
}