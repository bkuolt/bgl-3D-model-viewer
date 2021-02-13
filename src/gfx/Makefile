.DEFAULT_GOAL = libgfx.a
.PHONY = libgfx.a clean

CC = g++
INCLUDES_QT = -I/usr/include/x86_64-linux-gnu/qt5                   \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL   \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui

FLAGS = -std=c++17 -pthread $(INCLUDES_QT) -Wall -fPIC -O3 -Igl/

gfx.o: gfx.cpp gfx.hpp
	@$(CC) $(FLAGS) -c gfx.cpp

camera.o: camera.cpp camera.hpp
	@$(CC) $(FLAGS) -c camera.cpp

gl/libgl.a:
	@$(MAKE) -C gl

gui/libgui.a:
	@$(MAKE) -C gui

libgfx.a: gl/libgl.a   \
          gui/libgui.a \
          camera.o gfx.o
	ar rcs libgfx.a *.o

clean:
	@$(MAKE) -C gl clean
	@$(MAKE) -C gui clean
	@rm -f *.o
	@rm -f *.a