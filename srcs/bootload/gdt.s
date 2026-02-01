; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    gdt_flush.s                                        :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2026/01/20 11:46:02 by lumugot           #+#    #+#              ;
;    Updated: 2026/01/20 11:50:59 by lumugot          ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

BITS	32

global	gdt_flush

gdt_flush:
	mov		eax, [esp + 4]
	lgdt	[eax]

	mov		ax, 0x10
	mov		ds, ax
	mov		es, ax
	mov		fs, ax
	mov		gs, ax
	mov		ss, ax

	jmp		0x08:.flush

.flush:
	ret


section .gdt
; GDT define
gdt_start: ; Section to define gdt start with only 0
	dw 0x0
	dw 0x0
	dw 0x0
	dw 0x0
gdt_code: ; Section for code (read, exec)
	dw 0xffff ; End limit
	dw 0x0 ; Start limit
	db 0x0 ; Middle limit (db for 8 bit, not 16)
	db 10011010b ; Ring 0, code, readable+executable
	db 11001111b ; Flag
	db 0x0
gdt_data: ; Section for data (read, write)
	dw 0xffff
	dw 0x0
	db 0x0
	db 10010010b ; Ring 0, data, writable
	db 11001111b
	db 0x0
gdt_kernel_stack: ; Section for data (read, write)
	dw 0xffff
	dw 0x0
	db 0x0
	db 10010010b ; Ring 0, data, writable
	db 11001111b
	db 0x0
gdt_user_code:
	dw 0xffff
	dw 0x0
	db 0x0
	db 11111010b ; Ring 3, code, readable+executable
	db 11001111b
	db 0x0
gdt_user_data:
	dw 0xffff
	dw 0x0
	db 0x0
	db 11110010b ; Ring 3, data, writable
	db 11001111b
	db 0x0
gdt_user_stack:
	dw 0xffff
	dw 0x0
	db 0x0
	db 11110010b ; Ring 3, data, writable
	db 11001111b
	db 0x0

gdt_end: ; Section to define gdt end

gdt_descriptor: ; Section to define how load the gdt
	dw gdt_end - gdt_start - 1
	dd gdt_start

; gdt rights : 76543210
; 	7  -> descriptor valid (must be 1)
; 	65 -> privilege ring
; 	43 -> segment type (10 = code, 00 = data)
; 	2  -> conforming ?
; 	1  -> readable
; 	0  -> accessed

; gdt flags : 76543210
; 	7    -> granularity (1 = limit of 4 KB unit)
; 	6    -> 1 = 32 bit segment
; 	5    -> 0 = not 64 bit
; 	4    -> 0 = available for os (not used here)
; 	3210 -> limit high (0xF)
