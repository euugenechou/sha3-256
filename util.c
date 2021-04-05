#include "util.h"

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool verbose = false;

void output(char *fmt, ...) {
    if (verbose) {
        va_list arg;
        va_start(arg, fmt);
        vfprintf(stderr, fmt, arg);
        va_end(arg);
    }
}

void check(bool cond, char *fmt, ...) {
    if (!cond) {
        va_list arg;
        va_start(arg, fmt);
        vfprintf(stderr, fmt, arg);
        va_end(arg);
        exit(EXIT_FAILURE);
    }
}

void hexprint(uint64_t nbytes, uint8_t bytes[static nbytes]) {
    for (uint64_t i = 0; i < nbytes; i += 1) {
        output("%02" PRIx8, bytes[i]);
    }
    output("\n");
}
