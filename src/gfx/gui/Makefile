.DEFAULT_GOAL = bgl
.PHONY = run clean run

INCLUDES_QT = -I/usr/include/x86_64-linux-gnu/qt5                   \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL

LIBS_QT = -lQt5Widgets -lQt5Core -lGL
CPPFLAGS = -std=c++17 -fPIC $(INCLUDES_QT) -DQT_NO_KEYWORDS -o3
LDFLAGS = -lstdc++fs -lstdc++ $(LIBS_QT)


main.o: main.cpp
	g++ $(CPPFLAGS) -c main.cpp -o main.o

viewport.o: GLViewport.hpp viewport.cpp
	g++ $(CPPFLAGS) -c viewport.cpp -o viewport.o

bgl: main.o viewport.o
	g++ -o bgl main.o viewport.o $(LIBS_QT) $(LDFLAGS) 

run: bgl
	@./bgl

clean:
	@rm -f *.o
	@rm -f bgl