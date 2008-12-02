; void generarDCT(float DCT[8][8]) {
; 	int i, j;
; 	float ci;
;
; 	for (i = 0 ; i < 8 ; i++) {
; 		for (j = 0 ; j < 8 ; j++) {
; 			if (i == 0) {
; 				ci = (float)sqrt(1.0/8.0);
; 			} else {
; 				ci = (float)sqrt(2.0/8.0);
; 			}
;
; 			DCT[i][j] = (float) (  ci * cos(((2 * j + 1) * i * M_PI) / 16.0)  );
; 		}
; 	}
; }

global generarDCT

section .text

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

%define p_dct [ebp+8]


generarDCT:
	prologue 0

;	calcula sqrt(1/8), sqrt(2/8) y 16
 	finit								;[]
 	fld1								;[1]
 	fadd st0							;[2]
 	fadd st0							;[4]
 	fadd st0							;[8]
 	fadd st0							;[16]

 	fld1								;[1, 16]
 	fld1								;[1, 1, 16]
 	fadd st0							;[2, 1, 16]
 	fadd st0							;[4, 1, 16]
 	fadd st0							;[8, 1, 16]
 	fdivp st1, st0						;[1/8, 16]
 	fsqrt								;[sqrt(1/8), 16]
 	fld1								;[1, sqrt(1/8), 16]
 	fadd st0							;[2, sqrt(1/8), 16]
 	fld st0								;[2, 2, sqrt(1/8), 16]
 	fadd st0							;[4, 2, sqrt(1/8), 16]
 	fadd st0							;[8, 2, sqrt(1/8), 16]
 	fdivp st1, st0						;[2/8, sqrt(1/8), 16]
 	fsqrt								;[sqrt(2/8), sqrt(1/8), 16]

	fldz								;[i, sqrt(2/8), sqrt(1/8), 16]
	fldz								;[j, i, sqrt(2/8), sqrt(1/8), 16]

	mov edi, p_dct
	xor eax, eax			; i = 0
	xor ebx, ebx			; j = 0

ciclo_i:
ciclo_j:
	fld1								;[1, j, i, sqrt(2/8), sqrt(1/8), 16]
	fadd st0, st0						;[2, j, i, sqrt(2/8), sqrt(1/8), 16]
	fmul st0, st1						;[2j, j, i, sqrt(2/8), sqrt(1/8), 16]
	fld1								;[1, 2j, j, i, sqrt(2/8), sqrt(1/8), 16]
	faddp st1, st0						;[2j+1, j, i, sqrt(2/8), sqrt(1/8), 16]
	fldpi								;[pi, 2*j+1, j, i, sqrt(2/8), sqrt(1/8), 16]
	fmul st0, st3						;[pi*i, 2*j+1, j, i, sqrt(2/8), sqrt(1/8), 16]
	fmulp st1, st0						;[(pi*i)*(2*j+1), j, i, sqrt(2/8), sqrt(1/8), 16]
	fdiv st0, st5						;[(pi*i)*(2*j+1)/16, j, i, sqrt(2/8), sqrt(1/8), 16]
	fcos								;[cos((pi*i)*(2*j+1)/16), j, i, sqrt(2/8), sqrt(1/8), 16]

	cmp eax, 0
	jne i_no_0
	fmul st0, st4						;[sqrt(1/8)*cos((pi*i)*(2*j+1)/16), j, i, sqrt(2/8), sqrt(1/8), 16]
	jmp almacenar

i_no_0:
	fmul st0, st3						;[sqrt(2/8)*cos((pi*i)*(2*j+1)/16), j, i, sqrt(2/8), sqrt(1/8), 16]

almacenar:
	fstp dword [edi]					;[j, i, sqrt(2/8), sqrt(1/8), 16]
	add edi, 4

	fld1								;[1, j, i, sqrt(2/8), sqrt(1/8), 16]
	faddp st1, st0						;[j+1, i, sqrt(2/8), sqrt(1/8), 16]
	inc ebx

	cmp ebx, 8
	jb ciclo_j

	fcomp st0, st1						;[i, sqrt(2/8), sqrt(1/8), 16]
	fld1								;[1, i, sqrt(2/8), sqrt(1/8), 16]
	faddp st1, st0						;[i+1, sqrt(2/8), sqrt(1/8), 16]
	fldz								;[0, i+1, sqrt(2/8), sqrt(1/8), 16]

	xor ebx, ebx
	inc eax
	cmp eax, 8
	jb ciclo_i

	prelude 0
	ret







