#include "game.h"
#include <raylib.h>

void Game::init() {
    std::cout << "Game class init" << std::endl;
    InitAudioDevice();
}

void Game::update(float deltaTime) {
    std::cout << "Game class update" << std::endl;

}

void Game::draw() {
    std::cout << "Game class draw" << std::endl;

}

void Game::destroy() {
    std::cout << "Game class destroy" << std::endl;
    CloseAudioDevice();
}
