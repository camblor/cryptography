int* inversosMultiplicativos(int base, int* n_inversos) {
    
    /* Preparación de variables */
	int * inversos = (int*) malloc (base * sizeof(int));	
	int inv = 0;
	int num;
    *n_inversos = 0;
	

	for (num = 0; num < base; num++) {
		inv = euclidesExtendidoRec(num, base, 0, 0, 0);
		if(inv == 1) {
			inversos[*n_inversos] = num;
			(*n_inversos)++;
		}
	}

	inversos = (int *) realloc(inversos, (*n_inversos) * sizeof(int));
	return inversos;
}