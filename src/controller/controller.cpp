#include "controller.h"

Controller::Controller(Model &m, Renderer &r) : model(m), renderer(r) {
  isRunning = false;
  currentGameState = GameState::MAIN_MENU;
}

void Controller::run() {
  keypad(stdscr, TRUE);
  isRunning = true;

  while (isRunning) {
    handleGameState();
    handleInput();
  }
}

void Controller::handleGameState() {
  switch (currentGameState) {
  case GameState::MAIN_MENU:
    handleMainMenu();
    break;
  case GameState::GAMEPLAY:
    handleGamePlay();
    break;
  case GameState::PAUSE_MENU:
    handlePauseMenu();
    break;
  case GameState::GAME_OVER:
    handleGameOver();
    break;
  default:
    break;
  }
}

void Controller::handleGamePlay() {
  model.update(); // Update the game state
  auto stat = model.getPlayerStats();
  renderer.setState(GameState::GAMEPLAY);
  renderer.draw(model.map->grid, model.fightInfo, stat);

  // if (model.isGameOver()) {
  //   currentGameState = GameState::GAME_OVER;
  // }
}

void Controller::handleGameOver() {
  renderer.setState(GameState::GAME_OVER);
  // renderer.drawGameOverScreen(model.getScore());

  // Pause for a moment before returning to the main menu
  currentGameState = GameState::MAIN_MENU;
}

void Controller::handlePauseMenu() {
  renderer.setState(GameState::PAUSE_MENU);
  // renderer.drawPauseMenu(); // Draw the pause menu
}

void Controller::handleMainMenu() {
  renderer.setState(GameState::MAIN_MENU);

  std::vector<std::vector<CellType>> emptyGrid;
  std::vector<std::string> menuOptions;
  std::unordered_map<std::string, std::string> emptyStats;

  menuOptions.push_back("1. Start Game");
  menuOptions.push_back("2. Options");
  menuOptions.push_back("3. Quit");

  renderer.draw(emptyGrid, menuOptions, emptyStats);
}

void Controller::handleInput() {
  int ch = getch();

  switch (currentGameState) {
  case GameState::GAMEPLAY:
    handleGameplayInput(ch);
    break;
  case GameState::MAIN_MENU:
    handleMainMenuInput(ch);
    break;
  case GameState::PAUSE_MENU:
    handlePauseMenuInput(ch);
    break;
  default:
    break;
  }
}

void Controller::handleGameplayInput(int ch) {
  switch (ch) {
  case 'q':
    // case KEY_ESCAPE: // Escape key
    isRunning = false;
    break;
  case 'w':
  case KEY_UP: // Up arrow key
    model.movePlayer(Direction::UP);
    break;
  case 'a':
  case KEY_LEFT: // Left arrow key
    model.movePlayer(Direction::LEFT);
    break;
  case 's':
  case KEY_DOWN: // Down arrow key
    model.movePlayer(Direction::DOWN);
    break;
  case 'd':
  case KEY_RIGHT: // Right arrow key
    model.movePlayer(Direction::RIGHT);
    break;
  case 'p':
    currentGameState = GameState::PAUSE_MENU;
    break;
  default:
    break;
  }
}

void Controller::handleMainMenuInput(int ch) {
  switch (ch) {
  case '1':
    currentGameState = GameState::GAMEPLAY;
    break;
  case '2':
    // Go to options menu
    // currentGameState = GameState::OPTIONS;
    break;
  case '3':
    isRunning = false;
    break;
  default:
    break;
  }
}

void Controller::handlePauseMenuInput(int ch) {
  switch (ch) {
  case 'p':
    currentGameState = GameState::GAMEPLAY;
    break;
  default:
    break;
  }
}
