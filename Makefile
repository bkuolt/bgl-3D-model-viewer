.DEFAULT_GOAL = all
.PHONY = install \
         demo \
		 all run clean

CC = g++
FLAGS = -std=c++17 -pthread -Wall -O3
LIBS = -lSDL2 -lSDL2_image -lstdc++fs -lassimp -lGLEW -lGLU -lGL

#--------------------------------------#

main.o: main.cpp gfx.hpp
	$(CC) $(FLAGS) -c main.cpp

gfx.o: gfx.cpp gfx.hpp
	$(CC) $(FLAGS) -c gfx.cpp

demo: main.o gfx.o
	$(CC) -o demo \
	$(FLAGS) \
	main.o gfx.o \
	$(LIBS)

#--------------------------------------#

install:
	sudo apt-get install \
		libsdl2-dev libsdl2-image-dev \
		libassimp-dev \
		libglew-dev

all: demo

run: all
	@./demo ./assets/Ogros.md2

clean:
	@rm -f *.o
	@rm -f demo