// Copyright 2020 Bastian Kuolt
#include <csignal>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

#include "gfx.hpp"
#include "input.hpp"

namespace {

struct {
    bool run = true;
    shared_window window;
    shared_context context;
} App;

// forward declaration
void loop();

void signal_handler(int signal) {
    App.run = false;
}

}  // namespace

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "usage: demo <path-to-model>" << std::endl;
        return EXIT_FAILURE;
    }

    std::signal(SIGINT, signal_handler);
    std::signal(SIGHUP, signal_handler);

    try {
        App.window = create_fullscreen_window();
        App.context = create_GL_context(App.window);

        load_model(argv[1]);
        auto game_controller = get_game_controller();
        game_controller.get();

        loop();
    } catch (const std::exception &exception) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", exception.what(), nullptr);
        std:: cout << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

namespace {

void render(shared_window window) noexcept {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(window.get());
}

void handle_event(const SDL_Event &event) {
    switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                App.run = false;
            }
            break;
        case SDL_JOYAXISMOTION:
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:std::cout << "handle_event"<<std::endl;
            handle_input_event(event);
            break;
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            App.run = false;
            break;
    }
}

void loop() {
    using namespace std::chrono_literals;

    SDL_Event event;
    while (App.run) {
        while (SDL_PollEvent(&event)) {
            handle_event(event);
            render(App.window);
        }
        std::this_thread::sleep_for(100ms);
    }

    SDL_DestroyWindow(App.window.get());  // make sure that the window is destroyed before the context
}

}  // namespace
