#ifndef __WRITEBMP_H__
#define __WRITEBMP_H__

// valores boolean
#define false 0
#define true 1

#include <stdio.h>
#include <stdlib.h>

#include "joc22bmp.h"

int writebmp(char* file_path, BMPfileheader* fh, BMPinfoheader* ih, char* RBuffer, char* GBuffer, char* BBuffer) {

	int ok = false;
	int esValido = true;

	FILE* f;

	//file_path[0] = '_';

	f = fopen(file_path, "wb");

	// guardo el fileheader del bmp
	ok = fwrite ( &(fh->MagicWord1), sizeof(char), 1, f);
	if (!ok) esValido = false;
	ok = fwrite ( &(fh->MagicWord2), sizeof(char), 1, f);
	if (!ok) esValido = false;
	ok = fwrite ( &(fh->Size), sizeof(int), 1, f);
	if (!ok) esValido = false;
	ok = fwrite ( &(fh->Reserv1), sizeof(short), 1, f);
	if (!ok) esValido = false;
	ok = fwrite ( &(fh->Reserv2), sizeof(short), 1, f);
	if (!ok) esValido = false;
	ok = fwrite ( &(fh->Offbits), sizeof(int), 1, f);
	if (!ok) esValido = false;

	// guardo el infoheader del bmp
	ok = fwrite ( &(ih->Size), sizeof(int), 1, f);
	if (!ok) esValido = false;
	ok = fwrite ( &(ih->Width), sizeof(int), 1, f);
	if (!ok) esValido = false;
	ok = fwrite ( &(ih->Height), sizeof(int), 1, f);
	if (!ok) esValido = false;
	ok = fwrite ( &(ih->Planes), sizeof(short), 1, f);
	if (!ok) esValido = false;
	ok = fwrite ( &(ih->BitCount), sizeof(short), 1, f);
	if (!ok) esValido = false;
	ok = fwrite ( &(ih->Compression), sizeof(int), 1, f);
	if (!ok) esValido = false;
	ok = fwrite ( &(ih->SizeImage), sizeof(int), 1, f);
	if (!ok) esValido = false;
	ok = fwrite ( &(ih->XPelsPerM), sizeof(int), 1, f);
	if (!ok) esValido = false;
	ok = fwrite ( &(ih->YPelsPerM), sizeof(int), 1, f);
	if (!ok) esValido = false;
	ok = fwrite ( &(ih->ClrUsed), sizeof(int), 1, f);
	if (!ok) esValido = false;
	ok = fwrite ( &(ih->ClrImportant), sizeof(int), 1, f);
	if (!ok) esValido = false;

	// guardo los datos de la imagen
	char* TBuffer = malloc(fh->Size - 54);
// 		char* TGBuffer = malloc(fh->Size - 54);
// 		char* TBBuffer = malloc(fh->Size - 54);
	int fileRowSize = 4 * ((24 * ih->Width + 31) / 32);
	int validRowSize = ih->Width * 3;
	int channelRowSize = ih->Width;
	int bytesMetidos = 0;
	int j, k, indiceMatricial;

	if (TBuffer == NULL) {
		ok = false;
		esValido = false;
	} else {

		for (j = 0 ; j < ih->Height ; j++) {
			for (k = 0 ; k < fileRowSize ; k++) {
				TBuffer[j * fileRowSize + k] = 255;
			}
		}

		for (j = 0 ; j < ih->Height ; j++) {
			bytesMetidos = 0;
			for (k = 0 ; k < validRowSize ; k+=3) {
				indiceMatricial = ((bytesMetidos / 8) * 64) + ((j % 8)*8) + (bytesMetidos % 8) + (((j / 8) * 64) * ih->Width / 8);
				if (indiceMatricial > (fh->Size - 54) / 3) {
					printf("·");
				} else {
					TBuffer[j * fileRowSize + k] = BBuffer[indiceMatricial];
					TBuffer[j * fileRowSize + k + 1] = GBuffer[indiceMatricial];
					TBuffer[j * fileRowSize + k + 2] = RBuffer[indiceMatricial];
					bytesMetidos++;
				}
			}
			for (k = 0 ; k < (fileRowSize - validRowSize) ; k++) {
				TBuffer[bytesMetidos] = 0;
				printf("Mugre\n");
				bytesMetidos++;
			}
		}
	}

	if (esValido) {
		ok = fwrite ( TBuffer, (fh->Size - 54), 1, f);
		if (!ok) esValido = false;
	}

	if (TBuffer != NULL)
		free(TBuffer);

	fclose(f);

	return esValido;
}


#endif
