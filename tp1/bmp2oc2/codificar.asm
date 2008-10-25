; long long int codificar (char* buffer, int buf_size, codificacion* tc, int tc_size, int* ta, char** bitstream)
;
; typedef struct {
;     char simbolo;
;     char longCod;
;     int cod;
; } codificacion;
;

extern malloc

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

%define buffer [ebp+8]
%define buf_size [ebp+12]
%define tc [ebp+16]
%define tc_size [ebp+20]
%define ta [ebp+24]
%define bitstream [ebp+28]

%define size_bitstream_h [ebp-4]		; tamano en bits del bitstream, parte alta
%define size_bitstream_l [ebp-8]		; tamano en bits del bitstream, parte baja
%define size_buf_bitstream [ebp-12]		; tamaño del buffer del bitstream: size_bitstream / 8 (+ 1) si tiene resto
%define contador_size_buf_bitstream [ebp-16]  ; contador para saber si llegue a almacenar todos los bytes del bitstream al buffer
%define tmpbs [ebp-20]  				; puntero a la proxima direccion del bitstream donde agregar 32 bits

%define o_simbolo 0
%define o_longCod 1
%define o_cod 4
%define tc_row_size 8

codificar:
	prologue 20

	; hay que recorrer la tabla de codigos, y para cada simbolo, obtener la frecuencia
	; de la tabla de apariciones, multiplicarla por la long. del codigo de ese simbolo,
	; y sumarlo en suma_longitud (int).

	mov esi, tc
	mov edi, ta
	mov ecx, tc_size
	xor edx, edx		; edx = suma_longitud

	mov dword size_bitstream_l, 0
	mov dword size_bitstream_h, 0

ciclo_long:
	xor ebx, ebx
	mov bl, [esi + o_simbolo]	; ebx = tc[i].simbolo
	mov eax, [edi + ebx * 4]		; eax = frecuencia de simbolo ebx

	mov bl, [esi + o_longCod]
	mul bl

	add size_bitstream_l, eax	; suma_longitud = suma_longitud + (frec(simbolo) * longCod(simbolo))
	adc dword size_bitstream_h, 0

	add esi, tc_row_size
	loop ciclo_long

fin_ciclo_long:
	mov eax, size_bitstream_l
	mov edx, size_bitstream_h

	mov ecx, 32
	div ecx
	cmp edx, 0
	je reservar_memoria_buf_bitstream
	inc eax

reservar_memoria_buf_bitstream:
	shl eax, 2
	mov size_buf_bitstream, eax

	push eax
	call malloc
	add esp, 4
	cmp eax, 0
	jne mem_OK

	xor edx, edx
	jmp fin_codificar

mem_OK:
	mov ecx, bitstream
	mov [ecx], eax

	mov ecx, [ecx]
	mov tmpbs, ecx						; tmpbs = ptr al buffer bitstream


	; recorrer el buffer del bmp y buscar el código del simbolo en la tabla de codigos,
	; ahi obtenemos el codigo y su long
	; vamos almacenando el codigo en ebx y cuando este se llena lo bajamos al bitstream.


	mov ecx, buf_size					; ecx = buf_size
	xor ebx, ebx						; ebx = registro donde vamos a guardar los codigos
	xor edx, edx						; edx = contador de shifts
	; cuando el contador de shifts llegue a 32, tengo que almacenar en bitstream y limpiar ebx.

ciclo_codificacion:
	push ecx
	push edx

	push dword tc_size
	push dword tc
	mov eax, buffer
	push dword [eax]
	call buscarCodigo
	add esp, 12

	mov edx, tc
	mov esi, [edx + eax * tc_row_size + o_longCod]			; esi = longCod
	and esi, 0xFF											; mascara por mugre de struct (longCod es char)
	mov edi, [edx + eax * tc_row_size + o_cod]				; edi = cod

	mov ecx, esi
	neg ecx
	add ecx, 32
	shl edi, cl					; corro el codigo a la parte mas significativa del registro

	pop edx
	pop ecx


	; --------------

	; en almacenar_ebx_y_limpiar: edx = size_buf_bitstream

