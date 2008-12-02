// bmp2joc2

#include <stdlib.h>

#include "readbmp.h"
#include "funcionesASM.h"
#include "writecuadradito.h"

//#include "writejoc2.h"
#include "writecbmp.h"


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
		short * bloque_codificado = NULL;
		unsigned char Rbloque[8][8];
		unsigned char Gbloque[8][8];
		unsigned char Bbloque[8][8];

		// matriz temporal donde se va a guardar el bloque a transformar
		// en formato float.
		float MTemp[8][8];

		float DCT_Trasp[8][8];
		generarDCT(DCT_Trasp);
		trasponer(DCT_Trasp);

/*		short codificacion[16][16];
		int cantidad = codificar(bloque_cuantizado,(short*)codificacion);
		int	cantidad2 = decodificar((short*)codificacion, bloque_cuantizado);
		if(cantidad != cantidad2) 
			printf("TPario!!!!!!!!");*/
		
		int x, y;
		for (y = 0 ; y < cantRows ; y++) {
			for (x = 0 ; x < cantCols ; x++) {
				dividirEnBloques(RBuffer, cantCols, Rbloque, x, y);
				transformar(Rbloque, DCT, bloque_transformado, MTemp);
				cuantizar(bloque_transformado, Q_MATRIX, bloque_cuantizado);
				decuantizar(bloque_cuantizado, Q_MATRIX, bloque_transformado);
				antitransformar(bloque_transformado, DCT_Trasp, Rbloque, MTemp);

				dividirEnBloques(BBuffer, cantCols, Bbloque, x, y);
				transformar(Bbloque, DCT, bloque_transformado, MTemp);
				cuantizar(bloque_transformado, Q_MATRIX, bloque_cuantizado);
				decuantizar(bloque_cuantizado, Q_MATRIX, bloque_transformado);
				antitransformar(bloque_transformado, DCT_Trasp, Bbloque, MTemp);

				dividirEnBloques(GBuffer, cantCols, Gbloque, x, y);
				transformar(Gbloque, DCT, bloque_transformado, MTemp);
				cuantizar(bloque_transformado, Q_MATRIX, bloque_cuantizado);
				decuantizar(bloque_cuantizado, Q_MATRIX, bloque_transformado);
				antitransformar(bloque_transformado, DCT_Trasp, Gbloque, MTemp);

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
