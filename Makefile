SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf




main: main.o
	g++ main.o -o main $(SDL_LDFLAGS)
main.o: main.cpp
	g++ -c $(SDL_CFLAGS) main.cpp
clean:
	rm -f main main.o