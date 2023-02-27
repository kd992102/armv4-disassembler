all: main.out

CC = gcc
INCLUDE = .
CFLAGS = -g
main.o: main.c typedef.h cpu.h cart.h arm_decode.h arm_instruction.h decode.h
	$(CC) -I $(INCLUDE) $(CFLAGS) -c main.c

main.out: main.o
	$(CC) -o main main.o

clean:
	del -fR main.o main.out