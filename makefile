CC = gcc
CFLAGS = -O2 -g
OBJ = dynamem.o example.o

all: example

clean:
	-rm *.o

example: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
