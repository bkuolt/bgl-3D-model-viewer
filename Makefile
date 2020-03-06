.DEFAULT_GOAL = all
.PHONY = install \
         demo \
		 all run clean

CC = g++
FLAGS = -std=c++17 -pthread -Wall -O3
LIBS = -lSDL2 -lSDL2_image -lstdc++fs

#--------------------------------------#

main.o: main.cpp
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
	sudo apt-get install libsdl2-dev libsdl2-image-dev

all: demo

run: all
	@./demo

clean:
	@rm -f *.o
	@rm -f demo