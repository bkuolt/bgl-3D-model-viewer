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

using namespace std::chrono_literals;

namespace {

std::optional<SharedGameController> find_first_PS4_controller() {
    SDL_Init(SDL_INIT_GAMECONTROLLER);

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
            return game_controller;
        }
    }

    return {};
}

SharedGameController find_game_controller() {
    std::optional<SharedGameController> game_controller;
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
