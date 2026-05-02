#include <arch/arch.h>
#include <platform.h>

arch_int_state_t arch_int_disable(void)
{
  /* Save current PRIMASK state */
  uint32_t state = __get_PRIMASK();

  /* Disable all exceptions except HardFault (PRIMASK = 1) */
  __disable_irq();

  return state;
}

void arch_int_restore(arch_int_state_t state)
{
  /* Restore previous PRIMASK state */
  __set_PRIMASK(state);
}
