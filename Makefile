.DEFAULT_GOAL = all
.PHONY = install \
         demo \
		 all run clean

CC = g++-8
FLAGS = -std=c++17 -pthread -Wall -O3
LIBS = -lstdc++fs \
	-lSDL2 -lSDL2_image \
	-lassimp  \
	-lIL \
	-lGLEW -lGLU -lGL -lGLU

main.o: main.cpp
	@$(CC) \
		$(FLAGS) -c main.cpp

# ------------ libgfx ------------
gfx.o: gfx/gfx.cpp gfx/gfx.hpp
	@$(CC) \
		$(FLAGS) -c gfx/gfx.cpp

mesh.o: gfx/mesh.cpp gfx/mesh.hpp
	@$(CC) \
		$(FLAGS) -c gfx/mesh.cpp

shader.o: gfx/shader.cpp gfx/shader.hpp
	@$(CC) \
		$(FLAGS) -c gfx/shader.cpp

texture.o: gfx/texture.cpp gfx/texture.hpp
	@$(CC) \
		$(FLAGS) -c gfx/texture.cpp

# --------------------------------

input.o: input.cpp input.hpp
	$(CC) $(FLAGS) -c input.cpp

demo: main.o gfx.o input.o shader.o mesh.o texture.o
	$(CC) -o demo \
	$(FLAGS) \
	main.o gfx.o input.o shader.o mesh.o texture.o \
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
		libsdl2-2.0-0 libsdl2-image-2.0-0 libassimp4 \
		libdevil-dev

all: demo

run: all
	@./demo ./assets/MedHouse/housemedieval.obj 
	#./assets/dragon.ply
	#./assets/Ogros.md2

clean:
	@rm -f *.o
	@rm -f demo