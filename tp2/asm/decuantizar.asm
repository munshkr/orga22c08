; void decuantizar(short bloque_cuantizado[8][8], float q[8][8], float bloque_transformado[8][8]) {
; 	int i, j;
; 
; 	for (i = 0 ; i < 8 ; i++) {
; 		for (j = 0 ; j < 8 ; j++) {
; 			bloque_transformado[i][j] = bloque_cuantizado[i][j] * q[i][j];
; 		}
; 	}
; }

global decuantizar


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


%macro shorts_a_floats 2
	movdqu %1, [esi]
	lea esi, [esi + ebx]
	
	movdqu %2, %1
	punpcklwd %1, xmm7
	punpckhwd %2, xmm7
	cvtdq2ps %1, %1
	cvtdq2ps %2, %2
%endmacro

%macro multiplicar 2
	movups %2, [edi]
	lea edi, [edi + ebx]
	mulps %1, %2
%endmacro

%macro almacenar 1
	movups [edx], %1
	lea edx, [edx + ebx]
%endmacro

%macro decuant 0
	; xmm7 se usa para desempaquetar y convertir shorts a longs
	pxor xmm7, xmm7
	
	shorts_a_floats xmm0, xmm1
	shorts_a_floats xmm2, xmm3
	
	multiplicar xmm0, xmm4
	multiplicar xmm1, xmm5
	multiplicar xmm2, xmm6
	multiplicar xmm3, xmm7
	
	almacenar xmm0
	almacenar xmm1
	almacenar xmm2
	almacenar xmm3
%endmacro


%define p_bloque_in [ebp+8]
%define p_q [ebp+12]
%define p_bloque_out [ebp+16]


decuantizar:
	prologue 0

	mov ebx, 16
	mov esi, p_bloque_in
	mov edi, p_q
	mov edx, p_bloque_out

	decuant
	decuant
	decuant
	decuant
	
	prelude 0
	ret
