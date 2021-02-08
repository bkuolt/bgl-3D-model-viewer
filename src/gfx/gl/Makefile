.DEFAULT_GOAL = libgl.la
.PHONY = clean

CC = g++-8
FLAGS = -std=c++17 -pthread -Wall -O3

shader.o: shader.cpp shader.hpp
	@$(CC) $(FLAGS) -c shader.cpp

texture.o: texture.cpp texture.hpp
	@$(CC) $(FLAGS) -c texture.cpp

mesh.o: mesh.cpp mesh.hpp buffer.hpp
	@$(CC) $(FLAGS) -c mesh.cpp

libgl.la: shader.o mesh.o texture.o
	ar rcs libgl.a *.o

clean:
	@rm -f *.o
	@rm -f *.a