// bmp2oc2
// Lee un archivo en formato bmp, lo codifica con el algoritmo Huffman y lo
// escribe en un archivo en formato oc2.

#include <stdlib.h>

#include "readbmp.h"
// #include "writeoc2.h"
#include "writecbmp.h"

extern int* calcularApariciones(char* buffer, int size);
extern int armarTablaCodigos(int* tablaApariciones, codificacion** tablaCodigos);
extern long long int codificar (char* buffer, int buf_size, codificacion* tc, int tc_size, int* ta, char** bitstream);


/*
    Idea vaga de como seria este programa:

    BMPfileheader fh;
    BMPinfoheader ih;

    char* buffer = readbmp(argv[0] , &fh, &ih);

    int bufsize = fh.Size - 54

    TablaApariciones* ta = calcularApariciones(buffer, bufsize);

    TablaCodigos* tc = armarTablaCodigos(ta);

    char* bitstream = codificar(tc, buffer);

    int todoOK = writeoc2(fh, ih, tc, bitstream);

*/



int main (int argc, char* argv[]) {

	// si no recibe los 2 parametros, termina.
	if (argc != 3) {
		printf("Numero de parametros incorrecto.\n");
		printf("Uso:  bmp2joc2 FUENTE DESTINO\n");
        printf("\n");
        return EXIT_FAILURE;
	}

	unsigned int k;

    char* archivo_fuente = argv[1];
//    archivo_fuente = "../res/nhouse.bmp";

    char* archivo_destino = argv[2];

    // declara las estructuras de los headers del archivo bmp fuente.
    BMPfileheader fh;
    BMPinfoheader ih;

    // abre el archivo bmp, levanto los headers y carga los datos
    // de la imagen en un buffer.


	char* RBuffer;
	char* GBuffer;
	char* BBuffer;

	if (readbmp(archivo_fuente, &fh, &ih, &RBuffer, &GBuffer, &BBuffer)) {

		writecbmp(archivo_destino, &fh, &ih, RBuffer, GBuffer, BBuffer);

	}

    free(RBuffer);
    free(GBuffer);
    free(BBuffer);

    return EXIT_SUCCESS;

}
