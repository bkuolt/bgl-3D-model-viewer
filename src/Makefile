.DEFAULT_GOAL = run
.PHONY = demo run clean

INCLUDES_QT = -I/usr/include/x86_64-linux-gnu/qt5                   \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL   \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui

CC = g++
FLAGS = -std=c++17 -pthread            \
	    -DUSE_QT $(INCLUDES_QT) -fPIC  \
		-Wall -O3
# -fPIC is for QT only

LIBS = -lstdc++fs \
       -lQt5Widgets -lQt5Core -lQt5Gui -lGL \
	   -lassimp  \
	   -lGLEW -lGLU -lGL -lGLU
	
main.o: main.cpp
	@$(CC) $(FLAGS) -c main.cpp

gfx/libgfx.a:
	@$(MAKE) -C gfx

demo: main.o gfx/libgfx.a
	$(CC) -o demo   \
	$(FLAGS)        \
	main.o          \
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
