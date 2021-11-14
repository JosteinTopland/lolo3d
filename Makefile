CC=gcc
CFLAGS=`sdl2-config --cflags` -Wall
LDFLAGS=`sdl2-config --libs` -lSDL2_mixer -lGLEW -lm
OS=$(shell uname)
ifeq ($(OS), Windows_NT)
	LDFLAGS+= -lGL
else ifeq ($(OS), Darwin)
	LDFLAGS+= -framework OpenGL
endif

lolo3d: lolo3d.o globals.o obj_loader.o render.o shader_loader.o level.o input.o
	$(CC) -o lolo3d lolo3d.o globals.o obj_loader.o render.o shader_loader.o level.o input.o $(LDFLAGS)

lolo3d.o : src/lolo3d.c
	$(CC) -c $(CFLAGS) src/lolo3d.c

globals.o : src/globals.c
	$(CC) -c $(CFLAGS) src/globals.c

obj_loader.o : src/obj_loader.c
	$(CC) -c $(CFLAGS) src/obj_loader.c

render.o : src/render.c
	$(CC) -c $(CFLAGS) src/render.c

shader_loader.o : src/shader_loader.c
	$(CC) -c $(CFLAGS) src/shader_loader.c

level.o : src/level.c
	$(CC) -c $(CFLAGS) src/level.c

input.o : src/input.c
	$(CC) -c $(CFLAGS) src/input.c

clean:
	rm -f *.o lolo3d
