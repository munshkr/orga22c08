#ifndef __READBMP_H__
#define __READBMP_H__

// valores boolean
#define false 0
#define true 1

#include <stdio.h>
#include <stdlib.h>

#include "bmp2joc2.h"


void print_headers(BMPfileheader* fh, BMPinfoheader* ih);


int readbmp(const char* path, BMPfileheader* fh, BMPinfoheader* ih, unsigned char** RBuffer, unsigned char** GBuffer, unsigned char** BBuffer) {
	//char* buffer = NULL;

    // intenta abrir el archivo en path
    FILE* f = fopen(path, "rb");
    if (f == NULL) {
        return false;
    }

    // asume que el archivo es valido
    int esValido = true;

    // chequea si el archivo tiene por lo menos 54 bytes para poder
    // extraer los headers
    fseek(f, 0, SEEK_END);
    if ( ftell(f) < 54 ) {
        fclose(f);
        return false;
    }
    rewind(f);

    // asigna los headers a los structs
    fread (&(fh->MagicWord1), 1, sizeof(char), f);
    fread (&(fh->MagicWord2), 1, sizeof(char), f);
    fread (&(fh->Size), 1, sizeof(int), f);
    fread (&(fh->Reserv1), 1, sizeof(short), f);
    fread (&(fh->Reserv2), 1, sizeof(short), f);
    fread (&(fh->Offbits), 1, sizeof(int), f);

    fread (&(ih->Size), 1, sizeof(int), f);
    fread (&(ih->Width), 1, sizeof(int), f);
    fread (&(ih->Height), 1, sizeof(int), f);
    fread (&(ih->Planes), 1, sizeof(short), f);
    fread (&(ih->BitCount), 1, sizeof(short), f);
    fread (&(ih->Compression), 1, sizeof(int), f);
    fread (&(ih->SizeImage), 1, sizeof(int), f);
    fread (&(ih->XPelsPerM), 1, sizeof(int), f);
    fread (&(ih->YPelsPerM), 1, sizeof(int), f);
    fread (&(ih->ClrUsed), 1, sizeof(int), f);
    fread (&(ih->ClrImportant), 1, sizeof(int), f);

    #ifdef DEBUG
    print_headers(fh, ih);
    #endif

    // el archivo debe comenzar con la palabra magica "BM"

    if ( (fh->MagicWord1 != 'B') || (fh->MagicWord2 != 'M') ) {
        esValido = false;
    }

    // el tamano del archivo no debe sobrepasar los 4 Mbytes
    // 4 Mbytes = 4 * 2^20 bytes = 0040 0000h

//     if ( esValido && (fh->Size > 0x400000) ) {
//         printf("El tama�o del archivo es %d Kb, claramente mayor a 4 Mb.\n",
//             fh->Size / 1024);
//         esValido = false;
//     }

    // el ancho no debe ser mayor a 1000 pixeles

//     if ( esValido && (ih->Width > 1000) ) {
//         printf("El ancho de la imagen supera los 1000 pixeles :( \n");
//         esValido = false;
//     }

    // soporta solamente formato de color real de 24 bits (sin paleta).
    // esto esta dado por el BitCount

    if ( esValido && (ih->BitCount != 24) ) {
        printf("No hay soporte para bitmaps con paleta.\n");
        esValido = false;
    }

	if ( esValido && !((ih->Height % 8 == 0) && (ih->Width % 8 == 0))) {
		printf("Las medidas de la imágen no son múltiplos de 8.\n");
		esValido = false;
	}

    if ( esValido ) {

		int bytesPorCanal = ih->Width * ih->Height;

		*RBuffer = malloc(bytesPorCanal);
		*GBuffer = malloc(bytesPorCanal);
		*BBuffer = malloc(bytesPorCanal);



		int i;
		for (i = 0; i < bytesPorCanal; i++) {
			(*RBuffer)[i] = 0;
			(*GBuffer)[i] = 0;
			(*BBuffer)[i] = 0;
		}



        if (*RBuffer == NULL || *GBuffer == NULL || *BBuffer == NULL) {
        	esValido = false;
        } else {

        	int fileRowSize = 4 * ((24 * ih->Width + 31) / 32);
        	int validRowSize = ih->Width * 3;
        	int bytesMetidos = 0;
        	int j, k;
        	unsigned char* TBuffer = malloc(fileRowSize);

        	int indiceMatricial;

        	// cantidad de columnas y filas de matrices 8x8
        	int cantColumnasM = ih->Width / 8;
        	int cantFilasM = ih->Height / 8;

        	if (TBuffer == NULL) {
        		esValido = false;
        	} else {
				for (j = 0 ; j < ih->Height ; j++) {
					fread (TBuffer, 1, fileRowSize, f);
					bytesMetidos = 0;

					for (k = 0 ; k < validRowSize ; k += 3) {
						/*
							Armamos los buffers no como bits directos del BMP sino como Array de Matrices 8x8
							Por eso el indiceMatricial.
							Dolió.
						*/

 						//indiceMatricial = ( ((k/3) / 8) + (j/8 * (ih->Width / 8)) ) * 64  +  ((k/3) % 8)  +  ((j%8) * 8);
						indiceMatricial = ((bytesMetidos / 8) * 64) + ((j % 8)*8) + (bytesMetidos % 8) + (((j / 8) * 64) * ih->Width / 8);
						//printf("%d \n", indiceMatricial);
						(*BBuffer)[indiceMatricial] = TBuffer[k];
						(*GBuffer)[indiceMatricial] = TBuffer[k+1];
						(*RBuffer)[indiceMatricial] = TBuffer[k+2];
						bytesMetidos++;
					}
				}
				free(TBuffer);
        	}
        }
    }

    fclose(f);

    return esValido;

}


#ifdef DEBUG
void print_headers(BMPfileheader* fh, BMPinfoheader* ih) {
    printf("\nHeaders:\n");

    printf("\tFileHeader.MagicWord1 = %c\n", fh->MagicWord1);
    printf("\tFileHeader.MagicWord2 = %c\n", fh->MagicWord2);
    printf("\tFileHeader.Size = %d\n", fh->Size);
    printf("\tFileHeader.Reserv1 = %d\n", fh->Reserv1);
    printf("\tFileHeader.Reserv2 = %d\n", fh->Reserv2);
    printf("\tFileHeader.Offbits = %d\n", fh->Offbits);

    printf("\tInfoHeader.Size = %d\n", ih->Size);
    printf("\tInfoHeader.Width = %d\n", ih->Width);
    printf("\tInfoHeader.Height = %d\n", ih->Height);
    printf("\tInfoHeader.Planes = %d\n", ih->Planes);
    printf("\tInfoHeader.BitCount = %d\n", ih->BitCount);
    printf("\tInfoHeader.Compression = %d\n", ih->Compression);
    printf("\tInfoHeader.SizeImage = %d\n", ih->SizeImage);
    printf("\tInfoHeader.XPelsPerM = %d\n", ih->XPelsPerM);
    printf("\tInfoHeader.YPelsPerM = %d\n", ih->YPelsPerM);
    printf("\tInfoHeader.ClrUsed = %d\n", ih->ClrUsed);
    printf("\tInfoHeader.ClrImportant = %d\n", ih->ClrImportant);

    printf("\n");
}
#endif


#endif // __READBMP_H__
