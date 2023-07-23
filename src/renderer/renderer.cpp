#include "renderer.h"
#include "game_board_renderer.h"
#include "main_menu_renderer.h"

Renderer::Renderer() {
  initscr();   // Initialize the ncurses library
  noecho();    // Don't display key presses
  curs_set(0); // Don't display the cursor
  // Start color functionality

  init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_WHITE);
  init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_WHITE);
  init_pair(COLOR_RED, COLOR_RED, COLOR_WHITE);
}

Renderer::Renderer(const Renderer &) {
  currentStateRenderer = std::make_unique<MainMenuRenderer>();
}

Renderer::~Renderer() { endwin(); }

void Renderer::setState(GameState gameState) {
  switch (gameState) {
  case GameState::MAIN_MENU:
    currentStateRenderer = std::make_unique<MainMenuRenderer>();
    break;
  case GameState::GAMEPLAY:
    currentStateRenderer = std::make_unique<GameBoardRenderer>();
    break;
  // ... other cases to handle other game states
  default:
    break;
  }
}
void Renderer::draw(const std::vector<std::vector<CellType>> &grid,
                    const std::vector<std::string> &fightInfo,
                    const std::unordered_map<std::string, std::string> &stats) {
  currentStateRenderer->draw(grid, fightInfo, stats);
}
