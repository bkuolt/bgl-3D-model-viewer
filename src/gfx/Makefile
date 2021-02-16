.DEFAULT_GOAL = libgfx.so
.PHONY = libgfx.so clean

INCLUDES_QT = -I/usr/include/x86_64-linux-gnu/qt5                   \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL   \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui      \

FLAGS = -std=c++17  $(INCLUDES_QT) -Wall -fPIC   #-O3
LD_FLAGS = -ldl -fPIC   #-O3

gfx.o: gfx.cpp gfx.hpp gl.hpp
	@$(CC) $(FLAGS) -c gfx.cpp

camera.o: camera.cpp camera.hpp
	@$(CC) $(FLAGS) -c camera.cpp

mesh.o: mesh.cpp mesh.hpp
	@$(CC) $(FLAGS) -c mesh.cpp

box.o: box.cpp box.hpp
	@$(CC) $(FLAGS) -c box.cpp

gui/libgui.a:
	@$(MAKE) -C gui


LIBS =  -lGLEW -lstdc++fs \
        -lQt5Widgets -lQt5Core -lQt5Gui -lQt5OpenGL -lGL \
	   -lassimp  \
	   -lGLU -lGL -lGLU  -ldl  # weil es sowohl static als auch dyn von GLEW gibt
	

libgfx.so: mesh.o box.o  \
          camera.o gfx.o \
		  gui/libgui.a  
	$(CC) -shared       \
	-Wl,--whole-archive -Lgui -l:libgui.a -Wl,--no-whole-archive \
	 $(LD_FLAGS)        \
	*.o $(LIBS) \
	-o libgfx.so

clean:
	@$(MAKE) -C gui clean
	@rm -f *.o
	@rm -f *.so