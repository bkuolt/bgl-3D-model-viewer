.DEFAULT_GOAL = run
.PHONY = run clean run

INCLUDES_QT = -I/usr/include/x86_64-linux-gnu/qt5                   \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets  \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore     \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL   \
			  -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui      \
	          -isystem /usr/include/x86_64-linux-gnu/qt5/QtConcurrent

LIBS_QT =  -lGL -lQt5Widgets -lQt5Core -lQt5Gui
CPPFLAGS = -std=c++17 -fPIC $(INCLUDES_QT) -fPIC -o3  #  -DQT_NO_KEYWORDS
LDFLAGS = -fPIC  \
	      -lstdc++fs -lstdc++ -lpthread -lGLEW $(LIBS_QT)

moc_%.cpp: %.hpp
	moc $< -o $@ 
	#; echo $< $@
#Q_OBJECT




main.o: main.cpp
	g++ $(CPPFLAGS) -c main.cpp -o main.o

OpenGLViewport.o: OpenGLViewport.hpp OpenGLViewport.cpp
	g++ $(CPPFLAGS) -c OpenGLViewport.cpp -o OpenGLViewport.o

Window.o: Window.hpp Window.cpp
	g++ $(CPPFLAGS) -c Window.cpp -o Window.o

MenuBar.o: MenuBar.hpp MenuBar.cpp
	g++ $(CPPFLAGS) -c MenuBar.cpp -o MenuBar.o

StatusBar.o: StatusBar.hpp StatusBar.cpp
	g++ $(CPPFLAGS) -c StatusBar.cpp -o StatusBar.o

# --------------------------------------------------------

moc_StatusBar.o: moc_StatusBar.cpp
	g++ $(CPPFLAGS) -c moc_StatusBar.cpp -o moc_StatusBar.o

moc_MenuBar.o: moc_MenuBar.cpp
	g++ $(CPPFLAGS) -c moc_MenuBar.cpp -o moc_MenuBar.o

moc_Window.o: moc_Window.cpp
	g++ $(CPPFLAGS) -c moc_Window.cpp -o moc_Window.o

moc_OpenGLViewport.o: moc_OpenGLViewport.cpp
	g++ $(CPPFLAGS) -c moc_OpenGLViewport.cpp -o moc_OpenGLViewport.o

# TODO Scene
# TODO

qt-demo: main.o                                 \
		 Window.o moc_Window.o                  \
         StatusBar.o moc_StatusBar.o            \
		 MenuBar.o moc_MenuBar.o                \
		 OpenGLViewport.o moc_OpenGLViewport.o 
	g++ -o qt-demo                              \
	    main.o                                  \
		Window.o moc_Window.o                   \
		MenuBar.o moc_MenuBar.o               \
		StatusBar.o moc_StatusBar.o             \
		OpenGLViewport.o moc_OpenGLViewport.o   \
		$(LDFLAGS) 

run: qt-demo
	@./qt-demo

clean:
	@rm -f *.o
	@rm -f moc_*.cpp
	@rm -f qt-demo