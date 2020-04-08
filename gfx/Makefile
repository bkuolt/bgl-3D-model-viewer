.DEFAULT_GOAL = all
.PHONY = all clean

CC = g++-8
FLAGS = -std=c++17 -pthread -Wall -O3 -I./gl/

gfx.o: gfx.cpp gfx.hpp
	@$(CC) $(FLAGS) -c gfx.cpp

camera.o: camera.cpp camera.hpp
	@$(CC) $(FLAGS) -c camera.cpp

window.o: window.cpp window.hpp
	@$(CC) $(FLAGS) -c window.cpp

gl/libgl.a:
	@$(MAKE) -C gl

libgfx.a: camera.o window.o gfx.o
	ar rcs libgfx.a *.o

all: gl/libgl.a libgfx.a

clean:
	@$(MAKE) -C gl clean
	@rm -f *.o
	@rm -f *.a