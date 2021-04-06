.DEFAULT_GOAL = libgfx.a
.PHONY = libgfx.a clean

INCLUDES_QT = -I/usr/include -isystem /usr/include/x86_64-linux-gnu/qt5 \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL   \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui

FLAGS = $(INCLUDES_QT) -std=c++17 -Wall -fPIC -O3

OBJS = mesh.o importer.o model.o \
	   box.o grid.o     \
	   camera.o gfx.o

%.o: %.cpp %.hpp
	@$(CC) $(FLAGS) -c $<

libgfx.a:  $(OBJS)
	ar rcs libgfx.a *.o

clean:
	@rm -f *.o
	@rm -f *.a
