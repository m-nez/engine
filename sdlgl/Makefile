CC=gcc
LIBS=`pkg-config sdl2 glew SDL2_image` -lm
CFLAGS=-Wall -ggdb -pedantic

SDLGL_DEPS=\
		   window.o \
		   draw.o \
		   load.o debug.o \
		   container.o \
		   data_types.o \
		   anim.o \
		   hash_map.o \
		   assets.o \
		   model.o
sdlgl.a: $(SDLGL_DEPS)
	ar rvs $@ $^

window.o: window.c window.h
	$(CC) $< -c -o $@ $(CFLAGS)

draw.o: draw.c draw.h
	$(CC) $< -c -o $@ $(CFLAGS)

load.o: load.c load.h
	$(CC) $< -c -o $@ $(CFLAGS)

debug.o: debug.c debug.h
	$(CC) $< -c -o $@ $(CFLAGS)

container.o: container.c container.h
	$(CC) $< -c -o $@ $(CFLAGS)

data_types.o: data_types.c data_types.h
	$(CC) $< -c -o $@ $(CFLAGS)

anim.o: anim.c anim.h
	$(CC) $< -c -o $@ $(CFLAGS)

hash_map.o: hash_map.c hash_map.h
	$(CC) $< -c -o $@ $(CFLAGS)

assets.o: assets.c assets.h
	$(CC) $< -c -o $@ $(CFLAGS)

model.o: model.c model.h
	$(CC) $< -c -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm ./*.o
	rm sdlgl.a
