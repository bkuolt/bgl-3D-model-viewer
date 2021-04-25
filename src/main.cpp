#include "gui/gui.hpp"

int main(int argc, char **argv) {
    try {
        bgl::App app{argc, argv};
        bgl::gui::Window window;

        auto scene = std::make_shared<bgl::Scene>();  // TODO

        window.viewport()->makeCurrent();
        scene->load("/home/bastian/code/bgl-demo/src/assets/models/housemedieval.obj");  // TODO
        window.setScene(scene);

        switch (argc) {
        case 1:
            window.loadModel();
            break;
        case 2:
            //  window.loadModel(argv[2]);  // TODO: fix reload problem
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
