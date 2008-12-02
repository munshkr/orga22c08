;int decodificar(short bloque[8][8], short* stream)
global decodificar

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
	add ebx, edi	;ebx apunta a la pos de la matriz
	cmp cx, 0
	jne hay_ceros_%1
	mov ax, [esi]	; en eax tengo el numero a guardar
	times 2 inc esi	; muevo puntero
	mov [ebx], ax	; guardo el numero
	mov cx, [esi]	; cargo nueva cantidad de ceros
	times 2 inc esi	; muevo el puntero
	times 2 inc edx ; se adelantan dos elementos en el stream
	jmp fin_analisis_%1
hay_ceros_%1:
	xor eax, eax
	mov [ebx], ax	; guardo un cero
	dec cx			; decremento los ceros que faltan
fin_analisis_%1:
%endmacro

%define stream [ebp+8]
%define bloque [ebp+12]

decodificar:
	prologue 0
	;inicializo los registros
	mov esi, stream
	mov edi, bloque
	xor edx, edx		; contador de cuantos movi el stream
	xor ecx, ecx		; contador de ceros

	mov ebx, [esi]		; cargo en ebx el primer valor del stream
	mov [edi], bx		; lo guardo en la matriz porque va de una
	times 2 inc esi		; muevo ptr del stream al primer cantidad de ceros
	mov cx, [esi]		; cargo la primer cantidad de ceros
	times 2 inc esi		; adelanto el ptr del stream
	times 2 inc edx		; se adelantan dos posiciones en el stream

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
; ###### TERMINA LOOP UNROLLING
	;// El ultimo lo calculo aparte para que no me de un movimiento de más
	mov ebx, dword 126
	add ebx, edi	;ebx apunta a la pos de la matriz
	cmp cx, 0
	jne hay_ceros
	mov ax, [esi]	; en eax tengo el numero a guardar
	times 2 inc esi	; muevo puntero
	mov [ebx], ax	; guardo el numero
	inc edx 		; se adelanta el stream
	jmp fin_analisis
hay_ceros:
	xor eax, eax
	mov [ebx], ax	; guardo un cero
	inc edx 		; se adelanta el stream, porque si el ultimo es un cero
					; está guardado en la matriz como un cero. Como yo los
					; ceros no los leo de la memoria, sino que los fabrico,
					; tengo que "hacer como" que lei una vez más de memoria
					; para que el valor de retorno no de mal.
fin_analisis:
	times 2 inc edx	; me como los dos ceros del final del stream
	mov eax, edx
	prelude 0
	ret