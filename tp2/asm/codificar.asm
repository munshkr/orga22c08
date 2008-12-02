;void codificar(short * bloque_codificado, short bloque_cuantizado[8][8])
global codificar

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

%macro analizar_pos 1
	mov ebx, dword %1
	add ebx, esi	;en esi tengo el puntero al bloque
	mov bx, [ebx]	;son de shorts!
	and ebx, 0x00FF
	cmp bx, 0
	je falta_%1
	mov [edi],  cx	; guardo la cantidad de ceros
	times 2 inc edi	; muevo puntero
	inc eax
	mov [edi], bx	; guardo el numero
	times 2 inc edi	; incremento el puntero
	inc eax
	xor ecx, ecx	; limpio el contador de ceros
	jmp fin_analisis_%1
falta_%1:
	inc cx
fin_analisis_%1:
%endmacro

%define bloque [ebp+8]
%define stream [ebp+12]

codificar:
	prologue 0
	;inicializo los registros
	mov edi, stream
	mov esi, bloque
	xor eax, eax

	mov bx, [esi] 		;cargo en ebx el primer valor de la matriz
	xor ecx, ecx		; inicializo el contador en cero
	mov [edi], bx
	times 2 inc edi
	inc eax
; ###### EMPIEZA LOOP UNROLLING
	analizar_pos 2
	analizar_pos 16
	analizar_pos 32
	analizar_pos 18
	analizar_pos 4
	analizar_pos 6
	analizar_pos 20
	analizar_pos 34
	analizar_pos 48
	analizar_pos 64
	analizar_pos 50
	analizar_pos 36
	analizar_pos 22
	analizar_pos 8
	analizar_pos 10
	analizar_pos 24
	analizar_pos 38
	analizar_pos 52
	analizar_pos 66
	analizar_pos 80
	analizar_pos 96
	analizar_pos 82
	analizar_pos 68
	analizar_pos 54
	analizar_pos 40
	analizar_pos 26
	analizar_pos 12
	analizar_pos 14
	analizar_pos 28
	analizar_pos 42
	analizar_pos 56
	analizar_pos 70
	analizar_pos 84
	analizar_pos 98
	analizar_pos 112
	analizar_pos 114
	analizar_pos 100
	analizar_pos 86
	analizar_pos 72
	analizar_pos 58
	analizar_pos 44
	analizar_pos 30
	analizar_pos 46
	analizar_pos 60
	analizar_pos 74
	analizar_pos 88
	analizar_pos 102
	analizar_pos 116
	analizar_pos 118
	analizar_pos 104
	analizar_pos 90
	analizar_pos 76
	analizar_pos 62
	analizar_pos 78
	analizar_pos 92
	analizar_pos 106
	analizar_pos 120
	analizar_pos 122
	analizar_pos 108
	analizar_pos 94
	analizar_pos 110
	analizar_pos 124
	analizar_pos 126
; ###### TERMINA LOOP UNROLLING
	;si el ultimo numero es cero lo debo considerar aparte
	cmp bx, 0
	jne ultimo_no_cero
	mov [edi],  cx	; guardo la cantidad de ceros
	times 2 inc edi	; muevo puntero
	inc eax
	mov [edi], bx	; guardo el numero
	times 2 inc edi	; muevo puntero
	inc eax
ultimo_no_cero:
	xor ecx, ecx
	mov [edi],  cx	; guardo un cero
	times 2 inc edi	; muevo puntero
	inc eax
	mov [edi], cx	; guardo el otro cero
	times 2 inc edi	; muevo puntero
	inc eax
	prelude 0
	ret