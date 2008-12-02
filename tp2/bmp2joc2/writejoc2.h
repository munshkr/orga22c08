#ifndef __WRITEJOC2_H__
#define __WRITEJOC2_H__

// valores boolean
#define false 0
#define true 1

#include <stdio.h>
#include <stdlib.h>

#include "bmp2joc2.h"

int writejoc2(BMPfileheader* fh, BMPinfoheader* ih, short* codificacion, int codLen, char* file_path) {

	int ok = 0;
	int esValido = true;

	//declaro el fileheader de JOC2
	JOC2fileheader joc2fh;
	joc2fh.fType = 0x32434F4A;
	joc2fh.bSize = codLen;

	FILE * f2 = fopen(file_path, "wb");

	// guardo el header del joc2
	ok = fwrite( &(joc2fh.fType), sizeof(int), 1, f2 );
	if (!ok) esValido = false;
	ok = fwrite( &(joc2fh.bSize), sizeof(int), 1, f2 );
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

	ok = fwrite ( codificacion, sizeof(short), joc2fh.bSize, f2 );
	if (!ok) esValido = false;

	fclose(f2); // --> Cuando cerrabamos el archivo nos tiraba error el C
	return esValido;
}

int writeJOC2Deluxe(BMPfileheader* fh, BMPinfoheader* ih, short* codificacion, int codLen, char* file_path) {

	int ok = 0;
	int esValido = true;

	//declaro el fileheader de JOC2
	JOC2fileheader joc2fh;
	joc2fh.fType = 0x44434F4A;
	joc2fh.bSize = codLen;

	FILE * f2 = fopen(file_path, "wb");

	int tmpSize = ih->SizeImage;

	fh->Size = joc2fh.bSize * 2 + 54 + 8 + 4;
	printf("fh.Size : %d\n", fh->Size);
	ih->SizeImage = joc2fh.bSize * 2 + 8 + 4;
	printf("ih.SizeImage : %d\n", ih->SizeImage);

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

	// guardo el header del JOC2DELUXE!!!!
	ok = fwrite( &(tmpSize), sizeof(int), 1, f2 );
	if (!ok) esValido = false;

	// guardo el header del joc2
	ok = fwrite( &(joc2fh.fType), sizeof(int), 1, f2 );
	if (!ok) esValido = false;
	ok = fwrite( &(joc2fh.bSize), sizeof(int), 1, f2 );
	if (!ok) esValido = false;

	ok = fwrite ( codificacion, sizeof(short), joc2fh.bSize, f2 );
	if (!ok) esValido = false;

	fclose(f2); // --> Cuando cerrabamos el archivo nos tiraba error el C
	return esValido;
}


#endif
