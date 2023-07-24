#ifndef Controller_H
#define Controller_H

#include "game_state_handler.h"
#include "model/model.h"
#include "renderer/renderer.h"

class GameStateHandler;

class Controller {
public:
  Controller(Model &m, Renderer &r);

  void run();

  void handleGameState();
  void stopRunning();
  void setState(GameState gameState);
  void handleInput();
  GameState currentGameState;
  Model model;
  Renderer renderer;
  bool isRunning;

private:
  std::map<GameState, std::unique_ptr<GameStateHandler>> gameStateHandlers;
};

#endif
