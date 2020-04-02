.DEFAULT_GOAL = all
.PHONY = all clean

CC = g++-8
FLAGS = -std=c++17 -pthread -Wall -O3

gfx.o: gfx.cpp gfx.hpp \
       buffer.hpp gl.hpp
	@$(CC) $(FLAGS) -c gfx.cpp

camera.o: camera.cpp camera.hpp
	@$(CC) $(FLAGS) -c camera.cpp

mesh.o: mesh.cpp mesh.hpp \
        buffer.hpp gl.hpp
	@$(CC) $(FLAGS) -c mesh.cpp

shader.o: shader.cpp shader.hpp \
		  gl.hpp
	@$(CC) $(FLAGS) -c shader.cpp

texture.o: texture.cpp texture.hpp \
           gl.hpp
	@$(CC) $(FLAGS) -c texture.cpp

window.o: window.cpp window.hpp
	@$(CC) $(FLAGS) -c window.cpp

libgfx.la: gfx.o shader.o mesh.o texture.o camera.o window.o
	ar rcs libgfx.a *.o

all: libgfx.la

clean:
	@rm -f *.o
	@rm -f *.a