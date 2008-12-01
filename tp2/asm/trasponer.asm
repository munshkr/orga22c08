; void trasponer(float m[8][8]);

global trasponer


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;; TRA SPONER ;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	%macro tra_step 0
		movss xmm4, xmm0
		pslldq xmm4, 4
		psrldq xmm0, 4
		movss xmm5, xmm0
		pslldq xmm5, 4
		psrldq xmm0, 4
		movss xmm6, xmm0
		pslldq xmm6, 4
		psrldq xmm0, 4
		movss xmm7, xmm0
		pslldq xmm7, 4

		movss xmm4, xmm1
		pslldq xmm4, 4
		psrldq xmm1, 4
		movss xmm5, xmm1
		pslldq xmm5, 4
		psrldq xmm1, 4
		movss xmm6, xmm1
		pslldq xmm6, 4
		psrldq xmm1, 4
		movss xmm7, xmm1
		pslldq xmm7, 4

		movss xmm4, xmm2
		pslldq xmm4, 4
		psrldq xmm2, 4
		movss xmm5, xmm2
		pslldq xmm5, 4
		psrldq xmm2, 4
		movss xmm6, xmm2
		pslldq xmm6, 4
		psrldq xmm2, 4
		movss xmm7, xmm2
		pslldq xmm7, 4

		movss xmm4, xmm3
		psrldq xmm3, 4
		movss xmm5, xmm3
		psrldq xmm3, 4
		movss xmm6, xmm3
		psrldq xmm3, 4
		movss xmm7, xmm3

		shufps xmm4, xmm4, 00011011b
		shufps xmm5, xmm5, 00011011b
		shufps xmm6, xmm6, 00011011b
		shufps xmm7, xmm7, 00011011b
	%endmacro

	%macro tra_tomar 0
		movups xmm0, [ebx]
		add ebx, eax
		movups xmm1, [ebx]
		add ebx, eax
		movups xmm2, [ebx]
		add ebx, eax
		movups xmm3, [ebx]
	%endmacro

	%macro tra_almacenar 0
		movups [ebx], xmm4
		add ebx, eax
		movups [ebx], xmm5
		add ebx, eax
		movups [ebx], xmm6
		add ebx, eax
		movups [ebx], xmm7
	%endmacro


;|||||||||||||||||||||||||||||
;|||||||| TRA SPONER |||||||||
;|||||||||||||||||||||||||||||



%define p_matriz_float [ebp+8]

trasponer:
	push ebp
	mov ebp, esp
	push ebx
	push esi
	push edi


	mov esi, p_matriz_float
	xor eax, eax
	times 8 inc eax
	times 2 shl eax, 1

; Tomo 1er Cuadrante
	mov ebx, esi
	tra_tomar

; Traspongo
	tra_step

; Almaceno en 1er Cuadrante
	mov ebx, esi
	tra_almacenar

; Preparo puntero al 2do Cuadrante
	mov ebx, esi
	xor edx, edx
	times 4 inc edx
	times 2 shl edx, 1
	add ebx, edx

; Tomo 2do Cuadrante
	tra_tomar

; Traspongo
	tra_step

; Preparo puntero al 3er Cuadrante
	add ebx, edx

; Tomo 3er Cuadrante
	tra_tomar

; Almaceno 2do en 3er Cuadrante
	times 3 sub ebx, eax
	tra_almacenar

; Traspongo
	tra_step

; Preparo puntero al 2do Cuadrante
	mov ebx, esi
	add ebx, edx

; Almaceno
	tra_almacenar

; Preparo puntero al 4to Cuadrante
	add ebx, eax

; Tomo 4to Cuadrante
	tra_tomar

; Traspongo
	tra_step

; Almaceno 4to Cuadrante
	times 3 sub ebx, eax
	tra_almacenar


	pop edi
	pop esi
	pop ebx
	pop ebp
	ret
