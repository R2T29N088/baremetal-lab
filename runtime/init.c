#include <stdint.h>

/*----------------------------------------------------------------------------
  C Runtime initialization function
 *----------------------------------------------------------------------------*/
void CrtInit(void)
{
  extern uint32_t _siramfunc, _sramfunc, _eramfunc;
  extern uint32_t _sidata, _sdata, _edata;
  extern uint32_t _sbss, _ebss;

  uint32_t *src, *dst;

  /* .ramfunc */
  src = &_siramfunc;
  dst = &_sramfunc;
  while (dst < &_eramfunc)
    *dst++ = *src++;

  /* .data */
  src = &_sidata;
  dst = &_sdata;
  while (dst < &_edata)
    *dst++ = *src++;

  /* .bss */
  for (dst = &_sbss; dst < &_ebss; dst++)
    *dst = 0;
}
