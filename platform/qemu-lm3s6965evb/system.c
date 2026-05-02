/******************************************************************************
 * Derived from ARM CMSIS.5.9.0. Adapted for baremetal-lab.
 ******************************************************************************/

#include <platform.h>
#include <platform/uart.h>

/*----------------------------------------------------------------------------
  System initialization function
 *----------------------------------------------------------------------------*/
void SystemInit(void)
{
  /* Set vector table address if supported */
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
  SCB->VTOR = (uint32_t)&__VECTOR_TABLE;
#endif

  /* Disable SysTick to avoid unexpected interrupts */
  SysTick->CTRL = 0;

  /* Disable all NVIC interrupts */
  NVIC->ICER[0] = 0xFFFFFFFF;

  /* Clear all pending NVIC interrupts */
  NVIC->ICPR[0] = 0xFFFFFFFF;

  /* Platform-specific initialization (UART, clock, etc.) */
  platform_uart_init();

  /* Finally, enable CPU-wide interrupt/exception handling */
  __enable_irq();
}
