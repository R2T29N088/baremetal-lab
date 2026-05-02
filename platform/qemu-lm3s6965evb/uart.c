#include <platform/reg.h>
#include <platform/uart.h>

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

void platform_uart_init(void)
{
  putreg32(0x00, UART_CR);                        /* Disable UART */
  putreg32(1, UART_IBRD);                         /* This is only for QEMU */
  putreg32(0, UART_FBRD);
  putreg32(UART_LCRH_WLEN_8, UART_LCRH);          /* WLEN=11 : 8bit */
  putreg32(UART_CR_UARTEN | UART_CR_TXE, UART_CR);/* UARTEN | TXE */
}

void platform_uart_putc(char c)
{
  while (getreg32(UART_FR) & UART_FR_TXFF) {
    /* TXFF (FIFO full) */
  }
  putreg32((uint32_t)c, UART_DR);
}
