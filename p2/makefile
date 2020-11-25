all: seg-perf noLineal_AES sbox_AES

seg-perf: seg-perf.c
	gcc -o seg-perf seg-perf.c -lgmp -lm

noLineal_AES: noLineal_AES.c
	gcc -o noLineal_AES noLineal_AES.c

sbox_AES: sbox_AES.c
	gcc -o sbox_AES sbox_AES.c

clean:
	rm -rf seg-perf noLineal_AES sbox_AES