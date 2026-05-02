#include <runtime/console.h>
#include <platform/uart.h>

void console_putc(char c)
{
  platform_uart_putc(c);
}

void console_puts(const char *s)
{
  while (*s) {
    console_putc(*s++);
  }
}
