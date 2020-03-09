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
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

/* ------------------------- Game Controller Support --------------------------- */
namespace {

/**
 * @brief The ID of the currently used @p SDL_Joystick.
 * 
 */
std::optional<SDL_JoystickID> current_game_controller;

std::vector<SDL_JoystickID> find_game_controllers() {
    std::vector<SDL_JoystickID> game_controllers;

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

        const std::string device_name { SDL_GameControllerNameForIndex(index) };
        std::cout << "\t" << index << ": " << device_name << std::endl;
        if (device_name == "PS4 Controller") {
            game_controllers.emplace_back(index);
        }
    }

    return game_controllers;
}

SharedGameController find_game_controller() {
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) == -1) {
        throw std::runtime_error { SDL_GetError() };
    }

    std::vector<SDL_JoystickID> game_controllers;
    do {
        std::cout << "looking for game controllers..." << std::endl;
        game_controllers = find_game_controllers();
        if (game_controllers.empty()) {
            std::this_thread::sleep_for(1s);  // wait before another try
        }
    } while (App.run && game_controllers.empty());

    std::cout << "found " << game_controllers.size() << " game controller(s)" << std::endl;
    if (game_controllers.size() > 1) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Warning",
                                 "Found more than one game controller but currently only one is supported!", nullptr);
    }

    SDL_GameController *game_controller = SDL_GameControllerOpen(game_controllers[0]);
    if (game_controller == nullptr) {
        throw std::runtime_error { SDL_GetError() };
    }

    current_game_controller = game_controllers[0];  // TODO(bkuolt): remove this hack
    return { game_controller,
             [] (SDL_GameController *pointer) {
                 SDL_GameControllerClose(pointer);
           }};
}

ps4_button map_button_name(Uint8 button) {
    return {};  // TODO(bkuolt)
}

/* ---------------------------- Input Event Handling --------------------------- */

void handle_input_event(const SDL_Event &event) {
    switch (event.type) {
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            if (event.jbutton.which == current_game_controller) {
                on_button(map_button_name(event.jbutton.button), event.jbutton.state);
            }
            break;
        case SDL_JOYAXISMOTION:
            if (event.jbutton.which == current_game_controller) {
                const auto value = static_cast<float>(event.jaxis.value) / std::numeric_limits<decltype(event.jaxis.value)>::max();
                switch (event.jaxis.axis) {
                    case SDL_CONTROLLER_AXIS_TRIGGERLEFT: on_motion({ value, 0.0f }, {}); break;
                    case SDL_CONTROLLER_AXIS_RIGHTY: on_motion({ 0.0f, value }, {}); break;
                    case SDL_CONTROLLER_AXIS_LEFTX:  on_motion({}, { 0.0f, value }); break;
                    case SDL_CONTROLLER_AXIS_LEFTY:  on_motion({}, { 0.0f, value }); break;
                    case SDL_CONTROLLER_AXIS_RIGHTX:   // @note this a a workaround for a known SDL2 bug
                        on_trigger(value, 0.0f);
                        break;
                    case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                        on_trigger(0.0f, value);
                        break;
                    default:
                        std::cout << "Warning: got event from unsupported axis" << std::endl;
                }
            }
            break;
    }
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
        case SDL_JOYBUTTONUP:
            handle_input_event(event);
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

std::future<SharedGameController> get_game_controller() {
    return std::async(std::launch::async, &find_game_controller);
}

void loop() {
    using namespace std::chrono_literals;

    Uint32 timestamp_render = SDL_GetTicks();
    Uint32 timestamp_fps = SDL_GetTicks();
    size_t fps = 0;

    SDL_Event event;
    while (App.run) {
        while (SDL_PollEvent(&event)) {
            handle_event(event);
        }

        const Uint32 duration = SDL_GetTicks() - timestamp_render;
        const float delta = duration / 1000.0f;
        timestamp_render = SDL_GetTicks();
        on_render(App.window, delta);
        ++fps;

        // track fps
        if (SDL_GetTicks() - timestamp_fps >= 1000) {
            timestamp_fps = timestamp_render;
            std::cout << console_color::blue << "\r" << fps << " fps" << std::flush;
            fps = 0;
        }
    }

    std::cout << "\r" << console_color::white << std::endl;
    SDL_DestroyWindow(App.window.get());  // make sure that the window is destroyed before the context
}
