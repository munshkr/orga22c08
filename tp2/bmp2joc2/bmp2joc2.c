// bmp2joc2

#include <stdlib.h>

#include "readbmp.h"
#include "funcionesASM.h"
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

/*
//
// accepts a decimal integer and returns a binary coded string
//
void dec2bin(long decimal, int longCod, char *binary)
{
  int  k = 0, n = 0;
  int  neg_flag = 0;
  int  remain;
  int  old_decimal;  // for test
  char temp[80];

  int j = 0;
  while(j<32){
  	temp[j] = '0';
  	j++;
  }

  // take care of negative input
  if (decimal < 0)
  {
    decimal = -decimal;
    neg_flag = 1;
  }
  do
  {
    old_decimal = decimal;   // for test
    remain    = decimal % 2;
    // whittle down the decimal number
    decimal   = decimal / 2;
    // this is a test to show the action
    //printf("%d/2 = %d  remainder = %d\n", old_decimal, decimal, remain);
    // converts digit 0 or 1 to character '0' or '1'
    temp[k++] = remain + '0';
  } while (decimal > 0);

  if (neg_flag)
    temp[k++] = '-';       // add - sign
  else
    temp[k++] = '0';       // space

  // reverse the spelling
  k = longCod;
  while (k >= 0)
    binary[n++] = temp[--k];

  binary[n-1] = 0;         // end with NULL
}


*/

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
		unsigned char bloque[8][8];
		unsigned char Gbloque[8][8];
		unsigned char Bbloque[8][8];

int y;
int x;
for (y = 0 ; y < atoi(argv[3]) ; y++) {
	for (x = 0 ; x < atoi(argv[4]) ; x++) {
		dividirEnBloques(RBuffer, cantCols, bloque, x, y);
		printf("Bloque pre:\n");
    	for (i = 0 ; i < 8 ; i++) {
			for (j = 0 ; j < 8 ; j++) {
				printf("%d\t", bloque[i][j]);
			}
			printf("\n");
		}
		printf("\n");

// 		transformar(bloque, DCT, bloque_transformado);
//    		cuantizar(bloque_transformado, bloque_cuantizado);

// 		printf("Bloque trans:\n");
//     	for (i = 0 ; i < 8 ; i++) {
// 			for (j = 0 ; j < 8 ; j++) {
// 				printf("%f\t", bloque_transformado[i][j]);
// 			}
// 			printf("\n");
// 		}
// 		printf("\n");
//
// 		printf("Bloque cuanti:\n");
//     	for (i = 0 ; i < 8 ; i++) {
// 			for (j = 0 ; j < 8 ; j++) {
// 				printf("%d\t", bloque_cuantizado[i][j]);
// 			}
// 			printf("\n");
// 		}
// 		printf("\n");







//    		decuantizar(bloque_cuantizado, bloque_transformado);
//    		antitransformar(bloque_transformado, DCT, bloque);


// 		printf("Bloque post:\n");
//     	for (i = 0 ; i < 8 ; i++) {
// 			for (j = 0 ; j < 8 ; j++) {
// 				printf("%d\t", bloque[i][j]);
// 			}
// 			printf("\n");
// 		}
// 		printf("\n");


		dividirEnBloques(RBuffer, cantCols, bloque, x, y);
		transformar(bloque, DCT, bloque_transformado);
   		cuantizar(bloque_transformado, bloque_cuantizado);
		printf("Bloque cuantizado:\n");
    	for (i = 0 ; i < 8 ; i++) {
			for (j = 0 ; j < 8 ; j++) {
				printf("%d\t", bloque_cuantizado[i][j]);
			}
			printf("\n");
		}
		printf("\n");

   		decuantizar(bloque_cuantizado, bloque_transformado);
		printf("Bloque decuantizado:\n");
    	for (i = 0 ; i < 8 ; i++) {
			for (j = 0 ; j < 8 ; j++) {
				printf("%f\t", bloque_transformado[i][j]);
			}
			printf("\n");
		}
		printf("\n");

   		antitransformar(bloque_transformado, DCT, bloque);
		printf("Bloque antitrans:\n");
    	for (i = 0 ; i < 8 ; i++) {
			for (j = 0 ; j < 8 ; j++) {
				printf("%d\t", bloque[i][j]);
			}
			printf("\n");
		}
		printf("\n");

   		dividirEnBloques(BBuffer, cantCols, Bbloque, x, y);
		transformar(Bbloque, DCT, bloque_transformado);
   		cuantizar(bloque_transformado, bloque_cuantizado);
   		decuantizar(bloque_cuantizado, bloque_transformado);
   		antitransformar(bloque_transformado, DCT, Bbloque);

		dividirEnBloques(GBuffer, cantCols, Gbloque, x, y);
		transformar(Gbloque, DCT, bloque_transformado);
   		cuantizar(bloque_transformado, bloque_cuantizado);
   		decuantizar(bloque_cuantizado, bloque_transformado);
   		antitransformar(bloque_transformado, DCT, Gbloque);

		writecuadradito(archivo_destino, &fh, &ih, (char*)bloque, (char*)Gbloque, (char*)Bbloque, x, y);
	}
}
    }

	if (RBuffer != NULL)
		free(RBuffer);
	if (GBuffer != NULL)
	    free(GBuffer);
	if (BBuffer != NULL)
	    free(BBuffer);


    return EXIT_SUCCESS;
}
