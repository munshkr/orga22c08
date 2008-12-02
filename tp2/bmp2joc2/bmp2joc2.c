// bmp2joc2

#include <stdlib.h>

#include "readbmp.h"
#include "writejoc2.h"

int main (int argc, char* argv[]) {

	 // si no recibe los 2 parametros, termina.
	if (argc < 3) {
		printf("Número de parámetros incorrecto.\n");
		printf("Uso:  bmp2joc2 FUENTE DESTINO\n");
        printf("\n");
        return EXIT_FAILURE;
	}

    char* archivo_fuente = argv[1];
    char* archivo_destino = argv[2];

	// declara las estructuras de los headers del archivo bmp fuente.
    BMPfileheader fh;
    BMPinfoheader ih;

    unsigned char* RBuffer = NULL;
	unsigned char* GBuffer = NULL;
	unsigned char* BBuffer = NULL;
	short* codificacion = NULL;

	//este es un backup para guardar al joc2
	short* codifBckp = NULL;

	bool bOK;

	// abre el archivo bmp, levanto los headers y carga los datos de la
	// imagen en los buffer de cada canal (RGB).
	bOK = readbmp(archivo_fuente, &fh, &ih, &RBuffer, &GBuffer, &BBuffer);

    if (bOK) {
		int i, j;
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
		generarDCT(DCT);

		float bloque_transformado[8][8];
		short bloque_cuantizado[8][8];
		unsigned char Rbloque[8][8];
		unsigned char Gbloque[8][8];
		unsigned char Bbloque[8][8];

		// matriz temporal donde se va a guardar el bloque a transformar
		// en formato float.
		float MTemp[8][8];


		//Pido un buffer de codificación del doble de la imagen
		codificacion = (short*)malloc(ih.SizeImage * 2);
		//este es un backup para guardar al joc2
		codifBckp = codificacion;

		if(codificacion == NULL){
			printf("Hubo un problema al intentar alocar memoria para codificar\n");
			return EXIT_FAILURE;
		}

		int x, y, codLen=0, longTemp;
		for (y = 0 ; y < cantRows ; y++) {
			for (x = 0 ; x < cantCols ; x++) {
				dividirEnBloques(RBuffer, cantCols, Rbloque, x, y);
				transformar(Rbloque, DCT, bloque_transformado, MTemp);
				cuantizar(bloque_transformado, Q_MATRIX, bloque_cuantizado);
				longTemp = codificar(bloque_cuantizado, codificacion);
				codificacion += longTemp;
				codLen += longTemp;
			}
		}

		for (y = 0 ; y < cantRows ; y++) {
			for (x = 0 ; x < cantCols ; x++) {
				dividirEnBloques(GBuffer, cantCols, Gbloque, x, y);
				transformar(Gbloque, DCT, bloque_transformado, MTemp);
				cuantizar(bloque_transformado, Q_MATRIX, bloque_cuantizado);
				longTemp= codificar(bloque_cuantizado, codificacion);
				codificacion += longTemp;
				codLen += longTemp;
			}
		}

		for (y = 0 ; y < cantRows ; y++) {
			for (x = 0 ; x < cantCols ; x++) {
				dividirEnBloques(BBuffer, cantCols, Bbloque, x, y);
				transformar(Bbloque, DCT, bloque_transformado, MTemp);
				cuantizar(bloque_transformado, Q_MATRIX, bloque_cuantizado);
				longTemp= codificar(bloque_cuantizado, codificacion);
				codificacion += longTemp;
				codLen += longTemp;
			}
		}

		//le paso el puntero a codificacion sin adelantar
		if (argc < 4) {
			writejoc2(&fh, &ih, codifBckp, codLen, archivo_destino);
		} else {
			writeJOC2Deluxe(&fh, &ih, codifBckp, codLen, archivo_destino);
		}

    } else {
    	printf("Hubo un error al leer el archivo .BMP\n");
	}

	if (codifBckp != NULL)
		free(codifBckp);

	if (RBuffer != NULL)
		free(RBuffer);
	if (GBuffer != NULL)
	    free(GBuffer);
	if (BBuffer != NULL)
	    free(BBuffer);

    return EXIT_SUCCESS;
}
