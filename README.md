# 🔧 Fonctions Assembleur - KFS-1

[🇬🇧 English version](README_EN.md)

Ce document explique en détail les différentes fonctions assembleur utilisées dans le projet KFS-1. Ces fonctions constituent la base bas-niveau du kernel et permettent l'interface entre le bootloader, le matériel et le code C.

---

## 📋 Table des matières
1. [Bootloader (boot.asm)](#1-bootloader-bootasm)
2. [ft_strlen](#2-ft_strlen)
3. [GDT (gdt.s)](#3-gdt)
4. [Raccourcis clavier du terminal](#4-raccourcis-clavier-du-terminal)
5. [Commandes du shell](#5-commandes-du-shell)

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
while (s[len] != '\0')
{
    len++;
}
return (len);
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
```

## 3. GDT (gdt.s)

### 📌 Vue d'ensemble
La **Global Descriptor Table (GDT)** décrit les segments de mémoire utilisés par le processeur en mode protégé.  
Le fichier `gdt.s` fait deux choses importantes :
- il définit la table GDT (segments noyau / utilisateur, code / données / pile) ;
- il fournit la fonction `gdt_flush` qui charge cette table dans le registre GDTR et met à jour les registres de segments.

Sans cette étape, le CPU ne sait pas comment accéder correctement à la mémoire en mode protégé.

### 🔍 Code complet
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

### 📖 Explication détaillée

#### gdt_flush : chargement de la GDT
```asm
gdt_flush:
    mov		eax, [esp + 4]
    lgdt	[eax]
```
- La fonction reçoit en paramètre (sur la pile) l'adresse d'un **descripteur de GDT** (`gdt_descriptor`).
- On la récupère dans `eax` puis on appelle `lgdt` pour charger cette adresse et la taille de la GDT dans le registre **GDTR**.

```asm
    mov		ax, 0x10
    mov		ds, ax
    mov		es, ax
    mov		fs, ax
    mov		gs, ax
    mov		ss, ax
```
- Une fois la nouvelle GDT chargée, on met à jour tous les registres de segment (`ds`, `es`, `fs`, `gs`, `ss`) avec le sélecteur **0x10**, qui correspond au segment **données noyau** (`gdt_data`).

```asm
    jmp		0x08:.flush

.flush:
    ret
```
- Le `jmp 0x08:.flush` est un **saut lointain** qui force le CPU à recharger le registre **CS** avec le sélecteur **0x08** (segment **code noyau**, `gdt_code`).
- L'étiquette `.flush` est simplement le point de retour après ce saut ; le `ret` renvoie ensuite au code C qui a appelé `gdt_flush`.

En résumé, `gdt_flush` :
1. Charge l'adresse de la GDT dans le CPU (`lgdt`).
2. Met tous les registres de segment sur les bons sélecteurs noyau.
3. Force le rechargement de `CS` via un saut lointain.

#### Définition de la table GDT
La section `.gdt` contient toutes les entrées de la GDT :

- `gdt_start` : entrée **nulle** (obligatoire, jamais utilisée directement).
- `gdt_code` : segment **code noyau** (ring 0, exécutable et lisible).
- `gdt_data` : segment **données noyau** (ring 0, lecture/écriture).
- `gdt_kernel_stack` : segment dédié à la **pile noyau**.
- `gdt_user_code` : segment **code utilisateur** (ring 3).
- `gdt_user_data` : segment **données utilisateur** (ring 3).
- `gdt_user_stack` : segment dédié à la **pile utilisateur**.

Chaque entrée suit le format classique des descripteurs de segment x86 :
- limites (`dw 0xffff`, `db 0x0`) ;
- base (ici laissée à 0 pour un adressage linéaire) ;
- octet de **droits** (par ex. `10011010b` pour code noyau) ;
- octet de **flags** (par ex. `11001111b` pour activer la granularité 4K, 32 bits, etc.).

Les commentaires en bas de `gdt.s` détaillent ces bits :
- bit 7 du champ de droits : descripteur valide ;
- bits 6–5 : niveau de privilège (ring 0 à 3) ;
- bits 4–3 : type de segment (code / data) ;
- bit 2 : conforming ;
- bit 1 : droit de lecture/écriture ;
- bit 0 : accès.

#### Descripteur de GDT
```asm
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start
```
- `dw` : taille de la table (nombre d'octets entre `gdt_start` et `gdt_end`, moins 1).
- `dd` : adresse de base de la table.

C'est exactement cette structure qui est passée à `gdt_flush` puis à l'instruction `lgdt`.

#### Utilisation dans le noyau
Au démarrage du noyau :
- le C prépare un `gdt_descriptor` qui pointe sur la table définie dans `gdt.s` ;
- puis appelle `gdt_flush` pour basculer proprement sur cette GDT ;
- ensuite, la commande shell `gdt` permet de **visualiser** les entrées de cette table (cf. ci-dessous).

---

## 4. Raccourcis clavier du terminal

Ces raccourcis permettent de contrôler facilement les différents terminaux virtuels et la navigation dans l'historique.

> `L-CTRL` / `L-SHIFT` signifient **touche gauche** (Left Control / Left Shift).

| Raccourci        | Action                                                |
|------------------|-------------------------------------------------------|
| L-CTRL + TAB     | Change de terminal actif (passe au terminal suivant) |
| L-SHIFT + TAB    | Bascule le **mode du terminal** (mode normal / autre)|
| L-CTRL + DELETE  | Nettoie entièrement le terminal courant              |
| L-CTRL + HOME    | Va tout en haut du terminal                          |
| L-CTRL + END     | Va tout en bas du terminal                           |
| L-CTRL + S       | Déclenche un **shutdown** propre du kernel           |

---

## 5. Commandes du shell

Le shell intégré permet d'exécuter quelques commandes de base :

| Commande   | Description                                                                 |
|-----------|-----------------------------------------------------------------------------|
| `clear`   | Efface le contenu du terminal courant                                       |
| `echo`    | Affiche un message, avec l'option `-n` pour ne pas ajouter de `\n` final   |
| `shutdown`| Termine proprement le kernel (équivalent d'un `exit` global)               |
| `gdt`     | Affiche un dump des entrées principales de la GDT en hexadécimal           |

---

Crédits : **lumugot** · **aderouba**