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
