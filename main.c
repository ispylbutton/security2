#include <stdio.h>
#include "DES2.h" //not sure if this is needed or not since its in DES2.c and we are linking that


int main (int argc, char* argv[]) {

    if (argc < 4)
        printf("Please provide the arguments as follows:\n./DES <mode encrypt decrypt> <key Y or N> <text file> [key file]");

    return 0;
}
