#include "renderer.h"
#include "game_over_renderer.h"
#include "main_menu_renderer.h"
#include <cstdlib>

Renderer::Renderer() {
  initscr(); // Call initscr() to initialize the library
  noecho();
  curs_set(0);
  if (has_colors()) {
    start_color();
    use_default_colors();
  }

  stateRendererMap[GameState::MAIN_MENU] = [](const RendererData &) {
    return std::make_unique<MainMenuRenderer>();
  };
  stateRendererMap[GameState::GAMEPLAY] = [](const RendererData &data) {
    return std::make_unique<GameBoardRenderer>(data);
  };
  stateRendererMap[GameState::GAME_OVER] = [](const RendererData &data) {
    return std::make_unique<GameOverRenderer>(data);
  };
  // ... other game states
}

Renderer::Renderer(const Renderer &other)
    : currentGameState(other.currentGameState),
      stateRendererMap(other.stateRendererMap) {
  initscr();
  noecho();
  curs_set(0);
  if (has_colors()) {
    start_color();
    use_default_colors();
  }
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
