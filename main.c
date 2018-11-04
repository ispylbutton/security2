#include <stdio.h>


int main (int argc, char* argv[]) {

    if (argc < 4)
        printf("Please provide the arguments as follows:\n./DES <mode encrypt decrypt> <key Y or N> <text file> [key file]");

    return 0;
}
