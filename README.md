# DES Implementation

DES Implementation in C for encrypting and decrypting input text with a given key.

Conditions:
CBC mode was not successfully implemented. DES is in EBC mode, so it is more vulnerable to a cut-and-paste attack.
The code that would theoretically implement CBC is indicated in DES.c for review.
The command line program accepts the IV parameter, but since CBC was not implemented, it is not actually used.
Both the 64bit key and the 64bit IV are accepted as binary instead of hex.

## Compiling
This implementation works best when run on a Unix platform. You may compile and run it on Windows if you have GDB and C correctly installed.

Run the makefile to clean any previously executed instances
```bash
make clean
```

Run the makefile to compile the code.
```bash
make all
```

This will compile from:
```bash
cc -std=c99  -c DES.c
cc -std=c99  -c main.c
cc -std=c99 -o DES DES.o main.o
```

## Usage

```bash
./DES [MODE] (optional: -o [SPECIFIED_OUTPUT_FILE]) -i [IV] -k [KEY] [INPUT_FILENAME]
```
#### MODE

```bash
-e 
    encrypt the given input file with the given key
-d
    decrypt the given input file with the given key
```

#### SPECIFIED_OUTPUT_FILENAME

Please provide the .txt filename for the decrypted text to print to within "".

#### INPUT_FILENAME

Please provide the .txt filename for the input text to encrypt or decrypt within "".
The file size should be a multiple of 8 bits.

#### IV

Please provide a bitwise IV for encryption or decryption of the same length as the key.

#### KEY

Please provide a bitwise key for encryption or decryption.
