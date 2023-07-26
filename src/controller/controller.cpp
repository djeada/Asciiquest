#include "controller.h"
#include <chrono>
#include <thread>

Controller::Controller(Model &m, Renderer &r)
    : model(m), renderer(r), isRunning(false),
      currentGameState(GameState::MAIN_MENU) {
  gameStateHandlers.emplace(GameState::MAIN_MENU,
                            std::make_unique<MainMenuStateHandler>());
  gameStateHandlers.emplace(GameState::GAMEPLAY,
                            std::make_unique<GameplayStateHandler>());
  gameStateHandlers.emplace(GameState::PAUSE_MENU,
                            std::make_unique<PauseStateHandler>());
  gameStateHandlers.emplace(GameState::GAME_OVER,
                            std::make_unique<GameOverStateHandler>());
  // Other game states...
}

void Controller::run() {
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE); // non-blocking input
  isRunning = true;

  while (isRunning) {
    handleGameState();
    handleInput();
    // Sleep for 50 milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}

void Controller::setState(GameState gameState) {
  if (!gameStateHandlers.count(gameState)) {
    return;
  }
  currentGameState = gameState;
}

void Controller::handleGameState() {
  if (auto handler = gameStateHandlers.find(currentGameState);
      handler != gameStateHandlers.end()) {
    handler->second->handleState(*this);
  }
}

void Controller::handleInput() {
  int ch = getch();
  if (ch != ERR) {
    if (auto handler = gameStateHandlers.find(currentGameState);
        handler != gameStateHandlers.end()) {
      handler->second->handleInput(*this, ch);
    }
    flushinp(); // clear the input buffer
  }
}

void Controller::stopRunning() { isRunning = false; }
