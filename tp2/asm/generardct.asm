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
 	fld1								;[1, 1]
 	fadd st0							;[2, 1]
 	fld st0								;[2, 2, 1]
 	fadd st0							;[4, 2, 1]
 	fadd st0							;[8, 2, 1]
 	fdiv st2, st0						;[8, 2, 1/8]
 	fdiv st1, st0						;[8, 2/8, 1/8]
 	fadd st0							;[16, 2/8, 1/8]
 	fld st2								;[1/8, 16, 2/8, 1/8]
 	fsqrt								;[sqrt(1/8), 16, 2/8, 1/8]
 	fld st2								;[2/8, sqrt(1/8), 16, 2/8, 1/8]
 	fsqrt								;[sqrt(2/8), sqrt(1/8), 16, ..]

	prelude 0
	ret
