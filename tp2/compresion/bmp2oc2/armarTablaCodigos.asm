; typedef struct {
; 	short simbolo;
; 	unsigned int apariciones;
; 	void* izquierda;
; 	void* derecha;
; 	void* parent;
; } nodoAB;
;
; typedef struct {
; 	nodoLista* primero;
; 	unsigned int cantidad;
; } Lista;
;
; typedef struct {
; 	nodoAB* elem;
; 	void* prox;
; } nodoLista;
;
; typedef struct {
; 	char simbolo;
; 	char longCod;
; 	int cod;
; } codificacion;
;
;
; void* armarTablaCodigos(int* tablaApariciones) {
; 	oLista = new Lista();
; 	oLista->primero = NULL;
; 	pLista = new Lista();
; 	pLista->primero = NULL;
;
; 	k = 0;													// ciclo_a
; 	while (k < 256) {
; 		if (tablaApariciones[k] > 0) {
; 			nNodo = new nodoAB();
; 			nNodo->simbolo = k;
; 			nNodo->apariciones = tablaApariciones[k];
; 			nNodo->izquierda = NULL;
; 			nNodo->derecha = NULL;
; 			nNodo->parent = NULL;
;
; 			insertarOrdenado(nNodo, oLista);
; 			insertarOrdenado(nNodo, pLista);
; 		}
; 	}
;
; 	while (oLista->cantidad > 1) {							// ciclo_b
; 		i = getPrimero(oLista)->elem;
; 		d = getPrimero(oLista)->elem;
;
; 		nNodo = new nodoAB();
; 		nNodo->simbolo = -1;
; 		nNodo->apariciones = i->apariciones + d->apariciones;
; 		nNodo->izquierda = i;
; 		nNodo->derecha = d;
; 		nNodo->parent = NULL;
;
; 		insertarOrdenado(nNodo, oLista);
; 	}
;
; 	void* abHuffman = getPrimero(oLista)->elem;
; 	codificacion* tc[pLista->cantidad];
;
; 	k = 0;
; 	while (pLista->cantidad > 0) {							// ciclo_c
; 		leaf = getPrimero(pLista)->elem;
; 		int cod = obtenerCodigo(leaf);
;
; 		tc[k]->simbolo = leaf->simbolo;
; 		tc[k]->longCod = longCod;
; 		tc[k]->cod = cod;
; 		k++;
; 	}
;
; 	free(pLista);
; 	free(oLista);
; 	liberar(abHuffman);		// hay que ver
; }

; int size_tablaCodigos = armarTablaCodigos(tablaApariciones, tablaCodigos);

; NOTA: tuvimos que escribir varios saltos "sin_memoria" porque "je fin_armarTablaCodigos" nos provocaba
; el error "short jump is out of range".


extern malloc
extern free

global armarTablaCodigos

%define arr_apariciones [ebp+8]
%define arr_codigos [ebp+12]
%define p_oLista [ebp-4]
%define p_pLista [ebp-8]
%define p_abHuffman [ebp-12]

%define o_simbolo 0
%define o_apariciones 4
%define o_izquierda 8
%define o_derecha 12
%define o_parent 16
%define o_primero 0
%define o_cantidad 4
%define o_elem 0
%define o_prox 4

%define o_simbolo_codif 0
%define o_longCod 1
%define o_cod 4

%define NULL 0

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

armarTablaCodigos:
	inicio_cc 12

	push dword 8
	call malloc
	add esp, 4
	cmp eax, 0
	je sin_memoria_1

	mov p_oLista, eax
	mov dword [eax + o_primero], NULL
	mov dword [eax + o_cantidad], NULL

	push dword 8
	call malloc
	add esp, 4
	cmp eax, 0
	je sin_memoria_1

	mov p_pLista, eax
	mov dword [eax + o_primero], NULL
	mov dword [eax + o_cantidad], NULL

	mov edi, arr_apariciones
	xor ecx, ecx
	jmp ciclo_a

