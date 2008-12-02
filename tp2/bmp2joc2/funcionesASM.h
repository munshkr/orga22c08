#ifndef __FUNCIONESASM_H__
#define __FUNCIONESASM_H__

#include <math.h>

// AUXILIARES


	float Q_MATRIX[8][8] = {
		{16, 11, 10, 16,  24,  40,  51,  61},
		{12, 12, 14, 19,  26,  58,  60,  55},
		{14, 13, 16, 24,  40,  57,  69,  56},
		{14, 17, 22, 29,  51,  87,  80,  62},
		{18, 22, 37, 56,  68, 109, 103,  77},
		{24, 35, 55, 64,  81, 104, 113,  92},
		{49, 64, 78, 87, 103, 121, 120, 101},
		{72, 92, 95, 98, 112, 100, 103,  99}
	};

/*
	// COMPRESION NORMAL COPADA CON DCT PSICODELICA
	float FQ_MATRIX[8][8] = {
		{20, 20, 20, 20, 20, 20, 20, 20},
 		{20, 20, 20, 20, 20, 20, 20, 20},
 		{20, 20, 20, 20, 20, 20, 20, 20},
 		{20, 20, 20, 20, 20, 20, 20, 20},
 		{20, 20, 20, 20, 20, 20, 20, 20},
 		{20, 20, 20, 20, 20, 20, 20, 20},
 		{20, 20, 20, 20, 20, 20, 20, 20},
 		{20, 20, 20, 20, 20, 20, 20, 20}
	};

	const unsigned char Q_MATRIX[8][8] = {
		{1, 1, 1, 1, 1, 1, 1, 1},
 		{1, 1, 1, 1, 1, 1, 1, 1},
 		{1, 1, 1, 1, 1, 1, 1, 1},
 		{1, 1, 1, 1, 1, 1, 1, 1},
 		{1, 1, 1, 1, 1, 1, 1, 1},
 		{1, 1, 1, 1, 1, 1, 1, 1},
 		{1, 1, 1, 1, 1, 1, 1, 1},
 		{1, 1, 1, 1, 1, 1, 1, 1}
	};

	const unsigned char Q_MATRIX[8][8] = {
		{1, 255, 255, 255, 255, 255, 255, 255},
 		{1, 1, 255, 255, 255, 255, 255, 255},
 		{1, 1, 1, 255, 255, 255, 255, 255},
 		{1, 1, 1, 1, 255, 255, 255, 255},
 		{1, 1, 1, 1, 1, 255, 255, 255},
 		{1, 1, 1, 1, 1, 1, 255, 255},
 		{1, 1, 1, 1, 1, 1, 1, 255},
 		{1, 1, 1, 1, 1, 1, 1, 1}
	};
	
	// LIMA LOS AZULES Y ROJOS CON DCT PSICODELICA
	// HACE UN SOMBREADO RARO CON DCT PSICODELICA
	float FQ_MATRIX[8][8] = {
		{100, 1, 1, 1, 1, 1, 1, 1},
 		{100, 100, 1, 1, 1, 1, 1, 1},
 		{100, 100, 100, 1, 1, 1, 1, 1},
 		{100, 100, 100, 100, 1, 1, 1, 1},
 		{100, 100, 100, 100, 100, 1, 1, 1},
 		{100, 100, 100, 100, 100, 100, 1, 1},
 		{100, 100, 100, 100, 100, 100, 100, 1},
 		{100, 100, 100, 100, 100, 100, 100, 100}
	};

	// LUM
	float FQ_MATRIX[8][8] = {
		{17, 18, 24, 47, 99, 99, 99, 99},
		{18, 21, 26, 66, 99, 99, 99, 99},
		{24, 26, 56, 99, 99, 99, 99, 99},
		{47, 66, 99, 99, 99, 99, 99, 99},
		{99, 99, 99, 99, 99, 99, 99, 99},
		{99, 99, 99, 99, 99, 99, 99, 99},
		{99, 99, 99, 99, 99, 99, 99, 99},
		{99, 99, 99, 99, 99, 99, 99, 99}
	};
	*/

	void transponer_char(unsigned char matrix[8][8]) {
		int i, j;
		unsigned char save;

		for (i = 0; i < 8; i++) {
			for (j = 0; j < i; j++) {
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
			for (j = 0; j < i; j++) {
				save = matrix[i][j];
				matrix[i][j] = matrix[j][i];
				matrix[j][i] = save;
			}
		}
	}

	void mult_matrices_float_char(float a1[8][8], unsigned char a2[8][8], float res[8][8]) {
		int i = 0;
		int j = 0;
		int k = 0;
		for(i = 0; i < 8; i++)
			for( j = 0; j < 8; j++){
				res[i][j] = 0;
				for( k = 0; k < 8; k++)
					res[i][j] +=  a1[i][k] * a2[k][j];
			}
	}

	void mult_matrices_float_float(float a1[8][8], float a2[8][8], float res[8][8]) {
		int i = 0;
		int j = 0;
		int k = 0;
		for(i = 0; i < 8; i++)
			for( j = 0; j < 8; j++){
				res[i][j] = 0;
				for( k = 0; k < 8; k++)
					res[i][j] +=  a1[i][k] * a2[k][j];
		}
	}

	void mult_matrices_float_float_toChar(float a1[8][8], float a2[8][8], unsigned char res[8][8]) {
		int i = 0;
		int j = 0;
		int k = 0;
		for(i = 0; i < 8; i++)
			for( j = 0; j < 8; j++){
				res[i][j] = 0;
				for( k = 0; k < 8; k++)
					res[i][j] +=  a1[i][k] * a2[k][j];
			}
	}


// FIN AUXILIARES

void dividirEnBloques_C(char* CBuffer, int cantCols, unsigned char retBuff[8][8], int x, int y) {
	CBuffer = CBuffer + (x * 64) + (y * cantCols * 64);
	int i,j;
	for (i = 0 ; i < 8 ; i++) {
		for (j = 0 ; j < 8 ; j++) {
			retBuff[i][j] = (unsigned char)(*CBuffer++);
		}
	}
}

void unirBloques_C(unsigned char* CBuffer, int cantCols, unsigned char Cbloque[8][8], int x, int y) {
	CBuffer = CBuffer + (x * 64) + (y * cantCols * 64);
	int i,j;
	for (i = 0 ; i < 8 ; i++) {
		for (j = 0 ; j < 8 ; j++) {
			(*CBuffer++) = Cbloque[i][j];
		}
	}
}

void generarDCT_C(float DCT[8][8]) {
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

void transformar_C(unsigned char bloque[8][8], float DCT[8][8], float bloque_transformado[8][8]) {
	float bloqueTemp[8][8];

	transponer_char(bloque);
	mult_matrices_float_char(DCT, bloque, bloqueTemp);
	transponer_float(DCT);
	mult_matrices_float_float(bloqueTemp, DCT, bloque_transformado);
	transponer_float(DCT);

// 		printf("Bloque trasp:\n");
// 		int i,j;
//     	for (i = 0 ; i < 8 ; i++) {
// 			for (j = 0 ; j < 8 ; j++) {
// 				printf("%d\t", bloque[i][j]);
// 			}
// 			printf("\n");
// 		}
// 		printf("\n");
}

void cuantizar_C(float bloque_transformado[8][8], short bloque_cuantizado[8][8]) {
	int i, j;
	//printf("Empieza la cuantizacion: \n");
	for (i = 0 ; i < 8 ; i++) {
		for (j = 0 ; j < 8 ; j++) {
			bloque_cuantizado[i][j] = bloque_transformado[i][j] / Q_MATRIX[i][j];
// 			printf("-------------------\n");
// 			printf("%f / %d = %f\n", bloque_transformado[i][j] , Q_MATRIX[i][j], resDiv);
// 			printf("%f / %d = %d\n", bloque_transformado[i][j] , Q_MATRIX[i][j], bloque_cuantizado[i][j]);
// 			printf("-------------------\n");
		}
	}
}

void decuantizar_C(short bloque_cuantizado[8][8], float bloque_transformado[8][8]) {
	int i, j;

	for (i = 0 ; i < 8 ; i++) {
		for (j = 0 ; j < 8 ; j++) {
			bloque_transformado[i][j] = bloque_cuantizado[i][j] * Q_MATRIX[i][j];
		}
	}
}

void antitransformar_C(float bloque_transformado[8][8], float DCT[8][8], unsigned char bloque[8][8]) {
	float bloqueTemp[8][8];

	transponer_float(DCT);
	mult_matrices_float_float(DCT, bloque_transformado, bloqueTemp);
	transponer_float(DCT);
	mult_matrices_float_float_toChar(bloqueTemp, DCT, bloque);
	transponer_char(bloque);
}

#endif
