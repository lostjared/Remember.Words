OBJ=$(patsubst %.c,%.o,$(wildcard *.c))
PROGRAM=Remember.Words
CFLAGS= -std=c11 `sdl2-config --cflags`
SDL_PATH=/usr/local/lib
LDFLAGS= `sdl2-config --libs` -lSDL2_ttf
%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

$(PROGRAM): $(OBJ)
	$(CC) -o $(PROGRAM) $(OBJ) $(LDFLAGS)

all: $(PROGRAM)

clean:
	rm -f *.o $(PROGRAM)
