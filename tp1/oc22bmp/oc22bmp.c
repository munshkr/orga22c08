#include <stdlib.h>

#include "readoc2.h"
#include "writebmp.h"


extern int decodificar(char* bitstream, long long int size_bs, codificacion* tablaCodigos, short size_tablaCodigos, char** buffer, int size_buffer);

int main (int argc, char* argv[]) {

	// si no recibe los 2 parametros, termina.
	if (argc != 3) {
		printf("Numero de parametros incorrecto.\n");
		printf("Uso:  bmp2oc2 FUENTE DESTINO\n");
        printf("\n");
        return EXIT_FAILURE;
	}

	unsigned int k;

    char* archivo_fuente = argv[1];
	char* archivo_destino = argv[2];


	OC2fileheader oc2fh;
	BMPfileheader fh;
    BMPinfoheader ih;
	codificacion* tablaCodigos;
	char* bitstream;

	int funciono = readoc2(archivo_fuente, &oc2fh, &fh, &ih, &tablaCodigos, &bitstream);

    if (bitstream == NULL || !funciono) {
        printf("\nEl archivo %s NO es un OC2 valido.\n", argv[1]);
        printf("\n");

        return EXIT_FAILURE;
    }

	char* buffer_bmp = NULL;
	int size_buffer = decodificar(bitstream, oc2fh.bsSize, tablaCodigos, oc2fh.tcSize, &buffer_bmp, fh.Size - 54);

	funciono = writebmp(&fh, &ih, buffer_bmp, size_buffer, archivo_destino);

    //free(buffer);
    return EXIT_SUCCESS;

}

