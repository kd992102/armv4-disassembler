all: main.out

CC = gcc
INCLUDE = .
CFLAGS = -g
main.out: main.o
	$(CC) -o main.out main.o
main.o: main.c typedef.h cpu.h cart.h decode.h arm_decode.h arm_instruction.h
	$(CC) -I$(INCLUDE) $(CFLAGS) main.c

clean:
	del -fR main.o main.out