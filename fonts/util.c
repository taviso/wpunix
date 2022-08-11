#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#include "util.h"

size_t putbyte(FILE *stream, uint8_t byte)
{
    return fwrite(&byte, sizeof byte, 1, stream);
}

size_t putword(FILE *stream, uint16_t word)
{
    return fwrite(&word, sizeof word, 1, stream);
}

size_t putdword(FILE *stream, uint32_t dword)
{
    return fwrite(&dword, sizeof dword, 1, stream);
}

size_t putstring(FILE *stream, const char *format, ...)
{
    va_list ap;
    int result;
    va_start(ap, format);
    result = vfprintf(stream, format, ap);
    va_end(ap);
    return result + putbyte(stream, 0);
}
