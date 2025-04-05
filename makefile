CC=g++
FLAGS=-I$(IDIR) -Wall -Wextra -O3 -g -std=c++23

SDIR=src
ODIR=obj
IDIR =inc

LIBS=-lraylib

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o sparseSet.o ecs.o quadtree.o player.o hex.o audioManager.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

.PHONY $(ODIR):


$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS) $(ODIR)
	$(CC) -c -o $@ $< $(FLAGS) $(LIBS) 

$(ODIR):
	mkdir -p $(ODIR)

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
