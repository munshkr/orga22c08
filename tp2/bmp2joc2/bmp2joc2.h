#ifndef __BMP2JOC2_H__
#define __BMP2JOC2_H__

#define bool int
#define false 0
#define true 1


extern void dividirEnBloques(char* CBuffer, int cantCols, char retBuff[8][8], int x, int y);
extern void generarDCT(float DCT[8][8]);
extern void unirBloques(char* CBuffer, int cantCols, char Cbloque[8][8], int x, int y);
extern void transformar(char bloque[8][8], float DCT[8][8], float bloque_transformado[8][8], float temp[8][8]);
extern void cuantizar(float bloque_transformado[8][8], float q[8][8], short bloque_cuantizado[8][8]);
extern void decuantizar(short bloque_cuantizado[8][8], float q[8][8], float bloque_transformado[8][8]);
extern void antitransformar(float bloque_transformado[8][8], float DCT_Trasp[8][8], char bloque[8][8], float temp[8][8]);

//codificar devuelve la longitud del bloque codificado
extern int codificar(short bloque_cuantizado[8][8], short* codificacion);

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



#endif // __BMP2JOC2_H__
