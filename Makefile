# compiler flags
CFLAGS = -g -o
all: main.c
    gcc -$(CFLAGS) mysh main.c  