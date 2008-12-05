; char* calcularApariciones(char* buffer, int size);
;
; int a[256] = {};
;
; int i = 0;
;
; while (i < size) {
;   a[(*buffer)]++;
;   i++;
;   buffer++;
; }
;

global calcularApariciones
extern malloc
; extern free

%define p_buffer [ebp+8]
%define bsize [ebp+12]

%define p_arr [ebp-4]

%macro inicio_cc 1
	push ebp
	mov ebp, esp
	sub esp, %1

	push ebx
	push edi
	push esi
%endmacro

%macro fin_cc 1
	pop esi
	pop edi
	pop ebx

	add esp, %1
	pop ebp
%endmacro

section .text

calcularApariciones:

	inicio_cc 4

	; reservo memoria para un arreglo de 256 ints
	lea eax, [256 * 4]
	push eax
	call malloc
	add esp, 4

	cmp eax, 0
	je fin

	mov p_arr, eax


	; inicializo el arreglo con 0s en cada elemento,
	; hasta alcanzar ecx = 0.
	mov edi, eax
	mov ecx, 256

	xor eax, eax
	xor ebx, ebx				; guarda el byte extraido del buffer
	cld
	rep stosd


	; restaura el ptr al arreglo y
	; coloco en registros el tamano del buffer y el ptr a este.
	mov edi, p_arr
	mov ecx, bsize
	shr ecx, 2
	mov esi, p_buffer


ciclo_apar:						; Ciclo dedicado a contar apariciones
	cmp ecx, 0
	je fin_ciclo_apar

	lodsd						; cargo en eax, pero de a byte
	mov bl, al
	inc dword [edi + ebx * 4]	; incremento en uno lo que haya en el indice al del array (al trae el valor de RGB 0-255)
	shr eax, 8
	mov bl, al
	inc dword [edi + ebx * 4]
	shr eax, 8
	mov bl, al
	inc dword [edi + ebx * 4]
	shr eax, 8
	mov bl, al
	inc dword [edi + ebx * 4]
	shr eax, 8

	loop ciclo_apar


fin_ciclo_apar:
	mov eax, p_arr


fin:
	fin_cc 4
	ret