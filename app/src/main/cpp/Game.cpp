//
// Created by pux19 on 1/25/2019.
//

#include <vulkan_wrapper.h>
#include "Game.h"
#include "Logger.h"

Game::Game() {
    InitVulkan();
    Logger::Info("Game Created!");
}

Game::~Game() {
    Logger::Info("Game Destroyed!");
}
