.DEFAULT_GOAL = run
.PHONY = demo gfx/libgfx.so run clean

INCLUDES_QT = -I/usr/include/x86_64-linux-gnu/qt5                   \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL   \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui

FLAGS = -std=c++17  -pthread            \
	    -DUSE_QT $(INCLUDES_QT) -fPIC  \
		-Wall -fPIC   #-O3
# -fPIC is for QT only

LIBS = -lstdc++fs \
       -lGL \
       -lQt5Widgets -lQt5Core -lQt5Gui -lQt5OpenGL  \
	   -lassimp  \
	   -lGLU  -lGLU  -ldl

main.o: main.cpp window.hpp
	@$(CC) $(FLAGS) -c main.cpp

gfx/libgfx.so:
	@$(MAKE) -C gfx

demo: main.o gfx/libgfx.so
	$(CC) \
	$(FLAGS)  main.o  \
	-Lgfx -lgfx  -lstdc++ -ldl $(LIBS) \
     -o demo   
	#-Wl,--as-needed -lGLEW
run: demo
	export LD_LIBRARY_PATH=/home/bastian/code/bgl-demo/src/:/home/bastian/code/bgl-demo/src/gfx; \
	 echo $(LD_LIBRARY_PATH); \
	./demo \
	/home/bastian/Downloads/Sponza-master/sponza.obj
	#./assets/models/housemedieval.obj    # the rendered 3D model
	#./assets/models/housemedieval.obj

clean:
	@$(MAKE) -C gfx clean
	@rm -f *.o
	@rm -f demo
