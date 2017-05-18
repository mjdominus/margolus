CFLAGS=-I/usr/include/SDL2 -g
LDLIBS=-lSDL2 -lGLEW -lGL -lglut

all: gas marg-try

marg-try: margolus.o marg-try.o
gas: margolus.o gas.o

margolus.o: margolus.c margolus.h
marg-try.o: marg-try.c margolus.h


