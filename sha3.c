#include "sha3.h"

#include <string.h>

#define ROTL64(x, y)  (((x) << (y)) | ((x) >> (64 - (y))))

static void theta(struct Sha3_256 *ctx) {
    uint64_t C[5] = { 0 };
    uint64_t D[5] = { 0 };

    for (int i = 0; i < 5; i += 1) {
        C[i]  = ctx->state.words[i];
        C[i] ^= ctx->state.words[i + 5];
        C[i] ^= ctx->state.words[i + 10];
        C[i] ^= ctx->state.words[i + 15];
        C[i] ^= ctx->state.words[i + 20];
    }

    for (int i = 0; i < 5; i += 1) {
        D[i] = C[(i + 4) % 5] ^ ROTL64(C[(i + 1) % 5], 1);
    }

    for (int i = 0; i < 5; i += 1) {
        for (int j = 0; j < 25; j += 5) {
            ctx->state.words[i + j] ^= D[i];
        }
    }
}

static void rho(struct Sha3_256 *ctx) {
    static const int rotations[25] = {
         0,  1, 62, 28, 27,
        36, 44,  6, 55, 20,
         3, 10, 43, 25, 39,
        41, 45, 15, 21,  8,
        18,  2, 61, 56, 14
    };

    for (int i = 0; i < 25; i += 1) {
        ctx->state.words[i] = ROTL64(ctx->state.words[i], rotations[i]);
    }
}

static void pi(struct Sha3_256 *ctx) {
    static const int switcheroo[25] = {
         0, 10, 20,  5, 15,
        16,  1, 11, 21,  6,
         7, 17,  2, 12, 22,
        23,  8, 18,  3, 13,
        14, 24,  9, 19,  4
    };

    uint64_t temp[25] = { 0 };

    for (int i = 0; i < 25; i += 1) {
        temp[i] = ctx->state.words[i];
    }

    for (int i = 0; i < 25; i += 1) {
        ctx->state.words[switcheroo[i]] = temp[i];
    }
}

static void chi(struct Sha3_256 *ctx) {
    uint64_t temp[5] = { 0 };

    for (int j = 0; j < 25; j += 5) {
        for (int i = 0; i < 5; i += 1) {
            temp[i] = ctx->state.words[i + j];
        }

        for (int i = 0; i < 5; i += 1) {
            ctx->state.words[i + j] ^= (~temp[(i + 1) % 5]) & temp[(i + 2) % 5];
        }
    }
}

static void iota(struct Sha3_256 *ctx, uint8_t r) {
    static const uint64_t constants[24] = {
        0x0000000000000001, 0x0000000000008082, 0x800000000000808a,
        0x8000000080008000, 0x000000000000808b, 0x0000000080000001,
        0x8000000080008081, 0x8000000000008009, 0x000000000000008a,
        0x0000000000000088, 0x0000000080008009, 0x000000008000000a,
        0x000000008000808b, 0x800000000000008b, 0x8000000000008089,
        0x8000000000008003, 0x8000000000008002, 0x8000000000000080,
        0x000000000000800a, 0x800000008000000a, 0x8000000080008081,
        0x8000000000008080, 0x0000000080000001, 0x8000000080008008
    };

    ctx->state.words[0] ^= constants[r];
}

static void keccak(struct Sha3_256 *ctx) {
    for (int i = 0; i < SHA3_256_ROUNDS; i += 1) {
        theta(ctx);
        rho(ctx);
        pi(ctx);
        chi(ctx);
        iota(ctx, i);
    }
}

static void absorb(struct Sha3_256 *ctx, uint8_t *data, uint64_t n) {
    for (uint64_t i = 0; i < n; i += 1) {
        ctx->state.bytes[ctx->absorbed++] ^= data[i];

        if (ctx->absorbed == SHA3_256_RATE) {
            keccak(ctx);
            ctx->absorbed = 0;
        }
    }

    ctx->padpoint = ctx->absorbed;
}

static void squeeze(struct Sha3_256 *ctx, uint8_t *digest) {
    ctx->state.bytes[ctx->padpoint] ^= 0x06;
    ctx->state.bytes[SHA3_256_RATE - 1] ^= 0x80;

    keccak(ctx);

    for (int i = 0; i < SHA3_256_MD_LEN; i += 1) {
        digest[i] = ctx->state.bytes[i];
    }

    ctx->padpoint = ctx->absorbed = 0;
    memset(&ctx->state.words, 0, sizeof(ctx->state.words));
}

struct Sha3_256 sha3_256_new(void) {
    struct Sha3_256 ctx;
    memset(&ctx, 0, sizeof(ctx));
    return ctx;
}

void sha3_256_update(struct Sha3_256 *ctx, uint8_t *data, uint64_t n) {
    absorb(ctx, data, n);
}

void sha3_256_finalize(struct Sha3_256 *ctx, uint8_t *digest) {
    squeeze(ctx, digest);
}

void sha3_256_digest(uint8_t *data, uint64_t n, uint8_t *digest) {
    struct Sha3_256 ctx = sha3_256_new();
    absorb(&ctx, data, n);
    squeeze(&ctx, digest);
}
