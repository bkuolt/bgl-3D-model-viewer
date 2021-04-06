.DEFAULT_GOAL = libgui.a
.PHONY = libgui.a clean

INCLUDES_QT = -I/usr/include/x86_64-linux-gnu/qt5                   \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL   \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui

FLAGS = -std=c++17               \
        -Wno-endif-labels -Wall  \
        $(INCLUDES_QT)           \
        -DQT_NO_KEYWORDS         \
		-o3

%.o: %.cpp %.hpp
	@$(CC) $(FLAGS) -c $<

libgui.a: window.o viewport.o menu.o  panel.o
	ar rcs libgui.a *.o

clean:
	rm -f *.o
	rm -f *.a
