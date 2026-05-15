/******************************************************************************
 * Derived from ARM CMSIS.5.9.0. Adapted for baremetal-lab.
 ******************************************************************************/

#include <platform.h>

/*----------------------------------------------------------------------------
  Vector Table
 *----------------------------------------------------------------------------*/

/* MSP */
extern uint32_t _msp_top;

/* Handler */
extern void Reset_Handler(void);

void Default_Handler(void)
{
  while(1);
}

#define NMI_Handler         Default_Handler   /* -14 NMI Handler */
#define HardFault_Handler   Default_Handler   /* -13 Hard Fault Handler */
#define SVC_Handler         Default_Handler   /*  -5 SVCall Handler */
#define PendSV_Handler      Default_Handler   /*  -2 PendSV Handler */
#define SysTick_Handler     Default_Handler   /*  -1 SysTick Handler */

/* Table */
const void* __VECTOR_TABLE[] __attribute__((used, section(".vectors"), aligned(256))) =
{
  /* MSP */
  &_msp_top,

  /* Exception */
  Reset_Handler,                              /*     Reset Handler */
  NMI_Handler,                                /* -14 NMI Handler */
  HardFault_Handler,                          /* -13 Hard Fault Handler */
  0,                                          /*     Reserved */
  0,                                          /*     Reserved */
  0,                                          /*     Reserved */
  0,                                          /*     Reserved */
  0,                                          /*     Reserved */
  0,                                          /*     Reserved */
  0,                                          /*     Reserved */
  SVC_Handler,                                /*  -5 SVCall Handler */
  0,                                          /*     Reserved */
  0,                                          /*     Reserved */
  PendSV_Handler,                             /*  -2 PendSV Handler */
  SysTick_Handler,                            /*  -1 SysTick Handler */

  /* Interrupt */
  Default_Handler,                            /*   Interrupt 0 */
  Default_Handler,                            /*   Interrupt 1 */
  Default_Handler,                            /*   Interrupt 2 */
  Default_Handler,                            /*   Interrupt 3 */
  Default_Handler,                            /*   Interrupt 4 */
  Default_Handler,                            /*   Interrupt 5 */
  Default_Handler,                            /*   Interrupt 6 */
  Default_Handler,                            /*   Interrupt 7 */
  Default_Handler,                            /*   Interrupt 8 */
  Default_Handler,                            /*   Interrupt 9 */

  Default_Handler,                            /*   Interrupt 10 */
  Default_Handler,                            /*   Interrupt 11 */
  Default_Handler,                            /*   Interrupt 12 */
  Default_Handler,                            /*   Interrupt 13 */
  Default_Handler,                            /*   Interrupt 14 */
  Default_Handler,                            /*   Interrupt 15 */
  Default_Handler,                            /*   Interrupt 16 */
  Default_Handler,                            /*   Interrupt 17 */
  Default_Handler,                            /*   Interrupt 18 */
  Default_Handler,                            /*   Interrupt 19 */

  Default_Handler,                            /*   Interrupt 20 */
  Default_Handler,                            /*   Interrupt 21 */
  Default_Handler,                            /*   Interrupt 22 */
  Default_Handler,                            /*   Interrupt 23 */
  Default_Handler,                            /*   Interrupt 24 */
  Default_Handler,                            /*   Interrupt 25 */
  Default_Handler,                            /*   Interrupt 26 */
  Default_Handler,                            /*   Interrupt 27 */
  Default_Handler,                            /*   Interrupt 28 */
  Default_Handler,                            /*   Interrupt 29 */

  Default_Handler,                            /*   Interrupt 30 */
  Default_Handler,                            /*   Interrupt 31 */
};
