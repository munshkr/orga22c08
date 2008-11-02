; int decodificar(char* buffer, long long int size, codificacion* tablaCodigos, short size_tablaCodigos, char** bitstream, int bsLen)

extern malloc

global decodificar

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
%define bf_size_l [ebp+12]
%define bf_size_h [ebp+16]
%define tc [ebp+20]
%define tc_size [ebp+24]
%define bitstream [ebp+28]
%define bsLen [ebp+32]

%define tmpbs [ebp-4]
%define contador [ebp-8]

%define assert_suma_bits [ebp-12]

%define o_simbolo 0
%define o_longCod 1
%define o_cod 4
%define tc_row_size 8

section .text

decodificar:
	prologue 12

	push dword bsLen
	call malloc
	add esp, 4

	mov ecx, bitstream
	mov [ecx], eax
	mov tmpbs, eax

mov dword assert_suma_bits, 0

	xor edx, edx
;								; uso bh para ver cuantos bits saque de eax
								; uso bl para ver en que indice del tc estoy
	xor eax, eax				; los 32 primeros bits del buffer
	xor ecx, ecx				; ecx = contador de bits sacados de eax
	xor edi, edi				; edi = tc

	mov ecx, bsLen
	mov contador, ecx
	mov ecx, 32					; para que de entrada cargue los primeros 4 bytes
	mov edi, tc

ciclo_un_byte_mas:
	xor ebx, ebx
	xor esi, esi
	cmp dword contador, 0		; si bytes restantes por escribir es cero, termine
	je fin_decodificar

ciclo_un_bit_mas:

	cmp ecx, 32
	jne puedo_sacar_bit

	mov eax, buffer
	mov eax, [eax]				; cargo el buffer si se me agoto
	bswap eax					; cambio el endianness

	xor ecx, ecx				; empiezo a contar desde cero la cantidad de bits

	add dword buffer, 4			; incrementamos el puntero al buffer codificado

puedo_sacar_bit:
	inc bh						; este es el largo del simbolo, se resetea

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
cmp bh, 18
ja assert_long_codigo
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	inc ecx						; lo uso para ver cuando recorri todo eax

	shl esi, 1
	shl eax, 1
	adc esi, 0					; traigo a esi el nuevo bit

ciclo_busco_codigo:
	mov edx, tc_size
	dec edx
	cmp bl, dl
	je pego_la_vuelta			; termine de recorrer la tabla y no encontre el codigo

	xor edx, edx
	mov dl, bl
	lea edx, [edi + edx * tc_row_size + o_cod]

	cmp esi, [edx]
	je comparo_longitud

	inc bl
	jmp ciclo_busco_codigo

comparo_longitud:
	xor edx, edx
	mov dl, bl
	lea edx, [edi + edx * tc_row_size + o_longCod]
	cmp bh, [edx]
	je meto_byte

	inc bl
	jmp ciclo_busco_codigo

pego_la_vuelta:
	mov bl, 0
	jmp ciclo_un_bit_mas	; vuelvo a recorrer la tabla con un bit más

meto_byte:
	add assert_suma_bits, bh
	xor edx, edx
	mov dl, bl
	mov edx, [edi + edx * tc_row_size + o_simbolo]

	mov esi, tmpbs
	mov [esi], dl

	inc dword tmpbs
	dec dword contador

	jmp ciclo_un_byte_mas

fin_decodificar:

	mov eax, bsLen

	prelude 12
	ret


assert_long_codigo:
	xor eax, eax;
	prelude 12
	ret