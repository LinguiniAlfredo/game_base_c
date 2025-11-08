OBJS = main.c
CC = gcc
MINGW_CC = x86_64-w64-mingw32-gcc
OPTIONS = -Wall -g -pedantic
RELEASE_OPTIONS = -O2 -s -Wall -pedantic
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
MINGW_LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm -mwindows
EXE = game

dev : $(OBJS)
	$(CC) $(OPTIONS) $(OBJS) $(LINKER_FLAGS) -o $(EXE)

release : $(OBJS)
	$(CC) $(RELEASE_OPTIONS) $(OBJS) $(LINKER_FLAGS) -o $(EXE)

windows : $(OBJS)
	$(MINGW_CC) $(RELEASE_OPTIONS) $(OBJS) $(MINGW_LINKER_FLAGS) -o $(EXE).exe

clean :
	rm -f ./game ./game.exe

count :
	find . -type f -not -path "./.git/*" -not -path "./resources/*" -print0 | xargs -0 wc -l
