#ifndef __WRITEBMP_H__
#define __WRITEBMP_H__

// valores boolean
#define false 0
#define true 1

#define TC_ROW_SIZE 8


#include <stdio.h>
#include <stdlib.h>

#include "bmp2joc2.h"


int writecbmp(char* file_path, BMPfileheader* fh, BMPinfoheader* ih, char* RBuffer, char* GBuffer, char* BBuffer) {

	int ok = false;
	FILE* f[3];

	int i;
	for (i = 0 ; i < 3 ; i++) {
		switch (i) {
			case 0:
				file_path[0] = 'B';
				break;
			case 1:
				file_path[0] = 'G';
				break;
			case 2:
				file_path[0] = 'R';
				break;
		}

		f[i] = fopen(file_path, "wb");

		// guardo el fileheader del bmp
		ok = fwrite ( &(fh->MagicWord1), sizeof(char), 1, f[i] );
		if (!ok) break;
		ok = fwrite ( &(fh->MagicWord2), sizeof(char), 1, f[i] );
		if (!ok) break;
		ok = fwrite ( &(fh->Size), sizeof(int), 1, f[i] );
		if (!ok) break;
		ok = fwrite ( &(fh->Reserv1), sizeof(short), 1, f[i] );
		if (!ok) break;
		ok = fwrite ( &(fh->Reserv2), sizeof(short), 1, f[i] );
		if (!ok) break;
		ok = fwrite ( &(fh->Offbits), sizeof(int), 1, f[i] );
		if (!ok) break;

		// guardo el infoheader del bmp
		ok = fwrite ( &(ih->Size), sizeof(int), 1, f[i] );
		if (!ok) break;
		ok = fwrite ( &(ih->Width), sizeof(int), 1, f[i] );
		if (!ok) break;
		ok = fwrite ( &(ih->Height), sizeof(int), 1, f[i] );
		if (!ok) break;
		ok = fwrite ( &(ih->Planes), sizeof(short), 1, f[i] );
		if (!ok) break;
		ok = fwrite ( &(ih->BitCount), sizeof(short), 1, f[i] );
		if (!ok) break;
		ok = fwrite ( &(ih->Compression), sizeof(int), 1, f[i] );
		if (!ok) break;
		ok = fwrite ( &(ih->SizeImage), sizeof(int), 1, f[i] );
		if (!ok) break;
		ok = fwrite ( &(ih->XPelsPerM), sizeof(int), 1, f[i] );
		if (!ok) break;
		ok = fwrite ( &(ih->YPelsPerM), sizeof(int), 1, f[i] );
		if (!ok) break;
		ok = fwrite ( &(ih->ClrUsed), sizeof(int), 1, f[i] );
		if (!ok) break;
		ok = fwrite ( &(ih->ClrImportant), sizeof(int), 1, f[i] );
		if (!ok) break;

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
			break;
		} else {
			for (j = 0 ; j < ih->Height ; j++) {
				for (k = 0 ; k < channelRowSize ; k++) {
					switch (i) {
						case 0:
							TBuffer[bytesMetidos] = BBuffer[j * channelRowSize + k];
							TBuffer[bytesMetidos + 1] = 0;
							TBuffer[bytesMetidos + 2] = 0;
						break;
						case 1:
							TBuffer[bytesMetidos] = 0;
							TBuffer[bytesMetidos + 1] = GBuffer[j * channelRowSize + k];
							TBuffer[bytesMetidos + 2] = 0;
						break;
						case 2:
							TBuffer[bytesMetidos] = 0;
							TBuffer[bytesMetidos + 1] = 0;
							TBuffer[bytesMetidos + 2] = RBuffer[j * channelRowSize + k];
						break;
					}
					bytesMetidos += 3;
				}
				for (k = 0 ; k < (fileRowSize - validRowSize) ; k++) {
					TBuffer[bytesMetidos] = 0;
					bytesMetidos++;
				}
			}
		}

		ok = fwrite ( TBuffer, (fh->Size - 54), 1, f[i]);
		if (!ok) break;

		fclose(f[i]);
	}

	if (!ok) {
		fclose(f[i]);
	}

	return ok;
}


#endif
