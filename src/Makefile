.DEFAULT_GOAL = run
.PHONY = demo run clean

CC = g++-8
FLAGS = -std=c++17 -pthread \
	    -DUSE_SDL2 \
        -Wall -O3 
LIBS = -lstdc++fs \
       -lSDL2  \
	   -lassimp  \
	   -lIL \
	   -lGLEW -lGLU -lGL -lGLU \
	   -lboost_timer



main.o: main.cpp \
        gfx/gfx.hpp gfx/camera.hpp gfx/camera.cpp
	@$(CC) $(FLAGS) -c main.cpp

gfx/libgfx.a:
	@$(MAKE) -C gfx

demo: main.o gfx/libgfx.a gfx/gl/libgl.a gfx/gui/libgui.a  # TODO: simplify
	$(CC) -o demo \
	$(FLAGS) \
	main.o \
	-Lgfx -lgfx     \
	-Lgfx/gl -lgl   \
	-Lgfx/gui -lgui \
	$(LIBS)

run: demo
	@./demo ./assets/models/housemedieval.obj

clean:
	@$(MAKE) -C gfx clean
	@rm -f *.o
	@rm -f demo
