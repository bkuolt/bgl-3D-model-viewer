#include <QApplication>
#include <QMessageBox>
#include <csignal>
#include <cstdlib>
#include <exception>

#include "gfx/gl.hpp"
#include "window.hpp"

namespace {

void signal_handler(int /*signal*/) {
    std::exit(EXIT_FAILURE);
}

}  // anonymous namespace

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    if (argc != 2) {
        QMessageBox::critical(nullptr, "Usage Error", "Usage: bgl_demo <path-to-model>");
        return EXIT_FAILURE;
    }

    std::signal(SIGINT, signal_handler);
    std::signal(SIGHUP, signal_handler);

    try {
        bgl::SimpleWindow window{"BGL 3D Model Viewer"};
        window.show();
        return app.exec();
    } catch (const std::exception& exception) {
        QMessageBox::critical(nullptr, "Fatal Error", exception.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
