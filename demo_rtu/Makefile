
SRC_DIR = .
OBJ_DIR = .

SRC = $(wildcard ${SRC_DIR}/*.c)
OBJ = $(patsubst %.c, ${OBJ_DIR}/%.o, $(notdir ${SRC}))

TARGET = $(patsubst %.c, %, $(notdir ${SRC}))

CC = gcc
CFLAGS = -Wall -O3
CFLAGS += $(shell pkg-config libmodbus --cflags)
LDFLAGS = $(shell pkg-config libmodbus --libs)

all:${TARGET}

${TARGET}:%:%.o
	$(CC) $^ ${LDFLAGS} -o $@

${OBJ}:%.o:%.c
	${CC} -c $^ ${CFLAGS} -o $@

clean:
	rm -rf *.o ${TARGET}

.PHONY: all clean
