ENTRY(_start)

SECTIONS
{
	. = 0x00000800;

	.gdt ALIGN(4) :
	{
		*(.gdt)
	}

	. = 0x00100000;

	.multiboot BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
	}

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}

	_kernel_end = .;
}
