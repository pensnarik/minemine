CC=gcc
PROGS=game

CFLAGS=`sdl-config --cflags`
LDFLAGS=`sdl-config --libs` -lpng -lglut -lGL -lGLU

all: $(PROGS)
	
game:	gl_png.o main.o block.o
	$(CC) $(CFLAGS) $(LDFLAGS) main.o gl_png.o -o game
	
main.o: main.cpp
	$(CC) -c $(CFLAGS) $(LDFLAGS) main.cpp
	
gl_png.o: gl_png.cpp
	$(CC) -c $(CFLAGS) $(LDFLAGS) gl_png.cpp

block.o: block.cpp
	$(CC) -c $(CFLAGS) $(LDFLAGS) block.cpp
	
clean:
	rm -rf *.o game			
