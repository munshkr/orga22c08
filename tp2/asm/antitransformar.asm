; void antitransformar(float bloque_transformado[8][8], float DCT_trasp[8][8], char bloque[8][8], float temp[8][8]);

global antitransformar
extern trasponer

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;; MUL TIPLICAR ;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%macro pohrmonheioh2 3	;%1 ~ %2 = %3    (REGISTROS)	;la escribimos asi porque es soft
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


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;; FLO ATTOCHAR ;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%macro fc_cargar 1
	movups %1, [esi]
	lea esi, [esi + ebx]
	cvttps2dq %1, %1
%endmacro

%macro fc_step 0
	fc_cargar xmm0
	fc_cargar xmm1
	fc_cargar xmm2
	fc_cargar xmm3
	fc_cargar xmm4
	fc_cargar xmm5
	fc_cargar xmm6
	fc_cargar xmm7
	
	packssdw xmm0, xmm1
	packssdw xmm2, xmm3
	packssdw xmm4, xmm5
	packssdw xmm6, xmm7
	
	packuswb xmm0, xmm2		; en xmm0 tengo 16 valores en bytes
	movdqu [edi], xmm0
	lea edi, [edi + ebx]
	packuswb xmm4, xmm6		; en xmm4 tengo otros 16 valores en bytes
	movdqu [edi], xmm4
	lea edi, [edi + ebx]
%endmacro

%macro float_to_char 2		;%2 = float_to_char(%1)
	mov esi, %1
	mov edi, %2

	mov ebx, 16			; para avanzar las matrices
	
	; en cada fc_step transfiero 32 valores floats de una matriz a otra
	; convirtiéndolos a char al mismo tiempo...
	fc_step
	fc_step
%endmacro



%define p_bloque_in [ebp+8]
%define p_dct [ebp+12]
%define p_bloque_out [ebp+16]
%define p_MTemp [ebp+20]

antitransformar:
	push ebp
	mov ebp, esp
	push ebx
	push esi
	push edi
	
	mov eax, p_bloque_in
	push eax
	call trasponer
	add esp, 4
	
pri_mult2:
	pohrmonheioh2 p_dct, p_bloque_in, p_MTemp
seg_mult2:
	pohrmonheioh2 p_MTemp, p_dct, p_MTemp
	
	mov eax, p_MTemp
	push eax
	call trasponer
	add esp, 4
	
	float_to_char p_MTemp, p_bloque_out
	
	pop edi
	pop esi
	pop ebx
	pop ebp
	ret
