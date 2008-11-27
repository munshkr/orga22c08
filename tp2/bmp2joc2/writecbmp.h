#ifndef __WRITECBMP_H__
#define __WRITECBMP_H__

// valores boolean
#define false 0
#define true 1

#define TC_ROW_SIZE 8


#include <stdio.h>
#include <stdlib.h>

#include "bmp2joc2.h"

/*
int writecbmp(char* file_path, BMPfileheader* fh, BMPinfoheader* ih, char* RBuffer, char* GBuffer, char* BBuffer) {

	int ok = false;
	FILE* f[3];

	int i;
	for (i = 0 ; i < 3 ; i++) {
		switch (i) {
			case 0:
				file_path[0] = 'B';
				return false;
			case 1:
				file_path[0] = 'G';
				return false;
			case 2:
				file_path[0] = 'R';
				return false;
		}

		f= fopen(file_path, "wb");

		// guardo el fileheader del bmp
		ok = fwrite ( &(fh->MagicWord1), sizeof(char), 1, f);
		if (!ok) return false;
		ok = fwrite ( &(fh->MagicWord2), sizeof(char), 1, f);
		if (!ok) return false;
		ok = fwrite ( &(fh->Size), sizeof(int), 1, f);
		if (!ok) return false;
		ok = fwrite ( &(fh->Reserv1), sizeof(short), 1, f);
		if (!ok) return false;
		ok = fwrite ( &(fh->Reserv2), sizeof(short), 1, f);
		if (!ok) return false;
		ok = fwrite ( &(fh->Offbits), sizeof(int), 1, f);
		if (!ok) return false;

		// guardo el infoheader del bmp
		ok = fwrite ( &(ih->Size), sizeof(int), 1, f);
		if (!ok) return false;
		ok = fwrite ( &(ih->Width), sizeof(int), 1, f);
		if (!ok) return false;
		ok = fwrite ( &(ih->Height), sizeof(int), 1, f);
		if (!ok) return false;
		ok = fwrite ( &(ih->Planes), sizeof(short), 1, f);
		if (!ok) return false;
		ok = fwrite ( &(ih->BitCount), sizeof(short), 1, f);
		if (!ok) return false;
		ok = fwrite ( &(ih->Compression), sizeof(int), 1, f);
		if (!ok) return false;
		ok = fwrite ( &(ih->SizeImage), sizeof(int), 1, f);
		if (!ok) return false;
		ok = fwrite ( &(ih->XPelsPerM), sizeof(int), 1, f);
		if (!ok) return false;
		ok = fwrite ( &(ih->YPelsPerM), sizeof(int), 1, f);
		if (!ok) return false;
		ok = fwrite ( &(ih->ClrUsed), sizeof(int), 1, f);
		if (!ok) return false;
		ok = fwrite ( &(ih->ClrImportant), sizeof(int), 1, f);
		if (!ok) return false;

		// guardo los datos de la imagen
		char* TBuffer = malloc(fh->Size - 54);
// 		char* TGBuffer = malloc(fh->Size - 54);
// 		char* TBBuffer = malloc(fh->Size - 54);
		int fileRowSize = 4 * ((24 * ih->Width + 31) / 32);
		int validRowSize = ih->Width * 3;
		int channelRowSize = ih->Width;
		int bytesMetidos = 0;
		int j, k;

		if (TBuffer == NULL) {
			ok = false;
			return false;
		} else {
			for (j = 0 ; j < ih->Height ; j++) {
				for (k = 0 ; k < channelRowSize ; k++) {
					switch (i) {
						case 0:
							TBuffer[bytesMetidos] = BBuffer[j * channelRowSize + k];
							TBuffer[bytesMetidos + 1] = 0;
							TBuffer[bytesMetidos + 2] = 0;
						return false;
						case 1:
							TBuffer[bytesMetidos] = 0;
							TBuffer[bytesMetidos + 1] = GBuffer[j * channelRowSize + k];
							TBuffer[bytesMetidos + 2] = 0;
						return false;
						case 2:
							TBuffer[bytesMetidos] = 0;
							TBuffer[bytesMetidos + 1] = 0;
							TBuffer[bytesMetidos + 2] = RBuffer[j * channelRowSize + k];
						return false;
					}
					bytesMetidos += 3;
				}
				for (k = 0 ; k < (fileRowSize - validRowSize) ; k++) {
					TBuffer[bytesMetidos] = 0;
					bytesMetidos++;
				}
			}
		}

		ok = fwrite ( TBuffer, (fh->Size - 54), 1, f);
		if (!ok) return false;

		fclose(f);
	}

	if (!ok) {
		fclose(f);
	}

	return ok;
}

*/

