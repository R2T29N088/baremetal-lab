#ifndef PLATFORM_H
#define PLATFORM_H

/*
 * ARMCM0plus.h is taken from the Cortex‑M0+ device template
 * provided in ARM CMSIS 5.9.0.
 *
 * It defines core‑level registers common to Cortex‑M0+ CPUs
 * (NVIC, SysTick, SCB, etc.) and does not include any SoC‑specific
 * peripherals or interrupt numbers.
 *
 * Core configuration macros such as __MPU_PRESENT and __VTOR_PRESENT
 * describe how the Cortex‑M0+ core is implemented in a given SoC,
 * and therefore depend on the SoC. For this reason, ARMCM0plus.h is
 * included from platform.h so that core definitions and SoC‑specific
 * settings can be combined.
 */
#include <ARMCM0plus.h>

/*
 * Platform-specific IRQ numbers for LM3S6965.
 * These values correspond to the NVIC interrupt numbers
 * defined in the device datasheet.
 */
#define PLATFORM_IRQ_GPIOA     0
#define PLATFORM_IRQ_GPIOB     1
#define PLATFORM_IRQ_GPIOC     2
#define PLATFORM_IRQ_GPIOD     3
#define PLATFORM_IRQ_GPIOE     4

#define PLATFORM_IRQ_UART0     5
#define PLATFORM_IRQ_UART1     6

#define PLATFORM_IRQ_TIMER0A   19
#define PLATFORM_IRQ_TIMER0B   20
#define PLATFORM_IRQ_TIMER1A   21
#define PLATFORM_IRQ_TIMER1B   22
#define PLATFORM_IRQ_TIMER2A   23
#define PLATFORM_IRQ_TIMER2B   24

#endif  /* PLATFORM_H */
