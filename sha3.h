#pragma once

#include <stdint.h>

#define SHA3_256_MD_LEN 32      // 256-bit digest length in bytes.
#define SHA3_256_ROUNDS 24      // KECCAK rounds to perform for SHA3-256.
#define SHA3_256_WIDTH  200     // 1600-bit width in bytes.
#define SHA3_256_LANES  25      // State is an unrolled 5x5 array of 64-bit lanes.
#define SHA3_256_RATE   136     // 1600-bit width - 512-bit capacity in bytes.

struct Sha3_256 {
    int padpoint;
    int absorbed;
    union {
        uint64_t words[SHA3_256_LANES];
        uint8_t bytes[SHA3_256_WIDTH];
    } state;
};

struct Sha3_256 sha3_256_new(void);

void sha3_256_update(struct Sha3_256 *ctx, uint8_t *data, uint64_t n);

void sha3_256_finalize(struct Sha3_256 *ctx, uint8_t *digest);

void sha3_256_digest(uint8_t *data, uint64_t n, uint8_t *digest);
