// bmp2joc2

#include <stdlib.h>

#include "readbmp.h"
#include "writecuadradito.h"

//#include "writejoc2.h"
//#include "writecbmp.h"


//extern int* calcularApariciones(char* buffer, int size);
//extern int armarTablaCodigos(int* tablaApariciones, codificacion** tablaCodigos);
//extern long long int codificar (char* buffer, int buf_size, codificacion* tc, int tc_size, int* ta, char** bitstream);

/*
	readbmp(archivo_fuente, &fh, &ih, &RBuffer, &GBuffer, &BBuffer);

	short r_bloque[8][8];
	short g_bloque[8][8];
	short b_bloque[8][8];

	float dct[8][8] = generarDCT();

	// canal R
	for (i=0;i<ancho/8;i++){
		for (j=0;j<alto/8;j++){
			r_bloque = dividirEnBloques(RBuffer, i, j);
			...
		}
	}


*/

int main (int argc, char* argv[]) {
	bool bOK = true;

	if (argc != 3) {													// si no recibe los 2 parametros, termina.
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
    	//writecbmp(archivo_destino, &fh, &ih, RBuffer, GBuffer, BBuffer);
		/*
		// ROMPE TOODO EN PEDAZOS
    	int y;
    	int x;
    	for (y = 0 ; y < ih.Width / 8 ; y++) {
    		for (x = 0 ; x < ih.Height / 8 ; x++) {
    			writecuadradito(archivo_destino, &fh, &ih, RBuffer, GBuffer, BBuffer, x, y);
    		}
    	}
  		*/
    	int cantCols = ih.Width / 8;

    	float DCT[8][8];
    	generarDCT(DCT);

    	printf("DCT :)\n");
    	int i, j;
    	for (i = 0 ; i < 8 ; i++) {
			for (j = 0 ; j < 8 ; j++) {
				printf("%f\t", DCT[i][j]);
			}
			printf("\n");
		}
		printf("\n");

		float bloque_transformado[8][8];
		short bloque_cuantizado[8][8];
		short * bloque_codificado = NULL;

		transformar(dividirEnBloques(RBuffer, cantCols, 0, 0), DCT, bloque_transformado);
   		cuantizar(bloque_transformado, bloque_cuantizado);

		char bloque[8][8];
   		decuantizar(bloque_cuantizado, bloque_transformado);
   		antitransformar(bloque_transformado, DCT, bloque);

    }

	if (RBuffer != NULL)
		free(RBuffer);
	if (GBuffer != NULL)
	    free(GBuffer);
	if (BBuffer != NULL)
	    free(BBuffer);


    return EXIT_SUCCESS;
}
