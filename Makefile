all: main.out

CC = gcc
INCLUDE = .
CFLAGS = -g
main.o: main.c typedef.h arm_decode.h arm_instruction.h decode.h thumb_decode.h thumb_instruction.h
	$(CC) -I $(INCLUDE) $(CFLAGS) -c main.c

main.out: main.o
	$(CC) -o main main.o

clean:
	del -fR main.o main.out