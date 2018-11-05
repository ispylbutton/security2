# DES Implementation

DES Implementation in C for encrypting and decrypting input text with a given key.

## Compiling

Run the makefile to compile the code with GDB.
```bash
make des
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