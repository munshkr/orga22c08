; void transformar(unsigned char bloque[8][8], float DCT[8][8], float bloque_transformado[8][8], float bloqueTemp[8][8]) {
; 	float bloqueTemp[8][8];
;
; 	transponer_char(bloque);
; 	mult_matrices_float_char(DCT, bloque, bloqueTemp);
; 	transponer_float(DCT);
; 	mult_matrices_float_float(bloqueTemp, DCT, bloque_transformado);
; 	transponer_float(DCT);
; }

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


global transformar

section .text


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;; MUL TIPLICAR ;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	%macro pohrmonheioh1 3	;%1 ~ %2 = %3    (REGISTROS)	;la escribimos asi porque es soft
		mov esi, %1
		mov edi, %2
		mov ecx, %3

		xor edx, edx
		times 4 inc edx
		times 2 shl edx, 1 									;lo escribimos asi porque 1 es constante para el opcode

		xor eax, eax
		xor ebx, ebx

	.ciclo_i:
		movups xmm0, [esi]
		add esi, edx
		movups xmm1, [esi]
		add esi, edx
	.ciclo_j:
		movups xmm2, [edi]
		add edi, edx
		movups xmm3, [edi]
		add edi, edx

		mulps xmm2, xmm0
		mulps xmm3, xmm1
		addps xmm3, xmm2
		movups xmm2, xmm3
		movhlps xmm2, xmm2
		addps xmm3, xmm2
		movups xmm2, xmm3
		psrldq xmm2, 4
		addss xmm3, xmm2

		inc ebx
		cmp ebx, 4
		ja .uso_5

		pslldq xmm4, 4
		movss xmm4, xmm3
		jmp .sigo
	.uso_5:
		pslldq xmm5, 4
		movss xmm5, xmm3
	.sigo:
		cmp ebx, 8
		jb .ciclo_j

	.fin_ciclo_j:
		shufps xmm4, xmm4, 00011011b
		movups [ecx], xmm4
		add ecx, edx
		shufps xmm5, xmm5, 00011011b
		movups [ecx], xmm5
		add ecx, edx

		times 16 sub edi, edx

		xor ebx, ebx
		inc eax
		cmp eax, 8
		jb .ciclo_i

	%endmacro


;|||||||||||||||||||||||||||||||
;|||||||| MUL TIPLICAR |||||||||
;|||||||||||||||||||||||||||||||

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;; CHA RTOFLOAT ;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	%macro char_to_float 2		;%2 = char_to_float(%1)
		mov esi, %1
		mov edi, %2

		pxor mm0, mm0
		mov ecx, 16

	.ciclo:
		movd mm1, [esi]
		punpcklbw mm1, mm0

		movq mm2, mm1
		punpcklwd mm1, mm0
		punpckhwd mm2, mm0

		cvtpi2ps xmm0, mm2
		movlhps xmm0, xmm0
		cvtpi2ps xmm0, mm1

		movdqu [edi], xmm0

		times 4 inc esi
		times 16 inc edi
		loop .ciclo

		emms
	%endmacro

;|||||||||||||||||||||||||||||||
;|||||||| CHA RTOFLOAT |||||||||
;|||||||||||||||||||||||||||||||

%define p_bloque_in [ebp+8]
%define p_dct [ebp+12]
%define p_bloque_out [ebp+16]
%define p_MTemp [ebp+20]


transformar:
	prologue 0

	char_to_float p_bloque_in, p_MTemp
pri_mult:
	pohrmonheioh1 p_dct, p_MTemp, p_bloque_out
seg_mult:
	pohrmonheioh1 p_bloque_out, p_dct, p_bloque_out

	prelude 0
	ret

