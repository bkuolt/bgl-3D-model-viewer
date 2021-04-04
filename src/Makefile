.DEFAULT_GOAL = run
.PHONY = demo gfx/libbgl.so run clean

INCLUDES_QT =  -I/usr/include -isystem /usr/include/x86_64-linux-gnu/qt5 \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL   \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui

FLAGS = -std=c++17  -pthread            \
	    -DUSE_QT $(INCLUDES_QT) -fPIC  \
		-Wall -fPIC -O3

LIBS = -lstdc++fs \
       -lGL \
       -lQt5Widgets -lQt5Core -lQt5Gui -lQt5OpenGL  \
	   -lassimp  \
	   -lGLU  -lGLU  -ldl

main.o: main.cpp
	@$(CC) $(FLAGS) -c main.cpp

window.o: window.cpp window.hpp
	@$(CC) $(FLAGS) -c window.cpp

gfx/libgfx.so:
	@$(MAKE) -C gfx

libbgl.so: gfx/libgfx.so
	cp gfx/libgfx.so libbgl.so

demo: main.o window.o libbgl.so
	$(CC) $(FLAGS) main.o window.o    \
	-L./ -lbgl -lstdc++ -ldl $(LIBS)  \
    -o demo   

run: demo  # TODO
	export LD_LIBRARY_PATH=/home/bastian/code/bgl-demo/src/:/home/bastian/code/bgl-demo/src/gfx; \
	echo $(LD_LIBRARY_PATH); \
	./demo assets/models/housemedieval.obj

install: libbgl.so demo
	sudo cp libbgl.so /usr/lib/libbgl.so ;  \
	sudo cp demo /usr/bin/bgl

uninstall:
	sudo rm -f /usr/lib/libbgl.so
	sudo rm -f /usr/bin/bgl

clean:
	@$(MAKE) -C gfx clean
	@rm -f *.o
	@rm -f *.so
	@rm -f demo
