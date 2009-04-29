;*******************************************************************************
;Programa que imprime Hola Mundo en assembler, escribiendo la RAM de video
;*******************************************************************************
USE16
ORG	0x7C00
	jmp	start

	mens:	db	'Hola Mundo!!. Booteable, escrito en assembler y con el VI. Solo para hombres de verdad...:-D',10
	longitud equ	$-mens

start:
;-------------------------------------------------------------------------------
;	jmp	$
	mov	cx,longitud	;Cantidad de bytes a presentar. 
	mov	si,mens		;ds:si -> string a presentar
	cld			; los punteros si y di se autoincrementan
	xor	bx,bx
	mov	ah,0x0E		; Función de escribir en modo teletipo
otro:
	lodsb			; Lee caracter
	int	0x10		; Llama función BIOS
	loop	otro

fin:	hlt			;fin......
	jmp fin

TIMES	510 - ($ - $$)	db	0;	Pone ceros desde el final del código hasta dos bytes antes del final del sector
dw	0xAA55
