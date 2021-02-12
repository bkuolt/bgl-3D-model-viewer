// Copyright 2021 Bastian Kuolt
#include <QApplication>
#include <iostream>

#include "Window.hpp"


int main(int argc, char **argv) {
    try {
        QApplication app(argc, argv);
        bgl::Window window;
        return app.exec();
    } catch (const std::exception &error) {
        std::cerr << "error: " << error.what();
        return EXIT_FAILURE;
    }
}
