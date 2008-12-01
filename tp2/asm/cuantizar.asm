; void cuantizar(float bloque_transformado[8][8], float q[8][8], short bloque_cuantizado[8][8]);
; 	int i, j;
; 	for (i = 0 ; i < 8 ; i++) {
; 		for (j = 0 ; j < 8 ; j++) {
; 			bloque_cuantizado[i][j] = bloque_transformado[i][j] / q[i][j];
; 		}
; 	}
; }

global cuantizar


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


%macro dividir 2			; %1 y %2 son registros XMM
	movups %1, [esi]
	movups %2, [edi]
	lea esi, [esi + ebx]
	lea edi, [edi + ebx]
	
	divps %1, %2
%endmacro

%macro dividir_y_almacenar 0
	dividir xmm0, xmm4
	dividir xmm1, xmm5
	dividir xmm2, xmm6
	dividir xmm3, xmm7

	; convierte los floats de xmm0/1/2/3 a shorts 
	; empaquetándolos en xmm0/2
	cvttps2dq xmm0, xmm0
	cvttps2dq xmm1, xmm1
	packssdw xmm0, xmm1
	cvttps2dq xmm2, xmm2
	cvttps2dq xmm3, xmm3
	packssdw xmm2, xmm3

	; almacena los shorts en memoria (son 8)
	movdqu [edx], xmm0
	lea edx, [edx + ebx]
	movdqu [edx], xmm2
	lea edx, [edx + ebx]
%endmacro


%define p_bloque_in [ebp+8]
%define p_q [ebp+12]
%define p_bloque_out [ebp+16]


cuantizar:
	prologue 0

	mov ebx, 16
	mov esi, p_bloque_in
	mov edi, p_q
	mov edx, p_bloque_out

	; en cada "dividir_y_almacenar" se trabajan 16 de los 64 datos.
	dividir_y_almacenar
	dividir_y_almacenar
	dividir_y_almacenar
	dividir_y_almacenar

	prelude 0
	ret
