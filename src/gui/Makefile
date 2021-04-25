.DEFAULT_GOAL = run
.PHONY = run clean

OBJ_FILES = Window.o moc_Window.o                  \
            StatusBar.o moc_StatusBar.o            \
            MenuBar.o moc_MenuBar.o                \
            OpenGLViewport.o moc_OpenGLViewport.o  \
			Scene.o moc_Scene.o

INCLUDES_QT = -I/usr/include/x86_64-linux-gnu/qt5                   \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL   \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui      \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtConcurrent

LIBS_QT =  -lGL -lQt5Widgets -lQt5Core -lQt5Gui
CPPFLAGS = -std=c++17 -fPIC $(INCLUDES_QT) -fPIC -o3
LDFLAGS = -fPIC  \
	      -lstdc++fs -lstdc++ -lpthread -lGLEW $(LIBS_QT)

moc_%.cpp: %.hpp
	moc $< -o $@

%.o: %.cpp  \
     %.cpp %.hpp
	g++ $(CPPFLAGS) -c  $< -o $@

main.o: main.cpp
	g++ $(CPPFLAGS) -c main.cpp -o main.o

libgui.la: $(OBJ_FILES)
	ar rcs libgui.la *.o

qt-demo: main.o libgui.la
	g++ -o qt-demo                 \
	    main.o -L./ -l:libgui.la   \
		$(LDFLAGS) 

run: qt-demo
	@./qt-demo example.jpg

clean:
	@rm -f *.o
	@rm -f *.la
	@rm -f moc_*.cpp
	@rm -f qt-demo