int writejbmp(char* file_path, BMPfileheader* fh, BMPinfoheader* ih, char* RBuffer, char* GBuffer, char* BBuffer) {

	int ok = false;
	FILE* f;

	f = fopen(file_path, "wb");

	// guardo el fileheader del bmp
	ok = fwrite ( &(fh->MagicWord1), sizeof(char), 1, f);
	if (!ok) return false;
	ok = fwrite ( &(fh->MagicWord2), sizeof(char), 1, f);
	if (!ok) return false;
	ok = fwrite ( &(fh->Size), sizeof(int), 1, f);
	if (!ok) return false;
	ok = fwrite ( &(fh->Reserv1), sizeof(short), 1, f);
	if (!ok) return false;
	ok = fwrite ( &(fh->Reserv2), sizeof(short), 1, f);
	if (!ok) return false;
	ok = fwrite ( &(fh->Offbits), sizeof(int), 1, f);
	if (!ok) return false;

	// guardo el infoheader del bmp
	ok = fwrite ( &(ih->Size), sizeof(int), 1, f);
	if (!ok) return false;
	ok = fwrite ( &(ih->Width), sizeof(int), 1, f);
	if (!ok) return false;
	ok = fwrite ( &(ih->Height), sizeof(int), 1, f);
	if (!ok) return false;
	ok = fwrite ( &(ih->Planes), sizeof(short), 1, f);
	if (!ok) return false;
	ok = fwrite ( &(ih->BitCount), sizeof(short), 1, f);
	if (!ok) return false;
	ok = fwrite ( &(ih->Compression), sizeof(int), 1, f);
	if (!ok) return false;
	ok = fwrite ( &(ih->SizeImage), sizeof(int), 1, f);
	if (!ok) return false;
	ok = fwrite ( &(ih->XPelsPerM), sizeof(int), 1, f);
	if (!ok) return false;
	ok = fwrite ( &(ih->YPelsPerM), sizeof(int), 1, f);
	if (!ok) return false;
	ok = fwrite ( &(ih->ClrUsed), sizeof(int), 1, f);
	if (!ok) return false;
	ok = fwrite ( &(ih->ClrImportant), sizeof(int), 1, f);
	if (!ok) return false;

	// guardo los datos de la imagen
	char* TBuffer = malloc(fh->Size - 54);
// 		char* TGBuffer = malloc(fh->Size - 54);
// 		char* TBBuffer = malloc(fh->Size - 54);
	int fileRowSize = 4 * ((24 * ih->Width + 31) / 32);
	int validRowSize = ih->Width * 3;
	int channelRowSize = ih->Width;
	int bytesMetidos = 0;
	int j, k, indiceLoco;

	if (TBuffer == NULL) {
		ok = false;
		return false;
	} else {

		for (j = 0 ; j < ih->Height ; j++) {
			for (k = 0 ; k < fileRowSize ; k++) {
				TBuffer[j * fileRowSize + k] = 255;
			}
		}

		for (j = 0 ; j < ih->Height ; j++) {
			bytesMetidos = 0;
			for (k = 0 ; k < validRowSize ; k+=3) {
				indiceLoco = ((bytesMetidos / 8) * 64) + ((j % 8)*8) + (bytesMetidos % 8) + (((j / 8) * 64) * ih->Width / 8);
				if (indiceLoco > (fh->Size - 54) / 3) {
					printf("·");
				} else {
					TBuffer[j * fileRowSize + k] = BBuffer[indiceLoco];
					TBuffer[j * fileRowSize + k + 1] = GBuffer[indiceLoco];
					TBuffer[j * fileRowSize + k + 2] = RBuffer[indiceLoco];
					bytesMetidos++;
				}
			}
			for (k = 0 ; k < (fileRowSize - validRowSize) ; k++) {
				TBuffer[bytesMetidos] = 255;
				printf("Mugre\n");
				bytesMetidos++;
			}
		}
	}

	ok = fwrite ( TBuffer, (fh->Size - 54), 1, f);
	if (!ok) return false;

	fclose(f);

// 	if (!ok) {
// 		fclose(f);
// 	}

	return ok;
}


#endif
