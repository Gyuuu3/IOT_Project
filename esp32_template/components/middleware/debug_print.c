#include "debug_print.h"
#include <stdio.h>
#include <string.h>

void Debug_Print(const char *format, ...) {
    char buffer[256];
    va_list ap;
    va_start(ap, format);
    vsnprintf(buffer, sizeof(buffer), format, ap);
    va_end(ap);
    printf("%s", buffer);
}

