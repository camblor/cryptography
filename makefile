all: afin afinmejorado vigenere criptanalisis flujo permutacion

afin:
	gcc -o afin src/afin.c -lgmp

afinmejorado:
	gcc -o afinmejorado src/afinmejorado.c -lgmp

vigenere:
	gcc -o vigenere src/vigenere.c

criptanalisis:
	gcc src/criptanalisis.c src/euclid.c src/jwHash.c -o criptanalisis

flujo:
	gcc -o stream src/stream.c

permutacion:
	gcc -o permutacion src/permutacion.c

clean:
	rm afin afinmejorado vigenere criptanalisis stream permutacion
