; void unirBloques(char* CBuffer, int cantCols, unsigned char Cbloque[8][8], int x, int y) {
; 	CBuffer = CBuffer + (x * 64) + (y * cantCols * 64);
; 	int i,j;
; 	for (i = 0 ; i < 8 ; i++) {
; 		for (j = 0 ; j < 8 ; j++) {
; 			(*CBuffer++) = Cbloque[i][j];
; 		}
; 	}
; }

; Asume que Cbloque apunta a un buffer preexistente de 64 bytes


global unirBloques


%define p_cbuffer [ebp+8]
%define cantCols  [ebp+12]
%define p_bloque  [ebp+16]
%define x		  [ebp+20]
%define y		  [ebp+24]


unirBloques:

	push ebp
	mov ebp, esp

	push ebx
	push esi
	push edi

	mov ecx, p_cbuffer
	mov eax, x
	times 6 shl eax, 1
	add ecx, eax

	mov eax, y
	times 6 shl eax, 1
	mov ebx, cantCols
	mul ebx

	add ecx, eax
	adc ecx, edx

	mov eax, p_bloque

	movdqu xmm0, [eax]
	movdqu [ecx], xmm0
	times 16 inc ecx
	times 16 inc eax
	movdqu xmm0, [eax]
	movdqu [ecx], xmm0
	times 16 inc ecx
	times 16 inc eax
	movdqu xmm0, [eax]
	movdqu [ecx], xmm0
	times 16 inc ecx
	times 16 inc eax
	movdqu xmm0, [eax]
	movdqu [ecx], xmm0


	pop edi
	pop esi
	pop ebx
	pop ebp
	ret
