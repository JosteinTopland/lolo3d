SRCDIR  = src/
SRCS    = $(wildcard $(SRCDIR)*.c)
OBJS    = $(notdir $(SRCS:.c=.o))
OUT     = lolo3d

CC      = gcc
CFLAGS  = $(shell sdl2-config --cflags) -Wall -std=c99 -g -pedantic
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_mixer -lGLEW -lm

#os specifics
OS = $(shell uname)
ifeq ($(OS),Windows_NT)
	LDFLAGS += -lGL
else ifeq ($(OS),Linux)
	LDFLAGS += -lGL
else ifeq ($(OS),Darwin)
	LDFLAGS += -framework OpenGL
endif

#link
$(OUT): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

#compile
%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(OBJS) $(OUT)
