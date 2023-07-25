#include "game_state_handler.h"
#include "renderer/renderer_data.h"

enum class GameplayControls {
  QUIT = 'q',
  UP = 'w',
  LEFT = 'a',
  DOWN = 's',
  RIGHT = 'd',
  PAUSE = 'p',
  ESC = KEY_EIC,
  ARROW_UP = KEY_UP,
  ARROW_LEFT = KEY_LEFT,
  ARROW_DOWN = KEY_DOWN,
  ARROW_RIGHT = KEY_RIGHT
};

enum class MainMenuOptions { START_GAME = '1', OPTIONS = '2', QUIT = '3' };

void MainMenuStateHandler::handleState(Controller &controller) {
  Renderer &renderer = controller.renderer;
  renderer.setState(GameState::MAIN_MENU);
  std::vector<std::vector<CellType>> emptyGrid;
  std::unordered_map<std::string, std::string> emptyStats;
  Point emptyPos;

  std::map<MainMenuOptions, std::string> mainMenuOptions = {
      {MainMenuOptions::START_GAME, "1. Start Game"},
      {MainMenuOptions::OPTIONS, "2. Options"},
      {MainMenuOptions::QUIT, "3. Quit"}};

  std::vector<std::string> menuOptions;
  for (const auto &option : mainMenuOptions) {
    menuOptions.push_back(option.second);
  }

  renderer.draw(RendererData(emptyGrid, menuOptions, emptyStats, emptyPos));
}

void MainMenuStateHandler::handleInput(Controller &controller, int ch) {
  switch (static_cast<MainMenuOptions>(tolower(ch))) {
  case MainMenuOptions::START_GAME:
    controller.setState(GameState::GAMEPLAY);
    break;
  case MainMenuOptions::OPTIONS:
    // controller.setCurrentGameState(GameState::OPTIONS);
    break;
  case MainMenuOptions::QUIT:
    controller.stopRunning();
    break;
  default:
    break;
  }
}

void GameplayStateHandler::handleState(Controller &controller) {
  auto &model = controller.model;
  auto &renderer = controller.renderer;
  auto stat = model.getPlayerStats();
  renderer.setState(GameState::GAMEPLAY);
  renderer.draw(
      RendererData(model.map->grid, model.info, stat, model.player.position));

  if (model.isGameOver()) {
    controller.setState(GameState::GAME_OVER);
  }
}

void GameplayStateHandler::handleInput(Controller &controller, int ch) {
  auto &model = controller.model;
  GameplayControls control = static_cast<GameplayControls>(tolower(ch));

  switch (control) {
  case GameplayControls::QUIT:
  case GameplayControls::ESC:
    controller.stopRunning();
    break;
  case GameplayControls::UP:
  case GameplayControls::ARROW_UP:
    model.movePlayer(Direction::UP);
    break;
  case GameplayControls::LEFT:
  case GameplayControls::ARROW_LEFT:
    model.movePlayer(Direction::LEFT);
    break;
  case GameplayControls::DOWN:
  case GameplayControls::ARROW_DOWN:
    model.movePlayer(Direction::DOWN);
    break;
  case GameplayControls::RIGHT:
  case GameplayControls::ARROW_RIGHT:
    model.movePlayer(Direction::RIGHT);
    break;
  case GameplayControls::PAUSE:
    controller.setState(GameState::PAUSE_MENU);
    break;
  default:
    break;
  }
}

void PauseStateHandler::handleState(Controller &controller) {
  controller.setState(GameState::PAUSE_MENU);
}

void PauseStateHandler::handleInput(Controller &controller, int ch) {
  switch (static_cast<GameplayControls>(tolower(ch))) {
  case GameplayControls::PAUSE:
    controller.setState(GameState::GAMEPLAY);
    break;
  default:
    break;
  }
}
