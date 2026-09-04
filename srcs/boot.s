; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    boot.s                                             :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2026/07/24 21:10:08 by lumugot           #+#    #+#              ;
;    Updated: 2026/07/24 21:10:10 by lumugot          ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

BITS 32

extern kernel_main

%define ALIGN      (1 << 0)
%define MEMINFO    (1 << 1)
%define FLAGS      (ALIGN | MEMINFO)
%define MAGIC      0x1BADB002
%define CHECKSUM   -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
global _start
_start:
    mov		esp, stack_top
    call	kernel_main
	cli

.hang:
    hlt
	jmp .hang
