#include "renderer.h"
#include "game_board_renderer.h"
#include "main_menu_renderer.h"
#include <cstdlib>

Renderer::Renderer() {
  initscr(); // Call initscr() to initialize the library
  noecho();
  curs_set(0);

  stateRendererMap[GameState::MAIN_MENU] = [](const RendererData &) {
    return std::make_unique<MainMenuRenderer>();
  };
  stateRendererMap[GameState::GAMEPLAY] = [](const RendererData &data) {
    return std::make_unique<GameBoardRenderer>(data);
  };
  // ... other game states
}

Renderer::Renderer(const Renderer &other)
    : currentGameState(other.currentGameState),
      stateRendererMap(other.stateRendererMap) {
  initscr();
  noecho();
  curs_set(0);
}

Renderer::~Renderer() {
  endwin();
  std::system("clear");
  exit(0);
}

void Renderer::setState(GameState gameState) { currentGameState = gameState; }

void Renderer::draw(const RendererData &data) {
  if (!stateRendererMap.count(currentGameState)) {
    return;
  }

  std::unique_ptr<StateRenderer> currentStateRenderer =
      stateRendererMap[currentGameState](data);
  currentStateRenderer->draw();
}
