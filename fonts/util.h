#ifndef __UTIL_H
#define __UTIL_H

size_t putbyte(FILE *stream, uint8_t byte);
size_t putword(FILE *stream, uint16_t word);
size_t putdword(FILE *stream, uint32_t dword);
size_t putstring(FILE *stream, const char *format, ...);

#define countof(n) (sizeof(n) / sizeof(*(n)))

#endif
