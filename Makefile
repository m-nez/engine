CC=gcc
CFALGS=-Wall -ggdb
LIBS=`pkg-config --cflags --libs sdl2 glew SDL2_image`
LIBS+=-lm
STATIC=sdlgl/sdlgl.a

default: main
.PHONY: main

main: main.o
	cd sdlgl && $(MAKE)
	$(CC) $^ $(STATIC) -o $@ $(LIBS) $(CFLAGS)

.PHONY: test

test: main
	cd test && $(MAKE) && ./load_test

.PHONY: clean
clean:
	rm ./*.o
