.DEFAULT_GOAL = all
.PHONY = install \
         demo \
		 all run clean

CC = g++
FLAGS = -std=c++17 -pthread -Wall -O3
LIBS = -lstdc++fs \
	-lSDL2 -lSDL2_image \
	-lassimp  \
	-lGLEW -lGLU -lGL

main.o: main.cpp gfx.hpp
	@$(CC) \
		$(FLAGS) -c main.cpp

gfx.o: gfx.cpp gfx.hpp
	@$(CC) \
		$(FLAGS) -c gfx.cpp

input.o: input.cpp input.hpp
	$(CC) $(FLAGS) -c input.cpp

demo: main.o gfx.o input.o
	$(CC) -o demo \
	$(FLAGS) \
	main.o gfx.o input.o \
	$(LIBS)

install:
	sudo apt-get install \
		libsdl2-dev libsdl2-image-dev \
		libassimp-dev \
		libglew-dev libglm-dev \
		libsdl2-2.0-0 libsdl2-image-2.0-0 libglew2.1 libassimp4

all: demo

run: all
	@./demo  ./assets/stanford_dragon/dragon_vrip.ply 
	#./assets/Ogros.md2

clean:
	@rm -f *.o
	@rm -f demo