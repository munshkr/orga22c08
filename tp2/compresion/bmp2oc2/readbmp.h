#ifndef __READBMP_H__
#define __READBMP_H__

// valores boolean
#define false 0
#define true 1


#include <stdio.h>
#include <stdlib.h>

#include "bmp2oc2.h"


void print_headers(BMPfileheader* fh, BMPinfoheader* ih);


char* readbmp(const char* path, BMPfileheader* fh, BMPinfoheader* ih) {
	char* buffer = NULL;

    // intenta abrir el archivo en path
    FILE* f = fopen(path, "rb");
    if (f == NULL) {
        return 0;
    }

    // asume que el archivo es valido
    int esValido = true;

    // chequea si el archivo tiene por lo menos 54 bytes para poder
    // extraer los headers
    fseek(f, 0, SEEK_END);
    if ( ftell(f) < 54 ) {
        fclose(f);
        return 0;
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

    // soporta solamente formato de color real de 24 bits (sin paleta).
    // esto esta dado por el BitCount

    if ( esValido && (ih->BitCount != 24) ) {
        printf("No hay soporte para bitmaps con paleta.\n");
        esValido = false;
    }

    if ( esValido ) {

        // reserva memoria para el buffer con los datos de la imagen
        buffer = malloc(fh->Size - 54);

        if (buffer != NULL) {
			// lee el resto del archivo y lo trata de colocar en el buffer
            int size = fread (buffer, 1, (fh->Size - 54), f);

            // comprueba si ocurrio un error al leer
            if (size != fh->Size - 54) {
                free(buffer);
                buffer = NULL;
            }
        }
    }

    fclose(f);

    return buffer;

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
