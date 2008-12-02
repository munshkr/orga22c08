#ifndef __WRITEJOC2_H__
#define __WRITEJOC2_H__

// valores boolean
#define false 0
#define true 1

#define TC_ROW_SIZE 8


#include <stdio.h>
#include <stdlib.h>

#include "bmp2joc2.h"

int writejoc2(JOC2fileheader* joc2fh, BMPfileheader* fh, BMPinfoheader* ih, short* codificacion, char* file_path) {

	int ok = 0;
	int esValido = true;

	FILE * f2 = fopen(file_path, "wb");

	// guardo el header del oc2
	ok = fwrite( &(joc2fh->fType), sizeof(int), 1, f2 );
	if (!ok) esValido = false;
	ok = fwrite( &(joc2fh->bSize), sizeof(int), 1, f2 );
	if (!ok) esValido = false;

	// guardo el fileheader del bmp
    ok = fwrite ( &(fh->MagicWord1), sizeof(char), 1, f2 );
	if (!ok) esValido = false;
    ok = fwrite ( &(fh->MagicWord2), sizeof(char), 1, f2 );
	if (!ok) esValido = false;
    ok = fwrite ( &(fh->Size), sizeof(int), 1, f2 );
	if (!ok) esValido = false;
    ok = fwrite ( &(fh->Reserv1), sizeof(short), 1, f2 );
	if (!ok) esValido = false;
    ok = fwrite ( &(fh->Reserv2), sizeof(short), 1, f2 );
	if (!ok) esValido = false;
    ok = fwrite ( &(fh->Offbits), sizeof(int), 1, f2 );
	if (!ok) esValido = false;

	// guardo el infoheader del bmp
    ok = fwrite ( &(ih->Size), sizeof(int), 1, f2 );
	if (!ok) esValido = false;
    ok = fwrite ( &(ih->Width), sizeof(int), 1, f2 );
	if (!ok) esValido = false;
    ok = fwrite ( &(ih->Height), sizeof(int), 1, f2 );
	if (!ok) esValido = false;
    ok = fwrite ( &(ih->Planes), sizeof(short), 1, f2 );
	if (!ok) esValido = false;
    ok = fwrite ( &(ih->BitCount), sizeof(short), 1, f2 );
	if (!ok) esValido = false;
    ok = fwrite ( &(ih->Compression), sizeof(int), 1, f2 );
	if (!ok) esValido = false;
    ok = fwrite ( &(ih->SizeImage), sizeof(int), 1, f2 );
	if (!ok) esValido = false;
    ok = fwrite ( &(ih->XPelsPerM), sizeof(int), 1, f2 );
	if (!ok) esValido = false;
    ok = fwrite ( &(ih->YPelsPerM), sizeof(int), 1, f2 );
	if (!ok) esValido = false;
    ok = fwrite ( &(ih->ClrUsed), sizeof(int), 1, f2 );
	if (!ok) esValido = false;
    ok = fwrite ( &(ih->ClrImportant), sizeof(int), 1, f2 );
	if (!ok) esValido = false;

	// guardo la codificación
	if ((joc2fh->bSize % 4) != 0) {
		//Esto supuestamente no debería pasar nunca porque la imagen
		//es multiplo de 8 en ambas dimensiones
		printf("HUBO UN GRAVE PROBLEMA AL GRABAR, BYTESTREAM%4 != 0");
	}

	ok = fwrite ( codificacion, joc2fh->bSize, sizeof(short), f2 );
	if (!ok) esValido = false;

	//fclose(f2 ); --> Cuando cerrabamos el archivo nos tiraba error el C
	return esValido;
}


#endif
