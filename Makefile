.DEFAULT_GOAL = all
.PHONY = install \
         demo \
		 all run clean

CC = g++
FLAGS = -std=c++17 -pthread -Wall -O3
LIBS =  -lstdc++fs \
	-lSDL2 -lSDL2_image \
	-lassimp \
	-lGLEW -lGLU -lGL

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
		libsdl2-dev \        # windowing toolkit
		libsdl2-image-dev \  # image loader
		libassimp-dev \      # 3D model loader
		libglew-dev \        # OpenGL support
		libglm-dev           # math library

all: demo

run: all
	@./demo ./assets/Ogros.md2

clean:
	@rm -f *.o
	@rm -f demo