CC=gcc
CFALGS=-Wall -ggdb
LIBS=`pkg-config --cflags --libs sdl2 glew SDL2_image`
LIBS+=-lm
STATIC=sdlgl/sdlgl.a

default: all

all:
	cd sdlgl && $(MAKE)
	$(MAKE) main.o
	$(CC) main.o $(STATIC) -o $@ $(LIBS) $(CFLAGS)

main.o: main.c
	$(CC) -c $^ -o $@ $(LIBS) $(CFLAGS)

.PHONY: clean
clean:
	rm ./*.o
