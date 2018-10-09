ENTRY(_Reset)

SECTIONS 
{
	.text : {
	fstartup.o (INTERRUPT_VECTOR)
	*(.text)
	}

	.data : {
		*(.data)
	}
	
	.bss : {
		*(.bss COMMON)
	}

	. = ALIGN(0x8) ;
	
	. = . + 0x1000 ;
	
	stack_top = . ;
}
