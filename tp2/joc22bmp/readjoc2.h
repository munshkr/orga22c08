#ifndef __READJOC2_H__
#define __READJOC2_H__

// valores boolean
#define false 0
#define true 1

#include <stdio.h>
#include <stdlib.h>

#include "joc22bmp.h"


int readjoc2(const char* path, JOC2fileheader* joc2fh, BMPfileheader* fh, BMPinfoheader* ih, short** codificacion) {
	//char* buffer = NULL;
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
    if ( ftell(f) < 62) {
        fclose(f);
        return 0;
    }
    rewind(f);

    // asigna los headers a los structs
	fread(&(joc2fh->fType), 1, sizeof(int), f);
	fread(&(joc2fh->bSize), 1, sizeof(int), f);

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

    // el archivo debe comenzar con la palabra magica "JOC2\0"

	if(joc2fh->fType != 0x32434F4A) {
        esValido = false;
    }

    if ( esValido ) {

        // reserva memoria para el buffer con los datos de la imagen
        int bytesBS = joc2fh->bSize * sizeof(short);
		*codificacion = malloc(bytesBS);

        if (*codificacion != NULL) {
			// lee el resto del archivo y lo trata de colocar en el buffer
			int bssize = fread (*codificacion, 1, bytesBS, f);

            // comprueba si ocurrio un error al leer
            if (bssize != bytesBS) {
				free(*codificacion);
				esValido = false;
			}
        }
    }

    fclose(f);

    return esValido;

}

int readJOC2Deluxe(const char* path, JOC2fileheader* joc2fh, BMPfileheader* fh, BMPinfoheader* ih, short** codificacion) {
	//char* buffer = NULL;
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
    if ( ftell(f) < 62) {
        fclose(f);
        return 0;
    }
    rewind(f);


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

	fread (&(ih->SizeImage), 1, sizeof(int), f);

    // asigna los headers a los structs
	fread(&(joc2fh->fType), 1, sizeof(int), f);
	fread(&(joc2fh->bSize), 1, sizeof(int), f);

    fh->Size = ih->SizeImage + 54;

    #ifdef DEBUG
    print_headers(fh, ih);
    #endif

    // el archivo debe comenzar con la palabra magica "JOC2\0"

	if(joc2fh->fType != 0x44434F4A) {
        esValido = false;
    }

    if ( esValido ) {

        // reserva memoria para el buffer con los datos de la imagen
        int bytesBS = joc2fh->bSize * sizeof(short);
		*codificacion = malloc(bytesBS);

        if (*codificacion != NULL) {
			// lee el resto del archivo y lo trata de colocar en el buffer
			int bssize = fread (*codificacion, 1, bytesBS, f);

            // comprueba si ocurrio un error al leer
            if (bssize != bytesBS) {
				free(*codificacion);
				esValido = false;
			}
        }
    }

    fclose(f);

    return esValido;

}


#endif // __READJOC2_H__
