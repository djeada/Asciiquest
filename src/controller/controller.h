#include "model/model.h"
#include "renderer/renderer.h"

class Controller {

public:
  Controller(Model &m, Renderer &r);

  void run();

private:
  void handleGameState();
  void handleGamePlay();
  void handleGameOver();
  void handlePauseMenu();
  void handleMainMenu();
  void handleInput();
  void handleGameplayInput(int ch);
  void handleMainMenuInput(int ch);
  void handlePauseMenuInput(int ch);

  Model model;
  Renderer renderer;
  bool isRunning;
  GameState currentGameState;
};