colocar_bits:
	cmp edx, 32
	je almacenar_ebx_y_limpiar

coloco_bit:
	cmp esi, 0
	je proximo_simbolo

	shl ebx, 1							; hago lugar en ebx para el bit
	inc edx
	shl edi, 1							; tomo un bit del código, me va a quedar en CARRY
	adc ebx, 0							; sumo el bit de carry a ebx
	;rcr ebx, 1
	;inc edx
	dec esi								; decremento la longitud del codigo pq ya agregue un bit
	jmp colocar_bits

proximo_simbolo:
	inc dword buffer
	loop ciclo_codificacion
	cmp edx, 0							; si quedaron bits, los guardo
	jne almacenar_ultimos_bits
	jmp fin_ciclo_codificacion

almacenar_ebx_y_limpiar:

	; tengo que almacenar cada byte de ebx en bitstream, teniendo en cuenta de no meter bytes de mas
	; en el buffer de bitstream. Cuando pase esto, termina ciclo_codificacion directamente (hacer pop edx!).
	mov eax, tmpbs					; eax = tmpbs
	;mov eax, [eax]

	bswap ebx
	mov [eax], ebx
	add dword tmpbs, 4

; 	push edx
; 	mov edx, contador_size_buf_bitstream
;
; 	mov [eax], bl
; 	cmp edx, size_buf_bitstream
; 	je fin_ciclo_codificacion_2
; 	inc edx
;
; 	mov [eax + 1], bh
; 	cmp edx, size_buf_bitstream
; 	je fin_ciclo_codificacion_2
; 	inc edx
;
; 	shr ebx, 16
;
; 	mov [eax + 2], bl
; 	cmp edx, size_buf_bitstream
; 	je fin_ciclo_codificacion_2
; 	inc edx
;
; 	mov [eax + 3], bh
; 	cmp edx, size_buf_bitstream
; 	je fin_ciclo_codificacion_2
; 	inc edx
;
; 	mov contador_size_buf_bitstream, edx
;
; 	add dword tmpbs, 4
;
; 	pop edx

	xor edx, edx
	xor ebx, ebx
	jmp colocar_bits

	; --------------

almacenar_ultimos_bits:
 	mov ecx, edx				; edx es la cantidad de bits validos en la parte menos significativa de ebx
 	neg ecx
	add ecx, 32					; comentado, porque nos olvidamos que se guarda en little-endian
	shl ebx, cl

	bswap ebx
	mov eax, tmpbs				; si en ebx quedaron bits de codigo, los almaceno
	mov [eax], ebx

fin_ciclo_codificacion:
	mov eax, size_bitstream_l
	mov edx, size_bitstream_h

fin_codificar:
	prelude 20
	ret





; int buscarCodigo(char simbolo, codificacion* tc, int tc_size)
; devuelve el indice en la tabla de codigos que corresponde al simbolo pasado por parametro
; si el simbolo no esta, devuelve -1.

%define bc_simbolo [ebp+8]
%define bc_tc [ebp+12]
%define bc_tc_size [ebp+16]

%define o_bc_simbolo 0

buscarCodigo:
	prologue 0

	mov ebx, bc_simbolo
	mov esi, bc_tc
	mov edx, bc_tc_size

	xor ecx, ecx			; contador indice

ciclo_tabla:
	cmp ecx, edx			; si ya llegue al final
	je no_encontro			; el simbolo no esta

	cmp bl, [esi + o_bc_simbolo]
	je encontro

	inc ecx
	add esi, tc_row_size
	jmp ciclo_tabla

encontro:
	mov eax, ecx
	jmp fin_buscarCodigo

no_encontro:
	mov eax, -1

fin_buscarCodigo:
	prelude 0
	ret
