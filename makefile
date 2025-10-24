
OBJS = main.c

CC = gcc
OPTIONS = -Wall -g -pedantic
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf 
EXE = game

all : $(OBJS)
	$(CC) $(OPTIONS) $(OBJS) $(LINKER_FLAGS) -o $(EXE)

clean :
	rm ./game

