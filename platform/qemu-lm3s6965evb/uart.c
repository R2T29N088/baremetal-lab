#include <platform/reg.h>
#include <platform/uart.h>

/* ===== SYSCTL ===== */
#define SYSCTL_BASE              0x400FE000
#define SYSCTL_RCGCUART          (SYSCTL_BASE + 0x618)
#define SYSCTL_RCGCGPIO          (SYSCTL_BASE + 0x608)

/* RCGCUART bits */
#define SYSCTL_RCGCUART_UART0    (1U << 0)

/* RCGCGPIO bits */
#define SYSCTL_RCGCGPIO_PORTA    (1U << 0)

/* ===== GPIO PORT A ===== */
#define GPIO_PORTA_BASE          0x40004000
#define GPIO_PORTA_AFSEL         (GPIO_PORTA_BASE + 0x420)
#define GPIO_PORTA_DEN           (GPIO_PORTA_BASE + 0x51C)

/* ===== PrimeCell UART PL011 ===== */
#define UART_BASE   0x4000C000
#define UART_DR     (UART_BASE + 0x00)
#define UART_FR     (UART_BASE + 0x18)
#define UART_CR     (UART_BASE + 0x30)
#define UART_LCRH   (UART_BASE + 0x2C)
#define UART_IBRD   (UART_BASE + 0x24)
#define UART_FBRD   (UART_BASE + 0x28)

/* UART_FR bits */
#define UART_FR_TXFF    (1 << 5)

/* UART_LCRH bits */
#define UART_LCRH_WLEN_8  (3 << 5)

/* UART_CR bits */
#define UART_CR_UARTEN  (1 << 0)
#define UART_CR_TXE     (1 << 8)

#include "platform.h"

void platform_uart_init(void)
{
    /* 1. Enable clocks for UART0 and GPIOA */
    putreg32(getreg32(SYSCTL_RCGCUART) | SYSCTL_RCGCUART_UART0, SYSCTL_RCGCUART);
    putreg32(getreg32(SYSCTL_RCGCGPIO) | SYSCTL_RCGCGPIO_PORTA, SYSCTL_RCGCGPIO);

    /* 2. Configure GPIOA pins for UART0 (PA0 = RX, PA1 = TX)
     * NOTE: This must be done BEFORE enabling UART0.
     */
    putreg32(getreg32(GPIO_PORTA_AFSEL) | (1U << 0) | (1U << 1), GPIO_PORTA_AFSEL);
    putreg32(getreg32(GPIO_PORTA_DEN)   | (1U << 0) | (1U << 1), GPIO_PORTA_DEN);

    /* 3. Configure UART0 */
    /* Disable UART before configuration */
    putreg32(getreg32(UART_CR) & ~UART_CR_UARTEN, UART_CR);

    /* Baud rate: 115200bps @ 16MHz */
    putreg32(27, UART_IBRD);
    putreg32(8,  UART_FBRD);

    /* 8-bit, no parity, FIFO disabled (more stable on QEMU) */
    putreg32(UART_LCRH_WLEN_8, UART_LCRH);

    /* Enable UART (TX enabled as well) */
    putreg32(getreg32(UART_CR) | UART_CR_UARTEN | UART_CR_TXE, UART_CR);
}

void platform_uart_putc(char c)
{
    /* Wait until TX FIFO is not full */
    while (getreg32(UART_FR) & UART_FR_TXFF) {
        /* wait */
    }
    putreg32((uint32_t)c, UART_DR);
}
