CC = gcc
OBJ = os2 main.o  hash.o frame.o
all:  os2



os2: main.o  hash.o frame.o
	$(CC) -o os2 main.o  hash.o frame.o



main.o:main.c
		$(CC)  main.c -c

frame.o:frame.c
		$(CC)  frame.c -c

hash.o:hash.c
		$(CC)  hash.c -c






clean:
	rm -f $(OBJ)
