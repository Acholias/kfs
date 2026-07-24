; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    ft_strlen.s                                        :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2026/07/24 21:10:22 by lumugot           #+#    #+#              ;
;    Updated: 2026/07/24 21:10:58 by lumugot          ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

; ft_strlen
; size_t	ft_strlen(const char *s)

section .text
	global	ft_strlen

ft_strlen:
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
