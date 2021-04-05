#ifndef __SHA3_H__
#define __SHA3_H__

#include <stdint.h>

#define DIGEST 32 // 256-bit digest in bytes.

void hash(uint64_t n, uint8_t data[static n], uint8_t digest[static DIGEST]);

#endif
