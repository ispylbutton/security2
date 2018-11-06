#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h> 
#include <time.h>
#include "DES.h"

static void genkey(uint64_t* key)
{
    uint64_t initBit=0x8000000000000000;
    int i;
    srand(time(NULL));


    for(i = 0; i < 64; i++) 
    {
            if(rand() % 2 == 1)
            {
                *key += (initBit >> i);
            }
    }
}


int main(int argc, char * argv[])
{
    // Vars
    uint64_t key = 0;
    uint64_t IV;
    uint64_t initBit=0x8000000000000000;
    static FILE * output = NULL;
    	uint64_t subKeyList[16];
	uint64_t singleKey;
	singleKey=0;
	bool show=false;
	uint64_t data_temp;
	subKeyList[0]=singleKey;
	uint64_t nextKey;
	size_t amount;
	uint64_t data;
	int i;
	bool encryptCheck;
    
    FILE * input = NULL;

    /*
     * https://stackoverflow.com/questions/192249/how-do-i-parse-command-line-arguments-in-bash
     * http://wiki.bash-hackers.org/howto/getopts_tutorial
     */
    int optionCheck;
    optionCheck= 0;

    

    const struct option long_opts[] = 
    { 
        {"encrypt",        no_argument, NULL, 'e'},
        {"decrypt",        no_argument, NULL, 'd'},
        {"output",   required_argument, NULL, 'o'},
        {"key",      required_argument, NULL, 'k'},
	{"IV",	     required_argument, NULL, 'i'},
        {NULL,                       0, NULL,   0}
    }; 

    const char* short_opts = "edk:i:o:";
    while((optionCheck = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1)
    { 
        switch(optionCheck)
        { 
	  case 'e': //encrypt
	    encryptCheck=true;
	    if (argv[optind]==NULL)
	    {
	      fprintf(stderr, "No input file provided\n");
	      return 1;
	    }
	    break;
        case 'd': // decrypt
            encryptCheck = false;
	        if(argv[optind] == NULL)
    {
        fprintf(stderr, "No input file provided\n");
	return 1;
    }
            break;


        case 'o': // Output file
            output = fopen(optarg, "w");
	        if(output == NULL)
    {
        fprintf(stderr, "Incorrect permissions for output file\n");
        return 1;
    }
            break;
	    
	    
/*
 * uses binary, not hex. if more time, convert using logic from here:
 *https://codeforwin.org/2015/08/c-program-to-convert-hexadecimal-to-binary-number-system.html	
 */    
	case 'i': //IV
	    if(optarg)
	      {
                for(i = 0; optarg[i] != '\0'; i++)
                {
		  //if the next number in the IV string is 1, then 1 is added to the stored IV in the position shifted i to the right
                    if(optarg[i] == '1')
                        IV += (initBit >> i);
		    
                }
                		    if (i>64)
		    {
		      printf("%d",i);
		      fprintf(stderr,"IV longer than key\n");
		      return 1;
		    }
		    else if (i<63)
		    {
		      printf("%d",i);
		      fprintf(stderr, "IV shorter than key\n");
		      return 1;
		    }
                    if(IV == 0)
    {
        fprintf(stderr, "No IV provided\n");
        return 1;
    }
    break;
            }
        case 'k': // Key
            if(optarg)
            {
                for(i = 0; optarg[i] != '\0'; i++)
                {
		  //if the next number in the key string is 1, then 1 is added to the stored key in the position shifted i to the right
                    if(optarg[i] == '1')
                        key += (initBit >> i);
                }
                    if(key == 0)
    {
        fprintf(stderr, "No key provided\n");
        return 1;
    }
            }
        }
    }

    input = fopen(argv[optind], "rb");

    if(input == NULL)
    {
        fprintf(stderr, "Incorrect permissions for input file\n");
        return 1;
    }

    //if no file is specified with -o, output is written to default output file
    if(output == NULL) 
    {
        output = fopen("output.txt", "w");
    }

    //populates list of subkeys for each round of DES
    for(i = 0; i < 16; i++)
    {
	keyScheduler(&subKeyList[i], &nextKey, i);
	if(i != 15)
	{
	    subKeyList[i + 1] = nextKey;
	}
    }

    //
    // 3. 16 Rounds of enc/decryption
    //
    if (show==false)
    {
    while((amount = fread(&data, 1, 8, input)) > 0)
    {
        if(amount != 8)
            data = data << (8 * (8 - amount));

        // Initial permutation
        permuteBlock(&data, true);

        // Encrypt rounds
        if(encryptCheck==true)
        {
            for(i = 0; i < 16; i++)
                singleRoundDES(&data, subKeyList[i]);
        }
        // Decrypt rounds
        else
        {
            // Switches left and right blocks
            data = (data << 32) + (data >> 32);

	    i=15;
	    while (i>=0)
	    {
	     singleRoundDES(&data, subKeyList[i]);
	     i=i-1;
	    }
            
            // Switching blocks back
            data = (data << 32) + (data >> 32);
        }

        // Final permutation
        permuteBlock(&data, false);

        if(amount != 8)
            data = data << (8 * (8 - amount));


        // Write output
        fwrite(&data, 1, amount, output);
        data = 0;
    }
    }
    else
    {
      
    while((amount = fread(&data, 1, 8, input)) > 0)
    {
        if(amount != 8)
            data = data << (8 * (8 - amount));

        // Initial permutation
        permuteBlock(&data, true);

        // Encrypt rounds
        if(encryptCheck==true)
        {
	  for(i = 0; i < 16; i++)
	  data_temp=data;
	    if (i=0)
	  {
	    
	    singleRoundDES2(XOR(IV,data), subKeyList[0]);
	  }
	  else
	  {
	    singleRoundDES2(XOR(singleRoundDES2(data, subKeyList[i-1]), data), subKeyList[i]);
	  }
            
                
        }
        // Decrypt rounds
        else
        {
            // Switching blocks
            data = (data << 32) + (data >> 32);

            for(i = 15; i >= 0; i--)
	    {
	      data_temp=data;
	      	  if (i=0)
	  {
	    XOR(singleRoundDES2(data, subKeyList[0]), IV);
	  }
	  else
	  {
	    XOR(singleRoundDES2(data, subKeyList[i-1]), singleRoundDES2(data, subKeyList[i-1]));
	  }
	    }
               
            
            // switching left block and right block
            data = (data << 32) + (data >> 32);
        }
}
    }
    

    fclose(input);
    fclose(output);

    return 0;
}