sin_memoria_1:
	jmp fin_armarTablaCodigos

ciclo_a:						; Ciclo dedicado a contar cuantos aparecen por lo menos una vez
	cmp ecx, 256
	je fin_ciclo_a

	cmp dword [edi + ecx * 4], 0
	je sigo_a


	push ecx

	push dword 20
	call malloc
	add esp, 4
	cmp eax, 0
	je sin_memoria_2

	pop ecx


	mov [eax + o_simbolo], ecx
	mov ebx, [edi + ecx * 4]
	mov [eax + o_apariciones], ebx
	mov dword [eax + o_izquierda], NULL
	mov dword [eax + o_derecha], NULL
	mov dword [eax + o_parent], NULL


	mov esi, eax
	push ecx

	push dword p_oLista
	push esi
	call insertarOrdenado
	add esp, 8

	push dword p_pLista
	push esi
	call insertarOrdenado
	add esp, 8

	pop ecx
	jmp sigo_a

sin_memoria_2:
	jmp fin_armarTablaCodigos

sigo_a:
	inc ecx
	jmp ciclo_a


fin_ciclo_a:

	mov ebx, p_oLista

ciclo_b:
	cmp dword [ebx + o_cantidad], 1
	jbe fin_ciclo_b

	push ebx
	call getPrimero
	add esp, 4
	mov esi, [eax + o_elem]		; esi es izquierdo

	push ebx
	call getPrimero
	add esp, 4
	mov edi, [eax + o_elem]		; edi es derecho

	push dword 20
	call malloc
	add esp, 4
	cmp eax, 0
	je sin_memoria_3

	;	asigno parent a i y d
		mov [esi + o_parent], eax
		mov [edi + o_parent], eax
	;	---------------------

	mov dword [eax + o_simbolo], -1
	mov edx, [esi + o_apariciones]
	add edx, [edi + o_apariciones]
	mov [eax + o_apariciones], edx
	mov [eax + o_izquierda], esi
	mov [eax + o_derecha], edi
	mov dword [eax + o_parent], NULL

	push ebx
	push eax
	call insertarOrdenado
	add esp, 8

	jmp ciclo_b

sin_memoria_3:
	jmp fin_armarTablaCodigos

fin_ciclo_b:

; 	void* abHuffman = getPrimero(oLista)->elem;
; 	codificacion* tc[pLista->cantidad];

	push ebx
	call getPrimero
	add esp, 4
	mov eax, [eax + o_elem]
	mov p_abHuffman, eax

	mov eax, p_pLista
	mov eax, [eax + o_cantidad]

		mov esi, eax

	mov ecx, 8
	mul ecx

	push eax
	call malloc
	add esp, 4
	cmp eax, 0
	je sin_memoria_4

		push ecx
		mov ecx, arr_codigos
		mov [ecx], eax
		pop ecx

	;mov arr_codigos, eax

		push eax
		mov ecx, esi
		mov edi, eax
		xor eax, eax
		cld
		rep stosb
		pop eax

	mov esi, p_pLista			; esi = pLista

	mov edi, eax				; edi = arr_codigos
	xor ebx, ebx				; ebx = k

; 	k = 0;
; 	while (pLista->cantidad > 0) {							// ciclo_c
; 		leaf = getPrimero(pLista)->elem;
; 		int cod = obtenerCodigo(leaf);
;
; 		tc[k]->simbolo = leaf->simbolo;
; 		tc[k]->longCod = longCod;
; 		tc[k]->cod = cod;
; 		k++;
; 	}
	jmp ciclo_c

sin_memoria_4:
	jmp fin_armarTablaCodigos

