#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h> 
#include <time.h>
#include "DES.h"


//works, but was not fully implemented
static void generateRandomKey(uint64_t * key)
{
    uint64_t initBit;
    initBit=0x8000000000000000;
    int i;
    srand(time(NULL));

    for(i = 0; i < 64; i++) 
    {
            if (rand()%2==1)
            {
	      *key+=(initBit>>i);
            }
    }
}

int main(int argc, char * argv[])
{
  static FILE * outputFile=NULL;
  uint64_t key;
  uint64_t IV;
  uint64_t initBit;
  uint64_t singleKey;
  uint64_t nextKey;
  uint64_t data;
  uint64_t data_temp;
  uint64_t subKeyList[16];
  size_t itemNumber;
  bool show;
  bool encryptCheck;
  int i;
  int optionCheck;

  FILE * inputFile=NULL;
  
  key=0;
  initBit=0x8000000000000000;
  singleKey=0;
  subKeyList[0]=singleKey;
  show=false;
  encryptCheck;
  optionCheck=0;
  
  /*command line arguments
   * https://stackoverflow.com/questions/192249/how-do-i-parse-command-line-arguments-in-bash
   * http://wiki.bash-hackers.org/howto/getopts_tutorial
   */
  
  const struct option full[]=
  {
    {"encrypt",        no_argument, NULL, 'e'},
    {"decrypt",        no_argument, NULL, 'd'},
    {"output",   required_argument, NULL, 'o'},
    {"key",      required_argument, NULL, 'k'},
    {"IV",	 required_argument, NULL, 'i'},
    {NULL,                       0, NULL,   0}
  };
  
  const char * abbr="edk:i:o:";
  while((optionCheck=getopt_long(argc, argv, abbr, full, NULL))!=-1)
  {
    switch (optionCheck)
    {
      case 'e': //encrypt
	encryptCheck=true;
	if (argv[optind]==NULL)
	{
	  fprintf(stderr, "No input file provided\n");
	  return 1;
	}
	break;
      case 'd': //decrypt
	encryptCheck=false;
	if (argv[optind]==NULL)
	{
	  fprintf(stderr, "No input file provided\n");
	  return 1;
	}
	break;
      case 'o': //specify output file destination
	outputFile=fopen(optarg, "w");
	if (outputFile==NULL)
	{
	  fprintf(stderr, "Incorrect permissions for output file\n");
	  return 1;
	}
	break;
      case 'i': //IV
	if (optarg)
	{
	  for (i=0; optarg[i]!='\0'; i++)
	  {

	    
	    if (optarg[i]=='1')
	    {
	      IV+=(initBit>>i);
	    }
	  }
	  if (IV==0)
	  {
	    fprintf(stderr, "No IV provided\n");
	    return 1;
	  }
	}
	break;
      case 'k': //key
	if (optarg)
	{
	  for (i=0; optarg[i]!='\0'; i++)
	  {

	    //if the next number in the key string is 1, then 1 is added to the stored key in the position shifted i to the right
	    if (optarg[i]=='1')
	    {
	      key+=(initBit>>i);
	    }
	  }
	  if (key==0)
	  {
	    fprintf(stderr, "No key provided\n");
	    return 1;
	  }
	}

    }
  }
  
  inputFile=fopen(argv[optind], "rb");
  
  if (inputFile==NULL)
  {
    fprintf(stderr, "Incorrect permissions for input file\n");
    return 1;
  }
  //if no file is specified with -o, output is written to default output file
  if (outputFile==NULL)
  {
    outputFile=fopen("outputFile.txt", "w");
  }
  
  for (i=0; i<=15; i++)
  {
    keyScheduler(&subKeyList[i], &nextKey, i);
    
    if (i!=15)
    {
      subKeyList[i+1]=nextKey;
    }
  }
  
  
  //EBC mode
  if (show==false)
  {
    while ((itemNumber=fread(&data, 1, 8, inputFile))>0)
    {
      if (itemNumber!=8)
      {
	data=data<<(8*(8-itemNumber));
      }
      // Initial permutation
      permuteBlock(&data, true);
      
      //if encrypting
      if (encryptCheck==true)
      {
	for (i=0; i<=15; i++)
	{
	  singleRoundDES(&data, subKeyList[i]);
	}
      }
      //if decrypting
      else
      {
	i=15;
	// Switches left and right blocks
	data=(data<<32)+(data>>32);
	
	while (i>=0)
	{
	  singleRoundDES(&data, subKeyList[i]);
	  i=i-1;
	}
	
	//Switches blocks back
	data=(data<<32)+(data>>32);
      }
      
      //On final permutation
      permuteBlock(&data, false);
      
      if (itemNumber!=8)
      {
	data=data<<(8*(8-itemNumber));
      }
      
      fwrite(&data, 1, itemNumber, outputFile);
      data=0;
    }
  }
  else
  {
    while ((itemNumber=fread(&data, 1, 8, inputFile))>0)
    {
      if (itemNumber!=8)
      {
	data=data<<(8*(8-itemNumber));
      }
      
      //intial permutation
      permuteBlock(&data, true);
      
      //if encrypting
      if (encryptCheck==true)
      {
	for (i-0; i<=15; i++)
	{
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
      }
      //if decrypting
      else
      {
	i=15;
	// Switches left and right blocks
	data=(data<<32)+(data>>32);
	
	while (i>=0)
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
	  i=i-1;
	}
	//Switches blocks back
	data=(data<<32)+(data>>32);	
      }      
    }
  }
  
  fclose(inputFile);
  fclose(outputFile);
  
  return 0; 
}
