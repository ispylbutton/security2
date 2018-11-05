# DES Implementation

DES Implementation in C for encrypting and decrypting input text with a given key.

## Compiling
This implementation works best when run on a Unix platform. You may compile and run it on Windows if you have GDB and C correctly installed.

Make sure you have DES2.c, DES2.h, and main.c in the same directory, then...

Run the makefile to compile the code with GDB.
```bash
make des
```

This will compile the code using the following:
```bash
gcc -o DES DES2.c main.c
```

## Usage

```bash
./DES [MODE] [INPUT_FILENAME] [KEY]
```
#### MODE

```bash
-e 
    encrypt the given input file with the given key
-d
    decrypt the given input file with the given key
```

#### INPUT_FILENAME

Please provide the .txt filename for the input text to encrypt or decrypt within "".

#### KEY

Please provide a bitwise key for encryption or decryption.