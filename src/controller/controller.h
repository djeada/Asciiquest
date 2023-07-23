#include "model/model.h"
#include "renderer/renderer.h"

class Controller {

  // GameState Managment

public:
  Controller();

  void run();

private:
  enum class GameState { MAINE_MENU, GAMEPLAY, PAUSE_MENU, GAME_OVER };

  void handleGamePlay();
  void handleGameOver();
  void handlePauseMenu();
  void handleMainMenu();
  void handleInput();
  Model model;
  Renderer renderer;
  bool isRunning;
  GameState currentGameState;
};
