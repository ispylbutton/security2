void bitwiseAddition(uint64_t from, uint64_t * block, int initPosition, int finPosition);

void permuteBlock(uint64_t * block, bool isInitialPermutation);

int XOR(int a, b);

void keyScheduler(uint64_t * initKey, uint64_t * nextKey, int roundNum);

void singleRoundDES(uint64_t * leftBlock, uint64_t key);
