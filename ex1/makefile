CC = gcc
OBJ = P1 ENC1 P2.o ENC2.o CHAN.o CHAN ENC1.o P1.o Memory.o semaph.o ENC2 P2
all:  P1 ENC1 CHAN ENC2 P2



CHAN: CHAN.c Memory.o semaph.o
	$(CC) -o CHAN CHAN.c Memory.o semaph.o

P1:Memory.o P1.o semaph.o
	$(CC) -o P1 P1.o Memory.o semaph.o

P2:Memory.o P2.o semaph.o
	$(CC) -o P2 P2.o Memory.o semaph.o

ENC1:Memory.o ENC1.o semaph.o
	$(CC) -o ENC1 Memory.o ENC1.o semaph.o

ENC2:Memory.o ENC2.o semaph.o
	$(CC) -o ENC2 Memory.o ENC2.o semaph.o



Memory.o:Memory.c
		$(CC)  Memory.c -c

CHAN.o:CHAN.c
		$(CC)  CHAN.c -c


P1.o:P1.c
	$(CC)  P1.c  -c

P2.o:P2.c
	$(CC)  P2.c  -c

ENC1.o:ENC1.c
	$(CC)  ENC1.c  -c

ENC2.o:ENC2.c
	$(CC)  ENC2.c -c

semaph.o: semaph.c
	$(CC)  semaph.c -c


clean:
	rm -f $(OBJ)
