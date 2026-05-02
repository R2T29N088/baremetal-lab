#ifndef UART_H
#define UART_H

#ifdef __cplusplus
extern "C" {
#endif

void platform_uart_init(void);
void platform_uart_putc(char c);

#ifdef __cplusplus
}
#endif

#endif  /* UART_H */
