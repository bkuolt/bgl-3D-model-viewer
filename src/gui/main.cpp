#include "gui.hpp"

int main(int argc, char **argv) {
    try {
        bgl::App app { argc, argv };
        bgl::gui::Window window;

        switch (argc) {
            case 1:
                window.loadImage();
                break;
            case 2:
                window.loadImage();
                break;
            default:
                bgl::prompt_error_message("Invalid command line argument");
                return EXIT_FAILURE;
        }

        bgl::print_debug_info();

        return app.exec();
    } catch (const std::exception &error) {
        bgl::prompt_error_message(error);
        return EXIT_FAILURE;
    }
}
