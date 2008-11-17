#ifndef __FUNCIONESASM_H__
#define __FUNCIONESASM_H__

#include <math.h>

// AUXILIARES

	const float Q_MATRIX[8][8] = {
		{16, 11, 10, 16,  24,  40,  51,  61},
		{12, 12, 14, 19,  26,  58,  60,  55},
		{14, 13, 16, 24,  40,  57,  69,  56},
		{14, 17, 22, 29,  51,  87,  80,  62},
		{18, 22, 37, 56,  68, 109, 103,  77},
		{24, 35, 55, 64,  81, 104, 113,  92},
		{49, 64, 78, 87, 103, 121, 120, 101},
		{72, 92, 95, 98, 112, 100, 103,  99}
	};

	void transponer_char(char matrix[8][8]) {
		int i, j;
		char save;

		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				save = matrix[i][j];
				matrix[i][j] = matrix[j][i];
				matrix[j][i] = save;
			}
		}
	}

	void transponer_float(float matrix[8][8]) {
		int i, j;
		float save;

		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				save = matrix[i][j];
				matrix[i][j] = matrix[j][i];
				matrix[j][i] = save;
			}
		}
	}

	void mult_matrices_char_float(char a1[8][8], float a2[8][8], float res[8][8]) {
		int i = 0;
		int j = 0;
		int k = 0;
		for(i = 0; i < 8; i++)
			for( j = 0; j < 8; j++)
				for( k = 0; k < 8; k++)
					res[i][j] +=  a1[i][k] * a2[k][j];
	}

	void mult_matrices_float_float(float a1[8][8], float a2[8][8], float res[8][8]) {
		int i = 0;
		int j = 0;
		int k = 0;
		for(i = 0; i < 8; i++)
			for( j = 0; j < 8; j++)
				for( k = 0; k < 8; k++)
					res[i][j] +=  a1[i][k] * a2[k][j];
	}



// FIN AUXILIARES





char* dividirEnBloques(char* CBuffer, int cantCols, int x, int y) {
	return CBuffer + (x * 64) + (y * cantCols * 64);
}

void generarDCT(float DCT[8][8]) {
	int i, j;
	float ci;

	for (i = 0 ; i < 8 ; i++) {
		for (j = 0 ; j < 8 ; j++) {
			if (i == 0) {
				ci = (float)sqrt(1.0/8.0);
			} else {
				ci = (float)sqrt(2.0/8.0);
			}

			DCT[i][j] = (float) (  ci * cos(((2 * j + 1) * i * M_PI) / 16.0)  );
		}
	}
}

// todos los [8][8] representan un puntero

void transformar(char* bloque, float DCT[8][8], float bloque_transformado[8][8]) {
	float bloqueTemp[8][8];

	transponer_char(bloque);
	mult_matrices_char_float(bloque, DCT, bloqueTemp);
	transponer_float(DCT);
	mult_matrices_float_float(bloqueTemp, DCT, bloque_transformado);
	transponer_float(DCT);
}

void cuantizar(float bloque_transformado[8][8], short bloque_cuantizado[8][8]) {
	int i, j;

	for (i = 0 ; i < 8 ; i++) {
		for (j = 0 ; j < 8 ; j++) {
			bloque_cuantizado[i][j] = bloque_transformado[i][j] / Q_MATRIX[i][j];

			// Lo que viene es un redondeo cabeza, maldito c que no sabe redondear
			// o google que no me lo encuentra (el problema claramente no soy yo)
			if (bloque_cuantizado[i][j] >= 0) {
				bloque_cuantizado[i][j] += 0.5;
			} else {
				bloque_cuantizado[i][j] -= 0.5;
			}

			// OJO, LE PUSE CHAR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				bloque_cuantizado[i][j] = (int)bloque_cuantizado[i][j];
			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		}
	}
}

// !! ver tipo de bloque_codificado[8][8], short o char?
void codificar(short bloque_cuantizado[8][8], short * bloque_codificado) {

/*
	int n = 0;
	int i = 0;
	int j = 0;
	int max = 1;
	int incI = 0;
	int incJ = 1;

	for (n = 0 ; n < 64 ; n++) {
		i += incI;
		i += incJ;
		if (j == max) {
			incJ = -1;
			incI = 1;
		}

	}
*/

	// DESISTÍ, ARMAR ESTE CAMINO ES UN BARDO
	// EL RECORRIDO DE ÍNDICES ESTÁ PLASMADO EN EL ARCHIVO
	// recorridoDeMatriz.txt



}

void decuantizar(short bloque_cuantizado[8][8], float bloque_transformado[8][8]) {
	int i, j;

	for (i = 0 ; i < 8 ; i++) {
		for (j = 0 ; j < 8 ; j++) {
			bloque_transformado[i][j] = bloque_cuantizado[i][j] * Q_MATRIX[i][j];
		}
	}
}

void antitransformar(float bloque_transformado[8][8], float DCT[8][8], char bloque[8][8]) {
	float bloqueTemp[8][8];

	transponer_float(DCT);
	mult_matrices_float_float(DCT, bloque_transformado, bloqueTemp);
	transponer_float(DCT);
	mult_matrices_float_float(bloqueTemp, DCT, bloque);
	transponer_char(bloque);
}





#endif
