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
    	int y;
    	int x;

    	for (y = 0 ; y < ih.Width / 8 ; y++) {
    		for (x = 0 ; x < ih.Height / 8 ; x++) {
    			writecuadradito(archivo_destino, &fh, &ih, RBuffer, GBuffer, BBuffer, x, y);
    		}
    	}

    	//writecuadradito(archivo_destino, &fh, &ih, RBuffer, GBuffer, BBuffer, 3, 3);

    	int bloquesAncho = ih.Width / 8;
    	int bloquesAlto = ih.Height / 8;

    	/*
    	NOTA:
    	Extraer las matrices de 8x8 de un buffer del color que sea, es equivalente a ordenar las tríadas de bytes.

    	B = Byte

		   A	    B		 C		  D		   E		F		 G

1    	BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB
2    	BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB
3    	BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB
4    	BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB
5    	BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB
6    	BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB
7    	BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB
8    	BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB

9    	BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB
10    	BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB
11   	BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB
12    	BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB
13    	BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB
14    	BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB
15    	BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB
16    	BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB

    	El nuevo orden debería ser

    	A1		A2		A3		A4		A5		A6		A7
    	A8		B1		B2		B3		B4		B5		B6
    	B7		B8		C1		C2		..		..		..
    	..		..		..		..		..		..		..

    	y asi...

    	Tal vez haya algo que podamos hacer para ordenar en el mismo buffer y entonces no tener que pedir memoria.
    	*/

    }

	if (RBuffer != NULL)
		free(RBuffer);
	if (GBuffer != NULL)
	    free(GBuffer);
	if (BBuffer != NULL)
	    free(BBuffer);


    return EXIT_SUCCESS;
}
