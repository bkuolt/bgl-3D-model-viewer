.DEFAULT_GOAL = libgfx.a
.PHONY = libgfx.a clean

CC = g++-8
FLAGS = -std=c++17 -pthread -Wall -O3 -Igl/

gfx.o: gfx.cpp gfx.hpp
	@$(CC) $(FLAGS) -c gfx.cpp

camera.o: camera.cpp camera.hpp
	@$(CC) $(FLAGS) -c camera.cpp

window.o: window.cpp window.hpp
	@$(CC) $(FLAGS) -c window.cpp

gl/libgl.a:
	@$(MAKE) -C gl
	@$(MAKE) -C gui

libgfx.a: gl/libgl.a\
          camera.o window.o gfx.o
	ar rcs libgfx.a *.o

clean:
	@$(MAKE) -C gl clean
	@rm -f *.o
	@rm -f *.a