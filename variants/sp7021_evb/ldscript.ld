OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
MEMORY
{
 public : ORIGIN = 0x200040, LENGTH = 32M
}
EXTERN(__sp_init)
PROVIDE(__sp_init = ORIGIN(public) + LENGTH(public));
SECTIONS
{
 .text :
 {
  start.o (.text)
  __vectors_start = .;
  KEEP(*vectors.o(.text*))
  KEEP(*vectors.o(.rodata*))
  __vectors_end = .;
  . = ALIGN(8);
  *(.rodata*)
  *(.text*)
  __iop_code_start_addr__ = .;
  *(.iop_bin*)
  __iop_code_end_addr__ = .;
  __etext = .;
 } > public
 .ARM.exidx :
 {
  __exidx_start = .;
  *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  __exidx_end = .;
 } > public
 .data :
 {
  . = ALIGN(8);
  __data = .;
  *(.data)
  . = ALIGN(0x4000);
  *(.mmu_tables)
  __edata = .;
 } > public
 .bss :
 {
  __bss_start = .;
  *(.bss)
  *(.bss.*)
  *(.sbss)
  *(.sbss.*)
  *(COMMON)
  . = ALIGN(8);
  __bss_end__ = .;
  __bss_end = .;
 } > public
}
