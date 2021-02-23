.DEFAULT_GOAL = run
.PHONY = run clean demo gfx/libgfx.so

INCLUDES_QT = -I/usr/include/x86_64-linux-gnu/qt5                   \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL   \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui

FLAGS = -std=c++17 $(INCLUDES_QT) -fPIC -Wall  #-O3

LIBS = -lstdc++fs -lstdc++ -ldl  \
       -lQt5Widgets -lQt5Core -lQt5Gui -lQt5OpenGL  \
       -lGL -lGLU -lGLEW     \
	   -lassimp

window.o: window.cpp window.hpp
	@$(CC) $(FLAGS) -c window.cpp

main.o: main.cpp window.o
	@$(CC) $(FLAGS) -c main.cpp

gfx/libgfx.so:
	@$(MAKE) -C gfx

demo: main.o gfx/libgfx.so
	$(CC)                       \
	$(FLAGS)  main.o window.o   \
	-Lgfx -lgfx $(LIBS)         \
    -o demo   

run: demo
	export LD_LIBRARY_PATH=/home/bastian/code/bgl-demo/src/:/home/bastian/code/bgl-demo/src/gfx; \
	echo $(LD_LIBRARY_PATH); \
	./demo ./assets/models/housemedieval.obj

clean:
	@$(MAKE) -C gfx clean
	@rm -f *.o
	@rm -f demo
	@rm -f screenshot.png
