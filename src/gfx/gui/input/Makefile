.DEFAULT_GOAL = bgl
.PHONY = all clean run

INCLUDES_QT = -I/usr/include/x86_64-linux-gnu/qt5                   \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui      \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGamepad

LIBS_QT = -lQt5Widgets -lQt5Gui -lQt5Core -lQt5Gamepad
CPPFLAGS = -std=c++17 -fPIC $(INCLUDES_QT) -o3
LDFLAGS = -lstdc++fs -lstdc++ $(LIBS_QT)

main.o: main.cpp
	g++ $(CPPFLAGS) -c main.cpp -o main.o

bgl: main.o 
	g++ -o bgl main.o $(LIBS_QT) $(LDFLAGS)

run: bgl
	@./bgl

clean:
	rm -f *.o
	rm -f bgl