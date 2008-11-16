#ifndef __FUNCIONESASM_H__
#define __FUNCIONESASM_H__

char* dividirEnBloques(char* CBuffer, int cantCols, int x, int y) {
	return CBuffer + (x * 64) + (y * cantCols * 64);
}





#endif
