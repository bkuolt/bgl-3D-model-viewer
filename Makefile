.DEFAULT_GOAL = demo
.PHONY = gfx/libgfx.a \
         demo \
		 all run clean

CC = g++-8
FLAGS = -std=c++17 -pthread -Wall -O3
LIBS = -lstdc++fs \
	-lSDL2 -lSDL2_image \
	-lassimp  \
	-lIL \
	-lGLEW -lGLU -lGL -lGLU \
	-lboost_timer

main.o: main.cpp \
        App.hpp gfx/gfx.hpp gfx/camera.hpp gfx/camera.cpp
	@$(CC) \
		$(FLAGS) -c main.cpp

input.o: input.cpp input.hpp \
         App.hpp gfx/gfx.hpp
	$(CC) $(FLAGS) -c input.cpp

gfx/libgfx.a:
	@$(MAKE) -C gfx

demo: main.o input.o \
      gfx/libgfx.a gfx/gl/libgl.a
	$(CC) -o demo \
	$(FLAGS) \
	main.o input.o \
	-Lgfx -lgfx -Lgfx/gl -lgl  \
	$(LIBS)

run: demo
	@./demo ./assets/models/housemedieval.obj

clean:
	@$(MAKE) -C gfx clean
	@rm -f *.png
	@rm -f *.o
	@rm -f demo