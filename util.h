#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

extern bool verbose;

void output(char *fmt, ...);

void check(bool cond, char *fmt, ...);

void hexprint(uint64_t nbytes, uint8_t bytes[static nbytes]);

#endif
