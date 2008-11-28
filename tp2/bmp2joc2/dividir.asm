; void dividirEnBloques(char* CBuffer, int cantCols, unsigned char retBuff[8][8], int x, int y) {
	 ; CBuffer = CBuffer + (x * 64) + (y * cantCols * 64);
		; int i,j;
    	; for (i = 0 ; i < 8 ; i++) {
			; for (j = 0 ; j < 8 ; j++) {
				; retBuff[i][j] = (char)(*CBuffer++);
			; }
		; }
; }

; Asume que retBuff apunta a un buffer preexistente de 64 bytes


global dividirEnBloques

%macro prologue 1
	push ebp
	mov ebp, esp
	sub esp, %1

	push ebx
	push esi
	push edi
%endmacro

%macro prelude 1
	pop edi
	pop esi
	pop ebx

	add esp, %1
	pop ebp
%endmacro


%define p_cbuffer [ebp+8]
%define cantCols  [ebp+12]
%define p_bloque  [ebp+16]
%define x		  [ebp+20]
%define y		  [ebp+24]


dividirEnBloques:
	prologue 0

	mov ecx, p_cbuffer
	mov eax, x
	shl eax, 6
	add ecx, eax

	mov eax, y
	shl eax, 6
	mov ebx, cantCols
	mul ebx

	add ecx, eax
	adc ecx, edx

	mov eax, p_bloque

	movq mm0, [ecx]
	movq qword [eax], mm0
	emms

	prelude 0
	ret
