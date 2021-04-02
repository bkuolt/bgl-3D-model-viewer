.DEFAULT_GOAL = libgfx.so
.PHONY = libgfx.so clean

INCLUDES_QT = -isystem/usr/include/x86_64-linux-gnu/qt5             \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL   \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui

FLAGS = $(INCLUDES_QT) -std=c++17 -Wall -fPIC -O3
LD_FLAGS = -ldl -fPIC

%.o: %.cpp %.hpp
	@$(CC) $(FLAGS) -c $<

gui/libgui.a:
	@$(MAKE) -C gui

LIBS = -lGLEW -lstdc++fs \
       -lQt5Widgets -lQt5Core -lQt5Gui -lQt5OpenGL -lGL \
	   -lassimp  \
	   -lGLU -lGL -lGLU -ldl

libgfx.so: mesh.o importer.o model.o \
           box.o grid.o     \
		   camera.o gfx.o   \
		   gui/libgui.a
	$(CC) -shared       \
	-Wl,--whole-archive -Lgui -l:libgui.a -Wl,--no-whole-archive \
	 $(LD_FLAGS)  \
	*.o $(LIBS)   \
	-o libgfx.so

clean:
	@$(MAKE) -C gui clean
	@rm -f *.o
	@rm -f *.so