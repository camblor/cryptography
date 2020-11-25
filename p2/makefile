all: seg-perf

seg-perf: src/seg-perf.c
	gcc -o seg-perf src/seg-perf.c -lgmp -lm

des.o: src/des.c
	gcc -c -o des.o src/des.c -include include/des.h
	echo "Objeto des generado"

des: des.o include/des.h
	gcc -o desCBC src/maindes.c des.o -include include/des.h
	echo "Ejecutable desCBC generado correctamente."

linedes: src/nolinealidad_DES.c des.o
	gcc -o nolinealidad_DES src/nolinealidad_DES.c des.o
	echo "nolinealidad_DES generado ejecutable"

avades: src/avalancha_des.c des.o
	gcc -o avalancha_des src/avalancha_des.c des.o -include include/des.h
	echo "avalancha_des generado ejecutable"

nolineal_AES: src/noLineal_AES.c
	gcc -o noLineal_AES src/noLineal_AES.c

sbox_AES: src/sbox_AES.c
	gcc -o sbox_AES src/sbox_AES.c

testdes: des
	echo "Ejecutando test DES"
	./desCBC -C -k 133457799BBCDFF1 -i doc/exam.pdf -o cifrado
	./desCBC -D -k 133457799BBCDFF1 -i cifrado -o exam2.pdf
	echo "Ejecutado test DES"

testnolinealidadDES: linedes
	echo "Ejecutando test no linealidad DES"
	./nolinealidad_DES
	echo "Ejecutado test no linealidad DES"

testavalanchaDES: avades
	echo "Ejecutando test efecto avalancha DES"
	./avalancha_des
	echo "Ejecutado test efecto avalancha DES"

testnolinealidadAES:
	echo "ejecutando test no linealidad AES"
	./nolinealidadAES

clean:
	rm -rf seg-perf des.o desCBC cifrado nolinealidad_DES avalancha_des exam2.pdf noLineal_AES sbox_AES