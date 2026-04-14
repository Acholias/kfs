# 🔧 Fonctions Assembleur - KFS-1

Ce document explique en détail les différentes fonctions assembleur utilisées dans le projet KFS-1. Ces fonctions constituent la base bas-niveau du kernel et permettent l'interface entre le bootloader, le matériel et le code C.

---

## 📋 Table des matières
1. [Bootloader (boot.asm)](#1-bootloader-bootasm)
2. [ft_strlen](#3-ft_strlen)

---

## 1. Bootloader (boot.asm)

### 📌 Vue d'ensemble
Le bootloader est le point d'entrée de notre kernel. Il contient le **header Multiboot** compatible avec GRUB et initialise l'environnement d'exécution avant de transférer le contrôle au code C.

### 🔍 Code complet
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

### 📖 Explication détaillée

#### Mode 32 bits
```asm
BITS 32
```
Notre kernel s'exécute en **mode protégé 32 bits**. GRUB configure déjà le CPU dans ce mode avant de transférer le contrôle.

#### Header Multiboot
```asm
%define MAGIC      0x1BADB002           ; Signature Multiboot obligatoire
%define FLAGS      (ALIGN | MEMINFO)
%define CHECKSUM   -(MAGIC + FLAGS)
```

Le header Multiboot permet à GRUB de reconnaître notre kernel. Il doit satisfaire :
```
MAGIC + FLAGS + CHECKSUM ≡ 0 (mod 2³²)
```

**Flags utilisés :**
- `ALIGN (1 << 0)` : Demande l'alignement des modules sur 4 octets
- `MEMINFO (1 << 1)` : GRUB fournit les informations sur la mémoire disponible

#### Section Multiboot
```asm
section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM
```

Cette section doit être :
- Placée dans les **8 premiers Ko** du fichier binaire
- **Alignée sur 4 octets**

GRUB scanne cette zone pour valider le kernel.

#### Création de la pile (stack)
```asm
section .bss
align 16
stack_bottom:
    resb 16384        ; Réserve 16 KB
stack_top:
```

La section `.bss` contient les données non initialisées. Nous réservons **16 KB** pour la pile.

**⚠️ Important :** La pile descend en mémoire, donc `ESP` pointe vers `stack_top`.

#### Point d'entrée
```asm
_start:
    mov     esp, stack_top    ; Initialise le pointeur de pile
    call    kernel_main       ; Appelle la fonction C principale
```

1. `ESP` est positionné au sommet de la pile
2. On appelle `kernel_main()` qui contient la logique du noyau en C

#### Boucle d'arrêt (fail-safe)
```asm
    cli                ; Désactive les interruptions
.hang:
    hlt                ; Met le CPU en pause
    jmp     .hang      ; Boucle infinie pour le CPU
```

Si `kernel_main()` retourne (ce qui ne devrait jamais arriver), le CPU :
- Désactive les interruptions (`cli`)
- Se met en pause (`hlt`)
- Reste bloqué dans une boucle infinie

Cela évite l'exécution d'instructions invalides.

---


## 2. ft_strlen

### 📌 Prototype
```c
size_t ft_strlen(const char *s);
```

### 🎯 Fonction
Calcule la longueur d'une chaîne de caractères (nombre de caractères avant `\0`).

### 🔍 Code complet
```asm
global  ft_strlen

ft_strlen:
    push    ebp                ; Sauvegarde le base pointer
    mov     ebp, esp           ; Établit le stack frame
    mov     eax, 0             ; Compteur = 0
    mov     edi, [ebp + 8]     ; edi = pointeur sur la chaîne

.loop:
    cmp     byte [edi + eax], 0  ; Compare avec '\0'
    je      .end                 ; Si '\0', termine
    inc     eax                  ; Compteur++
    jmp     .loop                ; Continue

.end:
    pop     ebp                ; Restaure ebp
    ret                        ; Retourne (eax contient la longueur)
```

### 📖 Explication détaillée

#### Setup et initialisation
```asm
push    ebp
mov     ebp, esp
mov     eax, 0             ; Le compteur commence à 0
mov     edi, [ebp + 8]     ; edi = paramètre 's'
```

`eax` servira de compteur et contiendra la valeur de retour.

#### Boucle de comptage
```asm
.loop:
    cmp     byte [edi + eax], 0  ; Compare l'octet à l'index eax avec 0
    je      .end                 ; Si c'est '\0', on a fini
    inc     eax                  ; Sinon, on incrémente le compteur
    jmp     .loop                ; Et on continue
```

**Détail de `byte [edi + eax]` :**
- `edi` contient l'adresse de base de la chaîne
- `eax` est l'index courant
- `byte` indique qu'on lit 1 octet
- On accède donc à `s[eax]`

Équivalent C :
```c
size_t len = 0;
while (s[len] != '\0') {
    len++;
}
return len;
```

#### Retour
```asm
.end:
    pop     ebp
    ret
```
La valeur dans `eax` (le compteur) est automatiquement retournée.

### 💡 Utilisation
Cette fonction est utilisée partout où on a besoin de connaître la longueur d'une chaîne :
```c
void terminal_write_string(const char *data)
{
    terminal_write(data, ft_strlen(data));
}

## Control
key combinaison | action                  |
----------------|-------------------------|
L-CTRL + TAB    | Switch terminal         |
L-CTRL + HOME   | Go to start of terminal |
L-CTRL + END    | Go to end of terminal   |
L-CTRL + DELETE | Clear terminal          |
L-CTRL + S      | Shutdown                |
L-SHIFT + TAB   | Switch terminal mode    |


## Commands
command | action                    |
--------|---------------------------|
clear   | Clear terminal            |
echo    | Basic echo with -n option |
