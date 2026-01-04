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
  ARROW_RIGHT = KEY_RIGHT,
  ENTER = '\n', // or '\r' depending on the system
  SPACE = '\0',
  INC_MSG_INDEX = 'k',
  DEC_MSG_INDEX = 'i',
  SPELL_1 = '1',
  SPELL_2 = '2',
  SPELL_3 = '3',
  SPELL_4 = '4',
  SPELL_5 = '5'
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

  InfoDeque menuOptions(mainMenuOptions.size());
  for (const auto &option : mainMenuOptions) {
    menuOptions.addMessage({option.second}); // put the option into a vector
  }

  renderer.draw(RendererData(emptyGrid, menuOptions, emptyStats, emptyPos));
}

void MainMenuStateHandler::handleInput(Controller &controller, int ch) {
  switch (static_cast<MainMenuOptions>(tolower(ch))) {
  case MainMenuOptions::START_GAME: {
    controller.setState(GameState::GAMEPLAY);
    auto &model = controller.model;
    model.restart();
    break;
  }
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
  model.update();
  auto &renderer = controller.renderer;
  auto stat = model.getPlayerStats();
  renderer.setState(GameState::GAMEPLAY);
  renderer.draw(
      RendererData(model.map->grid, *model.info, stat, model.player->position, &model.activeSpellEffects, &model.traps));

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
    model.queuePlayerMove(Direction::UP);
    break;
  case GameplayControls::LEFT:
  case GameplayControls::ARROW_LEFT:
    model.queuePlayerMove(Direction::LEFT);
    break;
  case GameplayControls::DOWN:
  case GameplayControls::ARROW_DOWN:
    model.queuePlayerMove(Direction::DOWN);
    break;
  case GameplayControls::RIGHT:
  case GameplayControls::ARROW_RIGHT:
    model.queuePlayerMove(Direction::RIGHT);
    break;
  case GameplayControls::PAUSE:
    controller.setState(GameState::PAUSE_MENU);
    break;
  case GameplayControls::INC_MSG_INDEX:

    model.info->increaseStartIndex();

    break;
  case GameplayControls::DEC_MSG_INDEX:
    model.info->decreaseStartIndex();
    break;
  case GameplayControls::SPELL_1:
    model.castPlayerSpell(0, model.player->getLastDirection());
    break;
  case GameplayControls::SPELL_2:
    model.castPlayerSpell(1, model.player->getLastDirection());
    break;
  case GameplayControls::SPELL_3:
    model.castPlayerSpell(2, model.player->getLastDirection());
    break;
  case GameplayControls::SPELL_4:
    model.castPlayerSpell(3, model.player->getLastDirection());
    break;
  case GameplayControls::SPELL_5:
    model.castPlayerSpell(4, model.player->getLastDirection());
    break;
  default:
    break;
  }
}

void PauseStateHandler::handleState(Controller &controller) {
  auto &renderer = controller.renderer;
  renderer.setState(GameState::PAUSE_MENU);
}

void PauseStateHandler::handleInput(Controller &controller, int ch) {
  switch (static_cast<GameplayControls>(tolower(ch))) {
  case GameplayControls::QUIT:
  case GameplayControls::ESC:
    controller.stopRunning();
    break;
  case GameplayControls::PAUSE:
    controller.setState(GameState::GAMEPLAY);
    break;
  default:
    break;
  }
}

void GameOverStateHandler::handleState(Controller &controller) {
  auto &model = controller.model;
  auto stat = model.getPlayerStats();
  Renderer &renderer = controller.renderer;
  renderer.setState(GameState::GAME_OVER);
  renderer.draw(
      RendererData(model.map->grid, *model.info, stat, model.player->position));
}

void GameOverStateHandler::handleInput(Controller &controller, int ch) {
  switch (static_cast<GameplayControls>(tolower(ch))) {
  case GameplayControls::ENTER:
  case GameplayControls::SPACE:
    controller.setState(GameState::MAIN_MENU);
    break;
  case GameplayControls::QUIT:
  case GameplayControls::ESC:
    controller.stopRunning();
    break;
  default:
    break;
  }
}
