// Copyright 2020 Bastian Kuolt
#ifndef INPUT_H_
#define INPUT_H_

#include <memory>
#include <future>

#include <SDL2/SDL_gamecontroller.h>

using SharedGameController = std::shared_ptr<SDL_GameController>;

/**
 * @brief Asynchronous search for a @p SDL_GameController .
 */
std::future<SharedGameController> get_game_controller();

#endif  // INPUT_H_
