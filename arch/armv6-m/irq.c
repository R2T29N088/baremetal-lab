#include <arch/arch.h>
#include <platform.h>

void arch_irq_enable(int irqn)
{
  /* Enable the specified IRQ line in the NVIC */
  __NVIC_EnableIRQ((IRQn_Type)irqn);
}

void arch_irq_disable(int irqn)
{
  /* Disable the specified IRQ line in the NVIC */
  __NVIC_DisableIRQ((IRQn_Type)irqn);
}
