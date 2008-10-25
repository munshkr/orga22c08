#ifndef __READOC2_H__
#define __READOC2_H__

// valores boolean
#define false 0
#define true 1


#include <stdio.h>
#include <stdlib.h>

#include "oc22bmp.h"


int readoc2(const char* path, OC2fileheader* oc2fh, BMPfileheader* fh, BMPinfoheader* ih, codificacion** tablaCodigos, char** buffer) {
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
    if ( ftell(f) < 68) {
        fclose(f);
        return 0;
    }
    rewind(f);

    // asigna los headers a los structs




	fread(&(oc2fh->fType), 1, sizeof(int), f);
	fread(&(oc2fh->tcSize), 1, sizeof(short), f);
	fread(&(oc2fh->bsSize), 1, sizeof(long long int), f);

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

    // el archivo debe comenzar con la palabra magica "OC2\0"

	if(oc2fh->fType != 3294031) {
        esValido = false;
    }

    // el tamano del archivo no debe sobrepasar los 4 Mbytes
    // 4 Mbytes = 4 * 2^20 bytes = 0040 0000h

    if ( esValido && ((oc2fh->tcSize <= 0) || (oc2fh->bsSize <= 0))) {
        printf("El archivo es incorrecto.\n");
        esValido = false;
    }

    if ( esValido ) {

        // reserva memoria para el buffer con los datos de la imagen
		int bytesBS = oc2fh->bsSize/32;
		if (oc2fh->bsSize % 32 > 0){
			bytesBS++;
		}
		bytesBS = bytesBS * 4; //Para que se arregle el tema del alineado a 32 bits y su correspondiente garbage.

        *buffer = malloc(bytesBS);
		*tablaCodigos = malloc(oc2fh->tcSize);

        if (*buffer != NULL) {
			// lee el resto del archivo y lo trata de colocar en el buffer
			int tcsize = fread (*tablaCodigos, 1, (oc2fh->tcSize), f);
			int bssize = fread (*buffer, 1, bytesBS, f);

            // comprueba si ocurrio un error al leer
            if (tcsize != oc2fh->tcSize) {
                free(*tablaCodigos);
                esValido = false;
            }
			if (bssize != bytesBS) {
				free(*buffer);
				esValido = false;
			}
        }
    }

    fclose(f);

    return esValido;

}


#endif // __READOC2_H__
