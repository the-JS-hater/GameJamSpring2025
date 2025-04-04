CC=g++
FLAGS=-I$(IDIR) -Wall -Wextra -O3 -g -std=c++23

SDIR=src
ODIR=obj
IDIR =inc

LIBS=-lraylib

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o sparseSet.o ecs.o quadtree.o player.o hex.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS) -fcompare-debug-second

game.exe: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

run: game.exe
	./game.exe

debug: game.exe
	gdb ./game.exe

cachetest: game.exe
	valgrind --tool=cachegrind ./game.exe 

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm game.exe
	rm cachegrind.*
