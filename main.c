#include <stdio.h>

// Declare action parameters
#define ACTION_GENERATE_KEY "-g"
#define ACTION_ENCRYPT "-e"
#define ACTION_DECRYPT "-d"


int main (int argc, char* argv[]) {

    if (argc =! 4){
        printf("Please provide the arguments as follows:\n./DES <mode encrypt decrypt> <text file> <key>");
        return 1;
    }
    else {
        // Declare file handlers
        FILE *input_file, *output_file;
        
        input_file=fopen(argv[2], "rb");
        if (!input_file){
            printf("Error opening input file to read data.");
            return 1;
        }

        output_file = fopen(argv[4], "wb");
		if (!output_file) {
			printf("Could not open output file to write data.");
			return 1;
		}

        if (strcmp(argv[1],ACTION_ENCRYPT)==0){
            encrypt(input_file, argv[3]); //sending input file and the key to encrypt function
            printf("Encrypting.\n");
        }

        else if (strcmp(argv[1], ACTION_DECRYPT)==0){
            decrypt(input_file, argv[3]); //sending input file and key to decrypt
            printf("Decrypting.\n");
        }

        fclose(input_file);
        fclose(output_file);
    }
    return 0;
}
