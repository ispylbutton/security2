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

        if (argv[1] == ACTION_ENCRYPT){
            encrypt()
        }

        fclose(input_file);
    }
    return 0;
}
