; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    ft_memcpy.s                                        :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2026/07/24 21:10:12 by lumugot           #+#    #+#              ;
;    Updated: 2026/07/24 21:10:14 by lumugot          ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

; ft_memcpy.s
; void	*ft_memcpy(void *dest, const void *src, size_t n)

section .text
	global ft_memcpy

ft_memcpy:
	push	ebp
	mov		ebp, esp
	push	esi
	push	edi

	mov		edi, [ebp + 8]
	mov		esi, [ebp + 12]
	mov		ecx, [ebp + 16]
	
	mov		eax, edi
	cmp		ecx, 0
	je		.end

.loop:
	mov		bl, [esi]
	mov		[edi], bl
	inc		esi
	inc		edi
	dec		ecx
	jnz		.loop

.end:
	pop		edi
	pop		esi
	pop		ebp
	ret
