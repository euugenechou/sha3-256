#include "sha3.h"
#include "util.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "hvl:i:o:"

void print_usage(char **argv) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Hashes inputs using the SHA-3 256 algorithm.\n"
        "\n"
        "USAGE\n"
        "   %s [-hv] -l length [-i input] [-o output]\n"
        "\n"
        "OPTIONS\n"
        "   -h          Display program help and usage\n"
        "   -v          Display verbose program output\n"
        "   -l length   Length of input in bytes\n"
        "   -i input    Specify input to hash (stdin by default)\n"
        "   -o output   Specify output of hashed input (stdout by default)\n",
        argv[0]);
}

void hash_file(int64_t length, FILE *infile, FILE *outfile) {
    uint8_t *msg = (uint8_t *)calloc(length ? length : 1, sizeof(uint8_t));
    check(msg, "Failed to allocate array for message.\n");

    output("Len = %" PRId64 "\n", length);
    fread(msg, sizeof(uint8_t), length, infile);

    output("Msg = ");
    hexprint(length, msg);

    uint8_t md[DIGEST] = { 0 };
    hash(length, msg, md);

    output("MD = ");
    hexprint(32, md);

    fwrite(md, sizeof(uint8_t), 32, outfile);
    free(msg);
}

int main(int argc, char **argv) {
    int opt = 0;
    int64_t length = -1;
    FILE *infile = stdin;
    FILE *outfile = stdout;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h':
            print_usage(argv);
            return 0;
        case 'v':
            verbose = true;
            break;
        case 'l':
            length = (int64_t)strtoll(optarg, NULL, 10);
            break;
        case 'i':
            infile = fopen(optarg, "rb");
            check(infile, "Failed to open %s.\n", optarg);
            break;
        case 'o':
            outfile = fopen(optarg, "wb");
            check(outfile, "Failed to open %s.\n", optarg);
            break;
        default:
            print_usage(argv);
            return 1;
        }
    }

    check(length >= 0, "Valid message length must be supplied.\n");

    hash_file(length, infile, outfile);

    fclose(infile);
    fclose(outfile);
    return 0;
}
