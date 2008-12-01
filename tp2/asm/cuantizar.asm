; void cuantizar(float bloque_transformado[8][8], float q[8][8], short bloque_cuantizado[8][8]);
; 	int i, j;
; 	for (i = 0 ; i < 8 ; i++) {
; 		for (j = 0 ; j < 8 ; j++) {
; 			bloque_cuantizado[i][j] = bloque_transformado[i][j] / Q_MATRIX[i][j];
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
	times 16 inc esi
	times 16 inc edi

	divps %1, %2
	cvttps2dq %1, %1
%endmacro


%define p_bloque_in [ebp+8]
%define p_q [ebp+12]
%define p_bloque_out [ebp+16]


cuantizar:
	prologue 0

	mov esi, p_bloque_in
	mov edi, p_q
	mov edx, p_bloque_out

	dividir xmm0, xmm4
	dividir xmm1, xmm5
	dividir xmm2, xmm6
	dividir xmm3, xmm7

	packssdw xmm0, xmm1
	packssdw xmm2, xmm3

	movdqu [edx], xmm0
	times 16 inc edx
	movdqu [edx], xmm2
	times 16 inc edx

	dividir xmm0, xmm4
	dividir xmm1, xmm5
	dividir xmm2, xmm6
	dividir xmm3, xmm7

	packssdw xmm0, xmm1
	packssdw xmm2, xmm3

	movdqu [edx], xmm0
	times 16 inc edx
	movdqu [edx], xmm2
	times 16 inc edx

	dividir xmm0, xmm4
	dividir xmm1, xmm5
	dividir xmm2, xmm6
	dividir xmm3, xmm7

	packssdw xmm0, xmm1
	packssdw xmm2, xmm3

	movdqu [edx], xmm0
	times 16 inc edx
	movdqu [edx], xmm2
	times 16 inc edx

	dividir xmm0, xmm4
	dividir xmm1, xmm5
	dividir xmm2, xmm6
	dividir xmm3, xmm7

	packssdw xmm0, xmm1
	packssdw xmm2, xmm3

	movdqu [edx], xmm0
	times 16 inc edx
	movdqu [edx], xmm2
	times 16 inc edx

	prelude 0
	ret
