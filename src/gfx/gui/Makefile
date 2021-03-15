.DEFAULT_GOAL = libgui.a
.PHONY = libgui.a clean

INCLUDES_QT = -I/usr/include/x86_64-linux-gnu/qt5                   \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL   \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui

CPPFLAGS = -std=c++17  -fPIC       \
           -Wno-endif-labels -Wall \
            $(INCLUDES_QT)         \
           -DQT_NO_KEYWORDS        \
		   -DUSE_QT -fPIC # -o3

viewport.o: viewport.hpp viewport.cpp
	@$(CC) $(CPPFLAGS) -c viewport.cpp -o viewport.o

window.o: window.hpp window.cpp
	@$(CC) $(CPPFLAGS) -c window.cpp -o window.o

panel.o: panel.hpp panel.cpp
	@$(CC) $(CPPFLAGS) -c panel.cpp -o panel.o

menu.o: menu.hpp menu.cpp
	@$(CC) $(CPPFLAGS) -c menu.cpp -o menu.o

libgui.a: window.o viewport.o menu.o # panel.o
	ar rcs libgui.a *.o

clean:
	rm -f *.o
	rm -f *.a
