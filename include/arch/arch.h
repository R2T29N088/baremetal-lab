#ifndef ARCH_H
#define ARCH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*
 * Type used to store the CPU interrupt/exception mask state.
 */
typedef uint32_t arch_int_state_t;

/*
 * Disable all maskable exceptions and return
 * the previous PRIMASK value so it can be restored later.
 *
 * This function is nestable: callers must pass the returned
 * state to arch_int_restore().
 *
 */
arch_int_state_t arch_int_disable(void);

/*
 * Restore the previous interrupt/exception mask state.
 * The state must be a value previously returned by arch_int_disable().
 */
void arch_int_restore(arch_int_state_t state);

/*
 * Enable a specific IRQ line in the interrupt controller.
 * The IRQ number is platform-specific (SoC dependent).
 */
void arch_irq_enable(int irqn);

/*
 * Disable a specific IRQ line in the interrupt controller.
 * The IRQ number is platform-specific (SoC dependent).
 */
void arch_irq_disable(int irqn);

#ifdef __cplusplus
}
#endif

#endif  /* ARCH_H */
