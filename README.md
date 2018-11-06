# security2

To build: 

make clean
make all

to run encrypt:

./DES -e -i <IV> -k <key> <plain text file>
    defaults output to outputfile.txt
  
to run decrypt:

 ./DES -d -o <new output file> -i <IV> -k <key> <encrypted file>
