.DEFAULT_GOAL = libgl.la
.PHONY = clean

CC = g++
FLAGS = -std=c++17 -pthread -Wall -fPIC -O3

shader.o: shader.cpp shader.hpp
	@$(CC) $(FLAGS) -c shader.cpp

texture.o: texture.cpp texture.hpp
	@$(CC) $(FLAGS) -c texture.cpp

mesh.o: mesh.cpp mesh.hpp buffer.hpp
	@$(CC) $(FLAGS) -c mesh.cpp

box.o: box.cpp box.hpp
	@$(CC) $(FLAGS) -c box.cpp

libgl.la: texture.o shader.o mesh.o box.o
	ar rcs libgl.a *.o

clean:
	@rm -f *.o
	@rm -f *.a