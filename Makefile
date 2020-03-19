.DEFAULT_GOAL = all
.PHONY = install \
         demo \
		 all run clean

CC = g++-8
FLAGS = -std=c++17 -pthread -Wall -O3
LIBS = -lstdc++fs \
	-lSDL2 -lSDL2_image \
	-lassimp  \
	-lGLEW -lGLU -lGL -lGLU

main.o: main.cpp
	@$(CC) \
		$(FLAGS) -c main.cpp

input.o: input.cpp input.hpp
	$(CC) $(FLAGS) -c input.cpp

gfx/libgfx.a:
	@$(MAKE) -C gfx

demo: main.o input.o gfx/libgfx.a
	$(CC) -o demo \
	$(FLAGS) \
	main.o input.o \
	-Lgfx -lgfx \
	$(LIBS)

install:
	sudo apt-get install g++-8
	sudo add-apt-repository main
	sudo add-apt-repository universe
	sudo add-apt-repository restricted
	sudo add-apt-repository multiverse
	sudo apt-get install \
		libsdl2-dev libsdl2-image-dev \
		libassimp-dev \
		libglew-dev libglm-dev \
		libsdl2-2.0-0 libsdl2-image-2.0-0 libassimp4

all: demo

run: all
	@./demo  ./assets/dragon.ply
	#./assets/Ogros.md2

clean:
	@$(MAKE) -C gfx clean
	@rm -f *.o
	@rm -f demo