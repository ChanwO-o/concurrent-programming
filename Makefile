CC=gcc

#Compile with: 
#-g = debug
#-m32 = 32bit x86 executable
CFLAGS=-lpthread


all: main

main: main.o
	$(CC) $(CFLAGS) main.o -o main

main.o: main.c
	$(CC) -c $(CFLAGS) main.c

clean:
	rm *.o main


