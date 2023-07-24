#ifndef GameStateHandler_H
#define GameStateHandler_H

#include "controller.h"

class Controller;
class GameStateHandler {
public:
  virtual void handleInput(Controller &controller, int input) = 0;
  virtual void handleState(Controller &controller) = 0;
  virtual ~GameStateHandler() = default;
};

class MainMenuStateHandler : public GameStateHandler {
public:
  void handleInput(Controller &controller, int input) override;
  void handleState(Controller &controller) override;
};

class GameplayStateHandler : public GameStateHandler {
public:
  void handleInput(Controller &controller, int input) override;
  void handleState(Controller &controller) override;
};

class PauseStateHandler : public GameStateHandler {
public:
  void handleInput(Controller &controller, int input) override;
  void handleState(Controller &controller) override;
};

// Other state handlers...

#endif
