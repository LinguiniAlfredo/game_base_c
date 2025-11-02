
OBJS = main.c

CC = gcc
OPTIONS = -Wall -g -pedantic
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf  -lm
EXE = game

all : $(OBJS)
	$(CC) $(OPTIONS) $(OBJS) $(LINKER_FLAGS) -o $(EXE)

clean :
	rm ./game

count :
	find . -type f -not -path "./.git/*" -not -path "./resources/*" -print0 | xargs -0 wc -l

