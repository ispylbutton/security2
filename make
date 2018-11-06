APP=DES
#
CFLAGS=-std=c99


#Rules and targets
all: $(APP)

$(APP): DES.o main.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c DES.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

DES.o: DES.c DES.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	@rm -f *.o $(APP) genkey
