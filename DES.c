#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


/********************************************/
/* Tables for initial and final permutation */
/********************************************/

// initial permutation
char IP[64] = {
	58,	50,	42,	34,	26,	18,	10,	2,
	60,	52,	44,	36,	28,	20,	12,	4,
	62,	54,	46,	38,	30,	22,	14,	6,
	64,	56,	48,	40,	32,	24,	16,	8,
	57,	49,	41,	33,	25,	17,	9,	1,
	59,	51,	43,	35,	27,	19,	11,	3,
	61,	53,	45,	37,	29,	21,	13,	5,
	63,	55,	47,	39,	31,	23,	15,	7
};

// final permutation
char FP[64] = {
	40,	8,	48,	16,	56,	24,	64,	32,
	39,	7,	47,	15,	55,	23,	63,	31,
	38,	6,	46,	14,	54,	22,	62,	30,
	37,	5,	45,	13,	53,	21,	61,	29,
	36,	4,	44,	12,	52,	20,	60,	28,
	35,	3,	43,	11,	51,	19,	59,	27,
	34,	2,	42,	10,	50,	18,	58,	26,
	33,	1,	41,	9,	49,	17,	57,	25
};


/*******************************/
/* Tables for the key schedule */
/*******************************/

// PC-1 table (initial key permutation)
int PC1[56] = {
	57,	49,	41,	33,	25,	17,	9,
	1,	58,	50,	42,	34,	26,	18,
	10,	2,	59,	51,	43,	35,	27,
	19,	11,	3,	60,	52,	44,	36,
	63,	55,	47,	39,	31,	23,	15,
	7,	62,	54,	46,	38,	30,	22,
	14,	6,	61,	53,	45,	37,	29,
	21,	13,	5,	28,	20,	12,	4
};

// PC-2 table (permutation for generating each subkey)
int PC2[48] = {
	14,	17,	11,	24,	1,	5,	3,	28,
	15,	6,	21,	10,	23,	19,	12,	4,
	26,	8,	16,	7,	27,	20,	13,	2,
	41,	52,	31,	37,	47,	55,	30,	40,
	51,	45,	33,	48,	44,	49,	39,	56,
	34,	53,	46,	42,	50,	36,	29,	32
};


/*********************************/
/* Tables for the round function */
/*********************************/

// expansion box
char E[48] = {
	32,	1,	2,	3,	4,	5,
	4,	5,	6,	7,	8,	9,
	8,	9,	10,	11,	12,	13,
	12,	13,	14,	15,	16,	17,
	16,	17,	18,	19,	20,	21,
	20,	21,	22,	23,	24,	25,
	24,	25,	26,	27,	28,	29,
	28,	29,	30,	31,	32,	1
};

// substitution boxes
// addressable using xxyyyy where xx are the first and last bits, and yyyy are the middle 4 bits

/*
 * changed from original form to make traversal easier with coordinates
 */

