// Copyright 2020 Bastian Kuolt
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_joystick.h>

#include "input.hpp"
#include "App.hpp"

#include <chrono>
#include <future>
#include <iostream>

#include <limits>
#include <memory>
#include <optional>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

namespace {

std::optional<SDL_JoystickID> current_game_controller;

std::vector<SharedGameController> find_game_controllers() {
    // TODO(bkuolt): lock SDL game controller handling
    std::vector<SharedGameController> game_controllers;

    SDL_GameControllerUpdate();
    const auto num_joysticks = SDL_NumJoysticks();
    for (SDL_JoystickID index = 0; index < num_joysticks; ++index) {
        if (!SDL_IsGameController(index)) {
            continue;
        }

        auto game_controller = SDL_GameControllerOpen(index);
        if (game_controller == nullptr) {
            throw std::runtime_error { SDL_GetError() };
        }

        std::cout << index << ": " << SDL_GameControllerName(game_controller) << std::endl;
        if (strcmp(SDL_GameControllerName(game_controller), "PS4 Controller") == 0) {
            game_controllers.emplace_back(game_controller,
                                          [] (SDL_GameController *pointer) { SDL_GameControllerClose(pointer); });
        }
    }

    // TODO(bkuolt): unlock SDL game controller handling
    return game_controllers;
}

SharedGameController find_game_controller() {
    SDL_Init(SDL_INIT_GAMECONTROLLER);

    std::vector<SharedGameController> game_controllers;
    do {
        game_controllers = find_game_controllers();
        if (game_controllers.empty()) {
            std::this_thread::sleep_for(1s);  // wait before another try
        }
    } while (App.run && game_controllers.empty());

    std::cout << "found " << game_controllers.size() << " game controllers" << std::endl;
    if (game_controllers.size() > 1) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Warning",
                                 "Found more than one game controller but currently only one is supported!", nullptr);
    }

    auto joystick = SDL_GameControllerGetJoystick(game_controllers[0].get());
    current_game_controller = SDL_JoystickInstanceID(joystick);  // TODO(bkuolt): remove this hack
    return game_controllers[0];
}

}  // namespace

std::future<SharedGameController> get_game_controller() {
    return std::async(std::launch::async, &find_game_controller);
}

/* ---------------------------------------------------------------------- */

namespace {

ps4_button map_button_name(Uint8 button) {
    return {};  // TODO(bkuolt)
}

void handle_input_event(const SDL_Event &event) {
    switch (event.type) {
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
        std::cout << event.jbutton.which  << current_game_controller.value()<< std::endl;
            if (event.jbutton.which == current_game_controller) {
                on_button(map_button_name(event.jbutton.button), event.jbutton.state);
            }
            break;
        case SDL_JOYAXISMOTION:
            if (event.jbutton.which == current_game_controller) {
                const auto value = static_cast<float>(event.jaxis.value) / std::numeric_limits<decltype(event.jaxis.value)>::max();
                switch (event.jaxis.axis) {
                    case SDL_CONTROLLER_AXIS_RIGHTX: on_motion({ value, 0.0f }, {}); break;
                    case SDL_CONTROLLER_AXIS_RIGHTY: on_motion({ 0.0f, value }, {}); break;
                    case SDL_CONTROLLER_AXIS_LEFTX:  on_motion({}, { 0.0f, value }); break;
                    case SDL_CONTROLLER_AXIS_LEFTY:  on_motion({}, { 0.0f, value }); break;
                    default:
                        std::cout << "Warning: got event from unsupported axis" << std::endl;
                }
            }
            break;
        default:
            std::cout << "did not handle this input event" << std::endl;
    }
}

}  // namespace


/* ---------------------------------------------------------- */

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
        case SDL_JOYBUTTONUP:
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