// Copyright 2020 Bastian Kuolt
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_joystick.h>

#include "input.hpp"

#include <chrono>
#include <iostream>
#include <future>
#include <memory>
#include <thread>
#include <optional>
#include <limits>

using namespace std::chrono_literals;

namespace {

std::optional<SDL_JoystickID> current_game_controller;

std::optional<SharedGameController> find_first_PS4_controller() {
    const auto num_joysticks = SDL_NumJoysticks();
    for (SDL_JoystickID index = 0; index < num_joysticks; ++index) {
        if (!SDL_IsGameController(index)) {
            continue;
        }

        constexpr auto Deleter = [] (SDL_GameController *pointer) { SDL_GameControllerClose(pointer); };
        auto game_controller = std::shared_ptr<SDL_GameController> { SDL_GameControllerOpen(index), Deleter };
        if (game_controller == nullptr) {
            throw std::runtime_error { SDL_GetError() };
        }

        std::cout << index << ": " << SDL_GameControllerName(game_controller.get()) << std::endl;
        if (strcmp(SDL_GameControllerName(game_controller.get()), "PS4 Controller") == 0) {
            current_game_controller = index;
            return game_controller;
        }
    }

    return {};
}

SharedGameController find_game_controller() {
    std::optional<SharedGameController> game_controller;

    SDL_Init(SDL_INIT_GAMECONTROLLER);
    do {
        SDL_GameControllerUpdate();
        game_controller = find_first_PS4_controller();
        if (!game_controller.has_value()) {
            std::this_thread::sleep_for(1s);
        }
    } while (!game_controller.has_value());

    std::cout << "found game controller" << std::endl;
    return game_controller.value();
}

}  // namespace

std::future<SharedGameController> get_game_controller() {
    return std::async(std::launch::async, &find_game_controller);
}

namespace {

void on_button(ps4_button, bool pressed) {
    std::cout << "game controller button" << pressed ? "pressed" : "released";
    // TODO(bkuolt): implement
}

void on_motion(const vec2 &lhs, const vec2 &rhs) {
    // TODO(bkuolt): implement
}

ps4_button map_button_name(Uint8 button) {
    return {};  // TODO(bkuolt)
}

}  // namespace

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
                        throw std::runtime_error { "unsupported axis" };
                }
            }
            break;
        default:
            std::cout << "did not handle this input event" << std::endl;
    }
}
