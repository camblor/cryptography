all: potencia primos lasvegas 

potencia: potencia.c
	gcc -o potencia potencia.c -lgmp -L.

primos: primos.c
	gcc -o primos primos.c -lgmp -L.

lasvegas: lasvegas.c
	gcc -o lasvegas lasvegas.c -lgmp -L.

clean:
	rm potencia primos lasvegas