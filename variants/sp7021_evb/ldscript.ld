OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

__TTB_SIZE = 0x00004000;

MEMORY
{
	public	(rwx) : ORIGIN = 0x200040, LENGTH = 8M
	shmem   (rw)  : ORIGIN = ORIGIN(public) + LENGTH(public), LENGTH = 0x80000
}

 /* Symbols needed for OpenAMP to enable rpmsg */
__OPENAMP_region_start__  = ORIGIN(shmem);
__OPENAMP_region_end__ = ORIGIN(shmem) + LENGTH(shmem);


EXTERN(__sp_init)
PROVIDE(__sp_init = ORIGIN(public) + LENGTH(public));
SECTIONS
{
	.text :
	{
		KEEP(*startup_sp7021_arm926.o (.text*))
		__vectors_start = .;
		KEEP(*vectors.o(.text*))
		KEEP(*vectors.o(.rodata*))
		__vectors_end = .;
		. = ALIGN(8);
		*(.rodata*)
		*(.text*)
		/* .ctors */

		/*
		*crtbegin.o(.ctors)
		*crtbegin?.o(.ctors)
		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
		*(SORT(.ctors.*))
		*(.ctors)

		/* .dtors */
		*crtbegin.o(.dtors)
		*crtbegin?.o(.dtors)
		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
		*(SORT(.dtors.*))
		*(.dtors) 
		*/
		Image$$RO_DATA$$Base = .;
		*(.rodata*)
		Image$$RO_DATA$$Limit = .;
		KEEP(*(.eh_frame*))
	} > public
 
	.ARM.extab : 
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > public
	 
	.ARM.exidx :
	{
		__exidx_start = .;
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
		__exidx_end = .;
	} > public
 
	.copy.table :
	{
		. = ALIGN(4);
		__copy_table_start__ = .;
		LONG (__etext)
		LONG (__data_start__)
		LONG (__data_end__ - __data_start__)
		__copy_table_end__ = .;
	} > public

	.zero.table :
	{
		. = ALIGN(4);
		__zero_table_start__ = .;
		LONG (__bss_start__)
		LONG (__bss_end__ - __bss_start__)
		__zero_table_end__ = .;
	} > public
		
	.ttb :
	{
	    	. = ALIGN(0x4000);
	        Image$$TTB$$ZI$$Base = .;
	        . += __TTB_SIZE;
	        Image$$TTB$$ZI$$Limit = .;
    	} > public

	__etext = ALIGN (4);
    
	.data : AT (__etext)
	{
		. = ALIGN(8);
		Image$$RW_DATA$$Base = .;
		__data_start__ = .;
		*(vtable)
		*(.data)
		. = ALIGN(0x4000);
  		*(.mmu_tables)
		Image$$RW_DATA$$Limit = .;
        	. = ALIGN(4);
	        /* preinit data */
	        PROVIDE (__preinit_array_start = .);
	        KEEP(*(.preinit_array))
	        PROVIDE (__preinit_array_end = .);
	                . = ALIGN(4);
	        /* init data */
	        PROVIDE (__init_array_start = .);
	        KEEP(*(SORT(.init_array.*)))
	        KEEP(*(.init_array))
	        PROVIDE (__init_array_end = .);

	        . = ALIGN(4);
	        /* finit data */
	        PROVIDE (__fini_array_start = .);
	        KEEP(*(SORT(.fini_array.*)))
	        KEEP(*(.fini_array))
	        PROVIDE (__fini_array_end = .);

	        . = ALIGN(4);
	        /* All data end */
	        __data_end__ = .;
    
		__edata = .;
	} > public
	 
	.bss  ALIGN(0x400):
	{
	 	Image$$ZI_DATA$$Base = .;
		__bss_start__ = .;
		*(.bss)
		*(.bss.*)
		*(.sbss)
		*(.sbss.*)
		*(COMMON)
		. = ALIGN(8);
		__bss_end__ = .;
		Image$$ZI_DATA$$Limit = .;
	  	__end__ = .;
	  	end = __end__;
	  	
	} > public

  	/* Remove information from the compiler libraries */
  	/*
  	/DISCARD/ :
  	{
    	libc.a ( * )
    	libm.a ( * )
    	libgcc.a ( * )
  	}

  	.ARM.attributes 0 : { *(.ARM.attributes) }
  	*/
}
