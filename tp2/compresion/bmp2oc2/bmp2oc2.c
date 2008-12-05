// bmp2oc2
// Lee un archivo en formato bmp, lo codifica con el algoritmo Huffman y lo
// escribe en un archivo en formato oc2.

#include <stdlib.h>

#include "readbmp.h"
#include "writeoc2.h"


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
		printf("Uso:  bmp2oc2 FUENTE DESTINO\n");
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


	char* buffer = readbmp(archivo_fuente, &fh, &ih);

    if (buffer == NULL) {
        printf("\nEl archivo %s NO es un BMP valido.\n", argv[1]);
        printf("\n");

        return EXIT_FAILURE;
    }

//	char* buffer = "aaabcbbddcdahhabcdh";
//	int size = 19;

	int size = fh.Size - 54;



	int* tablaApariciones;
	tablaApariciones = calcularApariciones(buffer, size);

	#ifdef DEBUG
		k = 0;
		int j = 0;
		while (k < 256) {
			if (tablaApariciones[k] != 0) {
				printf("Index %d : %d apariciones\n", k, tablaApariciones[k]);
				j++;
			}
			k++;
		}
		printf("Simbolos: %d\n",j);
	#endif

	codificacion* tablaCodigos;
	int size_tablaCodigos = 0;
	size_tablaCodigos = armarTablaCodigos(tablaApariciones, &tablaCodigos);



	//#ifdef DEBUG
		k = 0;
		int maxLongCod = 0;
		unsigned char index = 0;
		printf("Simbolo\tFrec\tLong\tCodigo\n", size_tablaCodigos);
		while (k < size_tablaCodigos) {
			char cod[80] = {};
			dec2bin(tablaCodigos[k].cod, tablaCodigos[k].longCod, cod);
			index = tablaCodigos[k].simbolo;
			printf("%d\t%d\t%d\t%s\n", index, tablaApariciones[index], tablaCodigos[k].longCod, cod);
			
			if(tablaCodigos[k].longCod>maxLongCod) maxLongCod = tablaCodigos[k].longCod;
			k++;
		}
		printf("Max Longitud Codigo: %d\n", maxLongCod);
		printf("sizeCod : %d\n", size_tablaCodigos);
	//#endif

	char* bitstream = NULL;
	long long int bsLen;
	bsLen = codificar(buffer, size, tablaCodigos, size_tablaCodigos, tablaApariciones, &bitstream);

	printf("El bs mide %d bits\n", bsLen);

    // aca sigue el resto del programa...

    // ...

    printf("\nEl archivo %s es un BMP valido.\n", argv[1]);
    printf("\n");


	OC2fileheader oc2fh;
	oc2fh.fType = 0x32434f;
	oc2fh.tcSize = size_tablaCodigos * TC_ROW_SIZE;
	oc2fh.bsSize = bsLen;

	int funciono = writeoc2(&oc2fh, &fh, &ih, tablaCodigos, bitstream, archivo_destino);
	if (funciono)
		printf("Se ha codificado el archivo %s a %s con exito.\n", archivo_fuente, archivo_destino);
	else
		printf("Ha fallado la codificacion.\n");


	#ifdef DEBUG
	if (buffer != NULL) {
		printf("Primeros 20 bytes:\n");
		int i;
		for (i = 0; i < 20; i++) {
			printf("%#hx ", (*buffer));
			buffer++;
		}
	}
	#endif

    //free(buffer);
    return EXIT_SUCCESS;

}
