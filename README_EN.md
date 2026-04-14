# 🔧 Assembly Functions - KFS-1

[🇫🇷 Version française](README.md)

This document explains in detail the different assembly functions used in the KFS-1 project. These functions form the low-level backbone of the kernel and provide the interface between the bootloader, the hardware and the C code.

---

## 📋 Table of contents
1. [Bootloader (boot.asm)](#1-bootloader-bootasm)
2. [ft_strlen](#2-ft_strlen)
3. [GDT (gdt.s)](#3-gdt-gdts)
4. [Terminal keyboard shortcuts](#4-terminal-keyboard-shortcuts)
5. [Shell commands](#5-shell-commands)

---

## 1. Bootloader (boot.asm)

### 📌 Overview
The bootloader is the entry point of our kernel. It contains the **Multiboot header** compatible with GRUB and initializes the execution environment before transferring control to the C code.

### 🔍 Full code
```asm
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
    mov     esp, stack_top
    call    kernel_main
    cli
.hang:
    hlt
    jmp     .hang
```

### 📖 Detailed explanation

#### 32-bit mode
```asm
BITS 32
```
Our kernel runs in **32-bit protected mode**. GRUB already configures the CPU in this mode before handing over control.

#### Multiboot header
```asm
%define MAGIC      0x1BADB002           ; Mandatory Multiboot magic value
%define FLAGS      (ALIGN | MEMINFO)
%define CHECKSUM   -(MAGIC + FLAGS)
```

The Multiboot header allows GRUB to recognize our kernel. It must satisfy:
```text
MAGIC + FLAGS + CHECKSUM ≡ 0 (mod 2³²)
```

**Used flags:**
- `ALIGN (1 << 0)`: Ask GRUB to align modules on 4-byte boundaries
- `MEMINFO (1 << 1)`: GRUB provides information about available memory

#### Multiboot section
```asm
section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM
```

This section must be:
- Located in the **first 8 KB** of the binary file
- **Aligned on 4 bytes**

GRUB scans this area to validate the kernel.

#### Stack creation
```asm
section .bss
align 16
stack_bottom:
    resb 16384        ; Reserve 16 KB
stack_top:
```

The `.bss` section contains uninitialized data. We reserve **16 KB** for the stack.

**⚠️ Important:** The stack grows downwards in memory, so `ESP` must point to `stack_top`.

#### Entry point
```asm
_start:
    mov     esp, stack_top    ; Initialize the stack pointer
    call    kernel_main       ; Call the main C function
```

1. `ESP` is set to the top of the stack
2. We call `kernel_main()`, which contains the main kernel logic in C

#### Halt loop (fail-safe)
```asm
    cli                ; Disable interrupts
.hang:
    hlt                ; Halt the CPU
    jmp     .hang      ; Infinite loop
```

If `kernel_main()` returns (which should never happen), the CPU:
- Disables interrupts (`cli`)
- Enters a halted state (`hlt`)
- Stays locked in an infinite loop

This avoids executing invalid instructions.

---

## 2. ft_strlen

### 📌 Prototype
```c
size_t ft_strlen(const char *s);
```

### 🎯 Purpose
Computes the length of a string (number of characters before `\0`).

### 🔍 Full code
```asm
global  ft_strlen

ft_strlen:
    push    ebp                ; Save base pointer
    mov     ebp, esp           ; Create stack frame
    mov     eax, 0             ; Counter = 0
    mov     edi, [ebp + 8]     ; edi = pointer to the string

.loop:
    cmp     byte [edi + eax], 0  ; Compare with '\0'
    je      .end                 ; If '\0', stop
    inc     eax                  ; Counter++
    jmp     .loop                ; Continue

.end:
    pop     ebp                ; Restore ebp
    ret                        ; Return (eax contains the length)
```

### 📖 Detailed explanation

#### Setup and initialization
```asm
push    ebp
mov     ebp, esp
mov     eax, 0             ; Counter starts at 0
mov     edi, [ebp + 8]     ; edi = parameter 's'
```

`eax` is used as a counter and will hold the return value.

#### Counting loop
```asm
.loop:
    cmp     byte [edi + eax], 0  ; Compare the byte at index eax with 0
    je      .end                 ; If it's '\0', we are done
    inc     eax                  ; Otherwise, increment the counter
    jmp     .loop                ; And continue
```

**About `byte [edi + eax]`:**
- `edi` contains the base address of the string
- `eax` is the current index
- `byte` indicates we read 1 byte
- So we access `s[eax]`

C equivalent:
```c
size_t len = 0;
while (s[len] != '\0')
{
    len++;
}
return (len);
```

#### Return
```asm
.end:
    pop     ebp
    ret
```
The value in `eax` (the counter) is automatically returned.

### 💡 Usage
This function is used anywhere we need to know the length of a string:

```c
void terminal_write_string(const char *data)
{
    terminal_write(data, ft_strlen(data));
}
```

---

## 3. GDT (gdt.s)

### 📌 Overview
The **Global Descriptor Table (GDT)** describes the memory segments used by the CPU in protected mode.  
The `gdt.s` file does two important things:
- it defines the GDT table (kernel / user segments, code / data / stack);
- it provides the `gdt_flush` function, which loads this table into the GDTR register and updates the segment registers.

Without this step, the CPU would not know how to access memory correctly in protected mode.

### 🔍 Full code
```asm
BITS	32

global	gdt_flush
global gdt_start

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
```

### 📖 Detailed explanation

#### gdt_flush: loading the GDT
```asm
gdt_flush:
    mov		eax, [esp + 4]
    lgdt	[eax]
```
- The function receives, as a parameter on the stack, the address of a **GDT descriptor** (`gdt_descriptor`).
- We load it into `eax` and then call `lgdt` to load this address and the size of the GDT into the **GDTR** register.

```asm
    mov		ax, 0x10
    mov		ds, ax
    mov		es, ax
    mov		fs, ax
    mov		gs, ax
    mov		ss, ax
```
- Once the new GDT is loaded, we update all the data segment registers (`ds`, `es`, `fs`, `gs`, `ss`) with the selector **0x10**, which corresponds to the **kernel data segment** (`gdt_data`).

```asm
    jmp		0x08:.flush

.flush:
    ret
```
- `jmp 0x08:.flush` is a **far jump** that forces the CPU to reload the **CS** register with the selector **0x08** (the **kernel code segment**, `gdt_code`).
- The `.flush` label is just the point we jump to after this far jump; the final `ret` returns to the C code that called `gdt_flush`.

In short, `gdt_flush`:
1. Loads the GDT address into the CPU (`lgdt`).
2. Sets all segment registers to the proper kernel selectors.
3. Forces `CS` to be reloaded via a far jump.

#### GDT table definition
The `.gdt` section contains all GDT entries:

- `gdt_start`: **null entry** (mandatory, never used directly).
- `gdt_code`: **kernel code segment** (ring 0, executable and readable).
- `gdt_data`: **kernel data segment** (ring 0, read/write).
- `gdt_kernel_stack`: segment dedicated to the **kernel stack**.
- `gdt_user_code`: **user code segment** (ring 3).
- `gdt_user_data`: **user data segment** (ring 3).
- `gdt_user_stack`: segment dedicated to the **user stack**.

Each entry follows the standard x86 segment descriptor format:
- limits (`dw 0xffff`, `db 0x0`);
- base (here left at 0 for linear addressing);
- **access rights** byte (e.g. `10011010b` for kernel code);
- **flags** byte (e.g. `11001111b` to enable 4K granularity, 32-bit segments, etc.).

The comments at the bottom of `gdt.s` detail these bits:
- bit 7 of the access byte: descriptor present/valid;
- bits 6–5: privilege level (ring 0 to 3);
- bits 4–3: segment type (code / data);
- bit 2: conforming bit;
- bit 1: readable/writable flag;
- bit 0: accessed flag.

#### GDT descriptor
```asm
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start
```
- `dw`: size of the table (number of bytes between `gdt_start` and `gdt_end`, minus 1).
- `dd`: base address of the table.

This structure is exactly what is passed to `gdt_flush` and then to the `lgdt` instruction.

#### Usage in the kernel
At kernel startup:
- C code prepares a `gdt_descriptor` pointing to the table defined in `gdt.s`;
- then calls `gdt_flush` to switch cleanly to this GDT;
- then, the `gdt` shell command allows you to **inspect** the main entries of this table (see below).

---

## 4. Terminal keyboard shortcuts

These shortcuts make it easy to control the different virtual terminals and navigate inside them.

> `L-CTRL` / `L-SHIFT` mean **left key** (Left Control / Left Shift).

| Shortcut        | Action                                                      |
|-----------------|-------------------------------------------------------------|
| L-CTRL + TAB    | Switches the active terminal (cycle to the next terminal)  |
| L-SHIFT + TAB   | Toggles the **terminal mode** (normal mode / alternate)    |
| L-CTRL + DELETE | Clears the entire current terminal                          |
| L-CTRL + HOME   | Jumps to the very top of the terminal                      |
| L-CTRL + END    | Jumps to the very bottom of the terminal                   |
| L-CTRL + S      | Triggers a clean **shutdown** of the kernel                |

---

## 5. Shell commands

The built-in shell lets you run a few basic commands:

| Command    | Description                                                                  |
|-----------|------------------------------------------------------------------------------|
| `clear`   | Clears the contents of the current terminal                                  |
| `echo`    | Prints a message, with the `-n` option to avoid adding a final `\n`         |
| `shutdown`| Cleanly terminates the kernel (similar to a global `exit`)                   |
| `gdt`     | Prints a hex dump of the main GDT entries                                    |

---

Credits: **lumugot** · **aderouba**