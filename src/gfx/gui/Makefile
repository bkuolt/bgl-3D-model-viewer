.DEFAULT_GOAL = libgui.a
.PHONY = clean

INCLUDES_QT = -I/usr/include/x86_64-linux-gnu/qt5                   \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL   \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui

CPPFLAGS = -std=c++17 -fPIC   \
           -Wno-endif-labels  \
            $(INCLUDES_QT)    \
           -DQT_NO_KEYWORDS   \
		   -DUSE_QT -fPIC     \
		   -o3

viewport.o: GLViewport.hpp viewport.cpp
	g++ $(CPPFLAGS) -c viewport.cpp -o viewport.o

window.o: window.hpp window.cpp
	g++ $(CPPFLAGS) -c window.cpp -o window.o

libgui.a: window.o viewport.o
	ar rcs libgui.a *.o

clean:
	rm -f *.o
	rm -f *.a
