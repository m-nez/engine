CFALGS=-Wall -ggdb
LIBS=`pkg-config --cflags --libs sdl2 glew`
main: main.o sdlgl/sdlgl.a
	$(CC) $^ -o $@ $(LIBS)

.PHONY: clean
clean:
	rm ./*.o
