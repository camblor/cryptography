all: afin afinmejorado vigenere criptanalisis flujo permutacion

tests: testinput testafin testafinmejorado testvigenere testflujo testpermutacion

testinput:
	echo "hola que tal" >> input

afin:
	gcc -o afin src/afin.c -lgmp

testafin:
	./afin -C -m 27 -a 2 -b 15 -i input -o output
	./afin -D -m 27 -a 2 -b 15 -i output -o salidaAfin

afinmejorado:
	gcc -o afinmejorado src/afinmejorado.c -lgmp

testafinmejorado:
	./afinmejorado -C -s 4 -i input -o output
	./afinmejorado -D -s 4 -i output -o salidaAfinMejorado

vigenere:
	gcc -o vigenere src/vigenere.c

criptanalisis:
	gcc src/criptanalisis.c src/euclid.c src/jwHash.c -o criptanalisis

testvigenere:
	./vigenere -C -k probando -i input -o output
	./vigenere -D -k probando -i output -o salidaVigenere

flujo:
	gcc -o stream src/stream.c

testflujo:
	./stream 40 testeando

permutacion:
	gcc -o permutacion src/permutacion.c

testpermutacion:
	./permutacion -C -k1 "2 4 0 1 3" -k2 "1 2 0" -i input -o output
	./permutacion -D -k1 "2 4 0 1 3" -k2 "1 2 0" -i output -o salidaPermutacion


clean:
	rm afin afinmejorado vigenere criptanalisis stream permutacion input output salida*
