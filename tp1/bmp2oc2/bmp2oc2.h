#ifndef __BMP2OC2_H__
#define __BMP2OC2_H__

#define TC_ROW_SIZE 8

typedef struct {
    char simbolo;
    char longCod;
    int cod;
} codificacion;

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

typedef struct _OC2fileheader {
	int fType;			// el string "OC2" -> 0x4f433200 (en little-endian)
	short tcSize;		// tamaño en bytes de la tabla de codigos
	long long int bsSize;	// tamaño en bits del bitstream
} OC2fileheader;


//
// accepts a decimal integer and returns a binary coded string
//
void dec2bin(long decimal, int longCod, char *binary)
{
  int  k = 0, n = 0;
  int  neg_flag = 0;
  int  remain;
  int  old_decimal;  // for test
  char temp[80];

  int j = 0;
  while(j<32){
  	temp[j] = '0';
  	j++;
  }

  // take care of negative input
  if (decimal < 0)
  {      
    decimal = -decimal;
    neg_flag = 1;
  }
  do 
  {
    old_decimal = decimal;   // for test
    remain    = decimal % 2;
    // whittle down the decimal number
    decimal   = decimal / 2;
    // this is a test to show the action
    //printf("%d/2 = %d  remainder = %d\n", old_decimal, decimal, remain);
    // converts digit 0 or 1 to character '0' or '1'
    temp[k++] = remain + '0';
  } while (decimal > 0);

  if (neg_flag)
    temp[k++] = '-';       // add - sign
  else
    temp[k++] = '0';       // space

  // reverse the spelling
  k = longCod;
  while (k >= 0)
    binary[n++] = temp[--k];

  binary[n-1] = 0;         // end with NULL
}

#endif // __BMP2OC2_H__