ciclo_c:
	cmp dword [esi + o_cantidad], 0
	je sin_memoria_5

	push esi
	call getPrimero
	add esp, 4
	mov edx, [eax + o_elem]		; edx = leaf

	push esi
	push edx					; porque mul lo borra

	mov ecx, [edx + o_simbolo]
	mov eax, ebx
	mov esi, 8
	mul esi
	add eax, o_simbolo_codif
	mov [edi + eax], ecx		; hasta aca, guarde simbolo, faltan longCod y cod en arr_codigos

	pop edx

	xor eax, eax				; eax = codigo
	xor esi, esi				; esi = longCod (durante ciclo_codigo, antes era pLista)
	jmp ciclo_codigo

sin_memoria_5:
	jmp fin_ciclo_c

ciclo_codigo:
	mov ecx, [edx + o_parent]
	cmp ecx, NULL
	je fin_ciclo_codigo

	inc esi
	shl eax, 1

	mov ecx, [ecx + o_izquierda]
	cmp edx, ecx

	je sigo_ciclo_codigo
	inc eax

sigo_ciclo_codigo:
	mov edx, [edx + o_parent]
	cmp edx, NULL
	jne ciclo_codigo
	;jmp ciclo_codigo

fin_ciclo_codigo:

	mov ecx, eax			; ecx = codigo

	mov eax, ebx
	mov edx, 8
	mul edx
	add eax, o_longCod
	mov [edi + eax], esi	; almacena en el indice k del array de codigos la longCod


	push eax

	push ecx
	call reverse
	add esp, 4
	mov ecx, eax

	pop eax


	neg esi
	add esi, 32
	xchg ecx, esi
	shr esi, cl
	xchg ecx, esi

	mov eax, ebx
	mov edx, 8
	mul edx
	add eax, o_cod
	mov [edi + eax], ecx	; almacena el codigo en el indice k

	pop esi

	inc ebx
	jmp ciclo_c

fin_ciclo_c:

	push dword p_oLista
	call free
	add esp, 4
	push dword p_pLista
	call free
	add esp, 4

; 	push dword p_abHuffman
; 	call liberarAB
; 	add esp, 4

	mov eax, ebx

fin_armarTablaCodigos:
	fin_cc 12
	ret




%define nNodo [ebp+8]
%define xLista [ebp+12]

insertarOrdenado:
	inicio_cc 0

	mov esi, xLista
	mov edi, esi
	lea esi, [esi + o_primero]

ciclo_io:
	cmp dword [esi], NULL
	je agregar_nodo

	mov eax, nNodo
	mov eax, [eax + o_apariciones]	; eax = apariciones de nNodo
	mov edx, [esi]
	mov ebx, [edx + o_elem]
	cmp eax, [ebx + o_apariciones]
	jb agregar_nodo
sigue_io:
	mov esi, [esi]
	lea esi, [esi + o_prox]
	jmp ciclo_io

agregar_nodo:
	; agregas el nodo, mas vale
	push dword 8
	call malloc
	add esp, 4
	cmp eax, 0
	je fin_ciclo_io

	mov edx, nNodo
	mov [eax + o_elem], edx
	mov edx, [esi]
	mov [eax + o_prox], edx
	mov [esi], eax

	inc dword [edi + o_cantidad]

fin_ciclo_io:
	fin_cc 0
	ret


%define lista_getPrimero [ebp+8]

getPrimero:
	inicio_cc 0
	xor ecx, ecx
	inc ecx

	mov esi, lista_getPrimero
	dec dword [esi + o_cantidad]

	lea esi, [esi + o_primero]

	mov edi, [esi]
	mov eax, edi

	mov ebx, [edi + o_prox]
	mov [esi], ebx

	fin_cc 0
	ret

liberarAB:
	inicio_cc 0
	fin_cc 0
	ret


%define param [ebp+8]
reverse:
	inicio_cc 0
	mov ebx, param
	mov ecx, 32

ciclo_reverse:
	rcl ebx, 1
	rcr eax, 1
	loop ciclo_reverse

	fin_cc 0
	ret
