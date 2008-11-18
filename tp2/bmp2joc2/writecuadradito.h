#ifndef __WRITECUADRADITO_H__
#define __WRITECUADRADITO_H__

#include "funcionesASM.h"
#include "bmp2joc2.h"

//int writecuadradito(char* file_path, BMPfileheader* fh, BMPinfoheader* ih, char* RBuffer, char* GBuffer, char* BBuffer, int x, int y);
//		writecuadradito(archivo_destino, &fh, &ih, (char*)bloque, (char*)Gbloque, (char*)Bbloque, 0, 0);

int writecuadradito(char* file_path, BMPfileheader* fh, BMPinfoheader* ih, char* RBuffer, char* GBuffer, char* BBuffer, int x, int y) {

	int ok = false;
	FILE* f[1];

	int i = 0;

	file_path[0] = x + '0';
	file_path[1] = 'x';
	file_path[2] = y + '0';

		f[i] = fopen(file_path, "wb");

		int cantCols = 1;//(ih->Width / 8); --> lo cambie a las 16:45

		int Size = 246;
		int SizeImage = 246-54;
		int Width = 8;
		int Height = 8;

		// guardo el fileheader del bmp
		ok = fwrite ( &(fh->MagicWord1), sizeof(char), 1, f[i] );
		if (!ok) return 0;
		ok = fwrite ( &(fh->MagicWord2), sizeof(char), 1, f[i] );
		if (!ok) return 0;
		ok = fwrite ( &(Size), sizeof(int), 1, f[i] );
		if (!ok) return 0;
		ok = fwrite ( &(fh->Reserv1), sizeof(short), 1, f[i] );
		if (!ok) return 0;
		ok = fwrite ( &(fh->Reserv2), sizeof(short), 1, f[i] );
		if (!ok) return 0;
		ok = fwrite ( &(fh->Offbits), sizeof(int), 1, f[i] );
		if (!ok) return 0;

		// guardo el infoheader del bmp
		ok = fwrite ( &(ih->Size), sizeof(int), 1, f[i] );
		if (!ok) return 0;
		ok = fwrite ( &(Width), sizeof(int), 1, f[i] );
		if (!ok) return 0;
		ok = fwrite ( &(Height), sizeof(int), 1, f[i] );
		if (!ok) return 0;
		ok = fwrite ( &(ih->Planes), sizeof(short), 1, f[i] );
		if (!ok) return 0;
		ok = fwrite ( &(ih->BitCount), sizeof(short), 1, f[i] );
		if (!ok) return 0;
		ok = fwrite ( &(ih->Compression), sizeof(int), 1, f[i] );
		if (!ok) return 0;
		ok = fwrite ( &(SizeImage), sizeof(int), 1, f[i] );
		if (!ok) return 0;
		ok = fwrite ( &(ih->XPelsPerM), sizeof(int), 1, f[i] );
		if (!ok) return 0;
		ok = fwrite ( &(ih->YPelsPerM), sizeof(int), 1, f[i] );
		if (!ok) return 0;
		ok = fwrite ( &(ih->ClrUsed), sizeof(int), 1, f[i] );
		if (!ok) return 0;
		ok = fwrite ( &(ih->ClrImportant), sizeof(int), 1, f[i] );
		if (!ok) return 0;

		// guardo los datos de la imagen
		char* TBuffer = malloc(Size - 54);


// 		RBuffer = (char*)dividirEnBloques(RBuffer, cantCols, x, y);
// 		GBuffer = (char*)dividirEnBloques(GBuffer, cantCols, x, y);
// 		BBuffer = (char*)dividirEnBloques(BBuffer, cantCols, x, y);






// 		char* TGBuffer = malloc(fh->Size - 54);
// 		char* TBBuffer = malloc(fh->Size - 54);
		int fileRowSize = 4 * ((24 * Width + 31) / 32); //si el ancho es 1, por lo menos voy a necesitar 4 bytes, por eso le sumo 31, para correjirlo
		int validRowSize = Width * 3;
		int channelRowSize = Width;
		int bytesMetidos = 0;
		int j, k;

		if (TBuffer == NULL) {
			ok = false;
			return 0;
		} else {
			for (j = 0 ; j < Height ; j++) {
				for (k = 0 ; k < channelRowSize ; k++) {
//					switch (i) {
//						case 0:
							TBuffer[bytesMetidos] = BBuffer[j * channelRowSize + k];
							TBuffer[bytesMetidos + 1] = GBuffer[j * channelRowSize + k];
							TBuffer[bytesMetidos + 2] = RBuffer[j * channelRowSize + k];
/*						return 0;
						case 1:
							TBuffer[bytesMetidos] = 0;
							TBuffer[bytesMetidos + 1] = GBuffer[j * channelRowSize + k];
							TBuffer[bytesMetidos + 2] = 0;
						return 0;
						case 2:
							TBuffer[bytesMetidos] = 0;
							TBuffer[bytesMetidos + 1] = 0;
							TBuffer[bytesMetidos + 2] = RBuffer[j * channelRowSize + k];
						return 0;
					}*/
					bytesMetidos += 3;
				}
				for (k = 0 ; k < (fileRowSize - validRowSize) ; k++) {
					return 0;
					TBuffer[bytesMetidos] = 0;
					bytesMetidos++;
				}
			}
		}

		ok = fwrite ( TBuffer, (Size - 54), 1, f[i]);
		if (!ok) return 0;

		fclose(f[i]);

	/*
	if (!ok) {
		fclose(f[i]);
	}
	*/

	return ok;
}


#endif
