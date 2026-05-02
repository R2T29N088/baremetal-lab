#ifndef CONSOLE_H
#define CONSOLE_H

#ifdef __cplusplus
extern "C" {
#endif

void console_putc(char c);
void console_puts(const char *s);

#ifdef __cplusplus
}
#endif

#endif  /* CONSOLE_H */
