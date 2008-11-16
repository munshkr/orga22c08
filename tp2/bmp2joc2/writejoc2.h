#ifndef __WRITEJOC2_H__
#define __WRITEJOC2_H__

// valores boolean
#define false 0
#define true 1

#define TC_ROW_SIZE 8


#include <stdio.h>
#include <stdlib.h>

#include "bmp2oc2.h"


int writeoc2(OC2fileheader* oc2fh, BMPfileheader* fh, BMPinfoheader* ih, codificacion* tc, char* bitstream, const char* file_path) {

	int ok = 0;
	int esValido = true;

	FILE * f2 = fopen(file_path, "wb");

	// guardo el header del oc2
	ok = fwrite( &(oc2fh->fType), sizeof(int), 1, f2 );
	if (!ok) esValido = false;
	ok = fwrite( &(oc2fh->tcSize), sizeof(short), 1, f2 );
	if (!ok) esValido = false;
	ok = fwrite( &(oc2fh->bsSize), sizeof(long long int), 1, f2 );
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

	// guardo la tabla de códigos
	ok = fwrite ( tc, oc2fh->tcSize, 1, f2 );
	if (!ok) esValido = false;

	// guardo el bitstream de codificación
	int bytes_buf_bitstream = oc2fh->bsSize / 32;
	if ((oc2fh->bsSize % 32) != 0) {
		bytes_buf_bitstream++;
	}
	bytes_buf_bitstream *= 4;

	ok = fwrite ( bitstream, bytes_buf_bitstream, 1, f2 );
	if (!ok) esValido = false;

	//fclose(f2 );
	return esValido;
}


#endif
