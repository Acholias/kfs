; strlen
; size_t	strlen(const char *s)

section .text
	global	strlen

strlen:
	push	ebp
	mov		ebp, esp
	mov		eax, 0
	mov		edi, [ebp + 8]

.loop:
	cmp		byte [edi + eax], 0
	je		.end
	inc		eax
	jmp		.loop

.end:
	pop		ebp
	ret
