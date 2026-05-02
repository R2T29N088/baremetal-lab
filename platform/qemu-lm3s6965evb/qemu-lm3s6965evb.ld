/******************************************************************************
 * Derived from ARM CMSIS.5.9.0. Adapted for baremetal-lab.
 ******************************************************************************/

ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 64K
  RAM   (rwx): ORIGIN = 0x20000000, LENGTH = 8K
}

SECTIONS
{
  .text :
  {
    _stext = .;
    KEEP(*(.vectors))
    KEEP(*(.reset_handler))
    *(.text*)
    *(.rodata*)
    KEEP(*(.init))
    KEEP(*(.fini))
    . = ALIGN(8);
    _etext = .;
  } > FLASH

  .ramfunc : AT(ADDR(.text) + SIZEOF(.text))
  {
    . = ALIGN(8);
    _sramfunc = .;
    *(.ramfunc*)
    _eramfunc = .;
  } > RAM
  _siramfunc = LOADADDR(.ramfunc);

  .data : AT(ADDR(.text) + SIZEOF(.text) + SIZEOF(.ramfunc))
  {
    . = ALIGN(8);
    _sdata = .;
    *(.data*)
    _edata = .;
  } > RAM
  _sidata = LOADADDR(.data);

  .bss :
  {
    . = ALIGN(8);
    _sbss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } > RAM

  .msp (NOLOAD):
  {
    . = ALIGN(8);
    _msp_top = ORIGIN(RAM) + LENGTH(RAM);
    _msp_bottom = _msp_top - 1024;
  } > RAM
}
