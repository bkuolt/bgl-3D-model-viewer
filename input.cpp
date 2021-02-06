// Copyright 2020 Bastian Kuolt
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>


#include "input.hpp"
#include "App.hpp"

#include <glm/glm.hpp>
#include <chrono>
#include <future>
#include <iostream>

#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <set>
#include <thread>
#include <vector>

namespace bgl {

using namespace std::chrono_literals;

namespace {

/* ---------------------------- Input Event Handling --------------------------- */

void handle_event(const SDL_Event &event) {
    switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            on_key(event.key);
            break;
        case SDL_JOYAXISMOTION:
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            // TODO
            break;
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            App.run = false;
            break;
    }
}

}  // namespace

/*-----------------------------------------------------------------------------
  -------------------------------- Interface ----------------------------------
  ----------------------------------------------------------------------------- */
extern std::set<Window*> windows;

void loop() {
    SDL_Event event;
    while (App.run) {
        while (SDL_PollEvent(&event)) {
            handle_event(event);
        }

        std::for_each(windows.begin(), windows.end(), [] (Window *window) { window->render(); });
    }

    std::cout << "\r" << console_color::white << std::endl;
    SDL_DestroyWindow(::App.window->getHandle());  // make sure that the window is destroyed before the context
}

}  // namespace bgl
