.DEFAULT_GOAL = libgfx.so
.PHONY = libgfx.so clean

INCLUDES_QT = -I/usr/include/x86_64-linux-gnu/qt5                   \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL   \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui

LIBS_QT =-lQt5Widgets -lQt5Core -lQt5Gui -lQt5OpenGL 

LIBS = -lGLEW -lstdc++fs \
       $(LIBS_QT)        \
	   -lassimp          \
	   -lGLU -lGL -lGLU -ldl

FLAGS = -std=c++17  $(INCLUDES_QT) -Wall -fPIC -O3
LD_FLAGS = -ldl -fPIC

mesh.o: mesh.cpp mesh.hpp
	@$(CC) $(FLAGS) -c mesh.cpp

model.o: model.cpp model.hpp
	@$(CC) $(FLAGS) -c model.cpp

box.o: box.cpp box.hpp
	@$(CC) $(FLAGS) -c box.cpp

grid.o: grid.cpp grid.hpp
	@$(CC) $(FLAGS) -c grid.cpp

camera.o: camera.cpp camera.hpp
	@$(CC) $(FLAGS) -c camera.cpp

importer.o: importer.cpp
	@$(CC) $(FLAGS) -c importer.cpp

gui/libgui.a:
	@$(MAKE) -C gui

libgfx.so: mesh.o importer.o \
           model.o   \
           box.o grid.o     \
           camera.o         \
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