CXX?=c++
SDL2FLAGS=$(shell pkg-config sdl2 --cflags --libs)
CXXFLAGS?=-std=c++11 -lGLEW -lGL -lGLU
DEBUG=-Wall -pedantic -Werror -Wshadow -Wstrict-aliasing -Wstrict-overflow
LODEPNG_FLAGS = resources/LodePNG/lodepng.cpp -ansi -O3 -std=c++11

INCLUDE_SDL_TTF=-I /usr/local/lib
GL_FLAGS = -lglut -lGLEW -lGL -lGLU

.PHONY: all msg clean fullclean

all: msg main

msg:
	@echo 'Compiling'


### -lSDL2_image -lSDL2_ttf
### ${CXX} ${CXXFLAGS} main.cc `sdl2-config --libs --cflags` -lGL -Wall

main: main.cc resources/*.h
	${CXX} ${CXXFLAGS} ${GL_FLAGS} -O3 -lSDL2  -o $@ $< ${SDL2FLAGS} $(LODEPNG_FLAGS)


run: msg main
	time ./main

clean:
	rm -f main *.o main.asm

fullclean: clean
