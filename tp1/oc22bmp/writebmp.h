#ifndef __WRITEBMP_H__
#define __WRITEBMP_H__

// valores boolean
#define false 0
#define true 1

#define TC_ROW_SIZE 8


#include <stdio.h>
#include <stdlib.h>

#include "oc22bmp.h"


int writebmp(BMPfileheader* fh, BMPinfoheader* ih, char* buffer_bmp, int size_buffer, const char* file_path) {

	int ok = 0;
	FILE * f = fopen(file_path, "wb");

	// guardo el fileheader del bmp
    ok = fwrite ( &(fh->MagicWord1), sizeof(char), 1, f );
	if (!ok) return false;
    ok = fwrite ( &(fh->MagicWord2), sizeof(char), 1, f );
	if (!ok) return false;
    ok = fwrite ( &(fh->Size), sizeof(int), 1, f );
	if (!ok) return false;
    ok = fwrite ( &(fh->Reserv1), sizeof(short), 1, f );
	if (!ok) return false;
    ok = fwrite ( &(fh->Reserv2), sizeof(short), 1, f );
	if (!ok) return false;
    ok = fwrite ( &(fh->Offbits), sizeof(int), 1, f );
	if (!ok) return false;

	// guardo el infoheader del bmp
    ok = fwrite ( &(ih->Size), sizeof(int), 1, f );
	if (!ok) return false;
    ok = fwrite ( &(ih->Width), sizeof(int), 1, f );
	if (!ok) return false;
    ok = fwrite ( &(ih->Height), sizeof(int), 1, f );
	if (!ok) return false;
    ok = fwrite ( &(ih->Planes), sizeof(short), 1, f );
	if (!ok) return false;
    ok = fwrite ( &(ih->BitCount), sizeof(short), 1, f );
	if (!ok) return false;
    ok = fwrite ( &(ih->Compression), sizeof(int), 1, f );
	if (!ok) return false;
    ok = fwrite ( &(ih->SizeImage), sizeof(int), 1, f );
	if (!ok) return false;
    ok = fwrite ( &(ih->XPelsPerM), sizeof(int), 1, f );
	if (!ok) return false;
    ok = fwrite ( &(ih->YPelsPerM), sizeof(int), 1, f );
	if (!ok) return false;
    ok = fwrite ( &(ih->ClrUsed), sizeof(int), 1, f );
	if (!ok) return false;
    ok = fwrite ( &(ih->ClrImportant), sizeof(int), 1, f );
	if (!ok) return false;

	// guardo los datos de la imagen
	ok = fwrite ( buffer_bmp, size_buffer, 1, f );
	if (!ok) return false;

	fclose(f);
	return true;
}


#endif
