// Copyright 2020 Bastian Kuolt
#ifndef INPUT_HPP_
#define INPUT_HPP_

#include <memory>
#include <future>
#include <glm/glm.hpp>

#include <SDL2/SDL_gamecontroller.h>

using vec2 = glm::vec2;
enum class ps4_button { Triangle, Cross, Square, Circle, L1, L2, R1, R2 };

using SharedGameController = std::shared_ptr<SDL_GameController>;

/**
 * @brief Asynchronous search for a @p SDL_GameController .
 */
std::future<SharedGameController> get_game_controller();

/**
 * @brief Performs game loop until windows is closed.
 */
void loop();

#endif  // INPUT_HPP_
