ENTRY(start) 


SECTIONS {
	. = 4K;

	

	.text : 
	{ 
		*(.text)
	}
}

