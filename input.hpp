// Copyright 2020 Bastian Kuolt
#ifndef INPUT_H_
#define INPUT_H_

#include <memory>
#include <future>
#include <glm/glm.hpp>

#include <SDL2/SDL_gamecontroller.h>

using vec2 = glm::vec2;
enum class ps4_button { triangle, cross, square, circle };

using SharedGameController = std::shared_ptr<SDL_GameController>;

/**
 * @brief Asynchronous search for a @p SDL_GameController .
 */
std::future<SharedGameController> get_game_controller();

void handle_input_event(const SDL_Event &event);

#endif  // INPUT_H_
