    uint64_t singleKey;
    singleKey=0;
    uint64_t initBit=0x8000000000000000;
    bool encrypt;
    encrypt=true;
    FILE * input;
    static FILE * output;
    ouput=NULL;
    
    input=NULL;
    uint64_t subKeyList[16];
    subKeyList[0] = singleKey;
    uint64_t nextKey;
    size_t amount; // Used for fwrite
    uint64_t data;
    int i;
  

    for(i = 0; i < 16; i++)
    {
        keyScheduler(&subKeyList[i], &nextKey, i);
        if(i != 15)
            subKeyList[i + 1] = nextKey;
    }


    while((amount = fread(&data, 1, 8, input)) > 0)
    {
        if(amount != 8)
            data = data << (8 * (8 - amount));

        // Initial permutation
        permuteBlock(&data, true);

        // Encrypt rounds
        if(encrypt)
        {
            for(i = 0; i < 16; i++)
                singleRoundDES(&data, subKeyList[i]);
        }
        // Decrypt rounds
        else
        {
            // Switching blocks
            data = (data << 32) + (data >> 32);

            for(int ii = 15; ii >= 0; ii--)
                singleRoundDES(&data, subKeyList[ii]);
            
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
