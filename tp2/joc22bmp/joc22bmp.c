// joc22bmp

#include <stdlib.h>

#include "readjoc2.h"
#include "writebmp.h"


int main (int argc, char* argv[]) {

	 // si no recibe los 2 parametros, termina.
	if (argc < 3) {
		printf("Número de parámetros incorrecto.\n");
		printf("Uso:  joc22bmp FUENTE DESTINO\n");
        printf("\n");
        return EXIT_FAILURE;
	}

    char* archivo_fuente = argv[1];
    char* archivo_destino = argv[2];

	// declara las estructuras de los headers del archivo joc2 fuente.
    JOC2fileheader joc2fh;
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
	if (argc < 4) {
		bOK = readjoc2(archivo_fuente, &joc2fh, &fh, &ih, &codificacion);
	} else {
		bOK = readJOC2Deluxe(archivo_fuente, &joc2fh, &fh, &ih, &codificacion);
	}
	codifBckp = codificacion;

    if (bOK) {

		//Pido un buffer de codificación del doble de la imagen
		int bytesPorCanal = ih.Width * ih.Height;

		RBuffer = malloc(bytesPorCanal);
		GBuffer = malloc(bytesPorCanal);
		BBuffer = malloc(bytesPorCanal);

		if (RBuffer == NULL || GBuffer == NULL || BBuffer == NULL) {
			printf("Hubo un problema al intentar alocar memoria para decodificar\n");
			return EXIT_FAILURE;
		}

		// genera la DCT y la traspone para poder antitransformar.
		float DCT_Trasp[8][8];
		generarDCT(DCT_Trasp);
		trasponer(DCT_Trasp);


		float bloque_transformado[8][8];
		short bloque_cuantizado[8][8];
		unsigned char Rbloque[8][8];
		unsigned char Gbloque[8][8];
		unsigned char Bbloque[8][8];

		// matriz temporal de floats para poder trabajar con datos del mismo tipo
		float MTemp[8][8];


		int cantCols = ih.Width / 8;
    	int cantRows = ih.Height / 8;

		int x, y, codLen=0, longTemp;
		for (y = 0 ; y < cantRows ; y++) {
			for (x = 0 ; x < cantCols ; x++) {
				codificacion += decodificar( codificacion, bloque_cuantizado);
				decuantizar(bloque_cuantizado, Q_MATRIX, bloque_transformado);
				antitransformar(bloque_transformado, DCT_Trasp, Rbloque, MTemp);
				unirBloques(RBuffer, cantCols, Rbloque, x, y);
			}
		}

		for (y = 0 ; y < cantRows ; y++) {
			for (x = 0 ; x < cantCols ; x++) {
				codificacion += decodificar(codificacion, bloque_cuantizado);
				decuantizar(bloque_cuantizado, Q_MATRIX, bloque_transformado);
				antitransformar(bloque_transformado, DCT_Trasp, Gbloque, MTemp);
				unirBloques(GBuffer, cantCols, Gbloque, x, y);
			}
		}

		for (y = 0 ; y < cantRows ; y++) {
			for (x = 0 ; x < cantCols ; x++) {
				codificacion += decodificar(codificacion, bloque_cuantizado);
				decuantizar(bloque_cuantizado, Q_MATRIX, bloque_transformado);
				antitransformar(bloque_transformado, DCT_Trasp, Bbloque, MTemp);
				unirBloques(BBuffer, cantCols, Bbloque, x, y);
			}
		}

		writebmp(archivo_destino, &fh, &ih, RBuffer, GBuffer, BBuffer);

    } else {
    	printf("Hubo un error al leer el archivo .JOC2\n");
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
