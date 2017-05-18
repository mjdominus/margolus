CFLAGS=-I/usr/include/SDL2 -O
LDLIBS=-lSDL2 -lGLEW -lGL -lglut

all: gas

gas: margolus.o gas.o

margolus.o: margolus.c margolus.h


