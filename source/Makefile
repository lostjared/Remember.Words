OBJ=$(patsubst %.c,%.o,$(wildcard *.c))
PROGRAM=Remember.Words
CFLAGS= -std=c11 `sdl2-config --cflags`
SDL_PATH=/usr/local/lib
LDFLAGS= `sdl2-config --static-libs` -lz -lbz2 -lpng -lfreetype -lSDL2_ttf
%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

$(PROGRAM): $(OBJ)
	$(CC) -o $(PROGRAM) $(OBJ) $(LDFLAGS)

all: $(PROGRAM)

clean:
	rm -f *.o $(PROGRAM)