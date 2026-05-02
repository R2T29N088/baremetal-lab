/******************************************************************************
 * Derived from ARM CMSIS.5.9.0. Adapted for baremetal-lab.
 ******************************************************************************/

    .syntax     unified
    .arch       armv6-m

    .thumb
    .section    .reset_handler
    .align      2

    .extern     _msp_top
    .extern     SystemInit
    .extern     CrtInit
    .extern     main

    .thumb_func
    .type       Reset_Handler, %function
    .globl      Reset_Handler
    .fnstart

Reset_Handler:
    ldr     r0,=_msp_top
    msr     msp, r0

    bl      SystemInit
    bl      CrtInit
    bl      main

1:
    b       1b
