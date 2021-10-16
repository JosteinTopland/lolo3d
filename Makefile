CC=gcc
CFLAGS=`sdl2-config --cflags` -Wall
LDFLAGS=`sdl2-config --libs` -lSDL2_mixer -lGL -lGLEW -lm

lolo3d: lolo3d.o globals.o obj_loader.o render.o shader_loader.o
	$(CC) -o lolo3d lolo3d.o globals.o obj_loader.o render.o shader_loader.o $(LDFLAGS)

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

clean:
	rm -f *.o lolo3d
