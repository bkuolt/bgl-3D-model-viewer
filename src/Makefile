.DEFAULT_GOAL = run
.PHONY = demo gfx/libbgl.so run clean

INCLUDES_QT =  -I/usr/include -isystem /usr/include/x86_64-linux-gnu/qt5  \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets        \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore           \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL         \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui
WARNINGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual  \
           -Wctor-dtor-privacy -Wdisabled-optimization       \
		   -Wformat=2 -Winit-self -Wlogical-op               \
		   -Wmissing-declarations -Wmissing-include-dirs     \
		   -Wnoexcept -Wold-style-cast -Woverloaded-virtual  \
		   -Wredundant-decls -Wshadow -Wsign-conversion      \
		   -Wsign-promo -Wstrict-null-sentinel               \
		   -Wstrict-overflow=5 -Wswitch-default              \
		   -Wundef -Wno-unused # -Werror 

FLAGS = -std=c++17 -pthread   \
	    $(INCLUDES_QT)        \
	    -Wall                 \
		-fPIC -O3

LIBS = -lstdc++fs                                   \
       -lGLEW -lGL -lGLU                            \
       -lQt5Widgets -lQt5Core -lQt5Gui -lQt5OpenGL  \
	   -lassimp                                     \
	   -ldl

LD_FLAGS = -ldl -fPIC

main.o: main.cpp
	@$(CC) $(FLAGS) -c main.cpp
window.o: window.cpp window.hpp
	@$(CC) $(FLAGS) -c window.cpp

gfx/libgfx.a:
	@$(MAKE) -C gfx
gui/libgui.a:
	@$(MAKE) -C gui

libbgl.so: gfx/libgfx.a gui/libgui.a
	$(CC) -shared $(LD_FLAGS)  \
	$(LIBS)                    \
	-Wl,--whole-archive        \
	-Lgui -l:libgui.a          \
	-Lgfx -l:libgfx.a          \
	-Wl,--no-whole-archive     \
	-o libbgl.so

demo: libbgl.so main.o window.o
	$(CC) $(FLAGS) main.o window.o  \
	-Wl,-Bdynamic -L./ -lbgl        \
	-lstdc++ -ldl $(LIBS)           \
    -o demo   

run: demo
	export LD_LIBRARY_PATH=./;   \
	echo $(LD_LIBRARY_PATH);     \
	./demo assets/models/housemedieval.obj

install: libbgl.so demo
	sudo cp libbgl.so /usr/lib/libbgl.so ;  \
	sudo cp demo /usr/bin/bgl

uninstall:
	sudo rm -f /usr/lib/libbgl.so
	sudo rm -f /usr/bin/bgl

clean:
	@$(MAKE) -C gfx clean
	@$(MAKE) -C gui clean
	@rm -f *.o
	@rm -f *.so
	@rm -f demo
