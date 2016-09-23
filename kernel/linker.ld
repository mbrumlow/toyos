ENTRY(start) 
SECTIONS {

	.text 0x4000000 : 
	{ 
		*(.text)
		. = ALIGN(4096);
	}

	.data : 
	{
		*(.bss)
		. = ALIGN(4096);
	}

	end = .; _end = .; __end = .;	
}

