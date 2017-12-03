#include <stdio.h>
#include "print_helper.h"

int print_ascii_tbl (FILE *stream)
{
    for (char c = ' '; c <= '~'; c++) {
        if (!fprintf(stream, "%c ", c)) {
            return 0;
        };
    };

    return 0;
}

int print_for_human (FILE *stream, const unsigned char *array, const int len)
{
    if (len > 1) {
        for (int i = 0; i < len; i++) {
            fprintf(stream, "\"0x%02X\" ", array[i]);
        };
    };

    return 0;
}
