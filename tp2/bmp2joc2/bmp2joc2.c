// bmp2joc2

#include <stdlib.h>

#include "readbmp.h"
#include "funcionesASM.h"
#include "writecuadradito.h"

//#include "writejoc2.h"
#include "writecbmp.h"


int main (int argc, char* argv[]) {
	bool bOK = true;

	if (argc < 3) {													// si no recibe los 2 parametros, termina.
		printf("Numero de parametros incorrecto.\n");
		printf("Uso:  bmp2joc2 FUENTE DESTINO\n");
        printf("\n");
        return EXIT_FAILURE;
	}

    char* archivo_fuente = argv[1];
    char* archivo_destino = argv[2];

    BMPfileheader fh;													// declara las estructuras de los
    BMPinfoheader ih;													// headers del archivo bmp fuente.

    char* RBuffer = NULL;														// abre el archivo bmp, levanto los
	char* GBuffer = NULL;														// headers y carga los datos de la
	char* BBuffer = NULL;														// imagen en los buffer de cada canal (RGB).

	bOK = readbmp(archivo_fuente, &fh, &ih, &RBuffer, &GBuffer, &BBuffer);

    if (bOK) {
//     	//writecbmp(archivo_destino, &fh, &ih, RBuffer, GBuffer, BBuffer);
//
// 		// ROMPE TOODO EN PEDAZOS
//     	int y;
//     	int x;
//     	for (y = 0 ; y < ih.Width / 8 ; y++) {
//     		for (x = 0 ; x < ih.Height / 8 ; x++) {
//     			writecuadradito(archivo_destino, &fh, &ih, RBuffer, GBuffer, BBuffer, x, y);
//     		}
//     	}

    	int cantCols = ih.Width / 8;
    	int cantRows = ih.Height / 8;


/*
		// Psicodeliaaaa!
    	float DCT[8][8] = {
			{1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0},
			{0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0},
			{0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0},
			{0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0},
			{1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}
		};
*/

/*
		// Predator / Ascii Art
		float DCT[8][8] = {
			{3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0},
			{0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0},
			{0.0, 0.0, 3.0, 0.0, 0.0, 2.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 3.0, 2.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 2.0, 3.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 2.0, 0.0, 0.0, 3.0, 0.0, 0.0},
			{0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 3.0, 0.0},
			{2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3.0}
		};
*/


		float DCT[8][8];
		generarDCT_C(DCT);

		//generarDCT(DCT);



//     	printf("DCT :)\n");
//     	int i, j;
//     	for (i = 0 ; i < 8 ; i++) {
// 			for (j = 0 ; j < 8 ; j++) {
// 				printf("%f\t", DCT[i][j]);
// 			}
// 			printf("\n");
// 		}
// 		printf("\n");

		float bloque_transformado[8][8];
		short bloque_cuantizado[8][8];
		short * bloque_codificado = NULL;
		unsigned char Rbloque[8][8];
		unsigned char Gbloque[8][8];
		unsigned char Bbloque[8][8];

		int i, j;

		int y;
		int x;
		for (y = 0 ; y < cantRows ; y++) {
			for (x = 0 ; x < cantCols ; x++) {
// 				dividirEnBloquesC(RBuffer, cantCols, Rbloque, x, y);
				dividirEnBloques(RBuffer, cantCols, Rbloque, x, y);
//  				printf("Rbloque ASM pre:\n");
//  				for (i = 0 ; i < 8 ; i++) {
//  					for (j = 0 ; j < 8 ; j++) {
//  						printf("%d\t", Rbloque[i][j]);
//  					}
//  					printf("\n");
//  				}
//  				printf("\n");

				transformar(Rbloque, DCT, bloque_transformado);
				cuantizar(bloque_transformado, bloque_cuantizado);
// 				printf("Rbloque cuantizado:\n");
// 				for (i = 0 ; i < 8 ; i++) {
// 					for (j = 0 ; j < 8 ; j++) {
// 						printf("%d\t", bloque_cuantizado[i][j]);
// 					}
// 					printf("\n");
// 				}
// 				printf("\n");

				decuantizar(bloque_cuantizado, bloque_transformado);
// 				printf("Rbloque decuantizado:\n");
// 				for (i = 0 ; i < 8 ; i++) {
// 					for (j = 0 ; j < 8 ; j++) {
// 						printf("%f\t", bloque_transformado[i][j]);
// 					}
// 					printf("\n");
// 				}
// 				printf("\n");

				antitransformar(bloque_transformado, DCT, Rbloque);
// 				printf("Rbloque antitrans:\n");
// 				for (i = 0 ; i < 8 ; i++) {
// 					for (j = 0 ; j < 8 ; j++) {
// 						printf("%d\t", Rbloque[i][j]);
// 					}
// 					printf("\n");
// 				}
// 				printf("\n");

// 				dividirEnBloquesC(BBuffer, cantCols, Bbloque, x, y);
				dividirEnBloques(BBuffer, cantCols, Bbloque, x, y);
				transformar(Bbloque, DCT, bloque_transformado);
				cuantizar(bloque_transformado, bloque_cuantizado);
				decuantizar(bloque_cuantizado, bloque_transformado);
				antitransformar(bloque_transformado, DCT, Bbloque);

// 				dividirEnBloquesC(GBuffer, cantCols, Gbloque, x, y);
				dividirEnBloques(GBuffer, cantCols, Gbloque, x, y);
				transformar(Gbloque, DCT, bloque_transformado);
				cuantizar(bloque_transformado, bloque_cuantizado);
				decuantizar(bloque_cuantizado, bloque_transformado);
				antitransformar(bloque_transformado, DCT, Gbloque);

				//writecuadradito(archivo_destino, &fh, &ih, (char*)Rbloque, (char*)Gbloque, (char*)Bbloque, x, y);
				unirBloques(RBuffer, cantCols, Rbloque, x, y);
				unirBloques(GBuffer, cantCols, Gbloque, x, y);
				unirBloques(BBuffer, cantCols, Bbloque, x, y);
			}
		}

		writejbmp(archivo_destino, &fh, &ih, RBuffer, GBuffer, BBuffer);
    }

	if (RBuffer != NULL)
		free(RBuffer);
	if (GBuffer != NULL)
	    free(GBuffer);
	if (BBuffer != NULL)
	    free(BBuffer);

    return EXIT_SUCCESS;
}
