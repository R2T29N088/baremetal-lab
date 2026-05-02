/******************************************************************************
 * Derived from NuttX getreg/putreg macros. Adapted for baremetal-lab.
 ******************************************************************************/

#ifndef REG_H
#define REG_H

#include <stdint.h>

#define getreg8(addr)   (*(volatile uint8_t  *)(addr))
#define getreg16(addr)  (*(volatile uint16_t *)(addr))
#define getreg32(addr)  (*(volatile uint32_t *)(addr))

#define putreg8(val, addr)  (getreg8(addr)  = (uint8_t)(val))
#define putreg16(val, addr) (getreg16(addr) = (uint16_t)(val))
#define putreg32(val, addr) (getreg32(addr) = (uint32_t)(val))

#endif /* REG_H */