const int SBox[8][4][16] = {
   {
   {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
   { 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
   { 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
   {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},
   },
 
   {
   {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
   { 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
   { 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
   {13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},
   },
 
   {
   {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
   {13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
   {13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
   { 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},
   },
 
   {
   { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
   {13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
   {10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
   { 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},
   },
 
   {
   { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
   {14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
   { 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
   {11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},
   },
 
   {
   {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
   {10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
   { 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
   { 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},
   },
 
   {
   { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
   {13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
   { 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
   { 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},
   },
 
   {
   {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
   { 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
   { 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
   { 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11},
   },
};

/**************************** start added code*********************************/

/*
 * IP(x) = L0R0
 * Li=Ri-1
 * Ri=Li-1 XOR f(Ri-1,Ki)
 * y=IP^-1R16L16
 * 
 * key generation
 * Ci=Si(Ci-1)
 * Di=Si(Di-1)
 * Ki=PC2(CiDi)
 * 
 * S=Left shift (shift 1 position if i=1,2,9,or 16, else shift 2 positions)
 * x<<y (left shift) takes 2 numbers, shifts the bits of x by y places to the left
 * x>>y (right shift) takes 2 numbers, shifts the bits of x by y places to the right
 * & is bitwise AND
 * */
const int shiftBy[16]={
  1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};

uint64_t initBit=0x8000000000000000; //1000

int P[32]={
  16, 7, 20, 21,
  29, 12, 28, 17,
  1, 15, 23, 26,
  5, 18, 31, 10,
  2, 8, 24, 14,
  32, 27, 3, 9,
  19, 13, 30, 6,
  22, 11, 4, 25
};




/*
 * simple function because I keep forgetting which character is used for XOR
 */

uint64_t XOR(uint64_t a, uint64_t b)
{
  return (a^b);
}

/*
 * from https://stackoverflow.com/questions/4068033/add-two-integers-using-only-bitwise-operators
 * As long as the bit that you're looking at isn't the initial bit (which is just 0, but in hex), 
 * then the 64bit block=itself+the value of 0 shifted by the number of places indicated by the final position wanted.
 */

void bitwiseAddition(uint64_t init, uint64_t * block, int initPosition, int finPosition)
{
  if (((init<<initPosition)&initBit)!=0)
  {
    *block+=(initBit>>finPosition);
  }
}




/*
 * if this is the first permutation, then use the IP block 
 * otherwise, use the FP block
 * block used to shift current 64bit block the indicated number of placed
 */

void permuteBlock(uint64_t * block, bool isInitialPermutation)
{
  uint64_t temp;
  int i;
  
  temp=0;
  
  for (i=0; i<64; i++)
  {
    if (isInitialPermutation==0)
    {
      bitwiseAddition(*block, &temp, IP[i]-1, i);
    }
    else
    {
      bitwiseAddition(*block, &temp, FP[i]-1, i);
    }
  }
  *block=temp;
}



/*
 * depending on the round number, 
 */

void keyScheduler(uint64_t * initKey, uint64_t * nextKey, int roundNum)
{
  uint64_t leftKey;
  uint64_t tempLeftKey;
  uint64_t rightKey;
  uint64_t tempRightKey;
  int i;
  
  leftKey=0;
  tempLeftKey=0;
  rightKey=0;
  tempRightKey=0;
  *nextKey=0;
  
  if (roundNum==0)
  {
    for (i=0; i<56; i++)
    {
      /*if the current position is in the first half of the 56 bit key, 
       * then if the current key shifted to the left by the number of place indicated by PC1 (for the first round) or just i
       * (for the following 15 rounds) is not equal to 0x8000000000000000, the left key will be shifted to the right by i positions
       * Otherwise, if the current position is not in the first half (i.e. in the second half),
       * this same algorithm is followed, except if the current key shifted to the left by the number 
       * of place indicated by PC1 is not equal to 0x8000000000000000, the right key will be shifted 
       * to the right by i%28 positions (i.e. starting over from 1, just in the second half, so 29 = 1)
       */
      if (i<28)
      {
	bitwiseAddition(* initKey, &leftKey, PC1[i]-1, i);
      }
      else
      {
	bitwiseAddition(* initKey, &rightKey, PC1[i]-1, i%28);
      }
    }
  }
  else
  {
    for (i=0; i<56; i++)
    {
      if (i<28)
      {
	bitwiseAddition(* initKey, &leftKey, i, i);
      }
      else
      {
	bitwiseAddition(* initKey, &rightKey, i, i%28);
      }
    }
  }

  //depending on whether the round indicates the key should shift by 1 or 2 bits, set the placeholder for the left/right key = the corresponding number of bits
  if (shiftBy[roundNum]==1)
  {
    tempLeftKey=initBit;//aka 1
    tempRightKey=initBit;
  }
  else
  {
    tempLeftKey=0xC000000000000000; //11 aka 2
    tempRightKey=0xC000000000000000;
  }
  //the left/right placeholder key is now equal to the bitwise AND of the left/right key and 1 or 2 shifted to the right by either 27 or 26
  tempLeftKey=(leftKey&tempLeftKey)>>(28-shiftBy[roundNum]);
  tempRightKey=(rightKey&tempRightKey)>>(28-shiftBy[roundNum]);
  
  
  //placeholder key equals itself added to the actual key shifted to the left by 1 or 2 bits
  tempLeftKey+=(leftKey<<shiftBy[roundNum]);
  tempRightKey+=(rightKey<<shiftBy[roundNum]);
  //same as beginning, except with new key ordering
  for (i=0; i<56; i++)
  {
    if (i<28)
    {
      bitwiseAddition(tempLeftKey, nextKey, i, i);
    }
    else
    {
      bitwiseAddition(tempRightKey, nextKey, i%28, i);
    }
  }
  //resets initkey
  *initKey=0;
  /*as long as *nextKey shifted to the left by the number of place indicated by PC2 is not equal to 0x8000000000000000,
   * the new initkey (0) is shifted to the right i places
   */
  
  for (i=0; i<48; i++)
  {
    bitwiseAddition(*nextKey, initKey, PC2[i]-1, i);
  }
};

/*
 * dummy function
 */
uint64_t singleRoundDES2(uint64_t * leftBlock, uint64_t key)
{
  return key;
}

/*
 * runs a single round of DES 
 */

void singleRoundDES(uint64_t * leftBlock, uint64_t key)
{
  uint64_t rightBlock;
  uint64_t tempRightBlock;
  uint64_t subBlock;
  int i;
  int j;
  int x;
  int y;
  
  rightBlock=0;
  tempRightBlock=0;
  
  
  /*
   * Let E be such that the 48 bits of its output, written as 8 blocks of 6 bits each, are obtained by selecting the bits in its inputs in order
   * E[i]+31-->32 bits
   * 6*i
   */
  
  for (i=0; i<48; i++)
  {
    bitwiseAddition(* leftBlock, &rightBlock, (E[i]+31), i);
  }
  
  rightBlock=XOR(rightBlock,key);
  
  for (i=0; i<8; i++)
  {
    if ((((rightBlock<<6*i)&initBit))==initBit)
    {
      x=2;
    }
    else
    {
      x=0;
    }

    y=0;
    
    if (((rightBlock<<(6*i+5))&initBit)==initBit)
    {
      x++;
    }
    
    for (j=1; j<5; j++)
    {
      if (((rightBlock<<(6*i+j))&initBit)==initBit)
      {
	//basically just shifts the y coordinate to the left or right by 1 place
	y+=(2^(4-j));
      }
    }
    /*
     * gets specific
     */
    subBlock=SBox[i][x][y];
    subBlock=(subBlock<<(4-(4*i)));
    tempRightBlock+=subBlock;
  }
  
  rightBlock=tempRightBlock;
  
  tempRightBlock=0;
  
  for (i=0; i<32; i++)
  {
    bitwiseAddition(rightBlock, &tempRightBlock, P[i]-1, i);
  }
  
  rightBlock=tempRightBlock;
  
  rightBlock=XOR(rightBlock,* leftBlock);
  
  *leftBlock=(*leftBlock<<32)+(rightBlock>>32);
};
