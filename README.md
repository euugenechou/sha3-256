# SHA-3 256 in C

Credits to Dr. Markku-Juhani O. Saarinen for providing a readable implementation
of the SHA-3 library available here: https://github.com/mjosaarinen/tiny_sha3.
The code in this project was based on said library, but deviated with several
changes made to provide clarity in reference to the original SHA-3/KECCAK
standards found here: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf.
More specifically, rewriting significant portions of the library to showcase how
exactly the theta, rho, pi, chi, and iota transformations work.
