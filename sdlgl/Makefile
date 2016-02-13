LIBS=`pkg-config sdl2 glew`
CFLAGS=-Wall -ggdb

sdlgl.a: window.o draw.o load.o debug.o container.o
	ar rvs $@ $^

window.o: window.c
	$(CC) $^ -c -o $@ $(CFLAGS)

draw.o: draw.c
	$(CC) $^ -c -o $@ $(CFLAGS)

load.o: load.c
	$(CC) $^ -c -o $@ $(CFLAGS)

debug.o: debug.c
	$(CC) $^ -c -o $@ $(CFLAGS)

container.o: container.c
	$(CC) $^ -c -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm ./*.o
	rm sdlgl.a
