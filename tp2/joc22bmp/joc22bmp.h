#ifndef __JOC22BMP_H__
#define __JOC22BMP_H__

#define bool int
#define false 0
#define true 1


extern void dividirEnBloques(unsigned char* CBuffer, int cantCols, unsigned char retBuff[8][8], int x, int y);
extern void generarDCT(float DCT[8][8]);
extern void unirBloques(unsigned char* CBuffer, int cantCols, unsigned char Cbloque[8][8], int x, int y);
extern void decuantizar(short bloque_cuantizado[8][8], float q[8][8], float bloque_transformado[8][8]);
extern void antitransformar(float bloque_transformado[8][8], float DCT_Trasp[8][8], unsigned char bloque[8][8], float temp[8][8]);
//decodificar devuelve la cantidad de posiciones que se adelantaron en el stream
extern int decodificar(short* codificacion, short bloque_decodificado[8][8]);

extern void trasponer(float m[8][8]);



typedef struct _BMPfileheader {
    char MagicWord1;  // char "B"
    char MagicWord2;  // char "M"
    int Size;         // tamano del archivo
    short Reserv1;
    short Reserv2;
    int Offbits;      // offset desde el comienzo del archivo a los datos
} BMPfileheader;

typedef struct _BMPinfoheader {
    int Size;         // tamano de este header en bytes (40)
    int Width;        // ancho en pixeles
    int Height;       // alto en pixeles
    short Planes;     // 1
    short BitCount;   // 1, 4, 8, o 24 (debe ser 24)
    int Compression;
    int SizeImage;    // tamano de la imagen en bytes
    int XPelsPerM;
    int YPelsPerM;
    int ClrUsed;
    int ClrImportant;
} BMPinfoheader;

typedef struct _JOC2fileheader {
	int fType;			// el string "JOC2" -> 0x4A4F4332 (en little-endian)
	int bSize;			// tamaño en bytes del bitstream
} JOC2fileheader;

float Q_MATRIX[8][8] = {
	{16, 11, 10, 16,  24,  40,  51,  61},
	{12, 12, 14, 19,  26,  58,  60,  55},
	{14, 13, 16, 24,  40,  57,  69,  56},
	{14, 17, 22, 29,  51,  87,  80,  62},
	{18, 22, 37, 56,  68, 109, 103,  77},
	{24, 35, 55, 64,  81, 104, 113,  92},
	{49, 64, 78, 87, 103, 121, 120, 101},
	{72, 92, 95, 98, 112, 100, 103,  99}
};

#endif // __JOC22BMP_H__
