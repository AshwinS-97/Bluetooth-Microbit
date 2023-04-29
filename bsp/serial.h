#ifndef SERIAL_H
#define SERIAL_H
#include <stdint.h>

extern void serial_init(uint32_t rxpin, uint32_t txpin);
extern char getc_nowait(void);
extern char getc1(void);
extern void putc_nowait(char ch);
extern void putc1(char ch);
extern void puts1(char s[]);

#endif  /* SERIAL_H */
