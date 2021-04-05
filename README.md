# SHA-3 256 in C

This project is comprised of the singular program ```sha3-test```, which is
capable of performing the sponge-based, SHA-3 256 secure hash algorithm on
arbitrary amounts of input data.

### Design

Credits to Dr. Markku-Juhani O. Saarinen for providing a readable implementation
of the SHA-3 library available here: https://github.com/mjosaarinen/tiny_sha3.
The code in this project was based on said library, but deviated with several
changes made to provide clarity in reference to the original SHA-3/KECCAK
standards found here: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf.
More specifically, rewriting significant portions of the library to showcase how
exactly the theta, rho, pi, chi, and iota transformations work.

### Building

To build the program:

	$ make

Or:

	$ make all

### Usage

Note that the length of the input to hash must be specified using a
```getopt()``` command-line option as demonstrated below.  To hash a 64-byte
input (```stdin``` and ```stdout``` are the default for input and output,
respectively):

	$ ./sha3-test -l 64 [-i input] [-o output]

### Additional Program Options

The following ```getopt()``` command-line options below are available as well:
  - '-h': Display program help and usage
  - '-v': Display verbose program output (sent to stderr)

### Running Tests

Test vectors supplied by NIST are included in this project as well. To run all
the tests, just run the following (results are cleaned after):

    $ make test
