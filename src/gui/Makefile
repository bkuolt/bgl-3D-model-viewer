.DEFAULT_GOAL = libgui.a
.PHONY = libgui.a clean

INCLUDES_QT = -I/usr/include/x86_64-linux-gnu/qt5                   \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL   \
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

FLAGS = -std=c++17 -Wno-endif-labels     \
        $(WARNINGS)                      \
		$(INCLUDES_QT) -DQT_NO_KEYWORDS  \
		-fPIC -o3

viewport.o: viewport.hpp viewport.cpp
	@$(CC) $(FLAGS) -c viewport.cpp -o viewport.o

window.o: window.hpp window.cpp
	@$(CC) $(FLAGS) -c window.cpp -o window.o

panel.o: panel.hpp panel.cpp
	@$(CC) $(FLAGS) -c panel.cpp -o panel.o

menu.o: menu.hpp menu.cpp
	@$(CC) $(FLAGS) -c menu.cpp -o menu.o

libgui.a: window.o viewport.o menu.o panel.o
	ar rcs libgui.a *.o

clean:
	rm -f *.o
	rm -f *.a
