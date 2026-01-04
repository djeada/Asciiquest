#include "game_over_renderer.h"
#include "utils/global_config.h"
#include <ncurses.h>
#include <string>

GameOverRenderer::GameOverRenderer(const RendererData &_data) : data(_data) {}
GameOverRenderer::~GameOverRenderer() {}

void GameOverRenderer::draw() {
  // First, draw the game board content
  std::unique_ptr<GameBoardRenderer> gameBoardRenderer =
      std::make_unique<GameBoardRenderer>(data);
  gameBoardRenderer->drawContent();
  
  // Then draw "Game Over" on top
  drawGameOver();
  
  // Finally, apply all changes at once for double buffering
  wnoutrefresh(stdscr);
  doupdate();
}

void GameOverRenderer::drawGameOver() {
  getmaxyx(stdscr, termHeight, termWidth);
  std::string gameOver = "Game Over";

  // Calculate the position to center "Game Over" in the terminal window
  int xPos = (termWidth * GlobalConfig::getInstance().getConfig<double>(
                              "BoardRectRight") -
              gameOver.length()) /
             2;
  int yPos = (termHeight * GlobalConfig::getInstance().getConfig<double>(
                               "BoardRectBottom")) /
             2;

  // Use bold and red color for "Game Over"
  attron(A_BOLD | COLOR_PAIR(static_cast<int>(ColorPair::PLAYER)));
  mvprintw(yPos, xPos, "%s", gameOver.c_str());
  attroff(A_BOLD | COLOR_PAIR(static_cast<int>(ColorPair::PLAYER)));
}
