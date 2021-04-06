.DEFAULT_GOAL = libgfx.a
.PHONY = libgfx.a clean

INCLUDES_QT = -I/usr/include -isystem /usr/include/x86_64-linux-gnu/qt5  \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets       \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore          \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL        \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui

WARNINGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual  \
           -Wctor-dtor-privacy -Wdisabled-optimization       \
		   -Wformat=2 -Winit-self -Wlogical-op               \
		   -Wmissing-declarations -Wmissing-include-dirs     \
		   -Wnoexcept -Wold-style-cast -Woverloaded-virtual  \
		   -Wredundant-decls -Wshadow -Wsign-conversion      \
		   -Wsign-promo -Wstrict-null-sentinel               \
		   -Wstrict-overflow=5 -Wswitch-default              \
		   -Wundef -Wno-unused 

FLAGS = $(INCLUDES_QT) $(WARNINGS) -std=c++17 -Wall -fPIC -O3

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
