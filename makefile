CC = gcc
CFLAGS = -O3

OBJS = obj/otp.o
BIN = bin/otpcrypt

all: ${OBJS} ${BIN}

bin/otpcrypt: src/otpcrypt.c src/otp.h obj/otp.o
	${CC} ${CFLAGS} src/otpcrypt.c obj/otp.o -o bin/otpcrypt

obj/otp.o: src/otp.c src/otp.h
	${CC} ${CFLAGS} -c src/otp.c -o obj/otp.o

clean:
	rm -f ${OBJS} ${BIN}

