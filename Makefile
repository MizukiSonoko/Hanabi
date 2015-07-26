
CC=gcc
OPTION=-lncurses
TARGET=hanabi

all: hanabi.o
	$(CC)  hanabi.o -o $(TARGET) $(OPTION)

hanabi.o: hanabi.c
	$(CC) -c hanabi.c $(OPTION)

clean:
	rm  -f *.o $(TARGET)
